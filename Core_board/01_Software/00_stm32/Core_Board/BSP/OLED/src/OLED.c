#include "OLED.h"
#include "oledfont.h"

#define OLED_SDA_Pin GPIO_PIN_10
#define OLED_SDA_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_11
#define OLED_SCL_GPIO_Port GPIOB

#define OLED_W_SCL(x)		HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, (GPIO_PinState)(x))
#define OLED_W_SDA(x)		HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, (GPIO_PinState)(x))

void OLED_I2C_Init( void )
{

	//	GPIO_InitTypeDef GPIO_INITSTRUCTURE;
	//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//	
	//	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_OUT;
	//	GPIO_INITSTRUCTURE.GPIO_OType=GPIO_OType_PP;
	//	GPIO_INITSTRUCTURE.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	//	
	//	GPIO_INITSTRUCTURE.GPIO_PuPd=GPIO_PuPd_UP;
	//	GPIO_INITSTRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;
	//	GPIO_Init(GPIOB,&GPIO_INITSTRUCTURE);

	OLED_W_SCL( 1 );
	OLED_W_SDA( 1 );
}

void OLED_I2C_Start( void )
{
	OLED_W_SDA( 1 );
	vDelay_us( 1 );
	OLED_W_SCL( 1 );
	vDelay_us( 1 );
	OLED_W_SDA( 0 );
	vDelay_us( 1 );
	OLED_W_SCL( 0 );
	vDelay_us( 1 );
}

void OLED_I2C_Stop( void )
{
	OLED_W_SDA( 0 );
	vDelay_us( 1 );
	OLED_W_SCL( 1 );
	vDelay_us( 1 );
	OLED_W_SDA( 1 );
	vDelay_us( 1 );
}

void OLED_I2C_SendByte( uint8_t Byte )
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA( Byte & ( 0x80 >> i ) );
		vDelay_us( 1 );
		OLED_W_SCL( 1 );
		vDelay_us( 1 );
		OLED_W_SCL( 0 );
		vDelay_us( 1 );
	}
	OLED_W_SCL( 1 );
	vDelay_us( 1 );
	OLED_W_SCL( 0 );
	vDelay_us( 1 );
}

void OLED_WriteCommand( uint8_t Command )
{
	OLED_I2C_Start();
	OLED_I2C_SendByte( 0x78 );
	OLED_I2C_SendByte( 0x00 );
	OLED_I2C_SendByte( Command );
	OLED_I2C_Stop();
}


void OLED_WriteData( uint8_t Data )
{
	OLED_I2C_Start();
	OLED_I2C_SendByte( 0x78 );
	OLED_I2C_SendByte( 0x40 );
	OLED_I2C_SendByte( Data );
	OLED_I2C_Stop();
}

void OLED_SetCursor( uint8_t Y, uint8_t X )
{
	OLED_WriteCommand( 0xB0 | Y );
	OLED_WriteCommand( 0x10 | ( ( X & 0xF0 ) >> 4 ) );
	OLED_WriteCommand( 0x00 | ( X & 0x0F ) );
}
void OLED_Set_Pos( uint8_t x, uint8_t y )
{
	OLED_WriteCommand( 0xb0+y );
	OLED_WriteCommand( ( ( x&0xf0 )>>4 )|0x10 );
	OLED_WriteCommand( ( x&0x0f )|0x01 );
}

void OLED_Clear( void )
{
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor( j, 0 );
		for (i = 0; i < 128; i++)
		{
			OLED_WriteData( 0x00 );
		}
	}
}

void OLED_ShowChar( uint8_t Line, uint8_t Column, char Char )
{
	uint8_t i;
	OLED_SetCursor( ( Line - 1 ) * 2, ( Column - 1 ) * 8 );
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData( OLED_F8x16[Char - ' '][i] );
	}
	OLED_SetCursor( ( Line - 1 ) * 2 + 1, ( Column - 1 ) * 8 );
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData( OLED_F8x16[Char - ' '][i + 8] );
	}
}

void OLED_ShowString( uint8_t Line, uint8_t Column, char *String )
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar( Line, Column + i, String[i] );
	}
}

uint32_t OLED_Pow( uint32_t X, uint32_t Y )
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

void OLED_ShowNum( uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length )
{
	uint8_t i;
	for (i = 0; i < Length; i++)
	{
		OLED_ShowChar( Line, Column + i, Number / OLED_Pow( 10, Length - i - 1 ) % 10 + '0' );
	}
}

void OLED_ShowSignedNum( uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length )
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar( Line, Column, '+' );
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar( Line, Column, '-' );
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)
	{
		OLED_ShowChar( Line, Column + i + 1, Number1 / OLED_Pow( 10, Length - i - 1 ) % 10 + '0' );
	}
}

void OLED_ShowHexNum( uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length )
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)
	{
		SingleNumber = Number / OLED_Pow( 16, Length - i - 1 ) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar( Line, Column + i, SingleNumber + '0' );
		}
		else
		{
			OLED_ShowChar( Line, Column + i, SingleNumber - 10 + 'A' );
		}
	}
}

void OLED_ShowBinNum( uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length )
{
	uint8_t i;
	for (i = 0; i < Length; i++)
	{
		OLED_ShowChar( Line, Column + i, Number / OLED_Pow( 2, Length - i - 1 ) % 2 + '0' );
	}
}

void OLED_Init( void )
{



	OLED_I2C_Init();

	OLED_WriteCommand( 0xAE );

	OLED_WriteCommand( 0xD5 );
	OLED_WriteCommand( 0x80 );

	OLED_WriteCommand( 0xA8 );
	OLED_WriteCommand( 0x3F );

	OLED_WriteCommand( 0xD3 );
	OLED_WriteCommand( 0x00 );

	OLED_WriteCommand( 0x40 );

	OLED_WriteCommand( 0xA1 );

	OLED_WriteCommand( 0xC8 );

	OLED_WriteCommand( 0xDA );
	OLED_WriteCommand( 0x12 );

	OLED_WriteCommand( 0x81 );
	OLED_WriteCommand( 0xCF );

	OLED_WriteCommand( 0xD9 );
	OLED_WriteCommand( 0xF1 );

	OLED_WriteCommand( 0xDB );
	OLED_WriteCommand( 0x30 );

	OLED_WriteCommand( 0xA4 );

	OLED_WriteCommand( 0xA6 );

	OLED_WriteCommand( 0x8D );
	OLED_WriteCommand( 0x14 );

	OLED_WriteCommand( 0xAF );

	OLED_Clear();
}


