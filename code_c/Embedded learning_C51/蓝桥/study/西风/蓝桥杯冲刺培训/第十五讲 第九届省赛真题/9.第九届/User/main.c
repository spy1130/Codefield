/* ͷ�ļ���������ʼ */
#include "init.h"//ϵͳ��ʼ��

/* ������ */
#include "key.h"//����ɨ��
#include "seg.h"//�������ʾ
#include "led.h"//Led��ʾ

/* �������� */
#include "iic.h"

/* ͷ�ļ����������� */

/* ������������ʼ */
unsigned long int ms_tick;//�δ�ʱ��ר��
unsigned int Key_Slow_Down,Seg_Slow_Down,Led_Slow_Down;//������ɨ�����ר��
unsigned char Key_Old,Key_Val,Key_Up,Key_Down;//����ɨ��ר��
unsigned char Seg_Pos;//�������ʾר��
unsigned char Seg_String[8];//��������ݴ��ר��
unsigned char ucLed;//LED��ʾר��

//�û��Զ���������
unsigned char Seg_Disp_Mode;//�������ʾģʽ 0-Ϩ�� 1-����ģʽ��˸ 2-��ת�����˸
unsigned char Running_Mode = 1;//����ģʽ 1-4ѭ������
unsigned int Circulation_Tm_Invl[4] = {400,400,400,400};//ʱ����ת������ݴ������
unsigned char Star[2] = {0x02,0xf0};//�������˸ģʽ���ݴ������
unsigned char EEPROM_String[4] = {4,4,4,4};//EEPROM���ݴ������
unsigned char Right_Level;//���ȵȼ�
unsigned char Led_Disp_Flag;//LED��ʾ��־λ
unsigned char Led_Disp_Mode[4][8] = 
{
	0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,//��һ��ģʽ��������
	0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,//�ڶ���ģʽ�����ҵ���
	0x81,0x42,0x24,0x18,0x81,0x42,0x24,0x18,//������ģʽ�������ߵ��м�
	0x18,0x24,0x42,0x81,0x18,0x24,0x42,0x81 //������ģʽ�����м䵽����
};
unsigned long int ms_tick_point;//������ר��
unsigned char Led_Pwm;//LED���ȵȼ�����ר��
unsigned char Led_Num;//LED��ʾ����ר��
bit Right_Level_Flag;//���ȵȼ���ʾ��־λ 0-����ʾ 1-��ʾ
bit Star_Flag;//�������˸ʱ���־λ
bit Led_Start_Stop_Flag;//LED��˸����ֹͣ��־λ 0-ֹͣ 1-����
/* �������������� */

/* ������Ϣ������ */
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//����ר��
	
	Key_Val = Key_Read();
	Key_Down = Key_Val & (Key_Val ^ Key_Old);//ͨ���½��ؼ�ⰴ��
	Key_Up = ~ Key_Val & (Key_Val ^ Key_Old);//ͨ�������ؼ�ⰴ��
	Key_Old = Key_Val;
	
	//�û��Զ��������
	if(Seg_Disp_Mode == 0)
	{
		if(Key_Old == 4)
			Right_Level_Flag = 1;
		else
			Right_Level_Flag = 0;
	}
	
	switch(Key_Down)
	{
		case 6://��ʾģʽ���ð���
			if(++Seg_Disp_Mode == 3)//��ʾģʽ��0-2֮��ѭ���л�.
			{
				EEPROM_String[0] = Circulation_Tm_Invl[0] / 100;
				EEPROM_String[1] = Circulation_Tm_Invl[1] / 100;
				EEPROM_String[2] = Circulation_Tm_Invl[2] / 100;
				EEPROM_String[3] = Circulation_Tm_Invl[3] / 100;//���ݴ���֮����תʱ�����EEPROMר������
				EEPROM_Write(EEPROM_String,16,4);
				Seg_Disp_Mode = 0;
			}
			break;
		case 5://����+
			switch(Seg_Disp_Mode)
			{
				case 1://��ʾģʽ
					if(++Running_Mode == 5)
						Running_Mode = 4;//��������Ϊ4
					break;
				case 2://��תʱ��
					Circulation_Tm_Invl[Running_Mode - 1] += 100;
					if(Circulation_Tm_Invl[Running_Mode - 1] == 1300)
						Circulation_Tm_Invl[Running_Mode - 1] = 1200;//��������Ϊ1200
					break;
			}
			break;
		case 4://����-
			switch(Seg_Disp_Mode)
			{
				case 1:
					if(--Running_Mode == 0)
						Running_Mode = 1;//��������Ϊ1
					break;
				case 2:
					Circulation_Tm_Invl[Running_Mode - 1] -= 100;
					if(Circulation_Tm_Invl[Running_Mode - 1] == 300)
						Circulation_Tm_Invl[Running_Mode - 1] = 400;//��������Ϊ400
					break;
			}
			break;
		case 7://����ֹͣ
			if(Led_Start_Stop_Flag)
				ucLed = 0;
			else
				Running_Mode = 1;
			Led_Start_Stop_Flag ^= 1;
		  break;
	}
}

/* ��������ݴ����� */
void Seg_Proc()
{
	unsigned char i = 4;//�������ר��
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//����ר��

	//�û��Զ��������
	Right_Level = (Ad_Read(0x43) / 64) + 1;//������ȵȼ�
	
	if(Seg_Disp_Mode)//��ʾ����
	{
		Seg_String[0] = Seg_String[2] = 10;//��ʾ ����
		Seg_String[1] = Running_Mode;//��ʾ����ģʽ
		Seg_String[3] = 11;//Ϩ��
		Seg_String[4] = Circulation_Tm_Invl[Running_Mode - 1] / 1000 % 10;//��ʾ��תʱ����
		Seg_String[5] = Circulation_Tm_Invl[Running_Mode - 1] / 100 % 10;
		Seg_String[6] = Circulation_Tm_Invl[Running_Mode - 1] / 10 % 10;
		Seg_String[7] = Circulation_Tm_Invl[Running_Mode - 1] % 10;
		while(Seg_String[i] == 0)//����תʱ��Ϊ��λ�� �����λ����ʾ
		{
			Seg_String[i] = 11;
			i--;
		}
	}
	else//����ʾ����
	{
		Seg_String[0] = Seg_String[1] = Seg_String[2] = Seg_String[3] = Seg_String[4] = Seg_String[5] = 11;
		if(Right_Level_Flag)//��ʾ���ȵȼ�
		{
			Seg_String[6] = 10;
			Seg_String[7] = Right_Level;
		}
		else
			Seg_String[6] = Seg_String[7] = 11;
	}
}

/* Led��ʾ���� */
void Led_Proc()
{
//	if(Led_Slow_Down) return;
//	Led_Slow_Down = 1;//����ר��
	
	//�û��Զ��������
	if(Led_Start_Stop_Flag)
	{
		if((ms_tick - ms_tick_point) >= Circulation_Tm_Invl[Running_Mode - 1])
		{
			ms_tick_point = ms_tick;
			ucLed = Led_Disp_Mode[Running_Mode - 1][Led_Disp_Flag];
			if(++Led_Disp_Flag == 8)
			{
				Led_Disp_Flag = 0;
				if(++Running_Mode == 5)
					Running_Mode = 1;
			}
		}
	}
}

/* ��ʱ���жϳ�ʼ������ */
void Timer0Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x18;		//���ö�ʱ��ʼֵ
	TH0 = 0xFC;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;    //�򿪶�ʱ��0�ж�
	EA = 1;    //�����ж�
}

/* ��ʱ���жϷ����� */
void Timer0Sever() interrupt 1
{
	ms_tick++;//�δ�ʱ��
	
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//10msɨ��һ��
	if(++Seg_Slow_Down == 100) Seg_Slow_Down = 0;//200msɨ��һ��
//	if(++Led_Slow_Down == 300) Led_Slow_Down = 0;//300msɨ��һ��
	if(++Seg_Pos == 8) Seg_Pos = 0;//�������ʾר��
	
	if(!(ms_tick % 800))//800ms����һ��
	{
		Star_Flag ^= 1;
	}
	
//	Seg_Disp(Seg_String[Seg_Pos],Seg_Pos,0,0,0);//�������ʾ
//	Led_Disp(ucLed);//LED��ʾ
	
	//�û��Զ��������
	if(Seg_Disp_Mode)//��ʾ����
		Seg_Disp(Seg_String[Seg_Pos],Seg_Pos,0,Star[Seg_Disp_Mode - 1],Star_Flag);
	else//����ʾ����
		Seg_Disp(Seg_String[Seg_Pos],Seg_Pos,0,0,0);
	
	switch(Right_Level)
	{
		case 1:Led_Pwm = 0;break;
		case 2:Led_Pwm = 3;break;
		case 3:Led_Pwm = 8;break;
		case 4:Led_Pwm = 11;break;
	}
	
	if(++Led_Num == 13) Led_Num = 0;//������0-12֮��ѭ��
	
	if(Led_Num <= Led_Pwm)
		Led_Disp(ucLed);
	else
		Led_Disp(0);
}

/* Main */
void main()
{
	Cls_Init();//�ر�����
	EEPROM_Read(EEPROM_String,16,4);//�ϵ�ʱ��ȡ����
	Circulation_Tm_Invl[0] = EEPROM_String[0] * 100;
	Circulation_Tm_Invl[1] = EEPROM_String[1] * 100;
	Circulation_Tm_Invl[2] = EEPROM_String[2] * 100;
	Circulation_Tm_Invl[3] = EEPROM_String[3] * 100;	
	Timer0Init();//��ʱ��0��ʼ��
	while(1)
	{
		Key_Proc();//������ѭ��ɨ��
		Seg_Proc();
		Led_Proc();
	}
}