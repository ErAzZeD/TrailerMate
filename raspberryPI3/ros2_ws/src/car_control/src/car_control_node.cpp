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

#include "sensor_msgs/msg/imu.hpp"
#include <sensor_msgs/msg/magnetic_field.hpp>

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
        "trailer_angle", 10, std::bind(&car_control::trailerAngleCallback, this, _1));

        subscription_IMU_package_ = this->create_subscription<sensor_msgs::msg::Imu>(
        "imu/data", 10, std::bind(&car_control::imuCallback, this, _1));
        
        subscription_IMU_magnetic_ = this->create_subscription<sensor_msgs::msg::MagneticField>(
        "imu/mag", 10, std::bind(&car_control::imuMagCallback, this, _1));

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
   
    /* Update direction from IMU [callback function]  :
    *
    * This function is called when a message is published on the "/imu/data" topic
    * 
    */
    void imuCallback(const sensor_msgs::msg::Imu & IMU){
        geometry_msgs::msg::Quaternion q;
        q = IMU.orientation;
        roll = atan2(2 * (q.w * q.x + q.y * q.z), 1 - 2 * (q.x * q.x + q.y * q.y));
        pitch = asin(2 * (q.w * q.y - q.z * q.x));
        yaw = atan2(2 * (q.w * q.z + q.x * q.y), 1 - 2 * (q.y * q.y + q.z * q.z));
    }

    /* Update direction from IMU [callback function]  :
    *
    * This function is called when a message is published on the "/imu/mag" topic
    * 
    */
    void imuMagCallback(const sensor_msgs::msg::MagneticField & MAG){  
    	geometry_msgs::msg::Vector3 mag_q;
    	mag_q = MAG.magnetic_field;
	x_mag=mag_q.x;
	y_mag=mag_q.y;
	z_mag=mag_q.z;
        //direction = atan2(corrected_y, corrected_x) * rad2deg ;
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
        float SteerAngle = (trailerAngle / 90) * 6;	// trailerAngle normalization and proportional gain
        if (SteerAngle > 1.0) {
            SteerAngle=1.0f;
        } else if (SteerAngle < -1.0) {
            SteerAngle=-1.0f;
        }
        recurrence_PI_steering(SteerAngle, currentSteerAngle, ErrorAngle_last, PWM_angle, PWM_angle_last, direction_prec);
	//steeringCmd(SteerAngle, currentSteerAngle, PWM_angle);
        //RCLCPP_INFO(this->get_logger(), "Valeur de TrailerAngle : %.2f et de SteerAngle : %.2f", trailerAngle, SteerAngle);
    }
    
    
/* Calculate the recurrence equation based on the first order IMU filter to reduce noise disturbance
*	IF USED WITH IMU Mag
*	Roll  -> X
*	Pitch -> Y
*	Yaw   -> Z
*/
    void IMU_filter(float &Roll, float &Pitch, float &Yaw, IMU_filter_var &IMU_filter) {
        IMU_filter.Roll_filter = 0.024390243902439022 * (Roll + IMU_filter.Roll_last) + 0.9512195121951219 * IMU_filter.Roll_filter_last; 
	IMU_filter.Roll_filter_last = IMU_filter.Roll_filter;
        IMU_filter.Roll_last = Roll;  
        
        IMU_filter.Pitch_filter = 0.024390243902439022 * (Pitch + IMU_filter.Pitch_last) + 0.9512195121951219 * IMU_filter.Pitch_filter_last;  
	IMU_filter.Pitch_filter_last = IMU_filter.Pitch_filter;
        IMU_filter.Pitch_last = Pitch;  
        
        IMU_filter.Yaw_filter = 0.024390243902439022 * (Yaw + IMU_filter.Yaw_last) + 0.9512195121951219 * IMU_filter.Yaw_filter_last;  
	IMU_filter.Yaw_filter_last = IMU_filter.Yaw_filter;
        IMU_filter.Yaw_last = Yaw;        
    
    	//RCLCPP_INFO(this->get_logger(), "Roll : (%.2f,%.2f) , Pitch : (%.2f,%.2f) , Yaw : (%.2f,%.2f)", Roll,Roll_filter, Pitch,Pitch_filter, Yaw, Yaw_filter);
    	//RCLCPP_INFO(this->get_logger(), "Yaw : %.4f , Yaw_filter : %.4f || Yaw_filter_last : %.4f , Yaw_last : %.4f", Yaw,IMU_filter.Yaw_filter, IMU_filter.Yaw_filter_last,IMU_filter.Yaw_last);
    	
    	RCLCPP_INFO(this->get_logger(), "X : (%.2f,%.2f) , Y : (%.2f,%.2f) , Z : (%.2f,%.2f)", Roll,IMU_filter.Roll_filter, Pitch,IMU_filter.Pitch_filter, Yaw, IMU_filter.Yaw_filter);
    	
    	//Roll=Roll_filter;
    	//Pitch=Pitch_filter;
    	//Yaw=Yaw_filter;
    }


	void CarAngle(float angle_mag, int & init, Car_Angle & car_angle_var) {
	    if (init) {
	    	RCLCPP_INFO(this->get_logger(), "------ Car_angle INIT -----");
	    	car_angle_var.ref_angle = angle_mag;
	    	car_angle_var.car_angle = 0.0;
	    	init = 0;
	    } else {
	    	car_angle_var.car_angle = car_angle_var.ref_angle - angle_mag;
	    }
	    RCLCPP_INFO(this->get_logger(), "Car_angle : %.5f", car_angle_var.car_angle);
	    RCLCPP_INFO(this->get_logger(), "Car_angle : %.5f", car_angle_var.car_angle*1000000);
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

                //if ((!frontObstacle && !reverse) || (!rearObstacle && reverse) || (!frontObstacle && !rearObstacle)) {
                if (true) {
                    RPM_order = requestedThrottle*50.0f;
                    //IMU_filter(x_mag, y_mag, z_mag, imu_mag_filter);
                    CarAngle(y_mag, reinit, car_angle_var);
                    
                    if (reverse) {    // => PWM : [50 -> 0] (reverse)
                        recurrence_PI_motors(RPM_order, Error_last_right, PWM_order_right, PWM_order_last_right, currentRightSpeed);
                        recurrence_PI_motors(RPM_order, Error_last_left, PWM_order_left, PWM_order_last_left, currentLeftSpeed);
                        
                        PWM_order_filter = PWM_order_right;
                        attenuate_recurrence(PWM_order_filter, PWM_order_l, PWM_att_last);

                        rightRearPwmCmd = 50 - PWM_order_filter; 
                        //leftRearPwmCmd = 50 - PWM_order_left; // capteur cassé, donc on se base sur la roue droite
                        leftRearPwmCmd = rightRearPwmCmd; 
                        
                        trailer_angle_compensator(currentAngle, ErrorAngle_last, PWM_angle, PWM_angle_last, direction_prec, trailerAngle);
                    	steeringPwmCmd=PWM_angle;
                    	
                    	reinit = 1;
                        
                    } else {   // => PWM : [50 -> 100] (forward)
                        recurrence_PI_motors(RPM_order, Error_last_right, PWM_order_right, PWM_order_last_right, currentRightSpeed);
                        recurrence_PI_motors(RPM_order, Error_last_left, PWM_order_left, PWM_order_last_left, currentLeftSpeed);
                        
                        PWM_order_filter = PWM_order_right;
                        attenuate_recurrence(PWM_order_filter, PWM_order_l, PWM_att_last);

                        rightRearPwmCmd = PWM_order_filter + 50; 
                        //leftRearPwmCmd = PWM_order_left + 50; //capteur cassé, donc on se base sur la roue droite  
                        leftRearPwmCmd = rightRearPwmCmd; 
                        
                        recurrence_PI_steering(requestedSteerAngle, currentAngle, ErrorAngle_last, PWM_angle, PWM_angle_last,direction_prec);
                    	steeringPwmCmd=PWM_angle;
                    	
                    	
                    }

                }
                else {
                    leftRearPwmCmd = STOP;
                    rightRearPwmCmd = STOP;
                    steeringPwmCmd = STOP;
                }

            //Autonomous Mode
            } else if (mode==1){
                RPM_order = 10.0f;
                //reverse = 1;
                
                if (reverse) {    // => PWM : [50 -> 0] (reverse)
                    recurrence_PI_motors(RPM_order, Error_last_right, PWM_order_right, PWM_order_last_right, currentRightSpeed);
                    recurrence_PI_motors(RPM_order, Error_last_left, PWM_order_left, PWM_order_last_left, currentLeftSpeed);
                    
                    PWM_order_filter = PWM_order_right;
                    attenuate_recurrence(PWM_order_filter, PWM_order_l, PWM_att_last);

	            rightRearPwmCmd = 50 - PWM_order_filter; 
                    //leftRearPwmCmd = 50 - PWM_order_left; //capteur cassé, donc on se base sur la roue droite
                    leftRearPwmCmd = rightRearPwmCmd; 
                    
                    trailer_angle_compensator(currentAngle, ErrorAngle_last, PWM_angle, PWM_angle_last, direction_prec, trailerAngle);
                    steeringPwmCmd=PWM_angle;
                } else {   // => PWM : [50 -> 100] (forward)
                    recurrence_PI_motors(RPM_order, Error_last_right, PWM_order_right, PWM_order_last_right, currentRightSpeed);
                    recurrence_PI_motors(RPM_order, Error_last_left, PWM_order_left, PWM_order_last_left, currentLeftSpeed);
                    
                    PWM_order_filter = PWM_order_right;
                    attenuate_recurrence(PWM_order_filter, PWM_order_l, PWM_att_last);

		     rightRearPwmCmd = PWM_order_filter + 50; 
                    //leftRearPwmCmd = PWM_order_left + 50; // capteur cassé, donc on se base sur la roue droite  
                     leftRearPwmCmd = rightRearPwmCmd; 
                     steeringPwmCmd= STOP; 
                }
                               
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
    
    // IMU
    float roll;
    float pitch;
    float yaw;
    // IMU Mag
    float x_mag;
    float y_mag;
    float z_mag;
    
    // IMU Filter
    struct IMU_filter_var imu_filter;
    // IMU Mag Filter
    struct IMU_filter_var imu_mag_filter;
    // CarAngle
    struct Car_Angle car_angle_var;
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
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr subscription_IMU_package_;
    rclcpp::Subscription<sensor_msgs::msg::MagneticField>::SharedPtr subscription_IMU_magnetic_;

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
