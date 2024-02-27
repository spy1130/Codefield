#ifndef _SYSTICKTIMER_H
#define _SYSTICKTIMER_H

#include "stm32f10x.h"


void SysTick_Init(u8 SYSCLK);
void Tick_us(u32 nus);
void Tick_ms(u16 nms);
void delay(u16 y);

#endif
