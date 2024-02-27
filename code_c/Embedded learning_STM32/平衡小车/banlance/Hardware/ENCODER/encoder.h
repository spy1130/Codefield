#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "sys.h"
void Encoder_TIM2_Init(void);
void Encoder_TIM3_Init(void);
int Read_Speed(u8 TIMX);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);

#endif



