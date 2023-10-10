#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"

int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	OLED_ShowString(1, 1, "Freq:00000Hz");
	
	PWM_SetPrescaler(720 - 1);			//Freq = 72M / (PSC + 1) / (ARR+1)ÆµÂÊ
	PWM_SetCompare1(50);				//Duty = CCR / (AAR+1)Õ¼¿Õ±È
	
	while (1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq(), 5);
	}
}
