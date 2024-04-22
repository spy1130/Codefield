#include "Tim.h"

void TIM_Config(uint16_t psc,uint16_t arr)
{
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);//初始化
    TIM_TimeBaseStructure.TIM_Prescaler = psc;     //预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x00; //输入捕获，多少的输入信号进行采样一次
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;//不尽行重复计数
	TIM_TimeBaseStructure.TIM_Period = arr;//自动重装值
	
	
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);//设置TIME输出触发为更新模式
}
