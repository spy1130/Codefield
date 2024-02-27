#include <STC15F2K60S2.h>
#include <intrins.h>
#include <ds1302.h>

#define uint unsigned int
#define uchar unsigned char
#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}
#define TX P10
#define RX P11
uchar code tab[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar dsbuff[8]={11,11,11,11,11,11,11,11};
uchar discount;
uint distance;
uint distance1;
uchar count0;
uchar flag1;
uint t;
uint baojingdistance=20;

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

void sendwave()
{
	uchar i;
	for(i=0;i<7;i++)
	{
		TX=1;
		somenop;somenop;somenop;somenop;somenop;
		somenop;somenop;somenop;somenop;somenop;
		TX=0;
		somenop;somenop;somenop;somenop;somenop;
		somenop;somenop;somenop;somenop;somenop;
	}
}

uint getdistance()
{
	sendwave();  //发送方波信号
	TR1=1;  //启动计时
	while((RX==1) && (TF1==0));  //等待收到脉冲
	TR1=0;  //关闭计时		
	if(TF1==1)//发生溢出
		{
			TF1=0;
			distance=99;  //无返回
		}
	else
		{		
			t=TH1;// 计算时间  
			t<<=8;
			t|=TL1;
			distance=(uint)(t*0.017);  //计算距离	
			distance=distance/12;
		}
	TH1=0;
	TL1=0;
	return distance;
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

void Timer1Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0;				//设置定时初值
	TH1 = 0;				//设置定时初值
	TF1 = 0;				//清除TF1标志
	TR1 = 0;				//定时器1开始计时
}

void main()
{
	allinit();
	Timer0Init();
	Timer1Init();
	dswrite();
	while(1)
	{
		dsread();
		if(flag1==1)
		{
		flag1=0;
		distance1=getdistance();
		}
		dsbuff[7]=shijian[0]%10;
		dsbuff[6]=shijian[0]/10;
		dsbuff[5]=10;
		dsbuff[4]=shijian[1]%10;
		dsbuff[3]=shijian[1]/10;
		dsbuff[2]=10;
		dsbuff[1]=shijian[2]%10;
		dsbuff[0]=shijian[2]/10;
		
//		dsbuff[7]=distance1%10;
//		dsbuff[6]=distance1/10;
//		
//		dsbuff[1]=baojingdistance%10;
//		dsbuff[0]=baojingdistance/10;
		
		if(distance1<baojingdistance)
		{
			P2=0xa0;
			P0=0x40;
		}
		else 
		{
			P2=0xa0;
			P0=0x00;
		}
	}
}

void Timer0() interrupt 1
{
	display();
	count0++;
	if(count0==100)
	{
	count0=0;
	flag1=1;
	}
}