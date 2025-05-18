#include <Delay.h>

void vDelay_us( uint16_t xus )
{
	uint16_t differ = 0xffff-xus-0xff;

	__HAL_TIM_SetCounter( &htim5, differ );

	HAL_TIM_Base_Start( &htim5 );

	while (differ < 0xffff-0xff)
	{
		differ = __HAL_TIM_GetCounter( &htim5 );
	}

	HAL_TIM_Base_Stop( &htim5 );
}

void vDelay_ms( uint32_t xms )
{
	uint32_t num = 0;
	for (; num < xms; num++)
	{
		vDelay_us( 1000 );
	}
}
