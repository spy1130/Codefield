#include "reg52.h"

void Delay(unsigned int t)
{
	while(t--);
	while(t--);
}

void SelectHC573(unsigned char channel)
{
	switch(channel)
	{
		case 4:
			P2 = (P2 & 0x1f) | 0x80;
		break;
		
		case 5:
			P2 = (P2 & 0x1f) | 0xa0;
		break;
		
		case 6:
			P2 = (P2 & 0x1f) | 0xc0;
		break;
		
		case 7:
			P2 = (P2 & 0x1f) | 0xe0;
		break;
	}
}

void LEDRuning()
{
	SelectHC573(4);
	P0 = 0xf0;
	Delay(60000);
	Delay(60000);
	P0 = 0x0f; 
	Delay(60000);
	Delay(60000);
	P0 = 0xff; 
	Delay(60000);
	Delay(60000);
}

void SMGRunning()
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		SelectHC573(6);
		P0 = 0x01 << i; 
		SelectHC573(7);
		P0 = 0x00; 
		Delay(60000);
		Delay(60000);
	}
	P0 = 0xff; 
	Delay(60000);
	Delay(60000);
}

void main()
{
	while(1)
	{
		LEDRuning();
		SMGRunning();
	}
}