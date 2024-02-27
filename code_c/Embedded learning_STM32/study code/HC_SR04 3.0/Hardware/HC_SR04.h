#ifndef __HCSR04_H
#define __HCSR04_H

#define 	HCSR04_GETDATASUCCESSFUL			0x40
#define     HCSR04_GETRISINGSUCCESSFUL			0x80

#define 	__TRIQ_GPIO_CLK_ENABLE()			do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);}while(0);
#define 	__ECHO_GPIO_CLK_ENABLE()			do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);}while(0);
#define 	__ECHO_EXTI_CONFIG()				do{	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);}while(0);

#define		TRIQ_GPIO_PIN						GPIO_Pin_0
#define 	ECHO_GPIO_PIN						GPIO_Pin_1
#define     TRIQ_PORT							GPIOA
#define     ECHO_PORT							GPIOA

#define		Triq(x)								do{	x ? \
													GPIO_WriteBit(TRIQ_PORT, TRIQ_GPIO_PIN, (BitAction)1):\
													GPIO_WriteBit(TRIQ_PORT, TRIQ_GPIO_PIN, (BitAction)0);\
													} while(0);


extern uint8_t HCSR04_Status;													

typedef struct
{
	uint32_t DistanceInt;
	uint32_t DistanceFloat;
}HCSR04_DisTance;

void HCSR04_Init(void);	
void HCSR04_Start(void);													
void HCSR04_GetDistance(HCSR04_DisTance *HCSR04_distance);

#endif
