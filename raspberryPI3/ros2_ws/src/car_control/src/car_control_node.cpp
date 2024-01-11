#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include <functional>
#include <memory>

#include "interfaces/msg/motors_order.hpp"
#include "interfaces/msg/motors_feedback.hpp"
#include "interfaces/msg/steering_calibration.hpp"
#include "interfaces/msg/joystick_order.hpp"
#include "interfaces/msg/obstacle_detection.hpp"
#include "interfaces/msg/angle_trailer.hpp"

#include "std_srvs/srv/empty.hpp"

#include "../include/car_control/steeringCmd.h"
#include "../include/car_control/propulsionCmd.h"
#include "../include/car_control/car_control_node.h"

using namespace std;
using placeholders::_1;


class car_control : public rclcpp::Node {                 

public:
    car_control()
    : Node("car_control_node")
    {
        start = false;
        mode = 0;
        requestedThrottle = 0;
        requestedSteerAngle = 0;
    
        RCLCPP_INFO(this->get_logger(), "INIT of Error and PWM to 0.0");
        // Motors control loop
        Error_last_right = 0.0f;
        Error_last_left = 0.0f;
        PWM_order_right = 0.0f;
        PWM_order_left = 0.0f;
        PWM_order_last_right = 0.0f;
        PWM_order_last_left = 0.0f;
        // Steering control loop
        PWM_angle = 0.0f;
        PWM_angle_last = 0.0f;
        ErrorAngle_last = 0.0f;
        direction_prec = true;
        // Attenuation
        PWM_att_last= 0.0f;
        PWM_order_filter= 0.0f;
        PWM_order_l= 0.0f;

        publisher_can_= this->create_publisher<interfaces::msg::MotorsOrder>("motors_order", 10);

        publisher_steeringCalibration_ = this->create_publisher<interfaces::msg::SteeringCalibration>("steering_calibration", 10);

        

        subscription_joystick_order_ = this->create_subscription<interfaces::msg::JoystickOrder>(
        "joystick_order", 10, std::bind(&car_control::joystickOrderCallback, this, _1));

        subscription_motors_feedback_ = this->create_subscription<interfaces::msg::MotorsFeedback>(
        "motors_feedback", 10, std::bind(&car_control::motorsFeedbackCallback, this, _1));

        subscription_steering_calibration_ = this->create_subscription<interfaces::msg::SteeringCalibration>(
        "steering_calibration", 10, std::bind(&car_control::steeringCalibrationCallback, this, _1));

        subscription_obstacle_detection_ = this->create_subscription<interfaces::msg::ObstacleDetection>(
        "obstacle_detection", 10, std::bind(&car_control::obstacleDetectionCallback, this, _1));

        subscription_trailer_angle_package_ = this->create_subscription<interfaces::msg::AngleTrailer>(
        "trailer_angle_package", 10, std::bind(&car_control::trailerAngleCallback, this, _1));

        server_calibration_ = this->create_service<std_srvs::srv::Empty>(
                            "steering_calibration", std::bind(&car_control::steeringCalibration, this, std::placeholders::_1, std::placeholders::_2));

        timer_ = this->create_wall_timer(PERIOD_UPDATE_CMD, std::bind(&car_control::updateCmd, this));

        
        RCLCPP_INFO(this->get_logger(), "car_control_node READY");
    }

    
private:

    /* Update start, mode, requestedThrottle, requestedSteerAngle and reverse from joystick order [callback function]  :
    *
    * This function is called when a message is published on the "/joystick_order" topic
    * 
    */
    void joystickOrderCallback(const interfaces::msg::JoystickOrder & joyOrder) {

        if (joyOrder.start != start){
            start = joyOrder.start;

            if (start)
                RCLCPP_INFO(this->get_logger(), "START");
            else 
                RCLCPP_INFO(this->get_logger(), "STOP");

        }
        

        if (joyOrder.mode != mode && joyOrder.mode != -1){ //if mode change
            mode = joyOrder.mode;

            if (mode==0){
                RCLCPP_INFO(this->get_logger(), "Switching to MANUAL Mode");
            }else if (mode==1){
                RCLCPP_INFO(this->get_logger(), "Switching to AUTONOMOUS Mode");
            }else if (mode==2){
                RCLCPP_INFO(this->get_logger(), "Switching to STEERING CALIBRATION Mode");
                startSteeringCalibration();
            }
        }
        
        if ((mode ==0) && start){  //if manual mode -> update requestedThrottle, requestedSteerAngle and reverse from joystick order
            requestedThrottle = joyOrder.throttle;
            requestedSteerAngle = joyOrder.steer;
            reverse = joyOrder.reverse;
        }
    }

    /* Update currentAngle from motors feedback [callback function]  :
    *
    * This function is called when a message is published on the "/motors_feedback" topic
    * 
    */
    void motorsFeedbackCallback(const interfaces::msg::MotorsFeedback & motorsFeedback){
        currentAngle = motorsFeedback.steering_angle;
        currentLeftSpeed = motorsFeedback.left_rear_speed;
        currentRightSpeed = motorsFeedback.right_rear_speed;
    }

    /* Update command from stop car [callback function]  :
    *
    * This function is called when a message is published on the "/stop_car" topic
    * 
    */
    void obstacleDetectionCallback(const interfaces::msg::ObstacleDetection & obstacleDetection){
        frontObstacle = obstacleDetection.obstacle_detected_front;
        rearObstacle = obstacleDetection.obstacle_detected_rear;
    }

     /* Update command from Trailer Angle [callback function]  :
    *
    * This function is called when a message is published on the "/trailer_angle_package" topic
    * 
    */
    void trailerAngleCallback(const interfaces::msg::AngleTrailer & angleTrailer){
        trailerAngle = angleTrailer.trailer_angle;
    }

// --------------------------------------------------------------

/* Calculate the recurrence equation based on the compensator to move the car forward and backward
*   RPM_order -> Desired Speed (RPM)
*   PWM_order -> I(k+1)
*   PWM_order_last -> I(k)
*   Error -> Erreur(k+1)
*   Error_last -> Erreur(k)
*/
    void recurrence_PI_motors(float RPM_order, float& Error_last, float& PWM_order, float& PWM_order_last, float currentSpeed){
        float Error=RPM_order-currentSpeed; 
        Error=Error*0.9;
        //RCLCPP_INFO(this->get_logger(), "Valeur de Erreur(k+1) : %.2f et de Erreur(k) : %.2f", Error, Error_last);

        PWM_order = PWM_order_last + 1.002*Error - 0.998*Error_last;   // Ki = 4  et Kp = 1   ao = Ki*Te/2 + Kp, bo = Ki*Te/2 - Kp
        //RCLCPP_INFO(this->get_logger(), "Valeur de PWM_order : %.2f", PWM_order);

        if (PWM_order > 50.0) {
            PWM_order=50.0f;
        } else if (PWM_order < 0.0) {
            PWM_order=0.0f;
        }

        Error_last=Error;
        PWM_order_last=PWM_order;
    }


/* Calculate the recurrence equation based on the compensator to steer
*   requestedSteerAngle -> Desired angle [-1,1]
*   PWM_angle -> I(k+1)
*   PWM_angle_last -> I(k)
*   ErrorAngle -> Erreur(k+1)
*   ErrorAngle_last -> Erreur(k)
*   direction -> direction(k+1) : Gauche 0, Droite 1 
*   direction_prec -> direction(k) : Gauche 0, Droite 1 
*/
    void recurrence_PI_steering(float requestedSteerAngle, float currentSteerAngle, float& ErrorAngle_last, float& PWM_angle, float& PWM_angle_last, bool& direction_prec){
        // static bool direction_prec; // (a mettre en static si pas de pb)
        bool direction = requestedSteerAngle >= currentSteerAngle;  
        float ErrorAngle = currentSteerAngle - requestedSteerAngle;
        ErrorAngle = abs(ErrorAngle)*25;  // ErrorAngle : [-2,2] -> [0,50]
        //RCLCPP_INFO(this->get_logger(), "Valeur de Erreur(k+1) : %.2f et de Erreur(k) : %.2f", ErrorAngle, ErrorAngle_last);

        if (direction != direction_prec) {  // Si changement de direction reinit 
            PWM_angle_last=0.0;
            ErrorAngle_last=0.0;
        }
        //PWM_angle = PWM_angle_last + 0.5005*ErrorAngle - 0.4995*ErrorAngle_last;   // Ki = 1  et Kp = 0.5   ao = Ki*Te/2 + Kp, bo = Ki*Te/2 - Kp
        PWM_angle = 12*ErrorAngle;
        //RCLCPP_INFO(this->get_logger(), "Valeur de PWM_angle : %.2f", PWM_angle);

        if (PWM_angle > 50.0) {
            PWM_angle=50.0f;
        } else if (PWM_angle < 0.0) {
            PWM_angle=0.0f;
        }

        PWM_angle_last=PWM_angle;
        ErrorAngle_last=ErrorAngle;
        direction_prec=direction;

        if (direction) {  // Vers la droite
            //RCLCPP_INFO(this->get_logger(), "Tourne à droite");
            PWM_angle = 50 + PWM_angle;
        } else {          // Vers la gauche
            //RCLCPP_INFO(this->get_logger(), "Tourne à gauche");
            PWM_angle = 50 - PWM_angle;
        }
    }


/* Calculate the recurrence equation based on the first order attenuation filter to avoid the wheels losing grip
*   PWM_order -> Output of compensator(k+1)
*   PWM_order_last -> Output of compensator(k)
*   PWM_att_last -> Last attenuation output of first order filter
*   Update of PWM_order is ~~ update of PWM_att
*/
    void attenuate_recurrence(float& PWM_order,float& PWM_order_last,float& PWM_att_last){
        PWM_order = 0.004975124 * (PWM_order + PWM_order_last) + 0.990049751 * PWM_att_last;   // To = 0.1
        //RCLCPP_INFO(this->get_logger(), "Valeur PMW attenuated : %.2f", PWM_order);
	PWM_att_last=PWM_order;
        PWM_order_last = PWM_order;
    }

/* Calculate the recurrence equation based on the compensator to keep trailer and car aligned while reversing
*
*
* 
*/
    void trailer_angle_compensator(float currentSteerAngle, float& ErrorAngle_last, float& PWM_angle, float& PWM_angle_last, bool& direction_prec, float trailerAngle){
        float SteerAngle = (trailerAngle / 90) * 5;	// trailerAngle normalization and proportional gain
        if (SteerAngle > 1.0) {
            SteerAngle=1.0f;
        } else if (SteerAngle < -1.0) {
            SteerAngle=-1.0f;
        }
        RCLCPP_INFO(this->get_logger(), "Valeur de TrailerAngle : %.2f et de SteerAngle : %.2f", trailerAngle, SteerAngle);
        recurrence_PI_steering(SteerAngle, currentSteerAngle, ErrorAngle_last, PWM_angle, PWM_angle_last, direction_prec);
    }


// --------------------------------------------------------------



    /* Update PWM commands : leftRearPwmCmd, rightRearPwmCmd, steeringPwmCmd
    *
    * This function is called periodically by the timer [see PERIOD_UPDATE_CMD in "car_control_node.h"]
    * 
    * In MANUAL mode, the commands depends on :
    * - requestedThrottle, reverse, requestedSteerAngle [from joystick orders]
    * - currentAngle [from motors feedback]
    */
    void updateCmd(){

        auto motorsOrder = interfaces::msg::MotorsOrder();

        if (!start){    //Car stopped
            leftRearPwmCmd = STOP;
            rightRearPwmCmd = STOP;
            steeringPwmCmd = STOP;

        }else{ //Car started

            //Manual Mode
            if (mode==0){

                if ((!frontObstacle && !reverse) || (!rearObstacle && reverse) || (!frontObstacle && !rearObstacle)) {
                    RPM_order = requestedThrottle*50.0f;
                    
                    if (reverse) {    // => PWM : [50 -> 0] (reverse)
                        recurrence_PI_motors(RPM_order, Error_last_right, PWM_order_right, PWM_order_last_right, currentRightSpeed);
                        recurrence_PI_motors(RPM_order, Error_last_left, PWM_order_left, PWM_order_last_left, currentLeftSpeed);
                        
                        PWM_order_filter = PWM_order_right;
                        attenuate_recurrence(PWM_order_filter, PWM_order_l, PWM_att_last);

                        rightRearPwmCmd = 50 - PWM_order_filter; 
                        //leftRearPwmCmd = 50 - PWM_order_left; // capteur cassé, donc on se base sur la roue droite
                        leftRearPwmCmd = rightRearPwmCmd; 
                    } else {   // => PWM : [50 -> 100] (forward)
                        recurrence_PI_motors(RPM_order, Error_last_right, PWM_order_right, PWM_order_last_right, currentRightSpeed);
                        recurrence_PI_motors(RPM_order, Error_last_left, PWM_order_left, PWM_order_last_left, currentLeftSpeed);
                        
                        PWM_order_filter = PWM_order_right;
                        attenuate_recurrence(PWM_order_filter, PWM_order_l, PWM_att_last);

                        rightRearPwmCmd = PWM_order_filter + 50; 
                        //leftRearPwmCmd = PWM_order_left + 50; //capteur cassé, donc on se base sur la roue droite  
                        leftRearPwmCmd = rightRearPwmCmd; 
                    }

                    recurrence_PI_steering(requestedSteerAngle, currentAngle, ErrorAngle_last, PWM_angle, PWM_angle_last, direction_prec);
                    steeringPwmCmd=PWM_angle;
                    reinit = 1;
                }
                else {
                    leftRearPwmCmd = STOP;
                    rightRearPwmCmd = STOP;
                    steeringPwmCmd = STOP;
                }

            //Autonomous Mode
            } else if (mode==1){
                RPM_order = 10.0f;
                reverse = 1;
                
                if (reverse) {    // => PWM : [50 -> 0] (reverse)
                    recurrence_PI_motors(RPM_order, Error_last_right, PWM_order_right, PWM_order_last_right, currentRightSpeed);
                    recurrence_PI_motors(RPM_order, Error_last_left, PWM_order_left, PWM_order_last_left, currentLeftSpeed);
                    
                    PWM_order_filter = PWM_order_right;
                    attenuate_recurrence(PWM_order_filter, PWM_order_l, PWM_att_last);

	            rightRearPwmCmd = 50 - PWM_order_filter; 
                    //leftRearPwmCmd = 50 - PWM_order_left; //capteur cassé, donc on se base sur la roue droite
                    leftRearPwmCmd = rightRearPwmCmd; 
                    
                    trailer_angle_compensator(currentAngle, ErrorAngle_last, PWM_angle, PWM_angle_last, direction_prec, trailerAngle);
                    
                } else {   // => PWM : [50 -> 100] (forward)
                    recurrence_PI_motors(RPM_order, Error_last_right, PWM_order_right, PWM_order_last_right, currentRightSpeed);
                    recurrence_PI_motors(RPM_order, Error_last_left, PWM_order_left, PWM_order_last_left, currentLeftSpeed);
                    
                    PWM_order_filter = PWM_order_right;
                    attenuate_recurrence(PWM_order_filter, PWM_order_l, PWM_att_last);

		     rightRearPwmCmd = PWM_order_filter + 50; 
                    //leftRearPwmCmd = PWM_order_left + 50; // capteur cassé, donc on se base sur la roue droite  
                     leftRearPwmCmd = rightRearPwmCmd; 
                }
                steeringPwmCmd= STOP;                
            }  
        }
        //Send order to motors
        motorsOrder.left_rear_pwm = leftRearPwmCmd;
        motorsOrder.right_rear_pwm = rightRearPwmCmd;
        motorsOrder.steering_pwm = steeringPwmCmd;

        publisher_can_->publish(motorsOrder);
    }


    /* Start the steering calibration process :
    *
    * Publish a calibration request on the "/steering_calibration" topic
    */
    void startSteeringCalibration(){

        auto calibrationMsg = interfaces::msg::SteeringCalibration();
        calibrationMsg.request = true;

        RCLCPP_INFO(this->get_logger(), "Sending calibration request .....");
        publisher_steeringCalibration_->publish(calibrationMsg);
    }


    /* Function called by "steering_calibration" service
    * 1. Switch to calibration mode
    * 2. Call startSteeringCalibration function
    */
    void steeringCalibration([[maybe_unused]] std_srvs::srv::Empty::Request::SharedPtr req,
                            [[maybe_unused]] std_srvs::srv::Empty::Response::SharedPtr res)
    {

        mode = 2;    //Switch to calibration mode
        RCLCPP_WARN(this->get_logger(), "Switching to STEERING CALIBRATION Mode");
        startSteeringCalibration();
    }
    

    /* Manage steering calibration process [callback function]  :
    *
    * This function is called when a message is published on the "/steering_calibration" topic
    */
    void steeringCalibrationCallback (const interfaces::msg::SteeringCalibration & calibrationMsg){

        if (calibrationMsg.in_progress == true && calibrationMsg.user_need == false){
        RCLCPP_INFO(this->get_logger(), "Steering Calibration in progress, please wait ....");

        } else if (calibrationMsg.in_progress == true && calibrationMsg.user_need == true){
            RCLCPP_WARN(this->get_logger(), "Please use the buttons (L/R) to center the steering wheels.\nThen, press the blue button on the NucleoF103 to continue");
        
        } else if (calibrationMsg.status == 1){
            RCLCPP_INFO(this->get_logger(), "Steering calibration [SUCCESS]");
            RCLCPP_INFO(this->get_logger(), "Switching to MANUAL Mode");
            mode = 0;    //Switch to manual mode
            start = false;  //Stop car
        
        } else if (calibrationMsg.status == -1){
            RCLCPP_ERROR(this->get_logger(), "Steering calibration [FAILED]");
            RCLCPP_INFO(this->get_logger(), "Switching to MANUAL Mode");
            mode = 0;    //Switch to manual mode
            start = false;  //Stop car
        }
    
    }
    
    // ---- Private variables ----

    //General variables
    bool start;
    int mode;    //0 : Manual    1 : Auto    2 : Calibration

    //Control loop variables
        // Motors
            //Error
    float Error_last_right;
    float Error_last_left;
            //PWM
    float PWM_order_right;
    float PWM_order_left;
    float PWM_order_last_right;
    float PWM_order_last_left;
            //Others
    float RPM_order;
    int reinit;
            // Attenuation    
    float PWM_att_last;
    float PWM_order_filter;
    float PWM_order_l;
            // Steering
    float PWM_angle;
    float PWM_angle_last;
    float ErrorAngle_last;
    bool direction_prec;
    
    //Motors feedback variables
    float currentAngle;
    float currentRightSpeed;
    float currentLeftSpeed;
    
    //Traile Angle variables
    float trailerAngle;

    //Obstacles variables
    bool frontObstacle;
    bool rearObstacle;

    //Manual Mode variables (with joystick control)
    bool reverse;
    float requestedThrottle;
    float requestedSteerAngle;

    //Control variables
    uint8_t leftRearPwmCmd;
    uint8_t rightRearPwmCmd;
    uint8_t steeringPwmCmd;

    //Publishers
    rclcpp::Publisher<interfaces::msg::MotorsOrder>::SharedPtr publisher_can_;
    rclcpp::Publisher<interfaces::msg::SteeringCalibration>::SharedPtr publisher_steeringCalibration_;

    //Subscribers
    rclcpp::Subscription<interfaces::msg::JoystickOrder>::SharedPtr subscription_joystick_order_;
    rclcpp::Subscription<interfaces::msg::MotorsFeedback>::SharedPtr subscription_motors_feedback_;
    rclcpp::Subscription<interfaces::msg::SteeringCalibration>::SharedPtr subscription_steering_calibration_;
    rclcpp::Subscription<interfaces::msg::ObstacleDetection>::SharedPtr subscription_obstacle_detection_;
    rclcpp::Subscription<interfaces::msg::AngleTrailer>::SharedPtr subscription_trailer_angle_package_;

    //Timer
    rclcpp::TimerBase::SharedPtr timer_;

    //Steering calibration Service
    rclcpp::Service<std_srvs::srv::Empty>::SharedPtr server_calibration_;
};


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<car_control>();

  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}
