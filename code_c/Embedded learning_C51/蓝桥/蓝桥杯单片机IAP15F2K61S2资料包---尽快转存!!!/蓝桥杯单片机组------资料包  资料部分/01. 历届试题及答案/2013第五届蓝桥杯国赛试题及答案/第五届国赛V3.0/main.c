#include "STC15F2K60S2.H"
#include "onewire.h"
#include "iic.h"
#include "ds1302.h"

#define uint unsigned int
#define uchar unsigned char

uchar code tab[14]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff,0xc6,0x89};
uchar dsbuff[8]={11,11,11,11,11,11,11,11};
uchar discount;
uchar wendu,adc;
uchar s4=0;
uchar fan=0;
uint count0;

void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
	for(j=110;j>0;j--);
}

void allinit()
{
	P2=0XA0;P0=0X00;
	P2=0X80;P0=0XFF;
	P2=0XE0;P0=0XFF;
	P2=0XC0;P0=0XFF;
}
void display()
{
	P2=0XEF;
	P0=0XFF;
	P2=0X1F;
	
	P2=0XCF;
	P0=1<<discount;
	P2=0X1F;
	
	P2=0XEF;
	P0=tab[dsbuff[discount]];
	P2=0X1F;
	
	if(++discount==8) discount=0;
}

void Timer0Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x9A;		//设置定时初值
	TH0 = 0xA9;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	EA=1;
	ET0=1;
}

void key()
{
	if(P33==0)
	{
		delayms(10);
		if(P33==0)
		{
			if(s4==0) s4=1;
			else if(s4==1) s4=0;
		}
		while(!P33);
	}
}
void main()
{
	Timer0Init();
	allinit();
	dswrite();
	while(1)
	{
		wendu=read_temp();
		adc=read_adc(0x03);
		dsread();
		key();
		if(s4==0)
		{
			dsbuff[6]=adc%10;
			dsbuff[5]=adc/10;
			dsbuff[7]=13;
			dsbuff[4]=11;
			dsbuff[3]=11;
			dsbuff[2]=12;
			dsbuff[1]=wendu%10;
			dsbuff[0]=wendu/10;
		}
		else
		{
			dsbuff[7]=shijian[0]%10;
			dsbuff[6]=shijian[0]/10;
			
			dsbuff[4]=shijian[1]%10;
			dsbuff[3]=shijian[1]/10;
			
			dsbuff[1]=shijian[2]%10;
			dsbuff[0]=shijian[2]/10;
		}
	}
}

void Tmer0() interrupt 1
{
	display();
	count0++;	
	if(count0==500)
	{
		count0=0;
		if(fan==0)
		{
			fan=1;
			dsbuff[5]=10;
			dsbuff[2]=10;
		}
		else if(fan==1)
		{
			fan=0;
			dsbuff[5]=11;
			dsbuff[2]=11;
		}
	}
}