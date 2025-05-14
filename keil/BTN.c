#include <system.h>

#define KEY_NO		0
#define KEY_DOWN	1
#define KEY_UP		2

#define READ_PIN(GPIO_Port,GPIO_Pin)		(DL_GPIO_readPins(GPIO_Port,GPIO_Pin))

unsigned char Read_BTN_IO(void)
{
	unsigned char key_io = 0;
	
	if(READ_PIN(KEY_PORT, KEY_KEY_1_PIN) == 0)
		key_io = 1;
	if(READ_PIN(KEY_PORT, KEY_KEY_2_PIN) == 0)
		key_io = 2;
	if(READ_PIN(KEY_PORT, KEY_KEY_3_PIN) == 0)
		key_io = 3;
	return key_io;
}

unsigned char Read_BTN_State(void)
{
	unsigned char key_return = 0;
	static unsigned char key_state = 0;
	unsigned char key_io = Read_BTN_IO();
	
	switch(key_state)
	{
		case KEY_NO:	
			if(key_io)
				key_state = KEY_DOWN;
			break;
		case KEY_DOWN:	
			if(key_io)
			{
				key_return = key_io;
				key_state = KEY_UP;
			}
			break;
		case KEY_UP:	
			if(!key_io)
				key_state = KEY_NO;
			break;
	}
	
	return key_return;
}
