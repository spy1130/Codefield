# include "bsp_led.h"


void Led_Init(unsigned char ucled)
{
	//µÆÈ«¹Ø
	P0=0xff;
	P2=P2&0x1f|0x80;
	P2=P2&0X1F;
	//¿ªµÆ
	P0=ucled;
	P2=P2&0x1f|0x80;
	P2=P2&0X1F;
}