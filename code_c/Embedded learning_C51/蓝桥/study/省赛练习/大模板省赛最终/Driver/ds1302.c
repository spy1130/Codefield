/*	# 	DS1302代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/
/*<intrins.h>；sbit看原理图；
设置RTC顺序是在0x8e地址0开wp，0x84-i*2读h->m->s到输入数组，1关wp
读取是直接读0x85-i*2读到h->m->s
*/
#include <STC15F2K60S2.H>
#include "ds1302.h" 
#include <intrins.h>

sbit SCK =P1^7;
sbit SDA =P2^3;//对应原理图里的DS1302_IO
sbit RST =P1^3;//对应原理图里的DS1302_CE
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

void Set_Rtc(unsigned char* ucRTC)
{
	unsigned char i;
	Write_Ds1302_Byte(0x8e,0);//开wp(写入允许)
	
	for(i=0;i<3;i++)
		Write_Ds1302_Byte(0x84-i*2,ucRTC[i]);//h->m->s
	
	Write_Ds1302_Byte(0x8e,1);//关wp(写入禁止)
}

void Read_Rtc(unsigned char* ucRTC)
{
	unsigned char i;
	for(i=0;i<3;i++)
		ucRTC[i]=Read_Ds1302_Byte(0x85-i*2);//h->m->s
}