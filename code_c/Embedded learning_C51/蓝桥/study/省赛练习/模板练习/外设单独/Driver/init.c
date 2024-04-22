#include <init.h>

void System_Init()
{
	P0=0XFF;
	P2=P2&0X1F|0X80;//y4
	P2 &=0X1F;
	
	P0=0x00;
	P2=P2&0X1F|0Xa0;//y5
	P2 &=0X1F;
}