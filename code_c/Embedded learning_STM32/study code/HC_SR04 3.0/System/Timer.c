#include "stm32f10x.h"
#include "Timer.h"
#include "HC_SR04.h"
#include "OLED.H"

TIM_ICInitTypeDef TIM_ICInitStructure;
uint32_t TimeCount = 0;
uint32_t DelayTime = 0;
uint32_t BTim = 0;
uint32_t ATim = 0;

void Timer_Init(void)
{
	TimeCount = 0;
	__TIM_CLK_ENABLE();
	__ECHO_GPIO_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = ECHO_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ECHO_PORT, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100;	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;	
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM, TIM_SOURCES);
	TIM_ITConfig(TIM, TIM_SOURCES, ENABLE);		
	TIM_ClearFlag(TIM, TIM_FLAG_Update);
	TIM_ITConfig(TIM, TIM_FLAG_Update, ENABLE);		
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_CHANNEL;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_PWMIConfig(TIM, &TIM_ICInitStructure);

	TIM_SelectInputTrigger(TIM, TIM_TPx);
	TIM_SelectSlaveMode(TIM, TIM_SlaveMode_Reset);
	
	TIM_Cmd(TIM, ENABLE);
}

void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM, TIM_SOURCES) == SET)
	{
		if (HCSR04_Status & 0x80)
		{
			HCSR04_Status |= 0x40;
			ATim = TimeCount * 100 + TIM_GetCapture2(TIM);
			DelayTime = ATim - BTim;
		}
		else if (HCSR04_Status == 0x00)
		{
			HCSR04_Status |= 0x80;
			BTim = TimeCount * 100 + TIM_GetCapture2(TIM);			
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
			TIM_PWMIConfig(TIM, &TIM_ICInitStructure);

		}
		TIM_ClearITPendingBit(TIM, TIM_SOURCES);
	}
	if(TIM_GetITStatus(TIM, TIM_FLAG_Update) == SET)
	{
		TimeCount ++;
		TIM_ClearITPendingBit(TIM, TIM_FLAG_Update);
	}
	
}
