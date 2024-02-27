#include "BUZZ.h"

sbit BUZZ = P0^6;
sbit REALY = P0^4;

void Close_B()
{
	P2 = 0xA0;
	BUZZ = 0;
	P2 = 0x00;
}

void Oppen_B()
{
	P2 = 0xA0;
	BUZZ = 1;
	P2 = 0x00;
}

void Close_R()
{
	P2 = 0xA0;
	REALY = 0;
	P2 = 0x00;
}

void Oppen_R()
{
	P2 = 0xA0;
	REALY = 1;
	P2 = 0x00;
}