/******************************************************************************
 * Copyright (C) 2025 jiejielin, src.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_key.c
 *
 * @par dependencies
 * - bsp_key.h
 * - stdio.h
 * - stdint.h
 *
 * @author jiejielin
 *
 * @brief Provide the HAL APIs of Key and corresponding opetions.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2025-2-08
 *
 * @note 1 tab == 4 spaces!
 *
 *****************************************************************************/

 //******************************** Includes *********************************//

#include "bsp_key.h"

//******************************** Includes *********************************//


//******************************** Defines **********************************//
/* Definitions for Key_Read_Task */
osThreadId_t Key_Read_Handle;
const osThreadAttr_t Task_Key_Read_attributes = {
  .name = "Key_Read_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
QueueHandle_t key_queue;



/**
 * @brief scan the key status.
 * 
 * 
 * @param[in] key_value       : the key value. 
 * 
 * @return KEY_OK             : the key has been pressed
 *         KEY_ERROR_TIMEOUT  : the key has been released
 * */
static key_scan_status_t key_scan(void)
{   
    if(HAL_GPIO_ReadPin(USER_KEY_GPIO_Port, USER_KEY_Pin) == GPIO_PIN_RESET)
    {
        return KEY_ENTER;
    }
    else
    {
        return KEY_NOT_ENTER;//始终没有按键被按下，返回超时
    }
}

/**
 * @brief this function will check the key status between long press and 
 *        shrot press.
 * 
 * Steps:
 *  1.doing the specific operations
 *  
 * @param[in] key_value         : the key value.
 * @param[in] short_press_time  : the time of long press.
 * 
 * @return KEY_SHORT_PRESSED : key has been pressed with short time.
 *         KEY_LONG_PRESSED  : key has been pressed with long time.
 * 
 * */
static key_value_t key_scan_press_status(uint32_t       long_press_time)
{
    static   key_press_status_t  key_state      = KEY_NO;
    static   uint32_t            key_press_time = 0;
    volatile key_value_t         key_return     = KEY_NOT_PRESS;
    volatile key_scan_status_t   key_io         = key_scan();
    
    switch(key_state)
    {
        case KEY_NO:
             if(KEY_ENTER == key_io)
            {
                key_press_time++;
                key_state = KEY_DOWN;
            }
            break;
        case KEY_DOWN:
            if(KEY_ENTER == key_io)
            {
                key_press_time++;
                key_state = KEY_UP;
            }
            else
            {
                key_state = KEY_NO;
            }
            break;
			case KEY_UP:
#ifdef DEBUG
				printf("now key_press_time = %d\r\n",key_press_time);
#endif //DEBUG
				if(KEY_ENTER == key_io)
				{
					key_press_time++;
				}
				else
				{	
					if(key_press_time <= long_press_time)
					{
						key_return = KEY_SHORT_PRESS;
					}
					else
					{
						key_press_time = 0;
						key_return = KEY_LONG_PRESS;
						key_state      = KEY_NO;
					}
					key_press_time = 0;
					key_state      = KEY_NO;
				}
				break;   

    }

    return key_return;
}

void key_read_func(void *argument)
{
    key_value_t key_value = KEY_NOT_PRESS;
    key_queue = xQueueCreate(10, sizeof(key_value_t));
    if(NULL == key_queue)
    {
#ifdef DEBUG
        printf("key queue create failed\r\n");
#endif //DEBUG
    }
    for(;;)
    {
//		printf("now on the key task\n");
        key_value = key_scan_press_status(100);
        if(KEY_SHORT_PRESS == key_value)
        {
#ifdef DEBUG
            printf("key short press\n");
#endif //DEBUG
            if( pdPASS != xQueueSend( key_queue, &key_value, 100 ) )
            {
                // 队列已满，等待重试
#ifdef DEBUG
                printf("key_queue full now\n");
#endif //DEBUG
            }
        }
        else if(KEY_LONG_PRESS == key_value)
        {
#ifdef DEBUG
            printf("key long press\n");
#endif //DEBUG
            if( pdPASS != xQueueSend( key_queue, &key_value, 100 ) )
            {
                // 队列已满，等待重试
#ifdef DEBUG
                printf("key_queue full now\n");
#endif //DEBUG
            }
        }
        osDelay(10);
    }
}

//******************************** Defines **********************************//

//******************************** Declaring ********************************//


//******************************** Declaring ********************************//
