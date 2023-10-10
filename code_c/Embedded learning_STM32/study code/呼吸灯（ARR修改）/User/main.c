#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "Encoder.h"

int32_t Num;
extern uint8_t i;
int main(void)
{	
	OLED_Init();
	PWM_Init();
	Encoder_Init();
	OLED_ShowString(1,1,"PWM freq:");

	while(1)
	{	Num =Encoder_Get();
		OLED_ShowNum(1,10,72000000/720/(Num+1),5);//freqCNT的频率
		//CCR不断变化
		for(i=0;i<=100;i++)
		{	
			PWM_SetCompare1(i);
			Delay_ms(10);
		}
		for(i=0;i<=100;i++)
		{
			PWM_SetCompare1(100-i);
			Delay_ms(10);
			
		}
	}
}

