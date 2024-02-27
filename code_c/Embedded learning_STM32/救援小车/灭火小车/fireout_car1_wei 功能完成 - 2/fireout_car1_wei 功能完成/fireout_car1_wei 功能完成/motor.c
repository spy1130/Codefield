#include "motor.h"

sbit Lefe_Motor = P1^1;
sbit Right_Motor = P1^3;


/*1������Ҫװ�ظߵ�ƽ��ֵ 2������Ҫװ�ص͵�ƽ��ֵ*/
unsigned char Load_Left_flag = 1; 
unsigned char Load_Right_flag = 1;

unsigned char flag_100ms = 5;

volatile unsigned int i = 0;

unsigned int Left_H_Speed = 1335;
unsigned int Left_L_Speed = 18330 - 1335;

unsigned int Right_H_Speed = 1335;
unsigned int Right_L_Speed = 18330 - 1335;


void Timer1_Load_RightMotor(unsigned int RightspeedVal)
{
	RightspeedVal = 65536 - RightspeedVal;
	
	TH1 = RightspeedVal >> 8;	
	TL1 = RightspeedVal & 0xff;	

	TR1 = 1;		//������ʱ��
}


void Timer0_Load_LeftMotor(unsigned int LeftspeedVal)
{
	LeftspeedVal = 65536 - LeftspeedVal;
	
	TH0 = LeftspeedVal >> 8;	
	TL0 = LeftspeedVal & 0xff;	

	TR0 = 1;		//������ʱ��
}



void Timer0() interrupt 1
{
	TR0 = 0;//��ʱ��0ֹͣ��ʱ
		
	switch(Load_Left_flag)
	{
		case(1):
		{
			Lefe_Motor = 1;
			Load_Left_flag = 2;
			Timer0_Load_LeftMotor(Left_H_Speed);
			i++;
			break;
		}
		case(2):
		{
			Lefe_Motor = 0;
			Load_Left_flag = 1;
			Timer0_Load_LeftMotor(Left_L_Speed);
			break;
		}
		default:break;
	}
}

void Timer1() interrupt 3
{
	TR1 = 0;//��ʱ��1ֹͣ��ʱ
	
	switch(Load_Right_flag)
	{
		case(1):
		{
			Right_Motor = 1;
			Load_Right_flag = 2;
			Timer1_Load_RightMotor(Right_H_Speed);
			break;
		}
		case(2):
		{
			Right_Motor = 0;
			Load_Right_flag = 1;
			Timer1_Load_RightMotor(Right_L_Speed);
			break;
		}
		default:break;
	}
}

void Timer0Init()		//��ʱ��0��ʼ�� 10us
{
	TMOD |= 0x01;	//���ö�ʱ��0ģʽ
	TH0 = 0XFF;		//���ö�ʱ��ֵ
	TL0=  0XF6;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF1��־
	TR0 = 1;		//��ʱ��1��ʼ��ʱ
	ET0 = 1;        //�򿪶�ʱ��0�ж�
	EA  = 1;        //�����ж�
}

void Timer1Init()      //��ʱ��1��ʼ�� 20us
{
	TMOD |= 0x10;	//���ö�ʱ��1ģʽ
	TH1 = 0XFF;        //���ö�ʱ��ֵ
	TL1 = 0XEC;        //���ö�ʱ��ֵ
	TF1 = 0;
	TR1 = 1;
	ET1 = 1;
	EA  = 1;
}

