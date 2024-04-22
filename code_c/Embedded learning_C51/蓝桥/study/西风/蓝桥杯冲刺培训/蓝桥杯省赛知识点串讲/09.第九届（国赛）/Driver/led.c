#include "led.h"

void Led_Disp(unsigned char ucLed)
{
	P0 = ~ ucLed;
	P2 = P2 & 0x1f | 0x80;
	P2 &= 0x1f;
}