//Y4-0X80;Y5-0XA0;��LED-0XFF;��BUZZ��Realy-0X00
#include <Init.h>

void System_Init()
{	//��LED
	P0=0XFF;
	P2 =P2&0X1F |0X80;//Y4
	P2 &=0x1f;
	//�ر�BUZZ��Realy
	P0=0X00;
	P2 =P2&0X1F |0XA0;//Y5
	P2 &=0x1f;
}