/* ͷ�ļ������� */
#include <STC15F2K60S2.H>//��Ƭ���Ĵ���ר��ͷ�ļ�
#include <Init.h>//��ʼ���ײ�����ר��ͷ�ļ�
#include <Led.h>//Led�ײ�����ר��ͷ�ļ�
#include <Key.h>//�����ײ�����ר��ͷ�ļ�
#include <Seg.h>//����ܵײ�����ר��ͷ�ļ�
#include <stdio.h>//��׼��ײ�����ר��ͷ�ļ�
#include <onewire.h>
#include <iic.h>
/* ���������� */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//����ר�ñ���
unsigned char Key_Slow_Down;//��������ר�ñ���
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//�������ʾ���ݴ������
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//�����С�������ݴ������
unsigned char Seg_Pos;//�����ɨ��ר�ñ���
unsigned int Seg_Slow_Down;//����ܼ���ר�ñ���
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led��ʾ���ݴ������
bit Seg_Disp_Mode;//0-���ݽ��棬1-��������
float temperature;
unsigned char Temperature_Params_Disp[2];//�¶Ȳ�����ʾ����
unsigned char Temperature_Params_Ctrol[2] = {30,20};//�¶Ȳ�����������
unsigned char Temperature_Params_Index;//�¶Ȳ�������
bit ERROR;
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
	{	//�����л�
		case 4:Seg_Disp_Mode ^=1;
		//��λ
		if(Seg_Disp_Mode==1){
			//������ֵ��ֵ����ʾ���� �����޸�����
		Temperature_Params_Disp[0]=Temperature_Params_Ctrol[0];
		Temperature_Params_Disp[1]=Temperature_Params_Ctrol[1];
		Temperature_Params_Index=0;
		}
		else{
		if(Temperature_Params_Disp[0]>=Temperature_Params_Disp[1])
		{	//����ʾֵ��ֵ���������� ���ڱ�������
			Temperature_Params_Ctrol[0]=Temperature_Params_Disp[0];
			Temperature_Params_Ctrol[1]=Temperature_Params_Disp[1];
			ERROR=0;
		}
		else{ERROR=1;}
		}
		break;
		case 5://ģʽ�л�
			if(Seg_Disp_Mode==1)
			Temperature_Params_Index ^=1;
		break;
		case 6://�Լ�
			if(Seg_Disp_Mode==1)
			{	if(++Temperature_Params_Disp[Temperature_Params_Index]==100)
				Temperature_Params_Disp[Temperature_Params_Index]=99;
			}
		break;
		case 7://�Լ�
			if(Seg_Disp_Mode==1)
			{	if(--Temperature_Params_Disp[Temperature_Params_Index]==255)
				Temperature_Params_Disp[Temperature_Params_Index]=0;
			}
		break;
	}
}

/* ��Ϣ������ */
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//����ܼ��ٳ���
	temperature= rd_temperature();
	if(!Seg_Disp_Mode)
	{	//0-���ݽ���
		Seg_Buf[0]=12;//C
		Seg_Buf[3]=10;
		Seg_Buf[4]=10;
		Seg_Buf[6]=(unsigned char)temperature /10 %10;
		Seg_Buf[7]=(unsigned char)temperature %10;
	}
	else
	{//1-��������
		Seg_Buf[0]=13;//P
		Seg_Buf[3]=Temperature_Params_Disp[0]/10 %10;
		Seg_Buf[4]=Temperature_Params_Disp[0] %10;
		Seg_Buf[6]=Temperature_Params_Disp[1] /10 %10;
		Seg_Buf[7]=Temperature_Params_Disp[1] %10;
		
	}
	

}

/* ������ʾ���� */
void Led_Proc()
{	unsigned char i;
	ucLed[0]=(temperature>Temperature_Params_Disp[0]);
	ucLed[1]=((temperature<=Temperature_Params_Disp[0])&&(temperature>=Temperature_Params_Disp[1]));
	ucLed[2]=(temperature<Temperature_Params_Disp[1]);
	ucLed[3]=ERROR;
	for(i=0;i<3;i++)
	{	if(ucLed[i]){
		Da_Write(51*(4-i));
		break;
		}
	}
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

/* ��ʱ��0�жϷ����� */
void Timer0Server() interrupt 1
{  
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//���̼���ר��
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//����ܼ���ר��
	if(++Seg_Pos == 8) Seg_Pos = 0;//�������ʾר��
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
}

/* ��ʱ���� */
void Delay750ms()		//@12.000MHz
{
	unsigned char i, j, k;
	i = 35;
	j = 51;
	k = 182;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


/* Main */
void main()
{	rd_temperature();//�ϵ��ȡһ���¶Ȳ�����ʱ750MS�������ݳ���85
	Delay750ms();
	System_Init();
	Timer0Init();
	
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
		
	}
}