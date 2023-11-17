#ifndef __control_loop_H
#define __control_loop_H

#include <cstdint>
#include <stdint.h>


/* Calculate the recurrence equation based on the compensator to move the car forward and backward
*   RPM_order -> Desired Speed (RPM)
*   PWM_order -> I(k+1)
*   PWM_order_last -> I(k)
*   Error -> Erreur(k+1)
*   Error_last -> Erreur(k)
*/
void recurrence_equation(float RPM_order, float Error, float& Error_last, float& PWM_order, float& PWM_order_last, float currentSpeed);  

int * compensator_recurrence(float RPM_order, bool reverse, float currentRightSpeed, float currentLeftSpeed, uint8_t& rightRearPwmCmd, uint8_t& leftRearPwmCmd);

#endif /*__ control_loop_H */