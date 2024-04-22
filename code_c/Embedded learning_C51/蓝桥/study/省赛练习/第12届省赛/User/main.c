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
unsigned char Seg_Disp_Mode;//��ʾģʽ���� 0-�¶���ʾ 1-�������� 2-DAC���
float temperature;
unsigned char temp_disp=25;//�ݴ���ʾ
unsigned char temp_ctrl=25;//�ݴ����
float Voltage_Output;//�����ѹ
bit Output_Mode;//��ѹ���ģʽ�л�
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
			if(++Seg_Disp_Mode==3) Seg_Disp_Mode=0;
			if(Seg_Disp_Mode==1) temp_disp=temp_ctrl;//��ʼ��ʾ
			if(Seg_Disp_Mode==2) temp_ctrl=temp_disp;//������ʾ
		break;
		case 8:
			if(Seg_Disp_Mode==1){
			if(--temp_disp==255) temp_disp=0;
			}
		break;
		case 9:
			if(Seg_Disp_Mode==1){
			if(++temp_disp==100) temp_disp=99;
			}
		break;
		case 5:
			Output_Mode ^=1;
		break;		
	}
}

/* ��Ϣ������ */
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//����ܼ��ٳ���
	temperature=rd_temperature();
	switch(Seg_Disp_Mode)
	{
		case 0://�¶�
			Seg_Buf[0]=12;//C
			Seg_Buf[4]=(unsigned char)temperature /10 %10;
			Seg_Buf[5]=(unsigned char)temperature %10;
			Seg_Buf[6]=(unsigned int)(temperature*100) /10%10;
			Seg_Buf[7]=(unsigned int)(temperature*100) %10;
			Seg_Point[5]=1;
		break;
		case 1://����
			Seg_Buf[0]=13;//P
			Seg_Buf[4]=10;
			Seg_Buf[5]=10;
			Seg_Point[5]=0;
			Seg_Buf[6]=temp_disp /10%10;
			Seg_Buf[7]=temp_disp %10;
			
		break;
		case 2://DAC
			Seg_Buf[0]=11;//A
			Seg_Buf[5]=(unsigned char)Voltage_Output %10;
			Seg_Buf[6]=(unsigned int)(Voltage_Output*100) /10%10;
			Seg_Buf[7]=(unsigned int)(Voltage_Output*100) %10;
			Seg_Point[5]=1;
		break;
	}
}

/* ������ʾ���� */
void Led_Proc()
{	unsigned char i;
	/*--DAC--*/
	if(Output_Mode==0)
	{
		if(temperature<temp_ctrl)
		{
			Voltage_Output=0;
		}
		else{
			Voltage_Output=5;
		}
	}
	if(Output_Mode==1)
	{
		if(temperature<20)
		{
			Voltage_Output=1;
		}else if(temperature>40)
		{
			Voltage_Output=4;
		}
		else
		{
			Voltage_Output=temperature*0.15-2;
		}
	}
	Da_Write(Voltage_Output*51);
	
	/*--Led--*/
	ucLed[0]=!Output_Mode;
	for(i=0;i<3;i++)
	{
		ucLed[i+1]=(i==Seg_Disp_Mode);
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

void Delay750ms(void)	//@12.000MHz
{
	unsigned char data i, j, k;

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
{	rd_temperature();
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