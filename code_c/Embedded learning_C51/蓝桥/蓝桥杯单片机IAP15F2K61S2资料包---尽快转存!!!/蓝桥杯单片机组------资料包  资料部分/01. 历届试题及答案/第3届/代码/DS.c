#include "DS.h"
unsigned char w1 = 0x08;
unsigned char w2 = 0x80;

unsigned char display1[4] = { 0xFF, 0x40, 0x92, 0xc0};
unsigned char display2[4] = { 0, 0, 0, 0};
code unsigned char tab[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

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

void DS1()
{
	unsigned char i;
	for(i=0; i<4; i++)
	{
		P2 = 0xC0;
		P0 = w1>>i;
		P2 = 0xE0;
		P0 = display1[3-i];
		Delay1ms();
		P2 = 0x00;
	}
}
	
void DS2(unsigned long int da)
{
	display2[3] = da/1000;
	display2[2] = da%1000/100;
	display2[1] = da%1000%100/10;
	display2[0] = da%1000%100%10;

	P2 = 0xC0;
	P0 = w2>>0;
	P2 = 0xE0;
	P0 = tab[display2[0]];
	Delay1ms();
	P2 = 0x00;

	P2 = 0xC0;
	P0 = w2>>1;
	P2 = 0xE0;
	P0 = tab[display2[1]];
	Delay1ms();
	P2 = 0x00;	
	
	P2 = 0xC0;
	P0 = w2>>2;
	P2 = 0xE0;
	P0 = tab[display2[2]] & 0x7F;
	Delay1ms();
	P2 = 0x00;
	
	P2 = 0xC0;
	P0 = w2>>3;
	P2 = 0xE0;
	P0 = tab[display2[3]];
	Delay1ms();
	P2 = 0x00;	
}