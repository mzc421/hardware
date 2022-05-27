#include "Key.h"
#include "delay.h"
#include "Led.h"
#include "stm32f10x.h"

extern u8 sec,min,hour;

void Key_Init()
{
	GPIO_InitTypeDef GPIO_InitX;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC,ENABLE);
	//用于禁止JTAG，开启SWD，因为PA15占用了JTAG的一个IO，所以要禁止JTAG，从而让PA15用作普通IO输入。
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitX.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitX.GPIO_Pin = GPIO_Pin_15;  //KEY1
	GPIO_InitX.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitX);
	
	GPIO_InitX.GPIO_Pin = GPIO_Pin_5;  //KEY0
	GPIO_Init(GPIOC,&GPIO_InitX);
	
	GPIO_InitX.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitX.GPIO_Pin = GPIO_Pin_0;  //WK_UP
	GPIO_Init(GPIOA,&GPIO_InitX);
}

int min_Set()
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)//KEY1
		min++;
	else if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)//KEY0
		min--;
	if(min >= 60)
		min = 0;
	if(min <= 0)
		min = 59;
	
	return min;
}

int hour_Set()
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)
		hour++;
	else if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)
		hour--;
	if(hour >= 24)
		hour = 0;
	if(hour <= 0)
		hour = 23;
	
	return hour;
}
