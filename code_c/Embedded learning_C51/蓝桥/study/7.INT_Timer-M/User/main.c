#include "STC15F2K60S2.H"
#include "LED.h"
#include "init.h"
#include "INT.h"
#include "Delay.h"

/* �������� */
void LED_Proc(void);
void INT(void);


unsigned char stat_int = 0;

void main()
{
	init_close();
	INT0_init();
	EA=1;
	while(1)
	{	
		LED_Proc();
		INT();
	}
}

//-----------------------------------------
//�жϷ������
void exint0() interrupt 0       //INT0�ж������P32����
{
   stat_int = 1;                 
}

//-----------------------------------------------
void LED_Proc(void)
{	
	LED(0x01);
	Delay(200);
	LED(0x00);
	Delay(200);
}
void INT(void)
{
	if(stat_int == 1)
	{
		LED(0x80);
		Delay(1000);
		LED(0x00);
	}
	stat_int = 0;
}