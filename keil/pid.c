#include <pid.h>

extern uint8_t X_point, Y_point;
extern uint8_t Magnet;
	
//x + right
//y + up

int my_abs(int value) {
    return (value < 0) ? -value : value;
}

float X_PID(float kp,float ki,float kd)
{
static float error_now_pos=0;
static float error_last_pos=0;
static float error_sum_pos=0;
static float error;
float output = 0;
	
if(X_point == 255)
	return 0;

error = X_point-125;
if(my_abs(error) <= 2)
	return 0;
error_now_pos = error;
error_sum_pos = error_sum_pos + error_now_pos;
output = kp*error_now_pos + ki*error_sum_pos + kd*(error_now_pos - error_last_pos);
error_last_pos = error_now_pos;

if (output > 1000)
{  
	output = 1000;
}
if (output < -1000)
{
	output = -1000;
}
    return output;
}

float Y_PID(float kp,float ki,float kd)
{
static float error_now_pos=0;
static float error_last_pos=0;
static float error_sum_pos=0;
static float error;
float output = 0;
	
if(Y_point == 255)
	return 0;
	
error = Y_point-125;
if(my_abs(error) <= 2)
	return 0;
error_now_pos = error;
error_sum_pos = error_sum_pos + error_now_pos;
output = kp*error_now_pos + ki*error_sum_pos + kd*(error_now_pos - error_last_pos);
error_last_pos = error_now_pos;

	
if (output > 1000)
{  
	output = 1000;
}
if (output < -1000)
{
	output = -1000;
}
    return output;
}