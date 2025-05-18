/******************************************************************************
 * Copyright (C) 2025 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_uart.h
 *
 * @par dependencies
 * - stdio.h
 * - stdint.h
 * - stdlib.h
 *
 * - queue.h
 * - semphr.h
 *
 * - usart.h
 *
 * @author jiejielin
 *
 * @brief Provide the HAL APIs of UART opetions.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2025-2-25
 *
 * @note 1 tab == 4 spaces!
 *
 *****************************************************************************/

#ifndef __BSP_UART_H__
#define __BSP_UART_H__

 //******************************** Includes *********************************//
#include <stdio.h>
#include <stdint.h>
#include "cmsis_os.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "usart.h"
//******************************** Includes *********************************//

//******************************** Defines **********************************//
#define DEBUG
//#define AB_BUFFER
//#define CIRCULAR_BUFFER
#define IDLE_DMA

#define UART_BUFF_SIZE 1

#define BUFFER_FULL 0xff
#define BUFFER_EMPTY 0x0f

//*********************Thread_Func **********************//
extern osThreadId_t Uart_produce_TaskHandle;
extern const osThreadAttr_t Uart_produce_Task_attributes;

extern osThreadId_t Uart_consume_TaskHandle;
extern const osThreadAttr_t Uart_consume_Task_attributes;

typedef enum
{
    FRAME_HEAD_STATE = 0,
    FRAME_WAIT_STATE = 1
}frame_state_t;

typedef enum
{
    FRAME_HEAD = 0xff,
    FRAME_TAIL = 0xef
} uart_frame_t;

//*********************Thread_Func **********************//

//*********************Queue_Handler ********************//

//*********************Queue_Handler ********************//

//******************************** Defines **********************************//

//******************************** Declaring ********************************//
/**
 * @brief Uart_produce_func for thread to run.
 *
 * base AB_BUFFER:
 *  init:
 *      1. create the mutex
 *      2. start the uart_IT
 *  loop:
 *      1. wait the notify from uart isr
 *      2. check the buffer is full or not
 *      3. if full, notify the consume task to read the buffer and wait the buffer empty
 *      4. start the uart_IT base g_buffer_point
 * base CIRCULAR_BUFFER:
 *  init:
 *      1. create the mutex
 *      2. create the circular buffer
 *      3. start the uart_IT
 *  loop:
 *      1. wait the notify from uart isr
 *      2. noitfy the consume task to read the buffer
 *      3. wake the consume task
 *
 * All of these are based on mutex locks
 *
 * @param[in] void
 *
 * @return void
 *
 * */
void Uart_produce_func( void *argument );

/**
 * @brief Uart_produce_func for thread to run.
 *
 * base AB_BUFFER:
 *  init:
 *      1. Declaring rece_g_buffer_point
 *  loop:
 *      1. wait the notify from the produce task
 *      2. take the mutex block
 *      3. read the ABbuffer base rece_g_buffer_point
 *      4. notify the produce task
 *      5. give the mutex
 * base CIRCULAR_BUFFER and IDLE_DMA:
 *  init:
 *      1. Declaring dat, g_crc_sum, g_data_buff, g_data_count, g_frame_state
 *  loop:
 *      1. wait the notify from the produce task
 *      2. take the mutex block
 *      3. read the buffer until the buffer is empty
 *      4. solve the custom uart protocols
 *      5. notify the produce task(CIRCULAR_BUFFER)
 *      6. give the mutex
 * @param[in] void
 *
 * @return void
 *
 * */
void Uart_consume_func( void *argument );
//******************************** Declaring ********************************//

//******************************** priority *********************************//
/**
 * task priority:
 * high -----------------------------------------------------------------> low
 * Uart_produce_func -> Uart_consume_func
 *            
 **/

#endif // End of __BSP_UART_H__
