#include "stm32f10x.h"                  // Device header
#include "Delay.h"



void LED_Mode1() {
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
	Delay_ms(100);
}

void LED_Mode2() {
    for(int i=3;i>0;i--){
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
			Delay_ms(300);
			GPIO_SetBits(GPIOA, GPIO_Pin_1);
			Delay_ms(300);
		}
}

void LED_Mode3() {
	for (int i = 0; i < 4; i++) {
		for(int j=0; j<100; j++){
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		Delay_ms(i+1);
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
		Delay_ms(4-i);
    }}
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}


int main(void)
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启APB2外设时钟控制
	
	GPIO_InitTypeDef GPIO_Initstructure;//结构体
	GPIO_Initstructure.GPIO_Mode =GPIO_Mode_Out_PP;//推挽
	GPIO_Initstructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_2;//GPIO_Pin_0 | GPIO_Pin_1 | ...这里可以用按位或写法相当于都选上
	GPIO_Initstructure.GPIO_Speed =GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	
	GPIO_InitTypeDef GPIO_InitstructureA;//结构体
	GPIO_InitstructureA.GPIO_Mode =GPIO_Mode_Out_OD;//开漏
	GPIO_InitstructureA.GPIO_Pin =GPIO_Pin_1;
	GPIO_InitstructureA.GPIO_Speed =GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitstructureA);
	
	
	while(1)
	{	
		LED_Mode1();
		LED_Mode2();
		Delay_ms(500);
		LED_Mode3();
		Delay_ms(1000);
  
	}
}

