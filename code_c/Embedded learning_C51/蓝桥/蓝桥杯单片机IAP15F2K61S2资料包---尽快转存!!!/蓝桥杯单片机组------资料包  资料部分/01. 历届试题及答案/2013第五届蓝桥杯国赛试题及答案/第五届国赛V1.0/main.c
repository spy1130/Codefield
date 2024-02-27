#include "STC15F2K60S2.H"
#include "onewire.h"

#define uint unsigned int
#define uchar unsigned char

uchar code tab[13]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff,0xc6};
uchar dsbuff[8]={11,11,11,11,11,11,11,11};
uchar discount;
uchar wendu;

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

void Timer0Init(void)		//2����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x9A;		//���ö�ʱ��ֵ
	TH0 = 0xA9;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	EA=1;
	ET0=1;
}

void main()
{
	Timer0Init();
	allinit();
	while(1)
	{
		wendu=read_temp();
		dsbuff[7]=wendu%10;
		dsbuff[6]=wendu/10;
	}
}

void Tmer0() interrupt 1
{
	display();
}