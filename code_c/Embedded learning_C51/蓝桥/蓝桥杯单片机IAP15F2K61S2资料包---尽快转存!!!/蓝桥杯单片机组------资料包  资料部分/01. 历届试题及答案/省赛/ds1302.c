#include "ds1302.h"

#include <STC15F2K60S2.h>
#include <intrins.h>

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;   // DS1302¸´Î»					

unsigned char Write_addr[] = {0x84, 0x82, 0x80};
unsigned char Read_addr[] = {0x85, 0x83, 0x81};
unsigned char InitTime[] = {0x23, 0x59, 0x50};
unsigned char DisplayTime[] = {0x00, 0x00, 0x00};

void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK=0;
		SDA=temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}

void Init_DS1302()
{
	unsigned char i;
	for(i=0; i<3; i++)
	{
		Write_Ds1302_Byte(Write_addr[i], InitTime[i]);
	}
}

void Read_DS1302()
{
	unsigned char i;
	for(i=0; i<3; i++)
	{
		DisplayTime[i] = Read_Ds1302_Byte(Read_addr[i]);
	}
}
