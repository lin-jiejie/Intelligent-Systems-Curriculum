/******************************************************************************
 * Copyright (C) 2025 jiejielin, inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_step_motor.h
 *
 * @par dependencies
 * - 
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
 
#ifndef __BSP_STEP_MOTOR_H__
#define __BSP_STEP_MOTOR_H__



//******************************** Includes *********************************//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "main.h"

//******************************** Includes *********************************//


//******************************** Defines **********************************//

#define Y_STEP_Pin GPIO_PIN_6
#define Y_STEP_GPIO_Port GPIOB
#define Y_DIR_Pin GPIO_PIN_7
#define Y_DIR_GPIO_Port GPIOB

#define X_STEP_Pin GPIO_PIN_0
#define X_STEP_GPIO_Port GPIOA
#define X_DIR_Pin GPIO_PIN_1
#define X_DIR_GPIO_Port GPIOA

//#define DEBUG
#define CLK_FRE		(10000UL)

typedef struct{
	int32_t           motor_speed;
	TIM_HandleTypeDef*      p_tim;
	uint32_t              Channel;
	GPIO_TypeDef*     p_Dir_GPIOx;
	uint16_t         Dir_GPIO_Pin;
	uint16_t                  dir;
}step_motor_t;

//******************************** Defines **********************************//


//******************************** Declaring ********************************//

step_motor_t* creat_step_motor(int32_t              speed, 
							   TIM_HandleTypeDef*   p_tim,
							   uint32_t           Channel,
                               GPIO_TypeDef*  p_Dir_GPIOx, 
                               uint16_t      Dir_GPIO_Pin,
							   uint16_t               dir);
void step_motor_run(step_motor_t* motor_instance);
void step_motor_stop(step_motor_t* motor_instance);
void set_step_speed(step_motor_t* motor_instance, int32_t speed);

//******************************** Declaring ********************************//

#endif // End of __BSP_STEP_MOTOR_H__

