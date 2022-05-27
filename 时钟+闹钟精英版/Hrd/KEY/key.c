#include "stm32f10x.h"
#include "key.h"

extern u8 sec,min,hour;
								    
void Key_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}
int min_Set()
{
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)//KEY1
		min++;
	else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)//KEY0
		min--;
	if(min >= 60)
		min = 0;
	if(min <= 0)
		min = 59;
	
	return min;
}

int hour_Set()
{
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
		hour++;
	else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
		hour--;
	if(hour >= 24)
		hour = 0;
	if(hour <= 0)
		hour = 23;
	
	return hour;
}
