# include "bsp_led.h"


void Led_Init(unsigned char ucled)
{
	//��ȫ��
	P0=0xff;
	P2=P2&0x1f|0x80;
	P2=P2&0X1F;
	//����
	P0=ucled;
	P2=P2&0x1f|0x80;
	P2=P2&0X1F;
}