#ifndef __SMG_H
#define __SMG_H			

#include "stm32f10x.h"

//#define RCLK PBout(10)//时钟脉冲信号——上升沿有效
//#define SCLK PBout(11)//打入信号————上升沿有效
//#define DIO  PAout(8)//串行数据输入

void Smg_Init(void);
void Smg_DisPlay(void);
void Display_Data(u8 hour,u8 min,u8 sec);// Smg显示
void Smg_OUT(u8 X);// Smg单字节串行移位函数

#endif 