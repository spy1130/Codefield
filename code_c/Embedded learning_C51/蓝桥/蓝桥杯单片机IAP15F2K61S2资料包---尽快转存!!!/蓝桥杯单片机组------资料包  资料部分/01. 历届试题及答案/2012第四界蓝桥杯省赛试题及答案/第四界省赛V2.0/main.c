#include <STC15F2K60S2.h>
#include <delay.h>
#include <iic.h>

#define uchar unsigned char 
#define uint unsigned int

uchar dsbuff[]={11,11,11,11,11,11,11,11};
unsigned char code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
uchar discount = 0;
uchar num_1 =0;

void allinit()
{
	P2=0x80;P0=0xff;
	P2=0xa0;P0=0x00;
	P2=0xc0;P0=0xff;
	P2=0xe0;P0=0xff;
}

void display()
{
	P2=0xef;
	P0=0xff;
	P2=0x1f;
		
	P2=0xcf;
	P0=1<<discount;
	P2=0x1f;

	P2=0xef;
	P0=tab[dsbuff[discount]];
	P2=0x1f;
	
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

void  main()
{
	allinit();
	Timer0Init();
	while(1)
	{
				 num_1 = iic_read(0x03);
				 dsbuff[7]=num_1%10;
				 dsbuff[6]=num_1/10;
		   	 dsbuff[4]=11;
		   	 dsbuff[3]=11;
		   	 dsbuff[1]=11;
		   	 dsbuff[0]=11;
	   }
}

void Time_0() interrupt 1
{
	display();
}




