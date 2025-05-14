#include "system.h"

uint16_t cnt = 0, num = 0;
volatile uint8_t  key_val = 0,  mode = 0;
volatile uint8_t  Aim_Load = 0;
uint8_t  Magnet = 0;
uint8_t  Last_Magnet = 0;
uint8_t  Magnet_flag = 0;

extern uint8_t X_point,Y_point;

void BTN_Process();



int main(void)
{
	SYSCFG_DL_init();
	NVIC_EnableIRQ(TIMG_0_INST_INT_IRQN);//启动定时器中断
	DL_TimerG_startCounter(TIMG_0_INST);//启动定时器计数
	NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);//清除中断
	NVIC_EnableIRQ(UART_0_INST_INT_IRQN);//使能串口中断
//	DL_TimerG_startCounter(PWM_0_INST);
//	DL_TimerG_startCounter(PWM_1_INST);
	DL_TimerG_stopCounter(PWM_0_INST);
	DL_TimerG_stopCounter(PWM_1_INST);
	OLED_Init();
	OLED_Clear();
	OLED_ShowString(2,1,"AIM:");OLED_ShowString(2,8,"mode:");

	
	DL_TimerG_startCounter(PWM_0_INST);
	DL_TimerG_startCounter(PWM_1_INST);
//	motor_run(300,300);
//	delay_ms(1000);
//	DL_TimerG_stopCounter(PWM_0_INST);
//	DL_TimerG_stopCounter(PWM_1_INST);
	
//	set_relax(300);
//	DL_TimerG_startCounter(PWM_2_INST);

		set_relax(-400);
		DL_TimerG_startCounter(PWM_2_INST);
		delay_ms(900);
		DL_TimerG_stopCounter(PWM_2_INST);

	while(1)
	{
		
		if(Magnet == 0)
			DL_GPIO_setPins(Relay_PORT,Relay_relay_PIN);
		else
		{
			DL_GPIO_clearPins(Relay_PORT,Relay_relay_PIN);
		}
		if(Last_Magnet == 0 && Magnet == 1)
		{
			set_relax(800);
			DL_TimerG_startCounter(PWM_2_INST);
			delay_ms(900);
			set_relax(-800);
			delay_ms(900);
			DL_TimerG_stopCounter(PWM_2_INST);
		}
		

//		if(Magnet_flag == 0)
//		{
//			if(Magnet == 1)
//				set_relax(50);
//		}
		OLED_ShowNum(3,1,X_point,3);OLED_ShowNum(3,8,Y_point,3);
		if(cnt>100)	//0.5s刷新oled内容
		{
			Aim_Load = Aim_Load ;  mode = mode; 
//			OLED_ShowNum(1,1,num,3);
//			OLED_ShowNum(1,14,mode,1);
			OLED_ShowNum(1,1,Magnet,1);
			OLED_ShowNum(2,5,Aim_Load+1,1);OLED_ShowNum(2,13,mode+1,1);
			OLED_ShowNum(3,1,X_point,3);OLED_ShowNum(3,8,Y_point,3);OLED_ShowNum(3,12,Magnet,3);
			OLED_ShowNum(4,1,X_PID(10,0,0),3);OLED_ShowNum(4,8,Y_PID(10,0,0),3);
			cnt = 0;
		}
	}
}

void TIMG_0_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMG_0_INST)) {
        case DL_TIMERG_IIDX_LOAD:
				{
					cnt++;
					BTN_Process();
					motor_run(X_PID(90,0,0), Y_PID(90,0,0));
				}
				break;
        default:
            break;
    }
}

void UART_send_char(UART_Regs * uart, uint8_t dat)
{
    while( DL_UART_isBusy(uart) == true );
    //发送单个字符
    DL_UART_Main_transmitData(uart, dat);
}

void UART_0_INST_IRQHandler(void)
{
	switch (DL_UART_Main_getPendingInterrupt(UART0)) 
	{
		case DL_UART_MAIN_IIDX_RX:
		{
			uint8_t rx_data=DL_UART_Main_receiveData(UART_0_INST);
			Openmv_Receive_Data(rx_data);
			num = rx_data;
		}
			break;
		default:
			break;
	}
}

void BTN_Process()
{
	key_val = Read_BTN_State();
	switch(key_val)
	{
		case 1:	Aim_Load = (Aim_Load+1) % 9;	break;
		case 2:	Aim_Load = Aim_Load ;  mode = mode; UART_send_char(UART_0_INST ,'A');	UART_send_char(UART_0_INST ,'B'); UART_send_char(UART_0_INST ,Aim_Load);UART_send_char(UART_0_INST ,mode);break;
		case 3:	mode = (mode+1) % 4;  break;
	}
}