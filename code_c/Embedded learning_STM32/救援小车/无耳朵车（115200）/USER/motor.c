

#include <stdio.h>

#include "stm32f10x.h"

#include "delay.h"

#include "cfg.h"


/*��е�����ж�������*/
/*�Գ����������������ڵ�һ��Ϊǰ��*/
/*�ӳ���󷽿���ȥ��е�����������Ϊ1�ŵ�����Ҳ�Ϊ2�ŵ������������Ϊ3��4��5�ŵ��*/
/*��ʱ��Tim1���ĸ������ֱ����1��2��3��4�ŵ����Tim2�ĵ�һ����������5�ŵ��������3��������Ч*/

void catop(void) 
{

	int n1,n2,n3,n5;
	double k=0;
	Tim1_Init(1500,1560,1500,1500);//ֱ��ֵ
	delay_ms(1000);
	Tim2_Init(2100,1600,270,250);//����������ʼ��λ��	2100 
	for(n2=1560;n2<=2070;n2=n2+2)
	{
	  n1=3000-n2;
	  n3=1500-0.55*k;
		Tim1_Init(n1,n2,n3,1400);
		k=k+3;
		delay_ms(10);
	} 
	delay_ms(10);
	
	for(n5=2100;n5<=2540;n5=n5+8)
			{
				Tim2_Init(n5,1600,270,250);
				delay_ms(10);	
			}
	
	delay_ms(1500);
	Tim1_Init(1500,1560,1500,1500);//ֱ��ֵ
	
}

void camid(void)  //ץ���м�һ��
{
	int n1,n2,n3,n5;
	double k=0;
	Tim1_Init(1500,1560,1500,1500);//ֱ��ֵ
	delay_ms(1000);
	Tim2_Init(2100,1600,270,250);//����������ʼ��λ��	1150  1700
   
	for(n2=1560;n2<=2200;n2=n2+3)
	{
	  n1=3100-n2;
	  n3=1500-0.55*k;
		Tim1_Init(n1,n2,n3,1300);
		k=k+4;
		delay_ms(10);
	} 
	delay_ms(500);
	
	for(n5=2100;n5<=2540;n5=n5+10)
			{
				Tim2_Init(n5,1600,270,250);
				delay_ms(10);	
			}
	
	delay_ms(1000);
	Tim1_Init(1500,1560,1500,1500);//ֱ��ֵ

}

void cabottom(void)  //ץ��������һ��
{
	int n1,n2,n3,n5;
	double k=0;
	Tim1_Init(1500,1560,1500,1500);//ֱ��ֵ
	delay_ms(1000);
	Tim2_Init(2100,1600,270,250);//����������ʼ��λ��	1150 
   	
	for(n2=1560;n2<=2250;n2=n2+2)
	{
	  n1=3000-n2;
	  n3=1500-0.5*k;
		Tim1_Init(n1,n2,n3,1100);
		k=k+4;
		delay_ms(10);
	}
	
	delay_ms(500);
	for(n5=2100;n5<=2540;n5=n5+10)
			{
				Tim2_Init(n5,1600,270,250);
				delay_ms(10);	
			}
	
	delay_ms(500);
	Tim1_Init(1500,1560,1500,1500);//ֱ��ֵ
	
}

void putbottom(void)  //���µ���һ��
{

	int n1,n2,n3,n5;
	double k=0;
	Tim1_Init(1500,1560,1500,1500);//ֱ��ֵ
	Tim2_Init(2540,1600,270,250); //��������ץ�Ķ���
	delay_ms(1000);
	
  	for(n2=1560;n2<=2250;n2=n2+2)
	{
	  n1=3000-n2;
	  n3=1500-0.5*k;
		Tim1_Init(n1,n2,n3,1100);
		k=k+4;
		delay_ms(10);
	}
	delay_ms(500);
	
	for(n5=2540;n5>=2100;n5=n5-10)
	{
		Tim2_Init(n5,1600,270,250);
		delay_ms(100);	
	}
	delay_ms(500);

	Tim1_Init(1500,1560,1500,1500);//ֱ��ֵ

}

void putbottom1(void)  //���µ���һ��
{

	int n1,n2,n3,n5;
	double k=0;
	Tim1_Init(1500,1560,1500,1500);//ֱ��ֵ
	Tim2_Init(2540,1600,270,250); //��������ץ�Ķ���
	delay_ms(1000);
	
  	for(n2=1560;n2<=2250;n2=n2+2)
	{
	  n1=3000-n2;
	  n3=1500-0.5*k;
		Tim1_Init(n1,n2,n3,1100);
		k=k+4;
		delay_ms(10);
	}
	delay_ms(500);
	
	for(n5=2540;n5>=2100;n5=n5-10)
	{
		Tim2_Init(n5,1600,270,250);
		delay_ms(100);	
	}
	delay_ms(500);

	Tim1_Init(1500,1560,1500,1500);//ֱ��ֵ

}

void putmid(void)  //�����м�
{
	int n1,n2,n3,n5;
	double k=0;
	Tim1_Init(1500,1560,1500,1500);//ֱ��ֵ
	Tim2_Init(2540,1600,270,250); //��������ץ�Ķ���
	delay_ms(1000);
	
   		for(n2=1560;n2<=2200;n2=n2+3)
	{
	  n1=3100-n2;
	  n3=1500-0.55*k;
		Tim1_Init(n1,n2,n3,1300);
		k=k+4;
		delay_ms(10);
	} 
	delay_ms(500);
	
	for(n5=2540;n5>=2100;n5=n5-10)
	{
		Tim2_Init(n5,1600,270,250);
		delay_ms(10);	
	}
	
	delay_ms(600);
	Tim1_Init(1500,1540,1500,1500);//ֱ��ֵ
}

 void puttop()  //����������һ��
{
int n1,n2,n3,n5;
	double k=0;
	Tim1_Init(1500,1560,1500,1500);//ֱ��ֵ
	Tim2_Init(2540,1600,270,250); //��������ץ�Ķ���
	delay_ms(1000);
	
   		for(n2=1560;n2<=2070;n2=n2+2)
	{
	  n1=3000-n2;
	  n3=1500-0.55*k;
		Tim1_Init(n1,n2,n3,1400);
		k=k+3;
		delay_ms(10);
	} 
	delay_ms(800);
	
	for(n5=2540;n5>=2100;n5=n5-5)
	{
		Tim2_Init(n5,1600,270,250);
		delay_ms(10);	
	}
	
	delay_ms(300);
	Tim1_Init(1500,1560,1500,1500);//ֱ��ֵ
}

void turnmotordown()//���ĸ������ת90�ȣ�������ץ���ٺ�£��ֹ���ϵ���
{
	int n4,n5;
	for(n4=1500;n4>=560;n4=n4-5)
	{
		Tim1_Init(1500,1560,1500,n4);
		delay_ms(10);
	} 
	
		delay_ms(500);
	for(n5=2545;n5>=2100;n5=n5-5)
	{
		Tim2_Init(n5,1600,270,250);
		delay_ms(10);	
	}
	delay_ms(800);
	for(n5=2100;n5<=2545;n5=n5+10)
	{
		Tim2_Init(n5,1600,270,250);
		delay_ms(10);	
	}
	
}

void turnmotorup() //�պ���ץ������е�ָֻ�ֱ����̬
{
	int n4,n5;
	for(n5=2540;n5<=2100;n5=n5+5)
	{
		Tim2_Init(n5,1600,270,250);
		delay_ms(10);	
	}
	delay_ms(500);
	for(n4=560;n4<=1500;n4=n4+5)
	{
		Tim1_Init(1500,1560,1500,n4);
		delay_ms(10);
	} 
	delay_ms(500);
}

void holdhand()//����ֱ����̬
{
	Tim1_Init(1500,1560,1500,1500);
	Tim2_Init(2540,1600,270,250);	
}

void turnmotor()//ת����ץ
{

}

