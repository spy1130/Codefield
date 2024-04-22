#include "relay.h"
#include "STC15F2K60S2.h"
#include "Delay.h"

void Relay(unsigned char DelayNum)
{
	P0 = 0x10;//p04
	P2=P2 & 0X1F | 0xA0;//Y5
	Delay(DelayNum);
	P0 =0x00;
	P2 &=0x1f;
}
