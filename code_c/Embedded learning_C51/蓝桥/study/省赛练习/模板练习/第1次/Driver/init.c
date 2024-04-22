#include <init.h>


void init()
{
	P0=0xff;
	P2 =P2 &0X1F | 0X80;//Y4;
	P2 &=0X1F;
	
	P0=0x00;
	P2 =P2 &0X1F | 0Xa0;//Y5;
	P2 &=0X1F;
}