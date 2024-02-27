#include "STC15F2K60S2.h"

void Delay200ms()		//@12.000MHz
{
	unsigned char data i, j, k;

	i = 10;
	j = 31;
	k = 147;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}





void main ()
{	unsigned char i;
	P2= P2 & 0x1F | 0x80; //Y4
	for(i=1;i<=8;i++)
	{
		P0=~(0x01 <<i);
		Delay200ms();
	}
	for(i=1;i<=8;i++)
	{
		P0=~(0x80 >>i);
		Delay200ms();
	}
}