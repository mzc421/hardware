#include "stm32f10x.h"
#include "Key.h"
#include "Led.h"
#include "delay.h"
#include "smg.h"
#include "timer.h"
#include "delay.h"
#include "beef.h"

u8 sec,min,hour;//时钟
u8 h,m,s;//闹钟
u8 setflag;//时间设置
u8 a = 0;
u16 count;
u8 countflag;//闹钟设置值
u8 flag;//响铃设置

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Timer3_Init(9999,7199);
	Timer2_Init(9999,7199);
	LED_Init();
	Key_Init();
	delay_init();
	Smg_Init();
	
	hour = 13;
	min = 19;
	sec = 13;
	
	h = 0;
	m = 0;
	s = 0;
	
	GPIO_SetBits(GPIOD,GPIO_Pin_2);//灭 LED1
	
	while(1)
	{		
		//实时时间显示
		Smg_DisPlay();
		Display_Data(hour,min,sec);
		
		
		//时间设置
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)//WK_UP
		{
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
			{
				while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)));
				setflag = 1;
				TIM_Cmd(TIM3,DISABLE);
			}
		}
		while(setflag)
		{
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
			{
				//退出
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
				{
					while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)));
					setflag = 0;
					TIM_Cmd(TIM3,ENABLE);
					break;
				}
			}
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)//KEY1 hour
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)));
					hour++;
					if(hour == 24)
						hour = 0;
				}
			}
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)//KEY0 min
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)));
					min++;
					if(min == 60)
						min = 0;
				}
			}
			Smg_DisPlay();
			Display_Data(hour,min,sec);
		}
	
		//闹钟设置
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)//KEY0
		{
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)
			{
				while(!(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)));
				countflag = 1;//进入设置闹钟模式
				if(a == 0)
				{
					a = 1;
					TIM_Cmd(TIM2,DISABLE);
				}
				else
				{
					a = 0;
					countflag = 0;//退出定时模式
					TIM_Cmd(TIM2,ENABLE);//开启定时器停止定时
					count = 0;
				}
			}
		}
		while(countflag)
		{
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)//KEY0
		  {
				//退出设置闹钟模式 进入时钟运行模式
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)));
					countflag = 0;
					TIM_Cmd(TIM2,ENABLE);//开启定时器2定时
					flag = 1;
					break;
				}
		  }
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)//KEY1 h
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)));
					m++;
					if(m == 60)
						m = 0;
				}
			}
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)//WK_UP m
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
				{
					while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)));
					h++;
					if(h == 24)
						h = 0;
				}
			}
			Smg_DisPlay();
			Display_Data(h,m,s);
		}
		
		while(flag)
		{
			Smg_DisPlay();
			Display_Data(hour,min,sec);
			//响铃
			if(count == CountTime(h,m))
			{
				GPIO_ResetBits(GPIOD,GPIO_Pin_2);//亮
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)//KEY1
				{
					delay_ms(10);
					if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)
					{
						while(!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)));
						GPIO_SetBits(GPIOD,GPIO_Pin_2);//灭 LED1
						TIM_Cmd(TIM2,DISABLE);
						flag = 0;
						break;
					}
				}
			}
		}
		
	}
	return 0;
}