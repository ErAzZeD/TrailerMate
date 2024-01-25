#ifndef __car_control_node_H
#define __car_control_node_H

#include <stdint.h>
#include <string.h>  


#define PERIOD_UPDATE_CMD 1ms //Period to update proupulsion and steering command in [ms]

#define STOP 50 //PWM value to stop motors
#define FIXED_SPEED 20.0 //Fixed speed for Autonomous Mode

struct IMU_filter_var {
    float Roll_last;
    float Roll_filter;
    float Roll_filter_last;
    float Pitch_last;
    float Pitch_filter;
    float Pitch_filter_last;
    float Yaw_last;
    float Yaw_filter;
    float Yaw_filter_last;
};

struct Car_Angle {
    float car_angle;
    float ref_angle;
};

#endif /*__ car_control_node_H */
