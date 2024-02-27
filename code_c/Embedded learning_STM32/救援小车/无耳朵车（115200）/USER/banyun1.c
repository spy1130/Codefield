#include "stm32f10x.h"
#include "run.h"
#include "delay.h"
#include "cfg.h"
#include <stdio.h>
#include "stdbool.h"
#include "ult.h"
#include "motor.h"
#include "tcs.h"
#include "CorrectDirection.h"
extern u16 dis;
/*����E��������һ����������8�����ϰ��˻����Ķ�������*/

char func=1;

uint8_t APnum=3;// A����ŵ�ɫ�����
uint8_t BPnum=3;
uint8_t CPnum=3;
uint8_t DPnum=3;
uint8_t EPnum=3;


void caAtop()	//��A��top
{
	int i;
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	delay_ms(20);
	OCtoOA();
	TurnRightAnyPulse(5);//��ƫת��������
	LCorrectDir();
	catop();
	delay_ms(1000);	
	TurnLeftAnyPulse(58);  //ת�򷵻�
	while(!IsMRightQtiBlack())
	{
		TurnLeftAnyPulse(10);
		//SpinLeft();
	}
	stop();
	
	for(i=0;i<110;i++)
	{
		MoveMiddleQti();
		delay_ms(10);
	}
	for(i=0;i<23;i++)
	{
		MoveSlowQti();
		delay_ms(20);
	}
	stop();
	delay_ms(100);
	
	stop();
////	turnmotor();
	delay_ms(50);
	TurnLeftAnyPulse(6);
	putbottom1();
	delay_ms(1000);
	Turn180();
	stop();
}

void caAmid()	//A���mid
{
	int i;
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	delay_ms(20);
	OCtoOA();
	delay_ms(20);
	TurnRightAnyPulse(7);//��ƫת��������
	LCorrectDir();
	camid();
	delay_ms(1000);
	TurnLeftAnyPulse(58);
	while(!IsMRightQtiBlack())
	{
		TurnLeftAnyPulse(10); 
	}

	stop();
	for(i=0;i<100;i++)
	{
		MoveMiddleQti();
		delay_ms(10);
	}
	delay_ms(100);
	i=0;
	while(1)
	{
		MoveQti();
		delay_ms(20);
		PulseOut(leftservo,1500);//��ֹ������ʧЧ�ļ��ٴ�ʩ//ɾ�����Լ��٣���������ܻ����ƫ��
		PulseOut(rightservo,1500);
		delay_ms(20);
		dis = Get_Distance();
		if(dis<190 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}
	stop();
	delay_ms(10);
	while(1)
	{
		MoveSlowQti();
		delay_ms(20);
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(20);
		dis = Get_Distance();
		if(dis<168 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}
	
	stop();
//	turnmotor();
	TurnLeftAnyPulse(4);
	putmid();
	delay_ms(1000);
	Turn180();
	stop();
}

void caAbottom()	//��A��bottom
{
	int i;
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	delay_ms(20);
	OCtoOA();
	delay_ms(20);
	TurnRightAnyPulse(5);//��ƫת��������
	LCorrectDir();
	cabottom();
	delay_ms(1000);

	TurnLeftAnyPulse(58);
	while(!IsMRightQtiBlack())
	{
		TurnLeftAnyPulse(10);
	}
  
	stop();
	for(i=0;i<100;i++)
	{
		MoveMiddleQti();
	}
	stop();
	delay_ms(10);
	i=0;
	while(1)
	{
		MoveQti();
		delay_ms(20);
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(20);
		dis = Get_Distance();
		if(dis<190 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}
	stop();
	while(1)
	{
		MoveSlowQti();
		delay_ms(20);
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(20);
		dis = Get_Distance();
		if(dis<167 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}

	stop();
//	turnmotor();
	TurnLeftAnyPulse(5);
	puttop();
	delay_ms(1000);
	Turn180();
	stop();
}

void CarryAtoO(void) //	��A�����
{
	switch(APnum)
	{
		case 3:
			caAtop();
			break;
		case 2:
			caAmid();
			break;
		case 1:
			caAbottom();
			break;
		default:
			func = 2;
			break;
	}
	APnum = APnum - 1;
}

void caCtop()//��C��top
{
	int i;
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	TurnRightAnyPulse(6);//��ƫת��������
	LCorrectDir();
	catop();
	delay_ms(1000);
	Turn180();
	stop();
	
	for(i=0;i<100;i++)
	{
		MoveMiddleQti();
		delay_ms(10);
	}
	stop();
	
	delay_ms(10);
	i=0;
	while(1)
	{
		MoveQti();
		delay_ms(20);
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(20);
		dis = Get_Distance();
		if(dis<300 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}
	stop();
	delay_ms(10);
	while(1)
	{
		MoveSlowQti();
		delay_ms(20);
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500); 
		delay_ms(20);
		dis = Get_Distance();
		if(dis<280 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}
	stop();
	TurnLeftAnyPulse(2);
	putbottom1();
	delay_ms(1000);
	Turn180();
	stop();

}



void caCmid()  //��C��mid
{
	int i;
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	TurnRightAnyPulse(6);//��ƫת��������
	LCorrectDir();
	camid();
	delay_ms(1000);
	Turn180();


	stop();
	
	for(i=0;i<60;i++)
	{
		MoveMiddleQti();
		//		MoveSlowQti();
		delay_ms(5);
	}
	stop();
	delay_ms(10);
	i=0;
	while(1)
	{
		MoveQti();
		delay_ms(20);
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(20);
		dis = Get_Distance();
		if(dis<190 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}
	stop();
	while(1)
	{
		MoveSlowQti();
		delay_ms(20);
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(20);
		dis = Get_Distance();
		if(dis<168 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}
	stop();
	TurnLeftAnyPulse(2);
	putmid();
	delay_ms(1000);
	Turn180();
	stop();
}



void caCbottom() //��C��bottom
{
	
	int i;
	
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	TurnRightAnyPulse(6);//��ƫת��������
	LCorrectDir();
	cabottom();
	delay_ms(1000);
	Turn180();
	
	stop();
	
	for(i=0;i<60;i++)
	{
		MoveMiddleQti();
		delay_ms(5);
	}
	stop();
	
	delay_ms(10);
	i=0;
	while(1)
	{
		MoveQti();
		delay_ms(20);
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(20);
		dis = Get_Distance();
		if(dis<190 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}
	stop();
	while(1)
	{
		MoveSlowQti();
		delay_ms(20);
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(20);
		dis = Get_Distance();
		if(dis<168 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}

	if(IsMLeftQtiBlack())
	{
		while(IsMLeftQtiBlack())
		{
			SpinLeft();
		}

	}
	else if(IsMRightQtiBlack())
	{
		while(IsMRightQtiBlack())
		{
			SpinRight();
		}
	}
	stop();
//	turnmotor();
	TurnLeftAnyPulse(2);
	puttop();
	delay_ms(1000);
	Turn180();
	stop();
}

void CarryCtoO(void)   //��C�������
{
	switch(CPnum)
	{
	case 3:
		caCtop();
		break;
	case 2:
		caCmid();
		break;
	case 1:
		caCbottom();
		break;
	default:
		func = 3;
		break;
	}
	CPnum = CPnum - 1;
}



void caEtop()  //��E��top
{
	int i;
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	delay_ms(20);
	OCtoOE();
	delay_ms(20);
	TurnRightAnyPulse(2);//��ƫת��������(ע��Ӧ����)
	LCorrectDir();
	TurnLeftAnyPulse(12);//��ƫת��������(ע��Ӧ����)
	catop();
	delay_ms(800);
	for(i=0;i<5;i++)
	{
		GoBack();
		delay_ms(10);
	}
	delay_ms(800);
  TurnRightAnyPulse(80);//ת�򷵻�
	stop();
	
	delay_ms(500);
	for(i=0;i<30;i++)
	{
		MoveSlowQti();
		delay_ms(10);
	}
	stop();
	delay_ms(10);
	i=0;
	while(1)
	{
		MoveSlowQti();
		delay_ms(20);
//		PulseOut(leftservo,1500);
//		PulseOut(rightservo,1500);
//		delay_ms(10);
		dis = Get_Distance();
		if(dis<290 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}
	stop();
	
	while(1)
	{
		MoveSlowQti1();
		delay_ms(10);
		PulseOut(leftservo,1500);//
		PulseOut(rightservo,1500);//
		delay_ms(10);
		dis = Get_Distance();
		if(dis<265 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}
	
	stop();
//	turnmotor();
	TurnLeftAnyPulse(2);
	putbottom1();
	delay_ms(1000);
	stop();
	Turn180();
	stop();
} 



void caEmid()	 //��E��mid
{
	int i;
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	delay_ms(20);
	OCtoOE();
	delay_ms(20);
	TurnRightAnyPulse(2);//��ƫת��������(ע������)
    LCorrectDir();
    TurnLeftAnyPulse(12);//��ƫת��������(ע��Ӧ����)
	camid();
	delay_ms(800);
	
	for(i=0;i<10;i++)
	{
		GoBack();
		delay_ms(10);
	}
	delay_ms(800);
	TurnRightAnyPulse(80);//ת�򷵻�
	


	for(i=0;i<30;i++)
	{
		MoveSlowQti();
		delay_ms(10);
		
	}
	stop();
	delay_ms(100);
	i=0;
	while(1)
	{
		MoveSlowQti();
		delay_ms(10);
//		PulseOut(leftservo,1500);
//		PulseOut(rightservo,1500);
//		delay_ms(10);
		dis = Get_Distance();
		if(dis<190 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}
	stop();
	while(1)
	{
		MoveSlowQti1();
		delay_ms(10);
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(10);
		dis = Get_Distance();
		if(dis<169 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}
	
	stop();
//	turnmotor();
	TurnLeftAnyPulse(2);
	putmid();
	delay_ms(1000);
	stop();
	Turn180();
	stop();

}
