# include "STC15F2K60S2.H"
# include "init.h"
# include "timer.h"
# include "stdio.h"
#include "LED.h"


//-----------------------------------------------
/* ȫ�ֱ������� */


//���ݴ����������仯
unsigned char count=0;//׼����ʾ��������ֵ	


/* main */
void main()
{
	//���ݴ����������仯
	init_close();//�ر�����
	Timer0_Init();//��ʱ��0��ʼ��������ʹ�ܶ�ʱ��1�жϣ�1ms����һ��
	EA = 1;//�����ж�

	while(1)
	{
		
	}


}

//-----------------------------------------------
/* Timer1_interrupt routine */
void tm1_isr() interrupt 1
{	
	
	count++;
	if(count % 10 == 0)
	{
		LED(0X01);
	}
	else
	{
		LED(0X00);
	}
	
	if(count == 100)
	{
		LED(0X80);
		count = 0;
	}
	else
	{
		LED(0X00);
	}

	
}

