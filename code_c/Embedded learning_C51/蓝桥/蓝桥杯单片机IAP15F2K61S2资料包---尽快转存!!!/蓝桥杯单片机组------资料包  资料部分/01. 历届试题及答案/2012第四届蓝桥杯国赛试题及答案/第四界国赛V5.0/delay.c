#include <delay.h>

void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 54;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}
//void Delay2ms()		//@11.0592MHz
//{
//	unsigned char i, j;

//	_nop_();
//	_nop_();
//	i = 22;
//	j = 128;
//	do
//	{
//		while (--j);
//	} while (--i);
//}
//void Delay1ms()		//@11.0592MHz
//{
//	unsigned char i, j;

//	_nop_();
//	_nop_();
//	_nop_();
//	i = 11;
//	j = 190;
//	do
//	{
//		while (--j);
//	} while (--i);
//}

