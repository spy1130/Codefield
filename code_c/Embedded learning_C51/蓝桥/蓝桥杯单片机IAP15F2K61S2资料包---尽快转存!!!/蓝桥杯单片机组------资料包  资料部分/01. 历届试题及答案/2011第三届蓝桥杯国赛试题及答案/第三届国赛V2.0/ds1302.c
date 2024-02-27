/*
  程序说明: DS1302驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台 8051，12MHz
  日    期: 2011-8-9
*/

#include <STC15F2K60S2.h>
#include <intrins.h>
#include <ds1302.h>
												
unsigned  char ds[7]={0,59,6,2,2,2,17};
unsigned  char shijian[7];

void Write_Ds1302_Byte(unsigned  char temp) 
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

void Write_Ds1302( unsigned char address,unsigned char dat )     
{
 	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;	
 	_nop_();  
 	Write_Ds1302_Byte(address);	
	dat=(dat/10<<4)|(dat%10);
 	Write_Ds1302_Byte(dat);		
 	RST=0; 
}

unsigned char Read_Ds1302 ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;
	_nop_();
 	Write_Ds1302_Byte(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;
	_nop_();
 	RST=0;
	SCK=0;
	_nop_();
	SCK=1;
	_nop_();
	SDA=0;
	_nop_();
	SDA=1;
	_nop_();
	temp=temp/16*10+temp%16;
	_nop_();
	return (temp);			
}

void dswrite()
{
	unsigned char i,addr=0x80;
	Write_Ds1302(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		Write_Ds1302(addr,ds[i]);
		addr=addr+2;
	}
	Write_Ds1302(0x8e,0x80);
}

void dsread()
{
	unsigned char i,addr=0x81;
	Write_Ds1302(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		shijian[i]=Read_Ds1302(addr);
		addr=addr+2;
	}
	Write_Ds1302(0x8e,0x80);
}
