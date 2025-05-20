/******************************************************************************
 * Copyright (C) 2025 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_uart.c
 *
 * @par dependencies
 * - bsp_uart.h
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

//******************************** Includes *********************************//
#include "bsp_uart.h"

#ifdef CIRCULAR_BUFFER
#include "mid_circular_buffer.h"
#endif // end of CIRCULAR_BUFFER
#ifdef IDLE_DMA
#include "mid_circular_buffer.h"
#endif // end of IDLE_DMA

//******************************** Includes *********************************//

//*********************Thread_Func **********************//
osThreadId_t Uart_produce_TaskHandle;
const osThreadAttr_t Uart_produce_Task_attributes = {
    .name = "Uart_produce_Task",
    .stack_size = 512 * 4,
    .priority = ( osPriority_t ) osPriorityRealtime,
};

osThreadId_t Uart_consume_TaskHandle;
const osThreadAttr_t Uart_consume_Task_attributes = {
    .name = "Uart_consume_Task",
    .stack_size = 512 * 4,
    .priority = ( osPriority_t ) osPriorityNormal,
};
//*********************Thread_Func **********************//

//*********************Queue_Handler ********************//

//*********************Queue_Handler ********************//

//*********************Mutex_Handler ********************//
static SemaphoreHandle_t g_uart_mutex = NULL;
//*********************Mutex_Handler ********************//

//********************* global var **********************//
#ifdef AB_BUFFER
static uint8_t g_uart_buffer1[UART_BUFF_SIZE] = { 0x00 };
static uint8_t g_uart_buffer2[UART_BUFF_SIZE] = { 0x00 };
uint32_t g_buff_index[2] = { 0 };
static uint32_t g_buffer_point = 1;
#endif // end of AB_BUFFER
#ifdef CIRCULAR_BUFFER
uint8_t g_data_buffer = 0;
static circular_buffer_t *g_circular_buffer_irq_thread = NULL;
#endif // end of CIRCULAR_BUFFER
#ifdef IDLE_DMA
static circular_buffer_t *g_circular_buffer_irq_thread = NULL;
#endif // end of IDLE_DMA

target_location_t target_location = {
	.x_axis_location = 0xff,
	.y_axis_location = 0xff,
};
//********************* global var **********************//

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
void Uart_produce_func( void *argument )
{
#ifdef AB_BUFFER

    uint32_t buffer_state = BUFFER_FULL;
    HAL_UART_Receive_IT( &huart1, g_uart_buffer1, UART_BUFF_SIZE );
    g_uart_mutex = xSemaphoreCreateMutex();

    /************* check the point *************/
    if (NULL == g_uart_mutex)
    {
#ifdef DEBUG
        printf( "g_uart_mutex create failed\r\n" );
#endif // DEBUG
    }

    /************* first start uart ************/
    HAL_UART_Receive_IT( &huart1, g_uart_buffer1, UART_BUFF_SIZE );
    /************* loop ************************/
    for (;;)
    {
        xTaskNotifyWait( 0, 0, NULL, portMAX_DELAY );

        while (g_buff_index[g_buffer_point - 1] >= UART_BUFF_SIZE)
        {
            /************* enter the resume task ***********/
            if (pdTRUE != xTaskNotify( Uart_consume_TaskHandle,
                g_buffer_point,
                eSetValueWithOverwrite ))
            {
#ifdef DEBUG
                printf( "xTaskNotify to read_task failed on adc_collect_func\r\n" );
#endif // DEBUG
            }
            do
            {
                /************* wait the buffer empty ***********/
                xTaskNotifyWait( 0, 0, &buffer_state, portMAX_DELAY );
            } while (BUFFER_EMPTY != buffer_state);
        }
        xSemaphoreTake( g_uart_mutex, portMAX_DELAY );
        xSemaphoreGive( g_uart_mutex );

        if (1 == g_buffer_point)
        {
            HAL_UART_Receive_IT( &huart1, g_uart_buffer1, UART_BUFF_SIZE );
        }
        else if (2 == g_buffer_point)
        {
            HAL_UART_Receive_IT( &huart1, g_uart_buffer2, UART_BUFF_SIZE );
        }
#ifdef DEBUG
        printf( "The uart uses to start storing in buffer[%d] !\r\n", g_buffer_point );
#endif // DEBUG
    }
#endif // end of AB_BUFFER

#ifdef CIRCULAR_BUFFER
    uint32_t buffer_state = BUFFER_ERROR;
    circular_buffer_t *p_circular_buffer = NULL;
    g_uart_mutex = xSemaphoreCreateMutex();
    p_circular_buffer = create_circular_buffer();
    if (NULL == p_circular_buffer)
    {
#ifdef DEBUG
        printf( "create_circular_buffer failed in Uart_produce_func\r\n" );
#endif // DEBUG
    }
    if (NULL == g_uart_mutex)
    {
#ifdef DEBUG
        printf( "g_uart_mutex create failed in Uart_produce_func\r\n" );
#endif // DEBUG
    }
    g_circular_buffer_irq_thread = p_circular_buffer;
    HAL_UART_Receive_IT( &huart1, &g_data_buffer, 1 );
    /************* loop ************************/

    for (;;)
    {
        xTaskNotifyWait( 0, 0, NULL, portMAX_DELAY );
        do
        {
            buffer_state = buffer_is_full( g_circular_buffer_irq_thread );
            if (BUFFER_OK == buffer_state)
            {
                // notify the consume task read buffer when buffer is full
                if (pdFALSE == xTaskNotifyGive( Uart_consume_TaskHandle ))
                {
#ifdef DEBUG
                    printf( "xTaskNotifyGive failed in Uart_produce_func\r\n" );
#endif // DEBUG
                }
                else
                {
                    xTaskNotifyWait( 0, 0, &buffer_state, portMAX_DELAY );
                }
            }
        } while (BUFFER_OK == buffer_state);
        xSemaphoreTake( g_uart_mutex, portMAX_DELAY );
        xSemaphoreGive( g_uart_mutex );

        HAL_UART_Receive_IT( &huart1, &g_data_buffer, 1 );
    }

#endif // end of CIRCULAR_BUFFER
#ifdef IDLE_DMA
    // creat circular buffer and check the point
    circular_buffer_t * p_circular_buffer = create_circular_buffer();
    g_uart_mutex = xSemaphoreCreateMutex();
    if (NULL == p_circular_buffer)
    {
#ifdef DEBUG
        printf( "create_circular_buffer failed in Uart_produce_func\r\n" );
#endif // DEBUG
    }
    if (NULL == g_uart_mutex)
    {
#ifdef DEBUG
        printf( "g_uart_mutex create failed in Uart_produce_func\r\n" );
#endif // DEBUG
    }
    g_circular_buffer_irq_thread = p_circular_buffer;

    HAL_UARTEx_ReceiveToIdle_DMA( &huart1,
        g_circular_buffer_irq_thread->data,
                      CIRCULAR_BUFFER_SIZE );

    /************* loop ************************/
    for (;;)
    {
	#ifdef DEBUG		
        printf( "now on the Uart_produce_func\r\n" );
#endif // DEBUG	
        /*******Wait the uart irq notify */
        xTaskNotifyWait( 0, 0, NULL, portMAX_DELAY );
        /* When receive notify form isr ,start the consume thread */
        if (pdFALSE == xTaskNotifyGive( Uart_consume_TaskHandle ))
        {
#ifdef DEBUG
            printf( "xTaskNotifyGive failed in Uart_produce_func\r\n" );
#endif // DEBUG
        }
        //				vTaskDelay(2);
        portYIELD();
        xSemaphoreTake( g_uart_mutex, portMAX_DELAY );
        xSemaphoreGive( g_uart_mutex );

    }


#endif // end of IDLE_DMA

}

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
 *      4. solve the custom uart protocols(the stack space of slove == CIRCULAR_BUFFER_SIZE)
 *      5. notify the produce task(CIRCULAR_BUFFER)
 *      6. give the mutex
 * @param[in] void
 *
 * @return void
 *
 * */
void Uart_consume_func( void *argument )
{
#ifdef AB_BUFFER
    uint32_t rece_g_buffer_point = 0;
    BaseType_t notify_status = pdFALSE;

    /************* loop ************************/
    for (;;)
    {
        notify_status = xTaskNotifyWait( 0,
                                        0,
                                        &rece_g_buffer_point,
                                        portMAX_DELAY );

        if (pdTRUE == notify_status)
        {
            /************* take mutex block ************/
            xSemaphoreTake( g_uart_mutex, portMAX_DELAY );
            if (1 == rece_g_buffer_point)
            {
                for (int i = 0; i < UART_BUFF_SIZE; i++)
                {
                    g_buff_index[rece_g_buffer_point - 1]--;
#ifdef DEBUG
                    printf( "The uart data is [%x] in buffer[%d] !\r\n",
                           g_uart_buffer1[i],
                           g_buffer_point );
#endif
                }
            }
            else if (2 == rece_g_buffer_point)
            {
                for (int i = 0; i < UART_BUFF_SIZE; i++)
                {
                    g_buff_index[rece_g_buffer_point - 1]--;
#ifdef DEBUG
                    printf( "The uart data is [%x] in buffer[%d] !\r\n",
                           g_uart_buffer2[i],
                           g_buffer_point );
#endif
                }
            }
            /************* notify the Production task ************/
            xTaskNotify( Uart_produce_TaskHandle,
                        BUFFER_EMPTY,
                        eSetValueWithOverwrite );
            /************* give the mutex ************/
            xSemaphoreGive( g_uart_mutex );
        }
        //        printf("The adc_read_func running\r\n");
    }
#endif // end of AB_BUFFER

#ifndef AB_BUFFER
    uint8_t dat = 0;
    static uint8_t g_crc_sum = 0;
    static uint32_t g_data_buff[CIRCULAR_BUFFER_SIZE] = { 0 };
    static uint32_t g_data_count = 0;
    static uint32_t g_frame_state = FRAME_WAIT_STATE;
    /************* loop ************************/
    for (;;)
    {
#ifdef DEBUG		
        printf( "now on the Uart_consume_func\r\n" );
#endif // DEBUG		
        if (NULL == g_uart_mutex)
        {
            continue;
        }
        if (NULL == g_circular_buffer_irq_thread)
        {
            continue;
        }
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        xSemaphoreTake( g_uart_mutex, portMAX_DELAY );
        /************* MUTEX BEGIN ************************/
        while (BUFFER_OK !=
               buffer_is_empty( g_circular_buffer_irq_thread ))
        {
            buffer_pop( g_circular_buffer_irq_thread, &dat );
#ifdef DEBUG
            printf( "The data is [%x] in Uart_consume_func\r\n", dat );
#endif // DEBUG
            switch (g_frame_state)
            {
            case FRAME_HEAD_STATE:
            if (FRAME_TAIL == dat)
            {
                g_frame_state = FRAME_WAIT_STATE;
                for (uint32_t i = 0; i <= g_data_count - 2; i++)
                {
                    g_crc_sum += g_data_buff[i];	
                }
                if (g_crc_sum != g_data_buff[g_data_count - 1])
                {
#ifdef DEBUG
                    printf( "The crc sum is faild\r\n" );
#endif // DEBUG
                }
				else
				{
					target_location.x_axis_location = g_data_buff[g_data_count - 3];
					target_location.y_axis_location = g_data_buff[g_data_count - 2];
				}
#ifdef DEBUG
                printf( "The crc sum is [%x]\r\n", g_crc_sum );
				printf( "The x_axis_location is [%x]\r\n", target_location.x_axis_location );
				printf( "The y_axis_location is [%x]\r\n", target_location.y_axis_location );
#endif // DEBUG
#ifdef DEBUG
                printf( "enter FRAME_WAIT_STATE\r\n" );
#endif // DEBUG
                g_crc_sum = 0;
                g_data_count = 0;
                break;
            }
            g_data_buff[g_data_count] = dat;
            g_data_count++;
            // READ THE DATA
            break;
            case FRAME_WAIT_STATE:
            if (FRAME_HEAD == dat)
            {
                g_frame_state = FRAME_HEAD_STATE;
#ifdef DEBUG
                printf( "enter FRAME_HEAD_STATE\r\n" );
#endif // DEBUG
            }
            break;
            default:
            break;
            }
        }

        /************* MUTEX END ************************/

#ifdef CIRCULAR_BUFFER
        xTaskNotify( Uart_produce_TaskHandle,
                    BUFFER_OK,
                    eSetValueWithOverwrite );
#endif // end of CIRCULAR_BUFFER
        xSemaphoreGive( g_uart_mutex );

    }
#endif // end of not AB_BUFFER    

}

/**
 * @brief uart_IT callback function.
 *
 * AB_BUFFER:
 *  1. check the uart instance
 *  2. check the Uart_produce_TaskHandle
 *  3. notify the Uart_produce_TaskHandle
 *  4. change the buffer point
 * CIRCULAR_BUFFER:
 *  1. check the uart instance
 *  2. check the Uart_produce_TaskHandle
 *  3. push the data into the buffer
 *  4. notify the Uart_produce_TaskHandle
 * @param[in] UART_HandleTypeDef *huart : uart handle
 *
 * @return void
 *
 * */
void HAL_UART_RxCpltCallback( UART_HandleTypeDef *huart )
{
#ifdef AB_BUFFER
    if (huart->Instance == USART1)
    {
#ifdef DEBUG
        printf( "Now on the HAL_UART_RxCpltCallback.\r\n" );
#endif
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        BaseType_t xResult = pdFAIL;
        if (NULL == Uart_produce_TaskHandle)
        {
#ifdef DEBUG
            printf( "Uart_produce_TaskHandle is NULL.\n" );
#endif
        }
        xResult = xTaskNotifyFromISR( Uart_produce_TaskHandle,
                                     g_buffer_point,
                                     eSetBits,
                                     &xHigherPriorityTaskWoken );
        if (pdPASS == xResult)
        {
#ifdef DEBUG
            printf( "xTaskNotifyFromISR success.\r\n" );
#endif
        }
        g_buff_index[g_buffer_point - 1]++;
        if (1 == g_buffer_point)
        {
            g_buffer_point = 2;
        }
        else
        {
            g_buffer_point = 1;
        }
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
#ifdef DEBUG
        printf( "The HAL_UART_RxCpltCallback is finish.\r\n" );
#endif
    }
#endif // end of AB_BUFFER

#ifdef CIRCULAR_BUFFER
    if (huart->Instance == USART1)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        BaseType_t xResult = pdFALSE;
#ifdef DEBUG
        printf( "Now on the HAL_UART_RxCpltCallback.\r\n" );
#endif // DEBUG

        if (NULL == g_circular_buffer_irq_thread)
        {
#ifdef DEBUG
            printf( "the point of g_circular_buffer_irq_thread is NULL\r\n" );
#endif // DEBUG
        }
        buffer_state_t ret = BUFFER_ERROR;
        ret = buffer_push( g_circular_buffer_irq_thread, g_data_buffer );
        if (BUFFER_OK != ret)
        {
#ifdef DEBUG
            printf( "buffer_push failed in HAL_UART_RxCpltCallback\r\n" );
#endif // DEBUG
        }
        xResult = xTaskNotifyFromISR( Uart_produce_TaskHandle,
                                     1,
                                     eSetBits,
                                     &xHigherPriorityTaskWoken );
        if (pdPASS == xResult)
        {
#ifdef DEBUG
            printf( "xTaskNotifyFromISR success.\r\n" );
#endif // DEBUG
        }
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

#ifdef DEBUG
        printf( "The HAL_UART_RxCpltCallback is finish.\r\n" );
#endif // DEBUG
    }
#endif // end of CIRCULAR_BUFFER
}

#ifdef IDLE_DMA
/**
 * @brief uart_DMA_IDLE callback function.
 *
 * IDLE_DMA:
 *  1. check the uart instance
 *  2. check the Uart_produce_TaskHandle
 *  3. move the tail of the circular buffer
 *  4. notify the Uart_produce_TaskHandle
 *
 * @param[in] UART_HandleTypeDef *huart : uart handle
 *                        uint16_t Size : the tail location
 *
 * @return void
 *
 * This callback func is uses in hal_uart.c 2529(idle), 3110(half), 3075(compele)
 * */
void HAL_UARTEx_RxEventCallback( UART_HandleTypeDef *huart, uint16_t Size )
{
#ifdef DEBUG
    printf( "the HAL_UARTEx_RxEventCallback get [%d] datas.\r\n", Size );
#endif // DEBUG
    if (huart->Instance == USART1)
    {
        buffer_tail_move( g_circular_buffer_irq_thread, Size );
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        BaseType_t xResult = pdFALSE;
        xResult = xTaskNotifyFromISR( Uart_produce_TaskHandle,
                                     1,
                                     eSetValueWithOverwrite,
                                     &xHigherPriorityTaskWoken );
        if (pdPASS == xResult)
        {
#ifdef DEBUG
            printf( "xTaskNotifyFromISR success.\r\n" );
#endif // DEBUG
        }
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}
#endif // end of IDLE_DMA



