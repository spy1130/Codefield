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

void Timer1Init(void)		//2����@11.0592MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x9A;		//���ö�ʱ��ֵ
	TH1 = 0xA9;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	EA=1;
	ET1=1;
}

void display()
{
	P2 = (P2&0x1f)|0xe0;		//�򿪶�ѡ������
	P0 = 0xff;							//�ر����������
	P2 = P2&0x1f;
		
	P2 = (P2&0x1f)|0xc0;		//��λѡ������
	P0 = 1<<discount;
	P2 = P2&0x1f;
	
	P2 = (P2&0x1f)|0xe0;		//�򿪶�ѡ������
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

