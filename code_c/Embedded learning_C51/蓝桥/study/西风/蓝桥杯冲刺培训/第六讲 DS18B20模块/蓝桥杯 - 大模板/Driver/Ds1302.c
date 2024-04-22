#include <reg52.h>
#include <intrins.h>
#include "ds1302.h"
sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;  											

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

void Set_Rtc(unsigned char* ucRtc)
{
	unsigned char i;
	Write_Ds1302_Byte(0x8e,0);
	for(i=0;i<3;i++)
		Write_Ds1302_Byte(0x84-i*2,ucRtc[i]);
	Write_Ds1302_Byte(0x8e,1);
}


void Read_Rtc(unsigned char* ucRtc)
{
	unsigned char i;
	for(i=0;i<3;i++)
		ucRtc[i] = Read_Ds1302_Byte(0x85-i*2);
}

