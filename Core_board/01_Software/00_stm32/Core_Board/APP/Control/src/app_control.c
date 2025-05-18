/******************************************************************************
 * Copyright (C) 2025 jiejielin, src.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file app_control.c
 *
 * @par dependencies
 * - app_control.h
 * - bsp_uart.h
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
#include "app_control.h"
#include "stm32f4xx.h"

#define USER_LED_Pin GPIO_PIN_13
#define USER_LED_GPIO_Port GPIOC

/* Definitions for Key_Read_Task */
osThreadId_t Key_Read_Handle;
const osThreadAttr_t Task_Key_Read_attributes = {
  .name = "Key_Read_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};

/* Definitions for Display_Task */
osThreadId_t Sensor_DisplayHandle;
const osThreadAttr_t Task_Display_attributes = {
  .name = "Display_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Definitions for Motor_Control_Task */
osThreadId_t Motor_Control_Handle;
const osThreadAttr_t Task_Motor_Control_attributes = {
  .name = "Motor_Control_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};


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
void key_read_func(void *argument)
{
	for (;;)
	{
		vTaskDelay(1);
	}
}

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
void display_sensor_func(void *argument)
{
	uint32_t num = 0;
	for (;;)
	{
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
		
		taskENTER_CRITICAL();           /* Entering the critical */
		
		num++;
		OLED_ShowNum(1,1,num,2);
		
		taskEXIT_CRITICAL();            /* Exit the critical     */
		
		vTaskDelay(1000);
	}
}

/**
 * @brief motor_control_func for thread to run.
 * 
 * Steps:
 *  
 *  
 * @param[in] argument      : the thread argument.
 * 
 * @return  : void
 * 
 * */
void motor_control_func(void *argument)
{
	for (;;)
	{
		vTaskDelay(1);
	}
}
