#include <step_motor.h>

extern DL_TimerG_PWMConfig gPWM_0Config;
extern DL_TimerG_PWMConfig gPWM_1Config;
extern DL_TimerG_PWMConfig gPWM_2Config;

void motor_run(int x ,int y)
{//x+右,-左,y+上,y-下
	if(x > 0)
		DL_GPIO_setPins(DIR_PORT,DIR_x_axle_PIN);
	else
	{
		x = x*-1;
		DL_GPIO_clearPins(DIR_PORT,DIR_x_axle_PIN);
	}
	if(y > 0)
	{
		DL_GPIO_setPins(DIR_PORT,DIR_y_axle_PIN);
		DL_GPIO_setPins(DIR_PORT,DIR_y_axle1_PIN);
	}
	else
	{
		y = y*-1;
		DL_GPIO_clearPins(DIR_PORT,DIR_y_axle_PIN);
		DL_GPIO_clearPins(DIR_PORT,DIR_y_axle1_PIN);
	}
	
	if(x<5)//频率过小视为0hz
	{
		DL_TimerG_stopCounter(PWM_0_INST);
	}
	else
	{
		DL_TimerG_startCounter(PWM_0_INST);
		gPWM_0Config.period = (PWM_0_INST_CLK_FREQ/x);
		SYSCFG_DL_PWM_0_init();
		DL_TimerG_setCaptureCompareValue(PWM_0_INST,gPWM_0Config.period/2,GPIO_PWM_0_C0_IDX);
		DL_TimerG_startCounter(PWM_0_INST);
	}
	if(y<5)
	{
		DL_TimerG_stopCounter(PWM_1_INST);
	}
	else
	{
		//	DL_TimerG_startCounter(PWM_1_INST);
		gPWM_1Config.period = (PWM_1_INST_CLK_FREQ/y);
		SYSCFG_DL_PWM_1_init();
		DL_TimerG_setCaptureCompareValue(PWM_1_INST,gPWM_1Config.period/2,GPIO_PWM_1_C0_IDX);
		DL_TimerG_setCaptureCompareValue(PWM_1_INST,gPWM_1Config.period/2,GPIO_PWM_1_C1_IDX);
		DL_TimerG_startCounter(PWM_1_INST);
	}
	
}

void set_relax(int x)
{
	if (x == 0)
		DL_TimerG_stopCounter(PWM_2_INST);
	else
	{
		if(x < 0)
		{
			x = -x;
			DL_GPIO_clearPins(DIR_PORT,DIR_z_axle_PIN);
		}
		else
		{
			DL_GPIO_setPins(DIR_PORT,DIR_z_axle_PIN);
		}
		gPWM_2Config.period = (PWM_2_INST_CLK_FREQ/x);
		SYSCFG_DL_PWM_2_init();
		DL_TimerG_setCaptureCompareValue(PWM_2_INST,gPWM_2Config.period/2,GPIO_PWM_2_C0_IDX);
		DL_TimerG_startCounter(PWM_2_INST);
	}
}