#include "../include/car_control/control_loop.h"

float Fe = 1000;
float Te = 1/Fe;
float K = 4.59;  // Ki / Kp 
float ao = (K*Te/2) + 1;
float bo = (K*Te/2) - 1;

/* Calculate the recurrence equation based on the compensator to move the car forward and backward
*   RPM_order -> Desired Speed (RPM)
*   PWM_order -> I(k+1)
*   PWM_order_last -> I(k)
*   Error -> Erreur(k+1)
*   Error_last -> Erreur(k)
*/
void recurrence_equation(float RPM_order, float Error, float& Error_last, float& PWM_order, float& PWM_order_last, float currentSpeed){
    Error=RPM_order-currentSpeed;  // 
    Error=Error*0.9;

    PWM_order = PWM_order_last + ao*Error + bo*Error_last;
    //PWM_order = std::min(50,std::max(0,PWM_order));
    Error_last=Error;
    PWM_order_last=PWM_order;
}

int * compensator_recurrence(float RPM_order, bool reverse, float currentRightSpeed, float currentLeftSpeed, uint8_t& rightRearPwmCmd, uint8_t& leftRearPwmCmd){

    // Variable statique pour conserver la valeur entre les appels
    static float Left_PWM_order = 0.0;
    static float Right_PWM_order = 0.0;
    static float Left_PWM_order_last = 0.0;
    static float Right_PWM_order_last = 0.0;
    static float Left_Error_last = 0.0;
    static float Right_Error_last = 0.0;

    float Left_Error;
    float Right_Error;
    
    if (reverse){   // => PWM : [50 -> 0] (reverse)

        // currentRightSpeed = -currentRightSpeed;
        // ...
        // Sortie right/leftRearPwmCmd = 50 - recurrence_eq 

    } else{     // => PWM : [50 -> 100] (forward)

        recurrence_equation(RPM_order, Right_Error, Right_Error_last, Right_PWM_order, Right_PWM_order_last, currentRightSpeed);
        recurrence_equation(RPM_order, Left_Error, Left_Error_last, Left_PWM_order, Left_PWM_order_last, currentLeftSpeed);
        
        rightRearPwmCmd = Right_PWM_order + 50; 
        leftRearPwmCmd = Left_PWM_order + 50;    
    }

    return 0;

}