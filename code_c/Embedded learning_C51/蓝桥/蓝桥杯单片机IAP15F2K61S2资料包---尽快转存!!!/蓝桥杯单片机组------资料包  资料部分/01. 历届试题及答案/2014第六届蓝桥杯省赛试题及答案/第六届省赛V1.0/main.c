#include "STC15F2K60S2.h"
#include "onewire.h"
#include "intrins.h"
#define uint unsigned int
#define uchar unsigned char

uchar dsbuff[8]={11,11,11,11,11,11,0,0};
uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
uchar discount;

void Delay50us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 1;
	j = 134;
	do
	{
		while (--j);
	} while (--i);
}

uchar read_temperature()
{
	uchar high,low;
	char temp;
	Init_DS18B20();
	Write_DS18B20(0XCC);
	Write_DS18B20(0X44);
	Delay50us();
	Delay50us();
	
	Init_DS18B20();
	Write_DS18B20(0XCC);
	Write_DS18B20(0XBE);
	low=Read_DS18B20();
	high=Read_DS18B20();
	temp=high<<4;
	temp |= (low>>4);
	return temp;
}
void allinit()
{
		P2=0x80;P0=0XFF;
		P2=0XA0;P0=0X00;
		P2=0XC0;P0=0XFF;
		P2=0XEF;P0=0XFF;
}

void display()
{
	P2=0XEF;
	P0=0XFF;
	P2=0X1F;
	
	P2=0XCF;
	P0=1<<discount;
	P2=0x1f;
	
	P2=0XEF;
	P0=tab[dsbuff[discount]];
	P2=0X1F;
	
	if(++discount==8) discount=0;
}

void Timer0Init()		//2毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x9A;		//设置定时初值
	TH0 = 0xA9;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
}

void main()
{
	uchar wendu;
	allinit();
	Timer0Init();
	while(1)
	{
		wendu=read_temperature();
		dsbuff[7]=wendu%10;
		dsbuff[6]=wendu/10;
	}
}

void Timer0() interrupt 1
{
	display();
}