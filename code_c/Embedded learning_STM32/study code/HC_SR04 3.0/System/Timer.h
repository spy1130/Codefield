#ifndef __TIMER_H
#define __TIMER_H

#define 	__TIM_CLK_ENABLE()			do{RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);}while(0);

#define 	TIM							TIM2
#define 	TIM_IRQn					TIM2_IRQn
#define 	TIM_CHANNEL					TIM_Channel_2
#define 	TIM_TPx						TIM_TS_TI2FP2
#define 	TIM_SOURCES					TIM_EventSource_CC2

void Timer_Init(void);

extern uint32_t DelayTime;


#endif
