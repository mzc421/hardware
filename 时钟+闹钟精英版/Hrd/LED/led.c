#include "led.h"
#include "stm32f10x.h"

//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//LED0
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(GPIOB, &GPIO_InitStructure);			
	
 GPIO_SetBits(GPIOB,GPIO_Pin_5);//输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//LED1
 GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
 GPIO_SetBits(GPIOE,GPIO_Pin_5);  //输出高 
}
 
