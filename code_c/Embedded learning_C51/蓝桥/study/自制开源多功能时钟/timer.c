# include "timer.h"

void Timer0Init(void)		//1����@12.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x20;		//���ö�ʱ��ֵ
	TH0 = 0xD1;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	
	ET0 = 1;
}


void Timer1Init(void)		//10����@12.000MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xF0;		//���ö�ʱ��ֵ
	TH1 = 0xD8;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	
	ET1 = 1;
	EA = 1;
}
