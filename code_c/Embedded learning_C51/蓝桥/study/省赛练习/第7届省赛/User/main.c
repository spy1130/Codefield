/* ͷ�ļ������� */
#include <STC15F2K60S2.H>//��Ƭ���Ĵ���ר��ͷ�ļ�
#include <Init.h>//��ʼ���ײ�����ר��ͷ�ļ�
#include <Led.h>//Led�ײ�����ר��ͷ�ļ�
#include <Key.h>//�����ײ�����ר��ͷ�ļ�
#include <Seg.h>//����ܵײ�����ר��ͷ�ļ�
#include <stdio.h>//��׼��ײ�����ר��ͷ�ļ�
#include "onewire.h"

/* ���������� */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//����ר�ñ���
unsigned char Key_Slow_Down;//��������ר�ñ���
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//�������ʾ���ݴ������
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//�����С�������ݴ������
unsigned char Seg_Pos;//�����ɨ��ר�ñ���
unsigned int Seg_Slow_Down;//����ܼ���ר�ñ���
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led��ʾ���ݴ������
unsigned char Work_Time;//����ʱ�����
unsigned char Wind_Mode;//���� 0-˯�߷� 1-��Ȼ�� 2- ����
unsigned char Time_Data[3] = {0,60,120};//����ʱ�䴢������
unsigned char Time_Data_Index;//ʱ�䴢������ָ��
bit Seg_Disp_Mode;//0-���� 1-�¶�
unsigned int Timer_1000Ms;//һǧ�����ʱ����
unsigned char Temperature;//ʵʱ�¶ȱ���
unsigned char Pwm_Count;
unsigned char Pwm_Level;


/* ���̴����� */
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//���̼��ٳ���

	Key_Val = Key_Read();//ʵʱ��ȡ����ֵ
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//��׽�����½���
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//��׽�����Ͻ���
	Key_Old = Key_Val;//����ɨ�����
	
	switch(Key_Down)
	{
		case 4:
			if(++Wind_Mode == 3)
				Wind_Mode = 0;
		break;
		case 5:
			if(++Time_Data_Index == 3)
				Time_Data_Index = 0;
			Work_Time = Time_Data[Time_Data_Index];
		break;
		case 6:
			Work_Time = 0;
		break;
		case 7:
			Seg_Disp_Mode ^= 1;
		break;
	}

}

/* ��Ϣ������ */
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//����ܼ��ٳ���
	Temperature = rd_temperature();
	switch(Wind_Mode)
	{
		case 0:
			Pwm_Level = 2;
		break;
		case 1:
			Pwm_Level = 3;
		break;
		case 2:
			Pwm_Level = 7;
		break;		
	}
	if(Seg_Disp_Mode == 0)
	{
		Seg_Buf[1] = Wind_Mode + 1;
		Seg_Buf[0] = Seg_Buf[2] = 11;
		Seg_Buf[4] = 0;
		Seg_Buf[5] = Work_Time / 100 % 10;
		Seg_Buf[6] = Work_Time / 10 % 10;
		Seg_Buf[7] = Work_Time % 10;
	}
	else
	{
		Seg_Buf[1] = 4;
		Seg_Buf[4] = 10;
		Seg_Buf[5] = Temperature / 10 % 10;
		Seg_Buf[6] = Temperature % 10;
		Seg_Buf[7] = 12;		
	}
}

/* ������ʾ���� */
void Led_Proc()
{
	unsigned char i;
	for(i=0;i<3;i++)
		ucLed[i] = (i == Wind_Mode) * Work_Time;//ֻҪWork_Time��Ϊ0�Ϳ���
}


/* ��ʱ��0�жϳ�ʼ������ */
void Timer0Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x18;		//���ö�ʱ��ʼֵ
	TH0 = 0xFC;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;    //��ʱ���ж�0��
	EA = 1;     //���жϴ�
}
void Timer1Init(void)		//100΢��@12.000MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x9C;		//���ö�ʱ��ʼֵ
	TH1 = 0xFF;		//���ö�ʱ��ʼֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;
	EA = 1;
}

/* ��ʱ��0�жϷ����� */
void Timer0Server() interrupt 1
{  
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//���̼���ר��
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//����ܼ���ר��
	if(++Seg_Pos == 8) Seg_Pos = 0;//�������ʾר��
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	if(Work_Time != 0)
	{
		if(++Timer_1000Ms == 1000)
		{
			Timer_1000Ms = 0;
			Work_Time--;
		}
	}
}

void Timer1Server() interrupt 3
{
	if(++Pwm_Count == 10) Pwm_Count = 0;
	if(Work_Time != 0)
	{
		if(Pwm_Level >= Pwm_Count)
			P34 = 1;
		else
			P34 = 0;
	}
	else
		P34 = 0;
}

/* Main */
void main()
{
	System_Init();
	Timer0Init();
	Timer1Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
		
	}
}