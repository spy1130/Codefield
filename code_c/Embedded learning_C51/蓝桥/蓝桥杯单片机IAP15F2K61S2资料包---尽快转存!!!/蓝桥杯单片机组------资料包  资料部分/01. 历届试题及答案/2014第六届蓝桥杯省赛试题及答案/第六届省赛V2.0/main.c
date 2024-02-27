#include <STC15F2K60S2.h>
#include <intrins.h>
#include <ds1302.h>

#define uint unsigned int
#define uchar unsigned char

uchar discount;
uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
uchar dsbuff[8]={11,11,10,11,11,10,11,11};
uchar fan=0;
uint count;
void allinit()
{
	P2=0XA0;P0=0X00;
	P2=0X80;P0=0XFF;
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

void main()
{
	allinit();
	Timer0Init();
	ds_init();
	while(1)
	{
		read_time();
		dsbuff[0]=shijian[2]/10;
		dsbuff[1]=shijian[2]%10;
		dsbuff[3]=shijian[1]/10;
		dsbuff[4]=shijian[1]%10;
		dsbuff[6]=shijian[0]/10;
		dsbuff[7]=shijian[0]%10;
	}
}

void Timer0() interrupt 1
{
	display();	
	count++;
	if(count==250)
	{
		count=0;
		if(fan==0)
		{	
			fan=1;
			dsbuff[2]=11;
			dsbuff[5]=11;
		}
		else
		{
			fan=0;
			dsbuff[2]=10;
			dsbuff[5]=10;
		}
	}
}
