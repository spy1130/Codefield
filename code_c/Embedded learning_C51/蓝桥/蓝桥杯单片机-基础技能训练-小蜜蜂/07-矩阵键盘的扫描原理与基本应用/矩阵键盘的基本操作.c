#include "reg52.h"

sfr P4    = 0xC0;

sbit R1 = P3^0;
sbit R2 = P3^1;
sbit R3 = P3^2;
sbit R4 = P3^3;

sbit C4 = P3^4;
sbit C3 = P3^5;
sbit C2 = P4^2;
sbit C1 = P4^4;

unsigned char code SMG_duanma[18]=
		{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
     0xbf,0x7f};
		 
void SelectHC573(unsigned char channel)
{
	switch(channel)
	{
		case 4:
			P2 = (P2 & 0x1f) | 0x80;
		break;
		case 5:
			P2 = (P2 & 0x1f) | 0xa0;
		break;
		case 6:
			P2 = (P2 & 0x1f) | 0xc0;
		break;
		case 7:
			P2 = (P2 & 0x1f) | 0xe0;
		break;
	}
}

void DisplayKeyNum(unsigned char value)
{
	P0 = 0xff;
	SelectHC573(6);
	P0 = 0x01;
	SelectHC573(7);
	P0 = value;
}

unsigned char key_num;
void ScanKeysMulti()
{
	R1 = 0;
	R2 = R3 = R4 = 1;
	C1 = C2 = C3 = C4 =1;
	if(C1 == 0)
	{
		while(C1 == 0);
		key_num = 0;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C2 == 0)
	{
		while(C2 == 0);
		key_num = 1;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C3 == 0)
	{
		while(C3 == 0);
		key_num = 2;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C4 == 0)
	{
		while(C4 == 0);
		key_num = 3;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	
	R2 = 0;
	R1 = R3 = R4 = 1;
	C1 = C2 = C3 = C4 =1;
	if(C1 == 0)
	{
		while(C1 == 0);
		key_num = 4;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C2 == 0)
	{
		while(C2 == 0);
		key_num = 5;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C3 == 0)
	{
		while(C3 == 0);
		key_num = 6;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C4 == 0)
	{
		while(C4 == 0);
		key_num = 7;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	
	R3 = 0;
	R2 = R1 = R4 = 1;
	C1 = C2 = C3 = C4 =1;
	if(C1 == 0)
	{
		while(C1 == 0);
		key_num = 8;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C2 == 0)
	{
		while(C2 == 0);
		key_num = 9;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C3 == 0)
	{
		while(C3 == 0);
		key_num = 10;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C4 == 0)
	{
		while(C4 == 0);
		key_num = 11;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	
	R4 = 0;
	R2 = R3 = R1 = 1;
	C1 = C2 = C3 = C4 =1;
	if(C1 == 0)
	{
		while(C1 == 0);
		key_num = 12;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C2 == 0)
	{
		while(C2 == 0);
		key_num = 13;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C3 == 0)
	{
		while(C3 == 0);
		key_num = 14;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C4 == 0)
	{
		while(C4 == 0);
		key_num = 15;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
}

void main()
{
	while(1)
	{
		ScanKeysMulti();
	}
}