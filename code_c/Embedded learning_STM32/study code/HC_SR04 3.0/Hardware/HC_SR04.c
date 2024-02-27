#include "stm32f10x.h"                  // Device header
#include "HC_SR04.h"
#include "OLED.h"
#include "Timer.h"
#include "Delay.h"

uint8_t HCSR04_Status = 0x00;

void HCSR04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	__TRIQ_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = TRIQ_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TRIQ_PORT, &GPIO_InitStructure);
	
	Triq(0);
}

void HCSR04_Start(void)
{
	DelayTime = 0;
	HCSR04_Status = 0x00;
	Triq(1);
	Delay_us(15);
	Triq(0);
	Timer_Init();
}

void HCSR04_GetDistance(HCSR04_DisTance *HCSR04_distance)
{
	float DisTance = (float)(((DelayTime * 0.000001) * 34000) / 2);
	HCSR04_distance->DistanceInt = (uint32_t)DisTance;
	HCSR04_distance->DistanceFloat = (uint32_t)((DisTance - HCSR04_distance->DistanceInt) * 10000);
}

