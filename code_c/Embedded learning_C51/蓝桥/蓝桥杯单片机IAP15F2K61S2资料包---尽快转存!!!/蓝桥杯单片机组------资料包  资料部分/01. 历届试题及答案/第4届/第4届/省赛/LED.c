#include "LED.h"

code unsigned char tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90, 0xFF, 0xBF};


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

void DS1(unsigned char yi, unsigned char er, unsigned char san, unsigned char si, unsigned char wu)
{
	unsigned char wei = 0x01;
	
	P2 = Y6;
	P0 = wei<<0;
	P2 = Y7;
	P0 = tab[yi];
	Delay1ms();
	P2 = 0x00;
	
	P2 = Y6;
	P0 = wei<<1;
	P2 = Y7;
	P0 = tab[er];
	Delay1ms();
	P2 = 0x00;
	
	P2 = Y6;
	P0 = wei<<2;
	P2 = Y7;
	P0 = tab[san];
	Delay1ms();
	P2 = 0x00;
	
	P2 = Y6;
	P0 = wei<<3;
	P2 = Y7;
	P0 = tab[si];
	Delay1ms();
	P2 = 0x00;
	
	P2 = Y6;
	P0 = wei<<4;
	P2 = Y7;
	P0 = tab[wu];
	Delay1ms();
	P2 = 0x00;
}

void DS2(unsigned char yi, unsigned char er, unsigned char san)
{
	unsigned char wei = 0x80;
	
	P2 = Y6;
	P0 = wei>>2;
	P2 = Y7;
	P0 = tab[yi];
	Delay1ms();
	P2 = 0x00;
	
	P2 = Y6;
	P0 = wei>>1;
	P2 = Y7;
	P0 = tab[er];
	Delay1ms();
	P2 = 0x00;
	
	P2 = Y6;
	P0 = wei>>0;
	P2 = Y7;
	P0 = tab[san];
	Delay1ms();
	P2 = 0x00;
}