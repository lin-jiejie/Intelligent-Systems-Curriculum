#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx_hal.h"
#include "tim.h"

void vDelay_us( uint16_t xus );
void vDelay_ms( uint32_t xms );

#define MY_TIM	(htim9)

#endif
