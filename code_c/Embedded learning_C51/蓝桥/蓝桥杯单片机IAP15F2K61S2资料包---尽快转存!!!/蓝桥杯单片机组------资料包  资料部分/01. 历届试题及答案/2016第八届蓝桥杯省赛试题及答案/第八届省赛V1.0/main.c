#include <STC15F2K60S2.h>
#include <intrins.h>
#include <ds1302.h>
#include <onewire.h>

#define uint unsigned int
#define uchar unsigned char

uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
uchar dsbuff[8]={11,11,11,11,11,11,11,11};
uchar discount;

void allinit()
{
	P2=0xa0;P0=0x00;
	P2=0x80;P0=0xff;
	P2=0xc0;P0=0xff;
	P2=0xe0;P0=0xff;
}

void display()
{
	P2=0xef;
	P0=0xff;
	P2=0x1f;
	
	P2=0xcf;
	P0=01<<discount;
	P2=0x1f;
	
	P2=0xef;
	P0=tab[dsbuff[discount]];
	P2=0x1f;
	
	if(++discount==8) discount=0;
}

void Timer0Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x80;					//定时器时钟1T模式
	TMOD &= 0xF0;					//设置定时器模式
	TL0 = 0x9A;						//设置定时初值
	TH0 = 0xA9;						//设置定时初值
	TF0 = 0;							//清除TF0标志
	TR0 = 1;							//定时器0开始计时
	EA=1;
	ET0=1;
}

void key()
{
		if(P30==0)
			{
				Delay5ms();
				if(P30==0)
					{
						
					}
				while(!P30);
			}
		
		if(P31==0)
			{
				Delay5ms();
				if(P31==0)
					{
						
					}
				while(!P31);
			}
		if(P32==0)
			{
				Delay5ms();
				if(P32==0)
					{
						
					}
				while(!P32);
			}
		if(P33==0)
			{
				Delay5ms();
				if(P33==0)
					{
						
					}
				while(!P33);
			}
}

void main()
{
	uchar temp;
	allinit();
	Timer0Init();
	ds_write();
	while(1)
	{
		temp=wendu();
		ds_read();
		dsbuff[7]=shijian[0]%10;
		dsbuff[6]=shijian[0]/10;
		dsbuff[5]=10;
		dsbuff[4]=shijian[1]%10;
		dsbuff[3]=shijian[1]/10;
		dsbuff[2]=10;
		dsbuff[1]=shijian[2]%10;
		dsbuff[0]=shijian[2]/10;
	}
}

void Timer0() interrupt 1
{
	display();
}