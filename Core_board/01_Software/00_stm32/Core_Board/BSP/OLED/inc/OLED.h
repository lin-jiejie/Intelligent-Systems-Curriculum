/******************************************************************************
 * Copyright (C) 2024 jiejielin, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file app_sensor.h
 *
 * @par dependencies
 * - stdio.h
 * - stdint.h
 * - stdlib.h
 * - string.h
 *
 * @author jiejielin
 *
 * @brief Provide the freertos task of sensor.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2024-12-25
 *
 * @note 1 tab == 4 spaces!
 *
 *****************************************************************************/


#include "stm32f4xx.h"                  // Device header
#include "Delay.h"


#ifndef __OLED_H
#define __OLED_H

void OLED_Init( void );
void OLED_Clear( void );
void OLED_ShowChar( uint8_t Line, uint8_t Column, char Char );
void OLED_ShowString( uint8_t Line, uint8_t Column, char *String );
void OLED_ShowNum( uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length );
void OLED_ShowSignedNum( uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length );
void OLED_ShowHexNum( uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length );
void OLED_ShowBinNum( uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length );
void OLED_ShowSignedNum( uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length );
void delay_us( uint32_t xus );
void delay_ms( uint32_t xms );

#endif
