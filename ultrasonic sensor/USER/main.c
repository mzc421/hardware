#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "oled.h"
#include "show.h"
#include "led.h"
#include "hc_sr04.h"
#include "Timer.h"

float length;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	LED_Init();
	delay_init();
	Timer2_Init(999,71);//100Hz
	uart_init(9600);//串口1初始化

	OLED_Init();  //OLED初始化

	HC_SR04_Init(); //超声波 PA6为输出端Trig   PA7为输入端Echo
		
  while(1)	
	{
		LED0 =~ LED0;
		length = Senor_Using();
		//串口显示
		printf("测得的距离为：%.3f",length);
		//OLED显示
		oled_show();
		
//		GPIO_ResetBits(GPIOA,GPIO_Pin_8);//给PA8置0	
//		delay_ms(800);
//		GPIO_SetBits(GPIOA,GPIO_Pin_8);//给PA8置1
//		delay_ms(800);
	}
}
