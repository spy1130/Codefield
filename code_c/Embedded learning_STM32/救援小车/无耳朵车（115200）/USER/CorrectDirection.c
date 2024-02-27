
#include "stm32f10x.h"
#include "run.h"
#include "delay.h"
#include "cfg.h"
#include <stdio.h>
#include "stdbool.h"
#include "ult.h"
#include "motor.h"
#include "tcs.h"
//extern u16 dis;

/*��׼������*/

extern u16 dis;
unsigned long mindis=65535;
char cnts1=0,cnts2=0;

void LCorrectDir(void) //ת���ж�������˿����//�Ƿ�ʼ����
{
	cnts1=0;
	cnts2=0;
	delay_ms(100);
	mindis=500;
	while(1)
	{
		PulseOut(leftservo,1478);
		PulseOut(rightservo,1474);
		delay_ms(20);
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(20);						 
		dis = Get_Distance();//����ĵ�λ��mm
		delay_ms(55);
		if(!(IsMRightQtiBlack()||IsMLeftQtiBlack()))
		{
			stop();
			break;
		}
	}
	delay_ms(500);
	TurnLeftAnyPulse(4);
}

void LCorrectDir1(void)//��������׼�����������ʧЧ�������޷���׼
{
	cnts1=0;
	cnts2=0;
	delay_ms(100);
	mindis=500;
	while(1)
	{
		PulseOut(leftservo,1478);
		PulseOut(rightservo,1474);
		delay_ms(20);
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(20);						 
		dis = Get_Distance();//����ĵ�λ��mm
		delay_ms(55);
		if(dis < mindis && dis < 300)
		{
			cnts1++;
			if(cnts1==4)
			{
				mindis = dis;
				cnts1=0;
				cnts2=0;
			}
		}
		else if(dis > mindis && dis < 300)
		{
			cnts2++;
			if(cnts2>=11)
			{
				stop();
				break;
			}
		}
	}
	TurnLeftAnyPulse(2);
	delay_ms(500);
}


void Direction(void)
{
//	int i;
//	while(i<=3)
//	{
//		if( IsMRightQtiBlack()&&(!IsMLeftQtiBlack()) )
//		{
//			TurnRightAnyPulse(3);
//		}
//		else if( (!IsMRightQtiBlack())&&IsMLeftQtiBlack() )
//		{
//			TurnLeftAnyPulse(3);
//		}
//		else if( !(IsMRightQtiBlack()&&IsMLeftQtiBlack()) )
//		{
//			stop();
//			break;
//		}
//			i++;
//	}
}

void RCorrectDir(void)//������ƫȻ������ת��׼
{
	TurnRightAnyPulse(50);//��ƫת��������
	stop();
	cnts1=0;
	cnts2=0;
	delay_ms(100);
	mindis=500;
	while(1)
	{
		int i;
		PulseOut(leftservo,1508);
		PulseOut(rightservo,1508);
		delay_ms(20);
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(15);
		dis = Get_Distance();
		if(dis < mindis && dis < 500)
		{
			cnts1++;
			if(cnts1==3)
			{
				mindis = dis;
				cnts1=0;
				cnts2=0;
			}
		}
		else if(dis > mindis && dis < 500)
		{
			cnts2++;
			if(cnts2>=20)
			{
				stop();
				for(i=0;i<=30;i++)
				{
					PulseOut(leftservo,1492);
					PulseOut(rightservo,1492);
					delay_ms(15);
				}
				cnts2=0;
				stop();
				break;
			}
		}
	}
	while(1)
	{
		if(Get_Distance()>135)
		{
			slowforward();
		}
		else if(Get_Distance()<130)
		{
			GoBack();
		}
		else
		{
			stop();
			break;
		}
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(15);
	}
}



