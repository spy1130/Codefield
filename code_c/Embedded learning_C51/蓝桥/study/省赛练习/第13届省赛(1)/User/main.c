//��Seg���٣�˳����ʱ��������ET0=1,EA=1;Segɨ�裬led+ɨ�裬Key����+����������������
#include <STC15F2K60S2.H>
#include <init.h>
#include <Led.h>
#include <Key.h>
#include <Seg.h>
#include "ds1302.h"
#include "onewire.h"

unsigned char Seg_Slow_Down;
unsigned char Seg_Buf[8]={10,10,10,10,10,10,10,10};
unsigned char Seg_Point[8]={0,0,0,0,0,0,0,0};
unsigned char Seg_Pos;
unsigned char Seg_Mode_Disp;//0-�¶���ʾ��1-ʱ����ʾ��2-��������
unsigned char ucLed[8]={0,0,0,0,0,0,0,0};
unsigned char ucRtc[3]={0x07,0x59,0x20};//ʱ�䴢��
unsigned char Key_Val,Key_Old,Key_Down,Key_Up;
unsigned char Key_Slow_Down;
float temperature;//��ʵ�¶�
unsigned char temp_ctrl=23;//�¶���ֵ
unsigned char temp_disp=23;//�¶���ֵ
bit Relay_Buf;//�̵�������λ
bit Ctrl_Mode ;//0-�¶ȿ��ƣ�1-ʱ�����
bit Seg_Time_Mode;//ʱ����ʾģʽ
bit Alarm_Flag;//�����־λ
bit Led_Flash;//��˸�ж�
unsigned int Timer_5000ms;//�ӳٱ���
unsigned char Timer_100ms;//��˸��ʱ����
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
	{	case 12://�л�ģʽ
		if(++Seg_Mode_Disp==3)
			Seg_Mode_Disp=0;
		if(Seg_Mode_Disp==0)
			 temp_disp=temp_ctrl;
		if(Seg_Mode_Disp==1)
			temp_ctrl=temp_disp;
		break;
		case 13://����ģʽ�л�
			//��ʼ������
			ucLed[2]=0;
			Relay(0);
			
			Ctrl_Mode^=1;
		break;
		case 16:
			if(Seg_Mode_Disp==2){
			if(++temp_ctrl==100)
				temp_ctrl=99;}
		case 17:
			if(Seg_Mode_Disp==2){
			if(--temp_ctrl==9)
				temp_ctrl=10;}
			else if(Seg_Mode_Disp==1)
			{
				Seg_Time_Mode=1;
			}
		break;	
	}
	if(Key_Up==17 && Seg_Time_Mode==1)
	{
		Seg_Time_Mode=0;
	}
}


void Seg_Proc()
{	//����0ʱ����
	if(Seg_Slow_Down) return;
	Seg_Slow_Down =1;
	temperature=rd_temperature();
	Read_Rtc(ucRtc);
	Seg_Buf[0]=11;
	Seg_Buf[1]=Seg_Mode_Disp+1;
	if(Seg_Mode_Disp==0)//�¶�
	{
		Seg_Buf[5]=(unsigned char)temperature/10%10;
		Seg_Buf[6]=(unsigned char)temperature%10;
		Seg_Buf[7]=(unsigned int)(temperature*10)%10;
		Seg_Point[6]=1;
	}
	else if(Seg_Mode_Disp==1)//ʱ��
	{	Seg_Point[6]=0;
		if(Seg_Time_Mode==0){
		Seg_Buf[3]=ucRtc[0]/16;
		Seg_Buf[4]=ucRtc[0]%16;
		Seg_Buf[5]=12;
		Seg_Buf[6]=ucRtc[1]/16;
		Seg_Buf[7]=ucRtc[1]%16;
		}
		else
		{
		Seg_Buf[3]=ucRtc[1]/16;
		Seg_Buf[4]=ucRtc[1]%16;
		Seg_Buf[5]=12;
		Seg_Buf[6]=ucRtc[2]/16;
		Seg_Buf[7]=ucRtc[2]%16;
		}
	}
	else if(Seg_Mode_Disp==2)//����
	{
		Seg_Buf[6]=temp_disp/10;
		Seg_Buf[7]=temp_disp%10;
		Seg_Point[6]=0;
	}
}

void Led_Proc()
{
	if(ucRtc[1]==0x00 && ucRtc[2]==0x00 && Alarm_Flag==0)
		Alarm_Flag=1;
	
	ucLed[0]=Alarm_Flag;
	ucLed[1]=~Ctrl_Mode;
	ucLed[2]=Relay_Buf?Led_Flash:0;
	if(Ctrl_Mode==0)
	{
		Relay_Buf=(temperature>temp_ctrl);
	}
	else
	{
		Relay_Buf=Alarm_Flag;
	}
}

void Timer0_Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x18;				//���ö�ʱ��ʼֵ
	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0=1;
	EA=1;
}
/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//���̼���ר��
    if(++Seg_Slow_Down==500) Seg_Slow_Down=0;//����ܼ���ר��
	if(++Seg_Pos==8)Seg_Pos=0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	Relay(Relay_Buf);
	
	if(Alarm_Flag)Timer_5000ms++;
	else Timer_5000ms=0;
	if(Timer_5000ms==5000)
	{
		Timer_5000ms=0;
		Alarm_Flag=0;
	}
	if(Relay_Buf)Timer_100ms++;
	else Timer_100ms=0;
	if(Timer_100ms==100)
	{
		Timer_100ms=0;
		Led_Flash^=1;
	}
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


void main()
{	rd_temperature();
	Delay750ms();
	Set_Rtc(ucRtc);
	System_Init();
	Timer0_Init();
	while(1)
	{
		Seg_Proc();
		Key_Proc();
		Led_Proc();
		
	}
	
}