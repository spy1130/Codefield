#include "reg52.h"

sfr AUXR = 0x8e;

unsigned char urdat;

void SendByte(unsigned char dat);

void InitUart()
{
	TMOD = 0x20;
	TH1 = 0xfd;
	TL1 = 0xfd;
	TR1 = 1;
	
	SCON = 0x50;
	AUXR = 0x00;
	
	ES = 1;
	EA = 1;
}

void ServiceUart() interrupt 4
{
	if(RI == 1)
	{
		RI = 0;
		urdat = SBUF; 
		SendByte(urdat + 1);
	}
}

void SendByte(unsigned char dat)
{
	SBUF = dat;
	while(TI == 0);
	TI = 0;
}

void main()
{
	InitUart();
	SendByte(0x5a);
	SendByte(0xa5);
	while(1);
}