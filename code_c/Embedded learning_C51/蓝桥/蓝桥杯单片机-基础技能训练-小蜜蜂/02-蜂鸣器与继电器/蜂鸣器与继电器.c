#include "reg52.h"

void Delay(unsigned int t)
{
	while(t--);
	while(t--);
}

void InitHC138(unsigned char n)
{
	switch(n)
	{
		case 4:
			P2 = (P2 & 0X1f) | 0x80;
		break;
		case 5:
			P2 = (P2 & 0X1f) | 0xa0;
		break;
		case 6:
			P2 = (P2 & 0X1f) | 0xc0;
		break;
		case 7:
			P2 = (P2 & 0X1f) | 0xe0;
		break;
	}
}

void OutPutP0(unsigned char channel, unsigned char dat)
{
	InitHC138(channel);
	P0 = dat;
}

void LEDRunning()
{
	unsigned char i;
	
	for(i = 0; i < 3; i++)
	{
		OutPutP0(4,0x00);
		Delay(60000);
		Delay(60000);
		OutPutP0(4,0xff);
		Delay(60000);
		Delay(60000);
	}
	
	for(i = 1; i <= 8; i++)
	{
		OutPutP0(4,(0xff << i));
		Delay(60000);
	}
	
	OutPutP0(5,0x10);
	Delay(60000);
	Delay(60000);
	OutPutP0(5,0x00);
	
	InitHC138(4);
	for(i = 1; i <= 8; i++)
	{
		OutPutP0(4,~(0xff << i));
		Delay(60000);
	}
	
	OutPutP0(5,0x40);
	Delay(60000);
	Delay(60000);
	OutPutP0(5,0x00);
}

void InitSystem()
{
	OutPutP0(5,0x00);
}

void main()
{
	InitSystem();
	while(1)
	{
		LEDRunning();
	}
}