/******************************************************************************
 * Copyright (C) 2025 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file mid_circular_buffer.c
 *
 * @par dependencies
 * - mid_circular_buffer.h
 *
 * @author jiejielin
 *
 * @brief Provide the APIs of circluar buffer.
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
#include "mid_circular_buffer.h"
//******************************** Includes *********************************//

/**
 * @brief Create a circular buffer.
 *
 * Steps:
 *
 * @param[in] void
 *
 * @return circular_buffer_t *
 *
 * */
circular_buffer_t *create_circular_buffer(void)
{
    circular_buffer_t *p_buffer_temp = NULL;
    p_buffer_temp = (circular_buffer_t *)malloc(sizeof(circular_buffer_t));
    if (NULL == p_buffer_temp)
    {
#ifdef DEBUG
        printf("Malloc failed in create_circular_buffer\r\n");
#endif
        return NULL;
    }
    memset(p_buffer_temp, 0, sizeof(circular_buffer_t));
    return p_buffer_temp;
}

/**
 * @brief Check whether the buffer is full
 *
 *
 * @param[in] circular_buffer_t * p_buffer : the point of circluar buffer
 *
 * @return buffer_state_t
 *
 * */
buffer_state_t buffer_is_full(circular_buffer_t *p_buffer)
{
    if (NULL == p_buffer)
    {
        return BUFFER_ERROR_PARAMETER;
    }
    if ((p_buffer->tail + 1) % CIRCULAR_BUFFER_SIZE == p_buffer->head)
    {
        return BUFFER_OK;
    }
    return BUFFER_ERROR;
}

/**
 * @brief Check whether the buffer is empty
 *
 *
 * @param[in] circular_buffer_t * p_buffer : the point of circluar buffer
 *
 * @return buffer_state_t
 *
 * */
buffer_state_t buffer_is_empty(circular_buffer_t *p_buffer)
{
    if (NULL == p_buffer)
    {
        return BUFFER_ERROR_PARAMETER;
    }
    if (p_buffer->head == p_buffer->tail)
    {
        return BUFFER_OK;
    }
    return BUFFER_ERROR_NOMEMORY;
}

/**
 * @brief Push a data into the buffer
 *
 *
 * @param[in] circular_buffer_t * p_buffer : the point of circluar buffer
 * @param[in] data_type_t         data     : the data to be pushed
 *
 * @return buffer_state_t
 *
 * */
buffer_state_t buffer_push(circular_buffer_t *p_buffer, data_type_t data)
{
    if (NULL == p_buffer)
    {
        return BUFFER_ERROR_PARAMETER;
    }
    if (BUFFER_OK != buffer_is_full(p_buffer))
    {
        p_buffer->data[p_buffer->tail] = data;
        p_buffer->tail = (p_buffer->tail + 1) % CIRCULAR_BUFFER_SIZE;
#ifdef DEBUG
        printf("The data [%x] is pushed into the buffer.\r\n",
               p_buffer->data[p_buffer->tail - 1]);
#endif
        return BUFFER_OK;
    }
    return BUFFER_ERROR_NOMEMORY;
}

/**
 * @brief pop a data from the buffer
 *
 *
 * @param[in] circular_buffer_t * p_buffer : the point of circluar buffer
 * @param[in] data_type_t       * data     : the data to be poped
 *
 * @return buffer_state_t
 *
 * */
buffer_state_t buffer_pop(circular_buffer_t *p_buffer, data_type_t *data)
{
    if (NULL == p_buffer)
    {
        return BUFFER_ERROR_PARAMETER;
    }
    if (BUFFER_OK != buffer_is_empty(p_buffer))
    {
        *data = p_buffer->data[p_buffer->head];
        p_buffer->head = (p_buffer->head + 1) % CIRCULAR_BUFFER_SIZE;
#ifdef DEBUG
        printf("The data [%x] is pop a data from the buffer.\r\n", *data);
#endif
        return BUFFER_OK;
    }
    return BUFFER_ERROR;
}

/**
 * @brief Gets the remaining size of the buffer
 *
 *
 * @param[in] circular_buffer_t * p_buffer : the point of circluar buffer
 * @param[in] uint32_t          * size     : the remaining size of the buffer
 *
 * @return buffer_state_t
 *
 * */
buffer_state_t buffer_get_size( circular_buffer_t *p_buffer, uint32_t *size )
{
    if (NULL == p_buffer)
    {
        return BUFFER_ERROR_PARAMETER;
    }
    if(p_buffer->head > p_buffer->tail)
    {
        *size = p_buffer->head - p_buffer->tail - 1;
    }
    else
    {
        *size = CIRCULAR_BUFFER_SIZE - 1 - ( p_buffer->tail - p_buffer->head );
    }
    return BUFFER_OK;
}

/**
 * @brief Moves the head to the specified location
 *
 *
 * @param[in] circular_buffer_t * p_buffer : the point of circluar buffer
 * @param[in] uint32_t            location : the location to be moved
 *
 * @return buffer_state_t
 *
 * */
buffer_state_t buffer_tail_move( circular_buffer_t * p_buffer ,uint32_t location )
{
    if (NULL == p_buffer)
    {
        return BUFFER_ERROR_PARAMETER;
    }
    if(location > CIRCULAR_BUFFER_SIZE)
    {
#ifdef DEBUG
				printf( "move tail faild\r\n" );
#endif // DEBUG
        return BUFFER_ERROR_PARAMETER;
    }
    p_buffer->tail = location;
    return BUFFER_OK;
}







