//��Seg���٣�˳����ʱ��������ET0 =1;EA=1;����Segɨ�裬led+ɨ�裬Key����+����������������
#include <STC15F2K60S2.H>
#include <init.h>
#include <Led.h>
#include <Key.h>
#include <Seg.h>
#include "ds1302.h" 
#include "iic.h"

unsigned char Seg_Slow_Down;
unsigned char Seg_Buf[8]={10,10,10,10,10,10,10,10};
unsigned char Seg_Point[8]={0,0,0,0,0,0,0,0};
unsigned char Seg_Pos;
bit Seg_Disp_Mode;//�������ʾģʽ���� 0-������ʾ���� 1-�������ý���
unsigned char ucLed[8]={0,0,0,0,0,0,0,0};

unsigned char Key_Val,Key_Old,Key_Down,Key_Up;
unsigned char Key_Slow_Down;
unsigned char ucRtc[3]={0x08,0x30,0x00};
unsigned char wet;//ʪ�Ȱٷֱ�
unsigned char wetc_Disp=50;//�¶ȷ�ֵ��ʾ
unsigned char wetc_Save=50;//�¶ȷ�ֵ����
bit Work_Mode;//����״̬��־λ 0-�Զ�����ģʽ 1-�ֶ�����ģʽ
bit Beep_Enable_Flag;//����ʹ�ܱ�־λ
bit Relay_Flag;//�̵���������־λ
void Key_Proc()
{
	if(Key_Slow_Down)return;
	Key_Slow_Down =1;
	//�ص��
	Key_Val=Key_Read();
	Key_Down=Key_Val &(Key_Old^Key_Val);
	Key_Up=~Key_Val &(Key_Old^Key_Val);
	Key_Old =Key_Val;
	switch(Key_Down)
	{
		case 7:
			Work_Mode ^= 1;
		
		break;
		case 6:
			if(Work_Mode == 0)//�Զ�ģʽ
			{
				Seg_Disp_Mode ^= 1;
				if(Seg_Disp_Mode == 0)
				{
					wetc_Save=wetc_Disp;
					EEPROM_Write(&wetc_Save,0,1);
				}
				
			}
			else//�ֶ�ģʽ
				Beep_Enable_Flag ^= 1;
		break;
		case 5:
			if(Work_Mode == 0)
			{
				if(++wetc_Disp == 100)
					wetc_Disp = 99;
			}
			else
				Relay_Flag = 1;
		break;
		case 4:
			if(Work_Mode == 0)
			{
				if(--wetc_Disp == 255)
					wetc_Disp = 0;
			}
			else
				Relay_Flag = 0;
		break;
	}
}


void Seg_Proc()
{	//����0ʱ����
	unsigned char i;
	Read_Rtc(ucRtc);
	wet =Ad_Read(0x03)/2.56;//0-255һ��256������0%-99%����Ӧ������256/100=2.56
	if(Seg_Slow_Down) return;
	Seg_Slow_Down =1;
	if(Seg_Disp_Mode == 0)
	{
		for(i=0;i<2;i++)
		{
			Seg_Buf[i*3]=ucRtc[i]/16;
			Seg_Buf[i*3+1]=ucRtc[i]%16;
		}
		Seg_Buf[2] = 11;
		Seg_Buf[6] = wet / 10;
		Seg_Buf[7] = wet % 10;
	}
	else
	{
		Seg_Buf[0] = 11;
		Seg_Buf[1] = 11;		
		Seg_Buf[2] = Seg_Buf[3] = Seg_Buf[4] = 10;
		Seg_Buf[6] = wetc_Disp / 10;
		Seg_Buf[7] = wetc_Disp % 10;		
		
	}

}

void Led_Proc()
{
	ucLed[0] = !Work_Mode;
	ucLed[1] = Work_Mode;
}

void Timer0_Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x18;				//���ö�ʱ��ʼֵ
	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 =1;
	EA=1;
}
/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//���̼���ר��
    if(++Seg_Slow_Down==500) Seg_Slow_Down=0;//����ܼ���ר��
	if(++Seg_Pos==8)Seg_Pos=0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	Relay(Relay_Flag);
	Beep((wetc_Save / wet) & Beep_Enable_Flag);
}



void main()
{	Set_Rtc(ucRtc);
	wet = Ad_Read(0x03) / 2.56;//�ϵ��ȡʪ�� ���������Ч����
	EEPROM_Read(&wetc_Save,0,1);
	wetc_Disp = wetc_Save;
	System_Init();
	Timer0_Init();
	while(1)
	{
		Seg_Proc();
		Key_Proc();
		Led_Proc();
		
	}
	
}