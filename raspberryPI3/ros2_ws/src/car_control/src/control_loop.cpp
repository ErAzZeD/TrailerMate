#include "../include/car_control/control_loop.h"

ao = (K*Te/2) + 1;
bo = (K*Te/2) - 1;
eo = Te / (Te + 2*To);
fo = (Te - 2*To) / (Te + 2*To);

/* Calculate the recurrence equation based on the first order attenuation filter to avoid the wheels losing grip
*   PWM_order -> Output of compensator(k+1)
*   PWM_order_last -> Output of compensator(k)
*   PWM_att_last -> Last attenuation output of first order filter
*   Update of PWM_order is ~~ update of PWM_att
*/
void attenuation_recurrence(double& PWM_order,double PWM_order_last,double& PWM_att_last){
    PWM_order = eo * (PWM_order + PWM_order_last) - fo * PWM_att_last;
    PWM_att_last=PWM_order;
}
/* Calculate the recurrence equation based on the compensator to move the car forward and backward
*   RPM_order -> Desired Speed (RPM)
*   PWM_order -> I(k+1)
*   PWM_order_last -> I(k)
*   Error -> Erreur(k+1)
*   Error_last -> Erreur(k)
*/
void recurrence_equation(bool attenuation, double RPM_order, double Error, double& Error_last, double& PWM_order, double& PWM_order_last, double currentSpeed){
    Error=RPM_order-currentSpeed; 
    Error=Error*0.9;

    PWM_order = PWM_order_last + ao*Error + bo*Error_last;
    //PWM_order = PWM_order_last + 1.2*Error + 0.8*Error_last;
    //PWM_order = std::min(50,std::max(0,PWM_order));

    Error_last=Error;
    if (PWM_order > 50.0) {PWM_order=50.0;}
    if (PWM_order < 0.0) {PWM_order=0.0;}
    PWM_order_last=PWM_order;

    if (attenuation) {
        attenuation_recurrence(PWM_order, PWM_order_last, PWM_att_last)
    }
}

void compensator_recurrence(bool init, double RPM_order, bool reverse,double currentRightSpeed, double currentLeftSpeed, uint8_t& rightRearPwmCmd, uint8_t& leftRearPwmCmd){   

    // Variable statique pour conserver la valeur entre les appels
    static double Left_PWM_order;
    static double Right_PWM_order;
    static double Left_PWM_order_last;
    static double Right_PWM_order_last;
    static double Left_Error_last;
    static double Right_Error_last;
    static double PWM_att_last;

    double Left_Error=0.0;
    double Right_Error=0.0;

    if (init){                                                  // /!\ reinit if reverse mode changes ?? /!\  
        Left_PWM_order = 0.0;
        Right_PWM_order = 0.0;
        Left_PWM_order_last = 0.0;
        Right_PWM_order_last = 0.0;
        Left_Error_last = 0.0;
        Right_Error_last = 0.0;
        PWM_att_last = 0.0;
    }

    if (reverse){   // => PWM : [50 -> 0] (reverse)
        recurrence_equation(1, RPM_order, Right_Error, Right_Error_last, Right_PWM_order, Right_PWM_order_last, currentRightSpeed);
        recurrence_equation(1, RPM_order, Left_Error, Left_Error_last, Left_PWM_order, Left_PWM_order_last, currentLeftSpeed);
        
        rightRearPwmCmd = 50 - Right_PWM_order; 
        leftRearPwmCmd = 50 - Left_PWM_order;  

    } else{     // => PWM : [50 -> 100] (forward)
        recurrence_equation(1, RPM_order, Right_Error, Right_Error_last, Right_PWM_order, Right_PWM_order_last, currentRightSpeed);
        recurrence_equation(1, RPM_order, Left_Error, Left_Error_last, Left_PWM_order, Left_PWM_order_last, currentLeftSpeed);
        
        rightRearPwmCmd = Right_PWM_order + 50; 
        leftRearPwmCmd = Left_PWM_order + 50;    
    }
}
