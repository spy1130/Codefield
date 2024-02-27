#include "key.h"
#include "Ultrasonic.h"


sbit Key1 = P1^0;


volatile unsigned char start = 0;


void keyscan(void)
{
	if(Key1==0)
	{
		delay_ms(5);
		if(Key1==0)
		{
			
			start = 1;
		}
		while(Key1==0);
	}
}

