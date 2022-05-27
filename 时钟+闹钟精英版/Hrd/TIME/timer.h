#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"

void Timer3_Init(u16 arr,u16 pse);
void Timer2_Init(u16 arr,u16 pse);
void TimeZJ();
int CountTime(u8 h,u8 m);

#endif