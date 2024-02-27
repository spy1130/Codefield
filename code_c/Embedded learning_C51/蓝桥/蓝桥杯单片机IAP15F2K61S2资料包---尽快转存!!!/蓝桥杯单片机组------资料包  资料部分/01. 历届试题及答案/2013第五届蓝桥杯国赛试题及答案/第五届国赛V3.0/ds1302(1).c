/*
  ����˵��: DS1302��������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨ 8051��12MHz
  ��    ��: 2011-8-9
*/

#include "STC15F2K60S2.H"
#include <intrins.h>
#include <ds1302.h>

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST=P1^3;   // DS1302��λ
											
unsigned char shijian[7]={30,8,8,2,2,2,2};

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
	return (temp);			
}

void dswrite()
{
	unsigned char i,addr=0x80;
	Write_Ds1302(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		Write_Ds1302(addr,shijian[i]);
		addr=addr+2;
	}
	Write_Ds1302(0x8e,0x080);
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
	Write_Ds1302(0x8e,0x080);
}