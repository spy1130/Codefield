#include <STC15F2K60S2.h>
#include <delay.h>
#include <ds13022.h>
#include <iic.h>

#define uchar unsigned char 
#define uint unsigned int

uchar dsbuff[]={11,11,10,11,11,10,11,11};
unsigned char code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
uchar discount = 0;
uchar num_1 =0;

void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
	for(j=110;j>0;j--);
}
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

void  main()
{
	allinit();
	Timer0Init();
	ds_write();
	while(1)
	{
	   ds_read();
	   num_1 = iic_read(0x03);
		 dsbuff[7]=num_1%10;
		 dsbuff[6]=num_1/10;
		 dsbuff[4]=shijian[1]%10;
		 dsbuff[3]=shijian[1]/10;
		 dsbuff[1]=shijian[2]%10;
		 dsbuff[0]=shijian[2]/10;
	   delayms(100);		
	 }
}

void Time_0() interrupt 1
{
	display();
}




