#include "stm32f10x.h"
#include "Key.h"
#include "Led.h"
#include "delay.h"
#include "smg.h"
#include "timer.h"
#include "delay.h"
#include "beep.h"

u8 sec,min,hour;//时钟
u8 h,m,s;//闹钟
u8 setflag;//时间设置
u8 a = 0;
u16 count;
u8 countflag;//闹钟设置
u8 flag;//响铃

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Timer3_Init(9999,7199);
	Timer2_Init(9999,7199);
	LED_Init();
	Key_Init();
	delay_init();
	Smg_Init();
	Beep_Init();
	
	hour = 0;
	min = 0;
	sec = 0;
	
	h = 0;
	m = 0;
	s = 0;
	
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
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);//亮 LED0
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
			{
				//退出
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
				{
					while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)));
					setflag = 0;
					TIM_Cmd(TIM3,ENABLE);
					GPIO_SetBits(GPIOB,GPIO_Pin_5);//灭
					break;
				}
			}
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)//KEY1 hour
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)));
					hour++;
					if(hour == 24)
						hour = 0;
				}
			}
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)//KEY0 min
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)));
					min++;
					if(min == 60)
						min = 0;
				}
			}
			Smg_DisPlay();
			Display_Data(hour,min,sec);
		}
	
		//闹钟设置
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)//KEY0
		{
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
			{
				while(!(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)));
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
					TIM_Cmd(TIM2,ENABLE);//开启定时器
					count = 0;
				}
			}
		}
		while(countflag)
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_5);//亮 LED1
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)//KEY0
		  {
				//退出设置闹钟模式 进入时钟运行模式
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)));
					countflag = 0;
					flag = 1;
					TIM_Cmd(TIM2,ENABLE);//开启定时器2定时
					GPIO_SetBits(GPIOE,GPIO_Pin_5);//灭
					break;
				}
		  }
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)//KEY1 h
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)));
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
		
		//响铃
		while(flag)
		{
			Smg_DisPlay();
			Display_Data(hour,min,sec);
			if(count == CountTime(h,m))
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_8);//开
				GPIO_ResetBits(GPIOB,GPIO_Pin_5);//LED0 亮
				delay_ms(100);
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)//KEY1
				{
					delay_ms(10);
					if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
					{
						while(!(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)));
						flag = 0;
						GPIO_ResetBits(GPIOB,GPIO_Pin_8);//关
						GPIO_SetBits(GPIOB,GPIO_Pin_5);//灭 LED0
						TIM_Cmd(TIM2,DISABLE);
						break;
					}
				}
			}
		}
	}
	return 0;
}