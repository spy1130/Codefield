#include "STC15F2K60S2.h"
#include "onewire.h"

#define uint unsigned int
#define uchar unsigned char

uchar discount;
uchar dsbuff[8]={10,0,10,11,0,0,0,0};
uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF,0XC6};
uchar key1=0,key2=0,key3=0,key4=0;
uchar count0,count1;
uchar flag1,flag2;
uchar shijian=0;
uchar tt=0;
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
	P2=0XC0;P0=0XFF;
	P2=0XE0;P0=0XFF;
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
	if(P30==0)
	{
		delayms(10);
		if(P30==0)
		{
			key1=1;
			key2=0;
			key3=0;
			key4=0;
		}
		while(!P30);
	}
	if(P31==0)
	{
		delayms(10);
		if(P31==0)
		{
			key2=1;
			key1=0;
			key3=0;
			key4=0;
		}
		while(!P31);
	}
	if(P32==0)
	{
		delayms(10);
		if(P32==0)
		{
			key3=1;
			key2=0;
			key1=0;
			key4=0;
			tt++;
			shijian=60;
		}
		while(!P32);
	}
	if(P33==0)
	{
		delayms(10);
		if(P33==0)
		{
			key4++;
			key2=0;
			key3=0;
			key1=0;
			if(key4==4) key4=1;
		}
		while(!P33);
	}
}

void Timer1Init(void)		//5毫秒@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x00;		//设置定时初值
	TH1 = 0x28;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	EA=1;
	ET0=1;
}

void main()
{
	uchar wendu;
	allinit();
	Timer0Init();
	while(1)
	{
		if(flag1==1)
		{
			flag1=0;
			key();
		}
		if(key4==1)
		{
			P2=0X80;P0=0X7F;
			dsbuff[7]=shijian%10;
			dsbuff[6]=shijian/10%100;
			dsbuff[5]=shijian/100%10;
			dsbuff[4]=shijian/1000;
			dsbuff[2]=10;
			dsbuff[1]=1;
			dsbuff[0]=10;
		}
		if(key4==2)
		{
			P2=0X80;P0=0XBF;
			dsbuff[7]=shijian%10;
			dsbuff[6]=shijian/10%100;
			dsbuff[5]=shijian/100%10;
			dsbuff[4]=shijian/1000;
			dsbuff[2]=10;
			dsbuff[1]=2;
			dsbuff[0]=10;
		}
		if(key4==3)
		{
			P2=0X80;P0=0XDF;
			dsbuff[7]=shijian%10;
			dsbuff[6]=shijian/10%100;
			dsbuff[5]=shijian/100%10;
			dsbuff[4]=shijian/1000;
			dsbuff[2]=10;
			dsbuff[1]=3;
			dsbuff[0]=10;
		}
		if(key1==1)
		{
			wendu=readtemp();
			dsbuff[7]=12;
			dsbuff[6]=wendu%10;
			dsbuff[5]=wendu/10;
			dsbuff[4]=11;
			dsbuff[3]=11;
			dsbuff[2]=10;
			dsbuff[1]=4;
			dsbuff[0]=10;
		}
		
	}
}

void Timer0() interrupt 1
{
	display();
	count0++;
	count1++;
	if(count0==1) 
	{
		count0=0;
		flag1=1;
	}
}

void Timer1() interrupt 3
{
	if(key3==1)
	{
		count1++;
		if(count1==200) 
		{
			count1=0;
			shijian=tt*shijian;
			shijian--;
			if(shijian==0) shijian=0;
		}
	}
}