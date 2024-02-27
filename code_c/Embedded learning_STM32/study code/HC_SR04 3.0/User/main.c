#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "HC_SR04.h"


HCSR04_DisTance HCSR04_distance = {0};

int main(void)
{
	OLED_Init();
	HCSR04_Init();
	HCSR04_Start();
	
	OLED_ShowString(1, 1, "Distance:");
	OLED_ShowString(2, 1, "000.0000 CM");
	
	while (1)
	{
		if (HCSR04_Status & HCSR04_GETDATASUCCESSFUL)
		{
			HCSR04_GetDistance(&HCSR04_distance);
			OLED_ShowNum(2, 1, HCSR04_distance.DistanceInt, 3);
			OLED_ShowNum(2, 5, HCSR04_distance.DistanceFloat, 4);
			
			Delay_ms(100);
			HCSR04_Start();
		}
	}
}
//仅用来测试环境是否跑通