/* ͷ�ļ������� */
#include <STC15F2K60S2.H>//��Ƭ���Ĵ���ר��ͷ�ļ�
#include <Init.h>//��ʼ���ײ�����ר��ͷ�ļ�
#include <Led.h>//Led�ײ�����ר��ͷ�ļ�
#include <Key.h>//�����ײ�����ר��ͷ�ļ�
#include <Seg.h>//����ܵײ�����ר��ͷ�ļ�
#include "ds1302.h" 
#include "onewire.h"

/* ���������� */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//����ר�ñ���
unsigned char Key_Slow_Down;//��������ר�ñ���
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//�������ʾ���ݴ������
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//�����С�������ݴ������
unsigned char Seg_Pos;//�����ɨ��ר�ñ���
unsigned int Seg_Slow_Down;//����ܼ���ר�ñ���
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led��ʾ���ݴ������
bit Seg_Disp_Mode;//�������ʾģʽ���� 0-ʱ����� 1-�¶����
unsigned char ucRtc[3] = {0x23,0x59,0x50};
unsigned char ucRtc_Set[3] = {0x23,0x59,0x50};
unsigned char Alarm[3] = {0x00,0x00,0x00};
unsigned char Alarm_Set[3] = {0x00,0x00,0x00};
unsigned char ucRtc_Index;
unsigned int Timer_500Ms;
bit Seg_Star_Flag;
bit Beep_Flag;
unsigned char Timer_100Ms;
unsigned char Temperature;
bit Led_Star_Flag;
unsigned char* Data_Flag[3] = {ucRtc,ucRtc_Set,Alarm_Set};
unsigned char Seg_Index;//ʱ�������ʾ���� 0-ʱ����ʾ 1-ʱ������ 2-��������

/* ���̴����� */
void Key_Proc()
{
	unsigned char i;
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//���̼��ٳ���

	Key_Val = Key_Read();//ʵʱ��ȡ����ֵ
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//��׽�����½���
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//��׽�����Ͻ���
	Key_Old = Key_Val;//����ɨ�����

	if(Beep_Flag == 1)
	{
		if(Key_Down != 0)
			Beep_Flag = 0;
		return;//����Ҳʹ���˹���
	}
	
	if(Seg_Index == 0)
	{
		if(Key_Old == 4)
			Seg_Disp_Mode = 1;
		else
			Seg_Disp_Mode = 0;
	}
	switch(Key_Down)
	{
		case 7:
			if(Seg_Disp_Mode == 0) //ʱ�����
			{
				if(Seg_Index == 0)
				{
					for(i=0;i<3;i++)
						ucRtc_Set[i] = ucRtc[i];
					Seg_Index = 1;
				}
				else if(Seg_Index == 1)
				{
					if(++ucRtc_Index == 3)
					{
						Set_Rtc(ucRtc_Set);
						ucRtc_Index = 0;
						Seg_Index = 0;
					}
				}
			}
		break;
		case 5:
			if(Seg_Disp_Mode == 0) //ʱ�����
			{
//				if(Seg_Index == 1) //����ʱ��
//				{
//					ucRtc_Set[ucRtc_Index]++;
//					if(ucRtc_Set[ucRtc_Index] % 16 == 0x0a)
//						ucRtc_Set[ucRtc_Index] += 6;
//					if(ucRtc_Set[ucRtc_Index] == (ucRtc_Index?0x60:0x24))
//						ucRtc_Set[ucRtc_Index] = (ucRtc_Index?0x59:0x23);
//				}
				Data_Flag[Seg_Index][ucRtc_Index]++;
				if(Data_Flag[Seg_Index][ucRtc_Index] % 16 == 0x0a)//����16��1
					Data_Flag[Seg_Index][ucRtc_Index] += 6;
				if(Data_Flag[Seg_Index][ucRtc_Index] == (ucRtc_Index?0x60:0x24))//����������
					Data_Flag[Seg_Index][ucRtc_Index] = (ucRtc_Index?0x59:0x23);	
			}
		break;
		case 4:
			if(Seg_Disp_Mode == 0) //ʱ�����
			{
//				if(Seg_Index == 1) //����ʱ��
//				{
//					ucRtc_Set[ucRtc_Index]--;
//					if(ucRtc_Set[ucRtc_Index] % 16 == 0x0f)
//						ucRtc_Set[ucRtc_Index] -= 6;
//					if(ucRtc_Set[ucRtc_Index] == 0xf9)
//						ucRtc_Set[ucRtc_Index] = 0;
//				}
				Data_Flag[Seg_Index][ucRtc_Index]--;
				if(Data_Flag[Seg_Index][ucRtc_Index] % 16 == 0x0f)//16��������fת����BCD��
					Data_Flag[Seg_Index][ucRtc_Index] -= 6;
				if(Data_Flag[Seg_Index][ucRtc_Index] == 0xf9)//�������ã�0H-7H  ��
					Data_Flag[Seg_Index][ucRtc_Index] = 0;				
			}			
		break;
		case 6:
			if(Seg_Disp_Mode == 0) //ʱ�����
			{
				if(Seg_Index == 0)
					Seg_Index = 2;
				else if(Seg_Index == 2)
				{
					if(++ucRtc_Index == 3)
					{
						for(i=0;i<3;i++)
							Alarm[i] = Alarm_Set[i];
						ucRtc_Index = 0;
						Seg_Index = 0;
					}
				}
			}
		break;
	}
}

/* ��Ϣ������ */
void Seg_Proc()
{
	unsigned char i;
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//����ܼ��ٳ���

	
	/* ��Ϣ��ȡ���� */
	Read_Rtc(ucRtc);
	Temperature = rd_temperature();
	
	/* ������ʾ���� */
	if(Seg_Disp_Mode == 0) //ʱ��
	{
//		switch(Seg_Index)
//		{
//			case 0://ʱ����ʾ
//				Seg_Buf[2] = Seg_Buf[5] = 11;
//				for(i=0;i<3;i++)
//				{
//					Seg_Buf[3*i] = ucRtc[i] / 16;
//					Seg_Buf[3*i+1] = ucRtc[i] % 16;
//				}
//			break;
//			case 1://ʱ������
//				Seg_Buf[2] = Seg_Buf[5] = 11;
//				for(i=0;i<3;i++)
//				{
//					Seg_Buf[3*i] = ucRtc_Set[i] / 16;
//					Seg_Buf[3*i+1] = ucRtc_Set[i] % 16;
//				}
//				Seg_Buf[3*ucRtc_Index] = Seg_Star_Flag?10:ucRtc_Set[ucRtc_Index] / 16;
//				Seg_Buf[3*ucRtc_Index+1] = Seg_Star_Flag?10:ucRtc_Set[ucRtc_Index] % 16;
//			break;
				Seg_Buf[2] = Seg_Buf[5] = 11;
				for(i=0;i<3;i++)
				{
					Seg_Buf[3*i] = Data_Flag[Seg_Index][i] / 16;
					Seg_Buf[3*i+1] = Data_Flag[Seg_Index][i] % 16;
				}		
				if(Seg_Index > 0)
				{
					Seg_Buf[3*ucRtc_Index] = Seg_Star_Flag?10:Data_Flag[Seg_Index][ucRtc_Index] / 16;
					Seg_Buf[3*ucRtc_Index+1] = Seg_Star_Flag?10:Data_Flag[Seg_Index][ucRtc_Index] % 16;
				}
		}
	else
	{
		for(i=0;i<5;i++)
			Seg_Buf[i] = 10;
		Seg_Buf[5] = Temperature / 10;
		Seg_Buf[6] = Temperature % 10;
		Seg_Buf[7] = 12;
	}
}

/* ������ʾ���� */
void Led_Proc()
{
	if(Alarm[0] == ucRtc[0] && Alarm[1] == ucRtc[1] && Alarm[2] == ucRtc[2])
		Beep_Flag = 1;
	if((ucRtc[2] % 16) == (Alarm[2] % 16 + 5))
		Beep_Flag = 0;
	ucLed[0] = Led_Star_Flag & Beep_Flag;//��Ҫͬʱ����
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
	if(++Seg_Slow_Down == 200) Seg_Slow_Down = 0;//����ܼ���ר��
	if(++Seg_Pos == 8) Seg_Pos = 0;//�������ʾר��
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	
	if(++Timer_500Ms == 500)
	{
		Timer_500Ms = 0;
		Seg_Star_Flag ^= 1;
	}
	
	if(++Timer_100Ms == 100)
	{
		Timer_100Ms = 0;
		Led_Star_Flag ^= 1;
	}
}

/* Main */
void main()
{
	Set_Rtc(ucRtc);
	System_Init();
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}