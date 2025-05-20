/******************************************************************************
 * Copyright (C) 2025 jiejielin, src.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_step_motor.c
 *
 * @par dependencies
 * - bsp_step_motor.h
 *
 * @author jiejielin
 *
 * @brief Provide the HAL APIs of Key and corresponding opetions.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2025-5-19
 *
 * @note 1 tab == 4 spaces!
 *
 *****************************************************************************/
 
#include "bsp_step_motor.h"

/**
 * @brief Calculate the absolute value of the input value.
 * 
 * @param[in] int32_t num :    the input value. 
 * 
 * @return    int32_t     : the absolute value.
 * */
static int32_t abs(int32_t num)
{
	if (num < 0)
		return num*-1;
	else 
		return num;
}

/**
 * @brief Create a step motor object.
 * 
 * @param[in] int32_t              speed : the speed value(hz). 
 * @param[in] TIM_HandleTypeDef*   p_tim : the motor's tim
 * @param[in] uint32_t           Channel : the tim output channel
 * @param[in] GPIO_TypeDef*  p_Dir_GPIOx : the motor direction's Port.
 * @param[in] uint16_t      Dir_GPIO_Pin : the motor direction's Pin
 * @param[in] uint16_t               dir : the motor direction
 * 
 * @return    step_motor_t*              : the point step motor object.
 * */
step_motor_t* creat_step_motor(int32_t              speed, 
							   TIM_HandleTypeDef*   p_tim,
							   uint32_t           Channel,
                               GPIO_TypeDef*  p_Dir_GPIOx, 
                               uint16_t      Dir_GPIO_Pin,
							   step_motor_dir_t       dir)
{
    if (NULL == p_Dir_GPIOx)
	{
#ifdef DEBUG
        printf("Parameter of point error in creat_step_motor\r\n");
#endif
        return NULL;
	}
    if (NULL == p_tim)
	{
#ifdef DEBUG
        printf("Parameter of point error in creat_step_motor\r\n");
#endif
        return NULL;
	}
	
	step_motor_t *p_step_motor_instance = (step_motor_t *)malloc(sizeof(step_motor_t));
    if (NULL == p_step_motor_instance)
    {
#ifdef DEBUG
        printf("Malloc failed in creat_step_motor\r\n");
#endif
        return NULL;
    }
	
	p_step_motor_instance->motor_speed    =        speed;
	p_step_motor_instance->p_tim          =        p_tim;
	p_step_motor_instance->Channel		  =      Channel;
	p_step_motor_instance->p_Dir_GPIOx    =  p_Dir_GPIOx;
	p_step_motor_instance->Dir_GPIO_Pin   = Dir_GPIO_Pin;
	p_step_motor_instance->dir            =          dir;
	
	return p_step_motor_instance;
}

/**
 * @brief control the motor's speed(hz) and direction
 * 
 * @param[in] step_motor_t*              : the point step motor object.
 * 
 * @return    NULL
 * */
void step_motor_run(step_motor_t* motor_inst)
{
    if (NULL == motor_inst)
    {
#ifdef DEBUG
        printf("Parameter of point error in step_motor_run\r\n");
#endif
        return ;
    }
	

	printf("now speed = [%d]\n", motor_inst->motor_speed);

	/******* set motor direction *******/
	if(motor_inst->motor_speed <= 0)
	{
		if(motor_inst->dir == STEP_MOTOR_DIR_RESET)
			HAL_GPIO_WritePin(motor_inst->p_Dir_GPIOx, motor_inst->Dir_GPIO_Pin, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(motor_inst->p_Dir_GPIOx, motor_inst->Dir_GPIO_Pin, GPIO_PIN_RESET);
	}
	else 
	{
		if(motor_inst->dir == STEP_MOTOR_DIR_RESET)
			HAL_GPIO_WritePin(motor_inst->p_Dir_GPIOx, motor_inst->Dir_GPIO_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(motor_inst->p_Dir_GPIOx, motor_inst->Dir_GPIO_Pin, GPIO_PIN_SET);
	}
	
	/******* set motor speed (set tim auto_reload) *******/
	int32_t abs_speed = abs(motor_inst->motor_speed);
	if( abs_speed < 10)
	{
		HAL_TIM_PWM_Stop(motor_inst->p_tim, motor_inst->Channel);
	}
	else
	{	
		HAL_TIM_PWM_Stop(motor_inst->p_tim, motor_inst->Channel);
		__HAL_TIM_SET_AUTORELOAD(                  motor_inst->p_tim,
			                     ( (CLK_FRE / abs_speed) << 1 ) - 1 );
		__HAL_TIM_SET_COMPARE(    motor_inst->p_tim,
		                        motor_inst->Channel,
			                  (CLK_FRE / abs_speed));
		HAL_TIM_PWM_Start(motor_inst->p_tim, motor_inst->Channel);
	}
	
}

/**
 * @brief stop the motor
 * 
 * @param[in] step_motor_t*              : the point step motor object.
 * 
 * @return    NULL
 * */
void step_motor_stop(step_motor_t* motor_instance)
{
    if (NULL == motor_instance)
    {
#ifdef DEBUG
        printf("Parameter of point error in step_motor_stop\r\n");
#endif
        return ;
    }
	
	HAL_TIM_PWM_Stop(motor_instance->p_tim, motor_instance->Channel);
	
}

/**
 * @brief stop the motor
 * 
 * @param[in] step_motor_t*              : the point step motor object.
 * @param[in] int32_t                    : the speed of step motor.
 * 
 * @return    NULL
 * */
void set_step_speed(step_motor_t* motor_instance, int32_t speed)
{
    if (NULL == motor_instance)
    {
#ifdef DEBUG
        printf("Parameter of point error in step_motor_stop\r\n");
#endif
        return ;
    }
	
	motor_instance->motor_speed = speed;
	
}












