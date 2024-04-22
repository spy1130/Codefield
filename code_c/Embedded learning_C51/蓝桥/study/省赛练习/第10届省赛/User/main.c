/* ͷ�ļ������� */
#include <STC15F2K60S2.H>//��Ƭ���Ĵ���ר��ͷ�ļ�
#include <Init.h>//��ʼ���ײ�����ר��ͷ�ļ�
#include <Led.h>//Led�ײ�����ר��ͷ�ļ�
#include <Key.h>//�����ײ�����ר��ͷ�ļ�
#include <Seg.h>//����ܵײ�����ר��ͷ�ļ�
#include <stdio.h>//��׼��ײ�����ר��ͷ�ļ�
#include "iic.h"//��ģת���ײ�����ר��ͷ�ļ�

/* ���������� */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//����ר�ñ���
unsigned char Key_Slow_Down;//��������ר�ñ���
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//�������ʾ���ݴ������
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//�����С�������ݴ������
unsigned char Seg_Pos;//�����ɨ��ר�ñ���
unsigned int Seg_Slow_Down;//����ܼ���ר�ñ���
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led��ʾ���ݴ������
unsigned char Seg_Disp_Mode;//0-Ƶ����ʾ���棬1-��ѹ��ʾ����
unsigned int fre;//Ƶ�ʱ���
float Vol;//��ѹ����
bit Vol_Output_Mode;//0-��λ�����ģʽ��1-�̶����ģʽ
float Vol_Output;//��ʵ��ѹ���
bit Led_Enable_Flag = 1;//Ledʹ�ܱ�־λ Ĭ�Ͽ���
bit Seg_Enable_Flag = 1;//�����
unsigned int  Timer_1000Ms;
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
			Seg_Disp_Mode ^=1;
		break;
		case 5:
			Vol_Output_Mode ^=1;
		break;
		case 6:
			Led_Enable_Flag ^=1;
		break;
		case 7://�������ʾ���ܿ���
			Seg_Enable_Flag ^= 1;
		break;
			
		
	}
}

/* ��Ϣ������ */
void Seg_Proc()
{	
	unsigned char i=2;
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//����ܼ��ٳ���
	Vol = Ad_Read(0x43) / 51.0;//ʵʱ��ȡ��λ����ѹֵ
	if(Vol_Output_Mode==0)
		Vol_Output=2;
	else
		Vol_Output=Vol;
	if(Seg_Disp_Mode==0){//����Ƶ����ʾ
	Seg_Point[5]=0;
	Seg_Buf[0]=11;//F
	Seg_Buf[2]=fre/100000%10;
	Seg_Buf[3]=fre/10000%10;
	Seg_Buf[4]=fre/1000%10;
	Seg_Buf[5]=fre/100%10;
	Seg_Buf[6]=fre/10%10;
	Seg_Buf[7]=fre%10;
		
	while(Seg_Buf[i]==0)
	{
		Seg_Buf[i]=10;
		if(++i==7)break;
	}
	}
	else//���ڵ�ѹ��ʾ
	{
		Seg_Buf[0]=12;//U
		Seg_Buf[4]=Seg_Buf[3]=10;
		Seg_Buf[5]=(unsigned char)Vol_Output;
		Seg_Buf[6]=(unsigned int)(Vol_Output * 100)/10%10;
		Seg_Buf[7]=(unsigned int)(Vol_Output * 100)%10;
		Seg_Point[5]=1;
	}
}

/* ������ʾ���� */
void Led_Proc()
{	unsigned char i;
	/*DAC���*/
	Da_Write(Vol_Output*51);//ʵʱ�����ѹֵ
	for(i=0;i<2;i++)
	{
		ucLed[i]=(i=Seg_Disp_Mode);
		ucLed[2] = ((Vol >= 1.5 && Vol< 2.5) || (Vol >= 3.5));
		ucLed[3] = ((fre >= 1000 && fre< 5000) || (fre >= 10000));
		ucLed[4] = Vol_Output_Mode;
	}
}


/* ������0�жϳ�ʼ������ */

void Timer0_Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TMOD |= 0x05;			//����16λ�����ؼ���ģʽ
	TL0 = 0;				//���ö�ʱ��ʼֵ
	TH0 = 0;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
}
/* ��ʱ��1�жϳ�ʼ������ */
void Timer1_Init(void)		//1����@12.000MHz
{
	AUXR &= 0xBF;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TL1 = 0x18;				//���ö�ʱ��ʼֵ
	TH1 = 0xFC;				//���ö�ʱ��ʼֵ
	TF1 = 0;				//���TF1��־
	TR1 = 1;				//��ʱ��1��ʼ��ʱ
	ET1=1;					//��ʱ���ж�1��
	EA=1;					//���жϴ�		
}

/* ��ʱ��0�жϷ����� */
void Timer1Server() interrupt 3
{  
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//���̼���ר��
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//����ܼ���ר��
	if(++Seg_Pos == 8) Seg_Pos = 0;//�������ʾר��
	if(Seg_Enable_Flag == 1) //�����ʹ��
		Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	else
		Seg_Disp(Seg_Pos,10,0); //Ϩ�����������
	if(Led_Enable_Flag == 1) //Ledʹ��
		Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	else
		Led_Disp(Seg_Pos,0); //Ϩ������Led
	if(++Timer_1000Ms == 1000) //ʵʱ��ȡƵ��ֵ
	{
		Timer_1000Ms = 0;
		fre = TH0 << 8 | TL0;
		TH0 = TL0 = 0;
	}

}



/* Main */
void main()
{
	System_Init();
	Timer0_Init();	
	Timer1_Init();	
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
		
	}
}