#ifndef __PID_H__
#define __PID_H__

#include <system.h>

float X_PID(float kp,float ki,float kd);
float Y_PID(float kp,float ki,float kd);
int my_abs(int value);
#endif