#include <Camera_UART.h>

uint8_t X_point=125,Y_point=125;

extern uint8_t Magnet;
extern uint8_t  Last_Magnet;

void Openmv_Receive_Data(uint8_t Com_Data)
{
	static uint16_t RxCounter;
	static uint8_t RxBuffer[3]={0};
	static uint8_t  Rxstate;
	
	switch(Rxstate)
	{
		case 0:	
			if(Com_Data == 0x0C)
			{
				Rxstate = 1;
			}
			break;
			
		case 1:
			if(Com_Data == 0x0D)
			{
				Rxstate = 2;
//				OLED_ShowNum(1,7,2,3);
			}
			else
				Rxstate = 0;
			break;
			
		case 2:
			RxBuffer[RxCounter++]=Com_Data;
			if(RxCounter+1 >= 4)
			{
					X_point=RxBuffer[0];
					Y_point=RxBuffer[1];
					Last_Magnet = Magnet;
					Magnet=RxBuffer[2];
					
				
				RxCounter = 0;
				Rxstate = 0;
			}
			break;
	}
	
}
