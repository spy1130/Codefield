#include "DS1302.h"


sbit CE = P1^3;
sbit DA = P2^3;
sbit SCK = P1^7;
//8 ±30∑÷0√Î
unsigned char Write_Addr[3] = {0x84, 0x82, 0x80};
unsigned char Read_Addr[3] = {0x85, 0x83, 0x81};
unsigned char Init_Time[3] = {0x08, 0x30, 0x00};
unsigned char Display_time[3] = {0x00, 0x00, 0x00};




void Write_DS1302_Byte(unsigned char da)
{
	unsigned char i;
	for(i=0; i<8; i++)
	{
		SCK = 0;
		DA = da&0x01;
		da >>= 1;
		SCK = 1;
	}
}

unsigned char Read_DS1302_Byte(unsigned char addr)
{
	unsigned char i,time;
	CE = 0; _nop_();
	SCK = 0; _nop_();
	CE = 1; _nop_();
	Write_DS1302_Byte(addr);
	for(i=0; i<8; i++)
	{
		SCK = 0;
		time >>= 1;
		if(DA)
			time |= 0x80;
		SCK = 1;
	}
	CE=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	DA=0;	_nop_();
	DA=1;	_nop_();
	
	return time;
}

void Write_DS1302(unsigned char addr,unsigned da)
{
	CE = 0;	_nop_();
	SCK = 0; _nop_();
	CE = 1; _nop_();
	
	Write_DS1302_Byte(addr);
	Write_DS1302_Byte(da);
	CE = 0;
}

void Init_DS1302()
{
	unsigned char i;
	for(i=0; i<3; i++)
	{
		Write_DS1302(Write_Addr[i], Init_Time[i]);
	}
}	

void Read_DS1302()
{
	unsigned char i;
	for(i=0; i<3; i++)
	{
		Display_time[i] = Read_DS1302_Byte(Read_Addr[i]);
	}
}