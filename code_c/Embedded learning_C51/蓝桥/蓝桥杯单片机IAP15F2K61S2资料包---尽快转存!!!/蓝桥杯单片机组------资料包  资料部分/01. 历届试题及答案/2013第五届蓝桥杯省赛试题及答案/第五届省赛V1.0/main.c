#include "STC15F2K60S2.h"
#include "intrins.h"
#include "onewire.h"
#define uint unsigned int 
#define uchar unsigned char

uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
uchar qi=0,ba=0;
uchar count0=0,count1=0;
uchar flag1,flag2,flag3;
uchar dsbuff[8];
uchar discount;
uchar gongzuo;
uchar cishu;
uchar num[4];
uchar shezhi=0;
uchar clear;
uchar qingchu;

void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
	for(j=110;j>0;j--);
}

void banzi_init()
{
	P2=(P2&0x1f)|0x80;P0=0xff;
	P2=(P2&0x1f)|0xa0;P0=0x00;
	P2=(P2&0x1f)|0xc0;P0=0xff;
	P2=(P2&0x1f)|0xe0;P0=0xff;
}

void Timer1Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x9A;		//设置定时初值
	TH1 = 0xA9;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	EA=1;
	ET1=1;
}

void display()
{
	P2 = (P2&0x1f)|0xe0;		//打开段选锁存器
	P0 = 0xff;							//关闭所有数码管
	P2 = P2&0x1f;
		
	P2 = (P2&0x1f)|0xc0;		//打开位选锁存器
	P0 = 1<<discount;
	P2 = P2&0x1f;
	
	P2 = (P2&0x1f)|0xe0;		//打开段选锁存器
	P0 = tab[dsbuff[discount]];
	P2 = P2&0x1f;

	if(++discount==8)
	 discount = 0;
}

void keyscan()
{	
	uchar temp;
	P3=0x7f;
	P44=0;P42=1;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		delayms(10);
		temp=P3;
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			if(shezhi==1)
			{
				switch(temp)
				{
					case 0x0e:num[cishu]=0;cishu++;break;
					case 0x0d:num[cishu]=3;cishu++;break; 
					case 0x0b:num[cishu]=6;cishu++;break; 
					case 0x07:num[cishu]=9;cishu++;break;  
				}
			}
		}
		while(P3!=0x7f);
	}
	P3=0xbf;
	P44=1;P42=0;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		delayms(10);
		temp=P3;
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			switch(temp)
			{
				case 0x0e:
				if(shezhi==1){num[cishu]=1;cishu++;}break;
				case 0x0d:
				if(shezhi==1){num[cishu]=4;cishu++;}break; 
				case 0x0b:
				if(shezhi==1){num[cishu]=7;cishu++;}break; 
				case 0x07:
				{
					if(shezhi==0) {shezhi=1;cishu=0;}
					else if((shezhi==1)&&(flag3==0)) shezhi=0;
				}
				break;  
			}	
		}
		while(P3!=0xbf);
	}

	P3=0xdf;
	P42=1;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		delayms(10);
		temp=P3;
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			if(shezhi==1)
			{
				switch(temp)
				{
					
					case 0x0e:num[cishu]=2;cishu++;break;
					case 0x0d:num[cishu]=5;cishu++;break; 
					case 0x0b:num[cishu]=8;cishu++;break; 
					case 0x07:{clear=1;cishu=0;qingchu=1;}break;  
				}
			}	
		}
		while(P3!=0xdf);
	}
}

void main()
{
	uchar wendu;
	banzi_init();
	Timer1Init();
	while(1)
	{
		if(flag1==1)
			{
				wendu=rd_temperature();
				flag1=0;
			}
		if(flag2==1)
			{
				keyscan();
				flag2=0;
			}
		if(shezhi==0)
			{
				dsbuff[7]=wendu%10;
				dsbuff[6]=wendu/10;
				dsbuff[5]=11;
				dsbuff[0]=10;
				dsbuff[1]=gongzuo;
				dsbuff[2]=10;
				dsbuff[3]=11;
				dsbuff[4]=11;
			}
	}
}

void Timer1() interrupt 3
{
		display();
		if(count0++==100)
		{
			count0=0;
			flag1=1;
		}
		
		if(count1++==1)
		{
			count1=0;
			flag2=1;
		}
}

