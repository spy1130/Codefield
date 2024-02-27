#include <stc15f2k60s2.h>
#include "onewire.h"
#define uchar unsigned char 
#define uint unsigned int
uchar code tab[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};
uchar dspbuf[]={10,10,10,10,10,10,10,10,10};
uchar shezhi[]={10,3,0,10,10,10,2,0};
uchar sa[]={10,10,10,10};
uchar keyvalue,dspcom=0,wendu,Tflag=0,keyflag=0,keyflag1=0,keyflag2=0,i=0;
uchar a1=0,a2=0;s1=0;d1,d2,d3,b7=0,b4=0,b5=0,b6=0,a4=0,a5=0,a6=0,b10=0;
uint a;
sbit h1=P3^0;
sbit h2=P3^1;
sbit h3=P3^2;
sbit h4=P3^3;
sbit l1=P4^4;
sbit l2=P4^2;
sbit l3=P3^5;
void panduan1();
void keyscan();
void display();
void Delay5ms()		
{
	unsigned char i, j;
	i = 54;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}
void ledon()// 开小灯1
{
	P0=0XFE;
	P2=0X80;
	P2=P2&0X1F;
}
void ledoff()//关小灯1
{
	 P0=0XFF;
	 P2=0X80;
	 P2=P2&0X1F;
}
void ledon1()// 开小灯2
{
	 P0=0XFd;
	 P2=(P2&0X1F)|0X80;
	 P2=P2&0X1F;
}
void ledoff1()// 关小灯2
{
	 P0=0XFf;
	 P2=(P2&0X1F)|0X80;
	 P2=P2&0X1F;
}
void relayon()//继电器开
{P0=0x10;
 P2=(P2&0X1F)|0XA0;
 P2&=0X1F;
}
void relayoff()//继电器关
{
	 P0=0x00;
	 P2=(P2&0X1F)|0XA0;
	 P2&=0X1F;
}

void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x9A;		//设置定时初值
	TH0 = 0xA9;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

void main()
{
  Timer0Init();
	EA=1;
	ET0=1;
	ET1=1;
	ledoff();
	relayoff();
	while(1)
	{ 
			if(Tflag==1)
			{
				wendu=duqu();
				Tflag=0;
			}
			if(keyflag1==0)
			{
				a1=0;
			}
				keyscan();
				panduan1();
			if(keyflag1==1)
			{ 
				if(a2==0)
				{shezhi[0]=11;
				shezhi[5]=11;
				shezhi[1]=10;
				shezhi[2]=10;
				shezhi[3]=10;
				shezhi[4]=10;
				shezhi[6]=10;
				shezhi[7]=10;a2=1;
				sa[0]=10;
				sa[1]=10;
				sa[2]=10;
				sa[3]=10;
				
				}
				if(keyflag!=0)
				{
				sa[a1]=keyvalue;
				a1++;
				shezhi[1]=sa[0];
				shezhi[2]=sa[1];
				shezhi[6]=sa[2];
				shezhi[7]=sa[3];
				}
				
				
			 if((shezhi[1]*10+shezhi[2])<(shezhi[6]*10+shezhi[7])&&a1==4)
			 {
					b7=1;
			 }
			 
			 if(keyflag2==1)
			 {
			  a1=0;
			  b7=0;
				sa[0]=10;
				sa[1]=10;
				sa[2]=10;
				sa[3]=10;
				shezhi[1]=sa[0];
				shezhi[2]=sa[1];
				shezhi[6]=sa[2];
				shezhi[7]=sa[3];
				keyflag2=0;
				ledoff1();
			 }
			 for(i=0;i<8;i++)
			 {
					dspbuf[i]=shezhi[i];
			 }
					s1=1;
			}
	}
}
  
void panduan1()
{   
	  if(keyflag1!=1)
		{
		s1=0;
		dspbuf[6]=wendu/10;
		dspbuf[7]=wendu%10;
		dspbuf[0]=11;
		dspbuf[2]=11;
		if(shezhi[1]*10+shezhi[2]<wendu)
		{
			dspbuf[1]=2;
		}
		
		else	if(shezhi[6]*10+shezhi[7]>wendu)
		{
			dspbuf[1]=0;
		}
	else	if(wendu>=shezhi[6]*10+shezhi[7]&&wendu<=shezhi[1]*10+shezhi[2])
		{
			dspbuf[1]=1;
		}
	}
}

void keyscan()
{
	keyflag=0;
	h1=0;h2=1;h3=1;h4=1;
	if(l1==0)
	 {
			Delay5ms();
			if(l1==0)
		 {
				while(!l1);
				keyvalue=0;keyflag=1;	 
		 }
	 }
   if(l2==0)
		{
			 Delay5ms();
			 if(l2==0)
			 {while(!l2);
				 keyvalue=1;keyflag=1;	 
			 }
		}
 if(l3==0)
	{
			Delay5ms();
		 if(l3==0)
		 {
			 while(!l3);
			 keyvalue=2;
			 keyflag=1;	 
		 }
  }
 
	h1=1;h2=0;h3=1;h4=1;
  if(l1==0)
 {
 Delay5ms();
	 if(l1==0)
	 {
	 while(!l1);
     keyvalue=3;
		 keyflag=1;	 
	 }
  }
   if(l2==0)
 {
	Delay5ms();
	 if(l2==0)
	 {
	 while(!l2);
     keyvalue=4;
		 keyflag=1;	 
	 }
  }
 if(l3==0)
 {
 Delay5ms();
	 if(l3==0)
	 {
	 while(!l3);
     keyvalue=5;
		 keyflag=1;	 
	 }
 }
	 
	 h1=1;h2=1;h3=0;h4=1;
  if(l1==0)
	 {
		Delay5ms();
		 if(l1==0)
		 {
				while(!l1);
			 keyvalue=6;
			 keyflag=1;	 
		 }
		}
   if(l2==0)
 {
 Delay5ms();
	 if(l2==0)
	 {
	 while(!l2);
     keyvalue=7;
		 keyflag=1;}
  }
 if(l3==0)
 {
 Delay5ms();
	 if(l3==0)
	 {
	 while(!l3);
     keyvalue=8;
		 keyflag=1;	 
	 }
  }
 h1=1;h2=1;h3=1;h4=0;
  if(l1==0)
 {
 Delay5ms();
	 if(l1==0)
	 {
	 while(!l1);
     keyvalue=9;
		 keyflag=1;	 
	 }
  }
   if(l2==0)
 {
 Delay5ms();
	 if(l2==0)
	 {
	 while(!l2);
     keyvalue=10;
		 keyflag1=1;
	   if(s1==1&&a1==4&&b7==0)
		 {
		 s1=0;
		 keyflag1=0;
		 a2=0;
		 }
	 }
  }
 if(l3==0)
 {
 Delay5ms();
	 if(l3==0)
	 {
	 while(!l3);
     keyvalue=11;
		 keyflag2=1;	 
	 }
  }
 
	
}	


void display()//显示函数
{
	 P0=0XFF;
	 P2=(P2&0X1F)|0XE0;
	 P2&=0X1F;
	 
	 P0=1<<dspcom;
   P2=(P2&0X1F)|0XC0;
   P2&=0X1F;
   
   P0=tab[dspbuf[dspcom]];
   P2=(P2&0X1F)|0XE0;
   P2=P2&0X1F;
  
  if(++dspcom==8)
   dspcom=0;		
}	
void T0()interrupt 1//中断响应T0
{
	display();
 if(++a==50)//100ms启动一次温度转换
 {
 Tflag=1;
 a=0;
 }
 if(++a4==400)
	 {
	 a4=0;
	 b4++;
	 }
	if(++a5==200)
	 {
	 a5=0;b5++;
	 }
	 if(++a6==100)
	 {
	 a6=0;b6++;
	 }
	 if(b4==2)
	 {
	 b4=0;
	 }
	 if(b5==2)
	 {
	 b5=0;
	 }
	 if(b6==2)
	 {
	 b6=0;
	 }
	if(dspbuf[1]==0&&keyflag1==0&&b7==0)
		{ 
			if(b4%2==0)
			{
			ledon();
			relayoff();
			}
			if(b4%2!=0)
			{
			ledoff();
			relayoff();
			}
		}
			
	if(dspbuf[1]==1&&keyflag1==0&&b7==0)
		{ 
			if(b5%2==0)
			{
			ledon();
			relayoff();
			}
			if(b5%2!=0)
			{
			ledoff();
			relayoff();
			}
		}
		if(dspbuf[1]==2&&keyflag1==0&&b7==0)
		{ 
			if(b6%2==0)
			{
			ledon();relayon();}
			if(b6%2!=0)
			{
			ledoff();
			relayon();
			}
		}
    if(b7==1)
		{
		ledon1();
		}
		if(keyflag1==1&&b7!=1)
		{
		ledoff();
		}

}
	

