//Y4-0X80;Y5-0XA0;关LED-0XFF;关BUZZ和Realy-0X00
#include <Init.h>

void System_Init()
{	//关LED
	P0=0XFF;
	P2 =P2&0X1F |0X80;//Y4
	P2 &=0x1f;
	//关闭BUZZ和Realy
	P0=0X00;
	P2 =P2&0X1F |0XA0;//Y5
	P2 &=0x1f;
}