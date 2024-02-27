#include "reg52.h"

sbit L1 = P0^0;
sbit L8 = P0^7;

void Delay(unsigned int t)
{
	while(t--);
	while(t--);
	while(t--);
}

void SelectHC573()
{
	P2 = (P2 & 0x1f) | 0x80; 
}

void Working()
{
	SelectHC573();
	L1 = 0;
	Delay(60000);
	L1 = 1;
	Delay(60000);
}

//================================
void Init_INT0()
{
	IT0 = 1;
	EX0 = 1;
	EA = 1;
}

unsigned char stat_int = 0;
void ServiceINT0() interrupt 0
{
	stat_int = 1;
}

void LEDINT()
{
	if(stat_int == 1)
	{
		L8 = 0;
		Delay(60000);
		Delay(60000);
		Delay(60000);
		Delay(60000);
		Delay(60000);
		Delay(60000);
		L8 = 1;
	}
	stat_int = 0;
}
//================================
void main()
{
	Init_INT0();
	while(1)
	{
		Working();
		LEDINT();
	}
}

