#include "intrins.h"
void delay_ms(unsigned int xms)
{
	while(xms)
	{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
	xms--;
	}
}
void delay_us(unsigned int xus)		//@12.000MHz
{
	while(xus)
	{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	xus--;
	}
}
