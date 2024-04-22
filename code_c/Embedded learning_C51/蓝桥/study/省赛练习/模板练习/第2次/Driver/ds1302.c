/*	# 	DS1302代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/								
#include <STC15F2K60S2.H>
#include <intrins.h>
#include "ds1302.h"
sbit SCK=P1^7;
sbit SDA=P2^3;
sbit RST=P1^3;

//
void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK = 0;
		SDA = temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

//
void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

//
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

void ucRtc_Set(u8 *ucRtc)//?
{	u8 i;
	Write_Ds1302_Byte(0x8e,0);//?0x8e
	for(i=0;i<3;i++)
		Write_Ds1302_Byte(0x84-i*2,ucRtc[i]);//?
	Write_Ds1302_Byte(0x8e,1);//?0x8e
}
void ucRtc_Read(u8 *ucRtc)
{
	u8 i;
	for(i=0;i<3;i++)
		ucRtc[i]=Read_Ds1302_Byte (0x85-i*2);
}