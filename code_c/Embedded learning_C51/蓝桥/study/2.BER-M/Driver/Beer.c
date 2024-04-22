#include "Beer.h"
#include "STC15F2K60S2.h"
#include "Delay.h"
//输入为延时多少毫秒
void Beer(unsigned char DelayNum)
{	P0 = 0x40;//P06
	P2=P2 & 0X1F | 0xA0;//y5
	Delay(DelayNum);
	P0=0X00;
	P2 &=0x1f;
}