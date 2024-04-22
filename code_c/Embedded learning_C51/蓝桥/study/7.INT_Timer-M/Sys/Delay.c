#include "Delay.h"
#include "INTRINS.h"
//i是几，就是几毫秒
void Delay(unsigned char i)		//@11.0592MHz
{ while(i--)
	Delay1ms();
}
void Delay1ms()		//@11.0592MHz
{
	unsigned char data i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}

