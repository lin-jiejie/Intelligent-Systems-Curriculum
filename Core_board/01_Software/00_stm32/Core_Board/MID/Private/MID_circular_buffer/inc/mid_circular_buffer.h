/******************************************************************************
 * Copyright (C) 2025 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file mid_circular_buffer.h
 *
 * @par dependencies
 * - stdio.h
 * - stdint.h
 * - stdlib.h
 * - string.h
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

#ifndef __MID_CIRCULAR_BUFFER_H__
#define __MID_CIRCULAR_BUFFER_H__

 //******************************** Includes *********************************//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
//******************************** Includes *********************************//

//******************************** Defines **********************************//
//#define DEBUG

#define CIRCULAR_BUFFER_SIZE 15

typedef uint8_t data_type_t;

typedef enum
{
    BUFFER_OK               =    0,
    BUFFER_ERROR            =    1,
    BUFFER_ERROR_RESOURCE   =    2,
    BUFFER_ERROR_NOMEMORY   =    3,
    BUFFER_ERROR_PARAMETER  =    4,
    BUFFER_ERROR_ISR        =    5,
    BUFFER_ERROR_RESERVED   = 0xFF
} buffer_state_t;

typedef struct
{
    uint32_t head;
    uint32_t tail;
    data_type_t data[CIRCULAR_BUFFER_SIZE];
} circular_buffer_t;

//******************************** Defines **********************************//

//******************************** Declaring ********************************//
circular_buffer_t *create_circular_buffer( void );

buffer_state_t buffer_push( circular_buffer_t *p_buffer, data_type_t data );

buffer_state_t buffer_pop( circular_buffer_t *p_buffer, data_type_t *data );

buffer_state_t buffer_is_full( circular_buffer_t *p_buffer );

buffer_state_t buffer_is_empty( circular_buffer_t *p_buffer );

buffer_state_t buffer_get_size( circular_buffer_t *p_buffer, uint32_t *size );

buffer_state_t buffer_tail_move( circular_buffer_t * p_buffer, uint32_t location );
//******************************** Declaring ********************************//

#endif // End of __MID_CIRCULAR_BUFFER_H__
