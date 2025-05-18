#include <Delay.h>

void vDelay_us( uint16_t xus )
{
	uint16_t now_time = __HAL_TIM_GetCounter( &MY_TIM );
	
	HAL_TIM_Base_Start( &MY_TIM );
	
	while( (__HAL_TIM_GetCounter(&MY_TIM) - now_time) < xus );
	
	HAL_TIM_Base_Stop( &MY_TIM );
}


void vDelay_ms( uint32_t xms )
{
	uint32_t num = 0;
	for (; num < xms; num++)
	{
		vDelay_us( 1000 );
	}
}
