#ifndef __control_loop_H
#define __control_loop_H

#include <cstdint>
#include <stdint.h>

#define Te 0.1  // Motors data (right/left feedback) are periodically send with a period egals to 100ms.
#define Kcomp 4.59  // Ki/Kp 
#define To 0.333// Time constant of first order filter

#define ao 1.2295
#define bo 0.7705 // negatif
#define eo 0.1305
#define fo 0.7389 // negatif

// Calculate the recurrence equation based on the first order attenuation filter to avoid the wheels losing grip
void attenuation_recurrence(double& PWM_order,double PWM_order_last,double& PWM_att_last);

// Calculate the recurrence equation based on the compensator to move the car forward and backward
void recurrence_equation(bool attenuation, double RPM_order, double Error, double& Error_last, double& PWM_order, double& PWM_order_last, double currentSpeed);  

void compensator_recurrence(bool init, double RPM_order, bool reverse, double currentRightSpeed, double currentLeftSpeed, uint8_t& rightRearPwmCmd, uint8_t& leftRearPwmCmd);

#endif /*__ control_loop_H */
