#include "hc_sr04.h"
#include "stm32f10x.h"
#include "Timer.h"
#include "delay.h"

u8 overcount=0; //记录定时器溢出次数

void HC_SR04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//PA6为输出端Trig   PA7为输入端Echo
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//设置为推挽输出
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;//设置为浮空输入
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

//测距
float Senor_Using(void)
{
	float length = 0,sum = 0;
	u16 tim;
	unsigned int i = 0;
	//测五次数据计算一次平均值
	while(i != 5)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_6); //拉高信号，作为触发信号
		delay_us(20); //高电平信号超过10us
		GPIO_ResetBits(GPIOA,GPIO_Pin_6); //等待回响信号
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7) == RESET);
		//回响信号到来，开启定时器计数
		TIM_Cmd(TIM2,ENABLE);
		i += 1; //每收到一次回响信号+1,收到5次就计算均值
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7) == SET);//回响信号消失
		TIM_Cmd(TIM2,DISABLE);//关闭定时器
		tim = TIM_GetCounter(TIM2); //获取计TIM3数寄存器中的计数值
		length = (tim+overcount*1000)/58.0; //通过回响信号计算距离
		sum = length+sum;
		TIM2->CNT = 0; //将TIM3计数寄存器的计数值清零
		overcount = 0; //中断溢出次数清零
		delay_ms(100);
	}
	length = sum/5;
	return length; //距离作为函数返回值
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET) //检查是否发生TIM3中断
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update ); //清楚中断更新标志
		overcount++;
	}
}