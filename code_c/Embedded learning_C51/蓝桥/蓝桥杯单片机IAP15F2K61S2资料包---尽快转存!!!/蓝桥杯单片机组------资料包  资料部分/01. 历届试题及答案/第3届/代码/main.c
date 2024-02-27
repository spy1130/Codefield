#include <STC15F2K60S2.h>
#include <intrins.h>
#include "DS.h"
#include "RELAY.h"
#include "IIC.h"
//每10毫秒1ml的水
sbit S7 = P3^0;
sbit S6 = P3^1;

bit S7_flag = 0;
bit S6_flag = 0;

bit choose = 0;

unsigned long int Temp = 0;    
unsigned long int The_mL = 0;    //99.99L 时停止， 99990
float The_L = 0;
float The_Money = 0;

void Delay5ms();
void KeyPress();
void Close_buzz();
void Time0Init();

void main()
{
	Close_buzz();	
	Time0Init();

	while(1)
	{
		Pcf8591();
		KeyPress();	
		DS1();
		if(S7_flag == 1)
		{	
			TR0 = 1;		//定时器0开始计时
			S7_flag = 0;
			OppenRelay();
			choose = 0;			
		}
		if(S6_flag == 1)
		{
			TR0 = 0;		//定时器0停止计时
			S6_flag = 0;
			CloseRelay();
			choose = 1;
			TL0 = 0x00;		//设置定时初值
			TH0 = 0xDC;		//设置定时初值
			The_L = The_mL;
			The_mL = 0;
		}
		if (choose == 0)
		{
			if (The_mL > 99990)
			{
				TR0 = 0;		//定时器0停止计时
				CloseRelay();
				choose = 1;
				TL0 = 0x00;		//设置定时初值
				TH0 = 0xDC;		//设置定时初值
				The_L = The_mL;
				The_mL = 0;
			}
			DS2(The_mL/10);
		}
		else
		{
			The_Money = The_L*0.5/10;
			DS2(The_Money);			
		}	
	}
}

void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 54;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}

void KeyPress()
{
	if (S7 == 0)
	{
		Delay5ms();
		if (S7 == 0)
		{
			S7_flag = 1;
		}
		while(S7 == 0);
	}
	
	if (S6 == 0)
	{
		Delay5ms();
		if (S6 == 0)
		{
			S6_flag = 1;
		}
		while(S6 == 0);
	}
}

void Close_buzz()
{
	P2 = 0xA0;
	P0 = 0x00;
	P2 = 0x00;
	
	P2 = 0x80;
	P0 = 0xFF;
	P2 = 0x00;
}

void Time0Init()
{	
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	
	ET0 = 1;
	EA = 1;
}

void Time0int() interrupt 1
{
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	The_mL++;
	TF0 = 0;
}