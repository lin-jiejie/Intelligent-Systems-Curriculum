/******************************************************************************
 * Copyright (C) 2025 jiejielin, src.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file app_control.h
 *
 * @par dependencies
 * - 
 * - 
 * -
 * @author jiejielin
 *
 * @brief Provide the freertos task of control.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2025-5-18
 *
 * @note 1 tab == 4 spaces!
 *
 *****************************************************************************/
 
#ifndef __APP_CONTORL_H__
#define __APP_CONTORL_H__
 
//******************************** Includes *********************************//

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "cmsis_os.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "oled.h"
#include "bsp_key.h"
//******************************** Includes *********************************//


//******************************** Defines **********************************//

//*********************Thread_Func **********************//
extern osThreadId_t Sensor_DisplayHandle;
extern const osThreadAttr_t Task_Display_attributes;

extern osThreadId_t Key_Read_Handle;
extern const osThreadAttr_t Task_Key_Read_attributes;

extern osThreadId_t Motor_Control_Handle;
extern const osThreadAttr_t Task_Motor_Control_attributes;
//*********************Thread_Func **********************//

//*********************Queue_Handler ********************//
//*********************Queue_Handler ********************//

//******************************** Defines **********************************//


//******************************** Declaring ********************************//

/**
 * @brief key_read_func for thread to run.
 * 
 * Steps:
 *    Read the key value
 *  
 * @param[in] argument      : the thread argument.
 * 
 * @return  : void
 * 
 * */
void key_read_func(void *argument);

/**
 * @brief display_sensor_func for thread to run.
 * 
 * Steps:Display of collected sensor data
 *  1.display collect 
 *  
 * @param[in] argument      : the thread argument.
 * 
 * @return  : void
 * 
 * */
void display_sensor_func(void *argument);

void motor_control_func(void *argument);
//******************************** Declaring ********************************//

#endif // End of__APP_SENSOR_H__
