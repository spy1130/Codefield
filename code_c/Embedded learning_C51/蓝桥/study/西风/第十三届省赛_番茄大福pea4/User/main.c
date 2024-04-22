/*Created By pea4-*/

/* ͷ�ļ������� */
#include <STC15F2K60S2.H>//��Ƭ���Ĵ���ר��ͷ�ļ�
#include <Init.h>//��ʼ���ײ�����ר��ͷ�ļ�
#include <Led.h>//Led�ײ�����ר��ͷ�ļ�
#include <Key.h>//�����ײ�����ר��ͷ�ļ�
#include <Seg.h>//����ܵײ�����ר��ͷ�ļ�
#include <ds1302.h>//ds1302�ײ�����ר��ͷ�ļ�
#include <onewire.h>//ds18b20�ײ�����ר��ͷ�ļ�

#define u8 unsigned char
#define u16 unsigned int

/* ���������� */
u8 Key_Slow_Down;//�������ٱ���
u16 Seg_Slow_Down;//����ܼ��ٱ���
u8 Key_Val,Key_Down,Key_Old,Key_Up;//����ר�ñ���
u8 Seg_Buf[8] = {16,16,16,16,16,16,16,16};//�������ʾ���ݴ������
u8 Seg_Point[8] = {0,0,0,0,0,0,0,0};//�����С�������ݴ������
u8 Seg_Pos;//�����ɨ��ר�ñ���
u8 ucLed[8] = {0,0,0,0,0,0,0,0};//Led��ʾ���ݴ������

u16 Temperature=0;//�¶ȱ���
u8 Relay_Buf=0;//�̵�������
u8 Display_Mode = 0; // 0-�¶���ʾ 1-ʱ����ʾ 2- ��������
u16 Timer_5000ms=0;//5���ʱ����
u8 Timer_100ms=0;//0.1���ʱ����
u8 Temp_Thres=23;//�¶���ֵ
u8 Temp_Thres_Buf=0;//�¶���ֵ��ʾ����

bit Relay_Mode = 1;//�̵���
bit Alarm_Flag=0;
bit Led_Flash_Flag=0;
bit Flash=0;//��˸��־λ

u8 ucRtc[3]={23,59,30};//ds1302ʱ��Ĵ���

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
		case 12:
			switch(Display_Mode)
			{
				case 0:
					Display_Mode = 1;
				break;
				case 1:
					Temp_Thres_Buf=Temp_Thres;
					Display_Mode = 2;
				break;
				case 2:
					Temp_Thres=Temp_Thres_Buf;
					Display_Mode = 0;
				break;
			}
			break;
		case 17:
			if(Display_Mode == 1)
				Display_Mode = 3;
			else if(Display_Mode == 2)
			{
				if(Temp_Thres_Buf>10)
					Temp_Thres_Buf--;
			}
			break;
		case 16:
			if(Display_Mode == 2)
			{
				if(Temp_Thres_Buf<99)
					Temp_Thres_Buf++;
			}
			break;
		case 13:
			ucLed[2]=0;
			Relay_Buf=0;
			Relay_Mode ^= 1;
		break;
	}
	
	switch(Key_Up)
	{
		case 17:
			if(Display_Mode == 3)
				Display_Mode = 1;
		break;
	}
}

//

/* ��Ϣ������ */
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//����ܼ��ٳ���
	
	Read_Rtc(ucRtc);
	Temperature=rd_temperature();
	
	switch(Display_Mode)
	{
		case 0:
			Seg_Buf[0]=25;
			Seg_Buf[1]=1;
			Seg_Buf[2]=Seg_Buf[3]=Seg_Buf[4]=16;
			if(Temperature/100 == 0) Seg_Buf[5]=16;
			else Seg_Buf[5]=Temperature/100;
			Seg_Buf[6]=32 + (Temperature/10 % 10);
			Seg_Buf[7]=Temperature%10;
			break;
		case 1:
			Seg_Buf[0]=25;
			Seg_Buf[1]=2;
			Seg_Buf[2]=16;
			Seg_Buf[3]=ucRtc[0]/10;
			Seg_Buf[4]=ucRtc[0]%10;
			Seg_Buf[5]=17;
			Seg_Buf[6]=ucRtc[1]/10;
			Seg_Buf[7]=ucRtc[1]%10;
			break;
		case 3:
			Seg_Buf[0]=25;
			Seg_Buf[1]=2;
			Seg_Buf[2]=16;
			Seg_Buf[3]=ucRtc[1]/10;
			Seg_Buf[4]=ucRtc[1]%10;
			Seg_Buf[5]=17;
			Seg_Buf[6]=ucRtc[2]/10;
			Seg_Buf[7]=ucRtc[2]%10;
			break;
		case 2:
			Seg_Buf[0]=25;
			Seg_Buf[1]=3;
			Seg_Buf[2]=Seg_Buf[3]=Seg_Buf[4]=Seg_Buf[5]=16;
			if(Temp_Thres_Buf/10 == 0) Seg_Buf[6]=16;
			else Seg_Buf[6]=Temp_Thres_Buf/10;
			Seg_Buf[7]=Temp_Thres_Buf%10;
			break;
	}
}

/* ������ʾ���� */
void Led_Proc()
{
	//������㲢��Alarm_Flag��û�б���1
	if(ucRtc[1]==0x00 && ucRtc[2]==0x00 && Alarm_Flag==0)
		Alarm_Flag=1;
	
	ucLed[0]=Alarm_Flag;//L1������Ҳ��5��
	ucLed[1]=Relay_Mode;//L2��ʾ����ģʽ
	
	//�̵������Ͼ���˸
	ucLed[2]=Relay_Buf?Flash:0;
	
	if(Relay_Mode)//�¶ȿ���ģʽ
		Relay_Buf=(Temperature > (Temp_Thres * 10));
	else//ʱ�����ģʽ
		Relay_Buf=Alarm_Flag;
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
	if(++Seg_Slow_Down == 50) Seg_Slow_Down = 0;//����ܼ���ר��

	//����������
	if(Alarm_Flag)Timer_5000ms++;
	else Timer_5000ms=0;
	if(Timer_5000ms==5000)
	{
		Timer_5000ms=0;
		Alarm_Flag=0;
	}
	
	//�̵���������˸
	if(Relay_Buf)Timer_100ms++;
	else Timer_100ms=0; //�����Ͼ���ռ�ʱ����
	if(Timer_100ms==100)
	{
		Timer_100ms=0;
		Flash^=1;
	}
	
	
	if(++Seg_Pos == 8) Seg_Pos = 0;//�������ʾר��
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	Relay(Relay_Buf);
}

/* Main */
void main()
{
	System_Init();
	Set_Rtc(ucRtc);
	Temperature=rd_temperature();
	while(Temperature==850)Temperature=rd_temperature();
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}