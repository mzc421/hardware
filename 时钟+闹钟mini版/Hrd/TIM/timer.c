#include "timer.h"
#include "delay.h"
#include "stm32f10x.h"
#include "smg.h"

extern u8 sec,min,hour;
extern u16 count;

//定时1s 4999 7199
void Timer3_Init(u16 arr,u16 pse)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_Period =  arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler =  pse;
	TIM_TimeBaseInitStruct.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =2;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM3,ENABLE);
}

void Timer2_Init(u16 arr,u16 pse)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_Period =  arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler =  pse;
	TIM_TimeBaseInitStruct.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =2;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM2,DISABLE);
}

//时钟定时
void TIM3_IRQHandler()
{	
	sec++;
	TimeZJ();
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

//闹钟定时
void TIM2_IRQHandler()
{	
	count++;
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

void TimeZJ()
{
	if(sec >= 60)
	{
		sec=0;
		min++;
		if(min >= 60) 
		{
			min=0;
			hour++;
			if(hour >= 24) 
			{
				hour = 0;
			}
		}	
	}
}

int CountTime(u8 h,u8 m)
{
 if(h > hour)
	 return (h - hour - 1) * 3600 + (60 - min + m) * 60;
 else if(h < hour)
	 return (23 - hour + h) * 3600 + (60 - min + m) * 60;
 else
	 if(m >= min)
		 return (m - min) * 60;
	 else
		 return (23 - hour + h) * 3600 + (60 - min + m) * 60;
}