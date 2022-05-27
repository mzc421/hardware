#include "Led.h"
#include "stm32f10x.h"

void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
}

void LED_Dispaly()
{
	u8 i;
	while(1)
	{
		if(i == 0)//…¡À∏
		{
			if(i == 0)
			{
				i = 1;
				GPIO_ResetBits(GPIOD,GPIO_Pin_2);//LED1 ¡¡
			}
			if(i == 1)
			{
				i = 0;
				GPIO_SetBits(GPIOD,GPIO_Pin_2);//LED1  √
			}
		}
	}
}