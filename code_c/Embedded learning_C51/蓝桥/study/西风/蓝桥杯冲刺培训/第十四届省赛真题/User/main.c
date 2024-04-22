/* ͷ�ļ������� */
#include <STC15F2K60S2.H>//��Ƭ���Ĵ���ר��ͷ�ļ�
#include <Init.h>//��ʼ���ײ�����ר��ͷ�ļ�
#include <Led.h>//Led�ײ�����ר��ͷ�ļ�
#include <Key.h>//�����ײ�����ר��ͷ�ļ�
#include <Seg.h>//����ܵײ�����ר��ͷ�ļ�
#include "ds1302.h"//ʱ�ӵײ�����ר��ͷ�ļ�								
#include "onewire.h"//�¶ȵײ�����ר��ͷ�ļ�
#include "iic.h"//�����ߵײ�����ר��ͷ�ļ�


/* ���������� */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//����ר�ñ���
unsigned char Key_Slow_Down;//��������ר�ñ���
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//�������ʾ���ݴ������
idata unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//�����С�������ݴ������
unsigned char Seg_Pos;//�����ɨ��ר�ñ���
unsigned int Seg_Slow_Down;//����ܼ���ר�ñ���
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led��ʾ���ݴ������
unsigned char ucRtc[3] = {13,03,05};//ʱ�����ݴ������ �ϵ�Ĭ��ʱ�� 13:03:05
unsigned char Seg_Disp_Mode;//�������ʾģʽ���� 0-ʱ����� 1-���Խ��� 2-�������� 3-�¶Ƚ���
unsigned char Val_Max[2];//���ֵ�������� 0-�¶� 1-ʪ��
float Val_Average[2];//ƽ������������ 0-�¶� 1-ʪ��
unsigned int Average_Temperature_Num;//ƽ��ֵ��������(�¶�)
unsigned int Average_Humidity_Num;//ƽ��ֵ��������(ʪ��)
unsigned char Val_Index;//��������ָ�� 0-�¶� 1-ʪ�� 2-ʱ��
unsigned char Temperature_Val = 30;//�¶Ȳ�����ʾ���� �ϵ�Ĭ�� 30
unsigned char Temperature_Ctrl = 30;//�¶Ȳ������Ʊ��� �ϵ�Ĭ�� 30
unsigned char Message[2];//��Ϣ���ݴ������ 0-�¶� 1-ʪ��
unsigned char Message_Old[2];//�ϴ���Ϣ���ݴ������ 0-�¶� 1-ʪ��
unsigned int Freq;//ʵʱƵ�ʲ���
unsigned int Timer_1000Ms;//һǧ�����ʱ����
unsigned char Vollage_Val;//ʵʱ��ѹ�ɼ�����
unsigned char Time_Val[3];//�ɼ�ʱ���¼����
unsigned char Temperature;//ʵʱ�¶Ȳɼ�����
unsigned char Seg_Disp_Mode_Old;//��һ�����¼���� �ɼ��󷵻�ԭ״̬
unsigned char Message_Num;//�ɼ���������
unsigned int Timer_2000Ms;//��ǧ�����ʱ����
unsigned int Timer_100Ms;//һ�ٺ����ʱ����
bit Led_Star_Flag;//Led��˸��־λ
bit Message_Flag = 1;//�ɼ������־λ 0-������ɼ� 1-����ɼ�
bit Vollage_Flag;//��ѹ������־λ
bit Key_Flag;//���̰���־λ

/* ���̴����� */
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//���̼��ٳ���

	Key_Val = Key_Read();//ʵʱ��ȡ����ֵ
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//��׽�����½���
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//��׽�����Ͻ���
	Key_Old = Key_Val;//����ɨ�����

	if(Key_Down == 9) //�����½���
		Key_Flag = 1; //��ʼ��ʱ
	if(Key_Up == 9) //����������
	{
		if(Timer_2000Ms < 2000) //�̰�����
		{
			if(Seg_Disp_Mode == 2) //���ڲ�������
			{
				if(--Temperature_Val == 255) //��������ֵ
					Temperature_Val = 0;
			}
		}
		else //��������
			Message_Num = 0;//��λ����ֵ
		Timer_2000Ms = Key_Flag = 0;//��λ��־λ
	}
	
	switch(Key_Down * Message_Flag)//�ɼ��ȴ������� ����ʧЧ
	{
		case 4://���水��
			if(++Seg_Disp_Mode == 3) //ѭ���л�����
				Seg_Disp_Mode = 0;
			if(Seg_Disp_Mode == 1) //�л������Խ���ʱ ָ���ʼ��
				Val_Index = 0;
			if(Seg_Disp_Mode == 0) //�л���ʱ����ʾ���� ��������ֵ
				Temperature_Ctrl = Temperature_Val;
		break;
			
		case 5://�л�����
			if(Seg_Disp_Mode == 1) //���ڻ��Խ���
			{
				if(++Val_Index == 3) //ѭ���л����Բ���
					Val_Index = 0;
			}
		break;
			
		case 8://�ԼӰ���
			if(Seg_Disp_Mode == 2) //���ڲ�������
			{
				if(++Temperature_Val == 100) //��������ֵ
					Temperature_Val = 99;
			}
		break;
	}
}

/* ��Ϣ������ */
void Seg_Proc()
{
	unsigned char i;//Forѭ��ר�ñ���
	/* ���ݶ�ȡ���� */
	switch(Seg_Slow_Down) //��ѯ��ȡоƬ����
	{
		case 200:
			Seg_Slow_Down += 1;//����һ������ִ�ж��
			Read_Rtc(ucRtc);//ʵʱ��ȡʱ��
			if(Message_Flag == 0) //������ɼ�
			{
				if((ucRtc[2] % 10) == ((Time_Val[2] + 3) % 10)) //��������
				{
					Seg_Disp_Mode = Seg_Disp_Mode_Old;//����ԭ״̬
					Message_Flag = 1;//����ɼ�
				}
			}
		break;
		case 400:
			Seg_Slow_Down += 1;//����һ������ִ�ж��
			Temperature = rd_temperature();//ʵʱ��ȡ�¶�
		break;
		case 300:
			Seg_Slow_Down += 1;//����һ������ִ�ж��
			Vollage_Val = Ad_Read(0x01);//ʵʱ��ȡ��ѹֵ
			if(Vollage_Val > 100) //����ǿ��Ϊ���ȼ�
				Vollage_Flag = 1;//���߱�־λ
			else if(Vollage_Flag == 1 && Message_Flag == 1) //����ǿ��Ϊ���ȼ�
			{
				Vollage_Flag = 0;//���ͱ�־λ
				Message_Flag = 0;//���Ͳɼ���־λ �����������ظ��ɼ�
				
				Read_Rtc(Time_Val);//��¼�ɼ�ʱ��
				
				Seg_Disp_Mode_Old = Seg_Disp_Mode;//��¼ԭ״̬����
				Seg_Disp_Mode = 3;//�����¶Ƚ���
				
				Message_Num++;//�ɼ������Լ�
				
				Message_Old[0] = Message[0];//�����ϴ�����
				Message[0] = Temperature;//��¼��Ч�¶�
				Val_Average[0] = ((Val_Average[0] * Average_Temperature_Num) + Message[0]) / (float)(Average_Temperature_Num + 1);//�����¶�ƽ��ֵ
				Average_Temperature_Num++;
				
				if(Freq >= 200 && Freq <= 2000) //��Ч��Χ
				{
					Message_Old[1] = Message[1];//�����ϴ�����
					Message[1] = (unsigned char)(10 + (Freq - 200) * (80.0 / 1800.0)); //��¼��Чʪ��
					Val_Average[1] = ((Val_Average[1] * Average_Humidity_Num) + Message[1]) / (float)(Average_Humidity_Num + 1);//����ʪ��ƽ��ֵ
					Average_Humidity_Num++;
				}
				else //��Ч��Χ
					Message[1] = 0; //ʪ��Ϊ0
				
				for(i=0;i<2;i++) //�������ֵ
				{
					if(Message[i] > Val_Max[i])
						Val_Max[i] = Message[i];
				}
			}
		break;
	}
	
	/* ��Ϣ��ʾ���� */
	if(!(Seg_Slow_Down % 30))//�������ʾ����30����ˢ��һ��
	{
		Seg_Slow_Down += 1;//����һ������ִ�ж��
		switch(Seg_Disp_Mode)
		{
			case 0://ʱ�����
				Seg_Point[6] = 0;
				Seg_Buf[2] = Seg_Buf[5] = 11;//��ʶ�� -
				for(i=0;i<3;i++)
				{
					Seg_Buf[3*i] = ucRtc[i] / 10;
					Seg_Buf[3*i+1] = ucRtc[i] % 10;
				}
			break;
			case 1://���Խ���
				Seg_Buf[0] = 12 + Val_Index;//��ʶ��
				if(Val_Index < 2) //���ڷ�ʱ����Խ���
				{
					Seg_Point[6] = 1;
					Seg_Buf[1] = 10;
					Seg_Buf[2] = Val_Max[Val_Index] / 10;
					Seg_Buf[3] = Val_Max[Val_Index] % 10;
					Seg_Buf[4] = 11;
					Seg_Buf[5] = (unsigned char)Val_Average[Val_Index] / 10;
					Seg_Buf[6] = (unsigned char)Val_Average[Val_Index] % 10;
					Seg_Buf[7] = (unsigned int)(Val_Average[Val_Index] * 10) % 10;
				}
				else //����ʱ����Խ���
				{
					Seg_Point[6] = 0;
					Seg_Buf[1] = Message_Num / 10;
					Seg_Buf[2] = Message_Num % 10;
					Seg_Buf[3] = Time_Val[0] / 10;
					Seg_Buf[4] = Time_Val[0] % 10;
					Seg_Buf[5] = 11;
					Seg_Buf[6] = Time_Val[1] / 10;
					Seg_Buf[7] = Time_Val[1] % 10;
				}
				if(Message_Num == 0)
				{
					Seg_Point[6] = 0;
					for(i=2+(Val_Index == 2);i<8;i++)
						Seg_Buf[i] = 10;
				}
			break;
			case 2://��������
				Seg_Point[6] = 0;
				for(i=0;i<5;i++)
					Seg_Buf[1+i] = 10;
				Seg_Buf[0] = 15;
				Seg_Buf[6] = Temperature_Val / 10;
				Seg_Buf[7] = Temperature_Val % 10;
			break;
			case 3://�¶Ƚ���
				Seg_Point[6] = 0;
				Seg_Buf[0] = 16;
				Seg_Buf[1] = Seg_Buf[2] = 10; 
				Seg_Buf[3] = Message[0] / 10;
				Seg_Buf[4] = Message[0] % 10;
				Seg_Buf[5] = 11;
				if(Message[1] > 0) //��Ч����
				{
					Seg_Buf[6] = Message[1] / 10;
					Seg_Buf[7] = Message[1] % 10;
				}
				else //��Ч����
					Seg_Buf[6] = Seg_Buf[7] = 17;
			break;
		}
	}
}

/* ������ʾ���� */
void Led_Proc()
{
	ucLed[0] = (Seg_Disp_Mode == 0);
	ucLed[1] = (Seg_Disp_Mode == 1);
	ucLed[2] = (Seg_Disp_Mode == 3);	
	ucLed[3] = (Message[0] > Temperature_Ctrl) * Led_Star_Flag;
	ucLed[4] = !Message[1];//�ɼ�ָʾ��
	ucLed[5] = ((Message[0] > Message_Old[0]) && (Message[1] > Message_Old[1])) * (Message_Num / 2);
}


/* ��ʱ��0�жϳ�ʼ������ */
void Timer0Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x05;		//���ü���ģʽ
	TL0 = 0;		//���ö�ʱ��ʼֵ
	TH0 = 0;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}

/* ��ʱ��1�жϳ�ʼ������ */
void Timer1Init(void)		//1����@12.000MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x18;		//���ö�ʱ��ֵ
	TH1 = 0xFC;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;		//��ʱ��1�жϴ�
	EA = 1;			//���жϴ�
}

/* ��ʱ��1�жϷ����� */
void Timer1Server() interrupt 3
{  
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//���̼���ר��
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//����ܼ���ר��
	if(++Seg_Pos == 8) Seg_Pos = 0;//�������ʾר��
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	
	if(Key_Flag == 1)
	{
		if(++Timer_2000Ms > 2000)
			Timer_2000Ms = 3000;
	}
	
	if(++Timer_100Ms == 100)
	{
		Timer_100Ms = 0;
		Led_Star_Flag ^= 1;
	}
	
	if(++Timer_1000Ms == 1000)
	{
		Timer_1000Ms = 0;
		TR0 = 0;
		Freq = TH0 << 8 | TL0;
		TH0 = TL0 = 0;
		TR0 = 1;
	}
}


/* Main */
void main()
{
	System_Init();
	Set_Rtc(ucRtc);//��ʼ��ʱ������
	Read_Rtc(ucRtc);//��ȡ��ʼ��ʱ������
	while(rd_temperature() == 85);//�����ϵ��¶�����85.00
	Timer1Init();
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}