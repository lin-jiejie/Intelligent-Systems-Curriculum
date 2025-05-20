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

extern step_motor_t* g_step_motor_x_inst;
extern step_motor_t* g_step_motor_y_inst;
extern pid_controller_t* g_x_pid_controller;
extern pid_controller_t* g_y_pid_controller;

/* Definitions for Display_Task */
osThreadId_t Sensor_DisplayHandle;
const osThreadAttr_t Task_Display_attributes = {
  .name = "Display_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Definitions for Motor_Control_Task */
osThreadId_t Motor_Control_Handle;
const osThreadAttr_t Task_Motor_Control_attributes = {
  .name = "Motor_Control_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};


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
	for (;;)
	{
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
		
//		taskENTER_CRITICAL();           /* Entering the critical */
//		
//		OLED_ShowString(1,1,"X:");
//		OLED_ShowSignedNum(1,4,target_location.x_axis_location - TARGET_CENTER_X,3);
//		OLED_ShowString(2,1,"Y:");
//		OLED_ShowSignedNum(2,4,target_location.y_axis_location - TARGET_CENTER_Y,3);
//		OLED_ShowNum(4,1,num,2);
//		num++;
//		
//		taskEXIT_CRITICAL();            /* Exit the critical     */
		
		vTaskDelay(100);
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
//		portTASK_USES_FLOATING_POINT();
		
		printf("now on the motor_control_func\n");
		
		__disable_irq();
		taskENTER_CRITICAL();           /* Entering the critical */
		
		if( (target_location.x_axis_location <= 240) && (target_location.y_axis_location <= 240))
		{
			set_step_speed(             g_step_motor_x_inst, 
					       pid_compute(g_x_pid_controller, 
	    (int32_t)target_location.x_axis_location - TARGET_CENTER_X)
			                                              );
			set_step_speed(             g_step_motor_y_inst, 
					       pid_compute(g_y_pid_controller, 
	    (int32_t)target_location.y_axis_location - TARGET_CENTER_Y)
			                                              );
		}
		else
		{
			set_step_speed(g_step_motor_x_inst, 0);
			set_step_speed(g_step_motor_y_inst, 0);
		}
		
		step_motor_run(g_step_motor_x_inst);
		step_motor_run(g_step_motor_y_inst);
		
		taskEXIT_CRITICAL();            /* Exit the critical     */
		__enable_irq();
			
		vTaskDelay(10);
	}
}
