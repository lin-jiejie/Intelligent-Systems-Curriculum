/******************************************************************************
 * Copyright (C) 2025 jiejielin, inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_key.h
 *
 * @par dependencies
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

#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

//******************************** Includes *********************************//

#include <stdint.h> //  the compiling lib.
#include <stdio.h>

#include "main.h" //  Core / OS layer
#include "cmsis_os.h"

#include "queue.h" //  specific file for operations
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

//******************************** Includes *********************************//

//******************************** Defines **********************************//

#define 	DEBUG

#define 	USER_KEY_Pin GPIO_PIN_10
#define 	USER_KEY_GPIO_Port GPIOB

//*********************Thread_Func **********************//
extern osThreadId_t Key_Read_Handle;
extern const osThreadAttr_t Task_Key_Read_attributes;

//*********************Thread_Func **********************//

//*********************Queue_Handler ********************//
extern QueueHandle_t key_queue;


//*********************Queue_Handler ********************//

typedef enum
{
    KEY_OK = 0,               /* Operation completed successfully.  */
    KEY_ERROR = 1,            /* Run-time error without case matched*/
    KEY_ERROR_TIMEOUT = 2,    /* Operation failed with timeout      */
    KEY_ERROR_RESOURCE = 3,   /* Resource not available.            */
    KEY_ERROR_PARAMETER = 4,  /* Parameter error.                   */
    KEY_ERROR_NOMEMORY = 5,   /* Out of memory.                     */
    KEY_ERROR_ISR = 6,        /* Not allowed in ISR context         */
    KEY_RESERVED = 0x7FFFFFFF /* Reserved                           */
} key_status_t;

typedef enum
{
    KEY_NO    = 0,       	  /* Key not pressed */
    KEY_DOWN  = 1,       	  /* Key pressed     */
    KEY_UP    = 2,       	  /* key pressed up  */
} key_press_status_t;

typedef enum
{
    KEY_NOT_PRESS   = 0,      /* Key pressed       */
    KEY_PRESS       = 1,      /* Key not pressed   */
    KEY_SHORT_PRESS = 2,      /* key short pressed */
    KEY_LONG_PRESS  = 3,      /* key long pressed  */
}key_value_t;

typedef enum
{
    KEY_ENTER = 0,            /* Key pressed       */
    KEY_NOT_ENTER = 1,        /* Key not pressed   */
}key_scan_status_t;
//******************************** Defines **********************************//

//******************************** Declaring ********************************//

/**
 * @brief key_task_func for thread to run.
 * 
 * Steps:
 *  1.doing the specific operations
 *  
 * @param[in] argument      : the thread argument.
 * 
 * @return  : void
 * 
 * */
void         key_task_func                 (void *argument);


//******************************** Declaring ********************************//

#endif // End of __BSP_KEY_H__
