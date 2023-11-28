#ifndef __control_loop_H
#define __control_loop_H

#include <cstdint>
#include <stdint.h>

#define Te 0.1  // Motors data (right/left feedback) are periodically send with a period egals to 100ms.
#define K 4.59  // Ki/Kp 

static double ao;
static double bo;

/* Calculate the recurrence equation based on the compensator to move the car forward and backward
*   RPM_order -> Desired Speed (RPM)
*   PWM_order -> I(k+1)
*   PWM_order_last -> I(k)
*   Error -> Erreur(k+1)
*   Error_last -> Erreur(k)
*/
void recurrence_equation(double RPM_order, double Error, double& Error_last, double& PWM_order, double& PWM_order_last, double currentSpeed);  
// int *
void compensator_recurrence(bool init, double RPM_order, bool reverse, double currentRightSpeed, double currentLeftSpeed, uint8_t& rightRearPwmCmd, uint8_t& leftRearPwmCmd);

#endif /*__ control_loop_H */
