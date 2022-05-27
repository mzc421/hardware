#include "smg.h"
#include "stm32f10x.h"
#include "delay.h"

extern u8 sec,min,hour;
u16 smgduan[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
//u16 smgduan[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
u8 DisPlayData[8];

void Smg_Init(void) //IO初始化 
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTB时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}	

void Smg_DisPlay(void)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		Smg_OUT(DisPlayData[i]);			
		Smg_OUT(0x01<<i);	
		
		//RCLK
		GPIO_SetBits(GPIOB,GPIO_Pin_10);//位透明模式
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);//锁存模式
		
		delay_ms(1);
	}
}

void Display_Data(u8 hour,u8 min,u8 sec)
{
	DisPlayData[0] = smgduan[sec%10];//秒
	DisPlayData[1] = smgduan[sec/10];
	
	DisPlayData[2] = 0xbf;
	
	DisPlayData[3] = smgduan[min%10];//分
	DisPlayData[4] = smgduan[min/10];
	
	DisPlayData[5] = 0xbf;
	
	DisPlayData[6] = smgduan[hour%10];//时
	DisPlayData[7] = smgduan[hour/10];
}

void Smg_OUT(u8 x)
{
	u8 i;
	for(i=8;i>=1;i--)
	{
		if(x&0x80) 
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
		else 
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		x<<=1;
		//SCLK
		GPIO_SetBits(GPIOB,GPIO_Pin_11);//段透明模式
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	}
}

/*
void Showtime()//时间显示
{
	u8 i;
	delay_init();
	for(i=0;i<8;i++)
	{
		GPIOA->ODR=~(1<<i);//选位
		GPIO_SetBits(GPIOA,GPIO_Pin_8);//透明模式
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);//锁存模式
		GPIOA->ODR=DisPlayData[i];
		GPIO_SetBits(GPIOA,GPIO_Pin_9);//透明模式
		GPIO_ResetBits(GPIOA,GPIO_Pin_9);
		delay_ms(1);
	}
}
*/