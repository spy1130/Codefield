/*ͷ�ļ�������*/
#include <STC15F2K60S2.H>
#include <Init.H>
#include <Led.H>
#include <Key.H>
#include <Seg.H>
#include <iic.h>

/*����������*/
unsigned char Key_Slow_Down;//��������ר�ñ���
unsigned char Key_Val,Key_Old,Key_Down;
unsigned int Seg_Slow_Down;//����ܼ���ר�ñ���
unsigned char Pos;//ɨ��ר�ñ���
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//����ܶ�ѡ���ݴ洢����
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//�����С����״ֵ̬�洢����
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led״ֵ̬�洢����
unsigned char Dat;//����ADת������
float Voltage1;//����ADת�����ݵĵ�ѹֵ
unsigned char Seg_Mode ;//�����ģʽ���� 0-��ѹ��ʾ���� 1-��ѹ�������
unsigned char Write = 102;//DAת��д��ֵ Ĭ��Ϊ102 ��2v
float Voltage2;//����DAת�����ݵĵ�ѹֵ
bit Set_Mode_Flag;//���ģʽ��־λ 0-VDAC=2 1-VDAC=VRB2
bit Led_Flag;//Led��ʹ�ܱ�־λ 0-ʹ�� 1-�ر�
bit Led3_Flag;//L3ָʾ��ʹ�ܱ�־λ 1-ʹ�� 0-Ϩ��
bit Seg_Flag;//�����ʹ�ܱ�־λ 0-ʹ�� 1-�ر�


/*����������*/
void Key_Porc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//�������ٳ���
	
	Key_Val = Key_Read();//��ȡ����ֵ
	Key_Down = Key_Val & (Key_Val ^ Key_Old);//����½���
	Key_Old = Key_Val;//��������
	
	switch(Key_Down)
	{
		case 4://��ʾ�����л���ť
			if(++Seg_Mode == 2)
				Seg_Mode = 0;
		break;
		case 5://���ģʽ�л���ť
			if(Seg_Mode == 1)
			{
				Set_Mode_Flag ^= 1;
			}
		break;
		case 6://Led��ʹ�ܿ��ư�ť
			Led_Flag ^= 1;
		break;
		case 7://�����ʹ�ܿ��ư�ť
			Seg_Flag ^= 1;
		break;
	}
	
	
}

/*��Ϣ������*/
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//����ܼ��ٳ���
	
	/*ʵʱ��ȡ��������*/
	Dat = AD_Read(0x43);
	Voltage1 = ((Dat / 51.0)*1000+5)/1000.0;
	DA_Write(Write);
	Voltage2 = ((Write / 51.0)*1000+5)/1000.0;
	
	/*�������ʾ����*/
	if(Seg_Flag == 0)//�����ʹ���ж�
	{
		switch(Seg_Mode)
		{
			case 0://��ѹ��ʾ���� 
				Seg_Buf[0] = 11;
				Seg_Buf[5] = (unsigned char)Voltage1 % 10 ;	
				Seg_Buf[6] = (unsigned char)(Voltage1 * 10) % 10 ;	
				Seg_Buf[7] = (unsigned int)(Voltage1 * 100) % 10 ;
				Seg_Point[5] = 1;
			break;
			case 1://��ѹ������� 
				if(Set_Mode_Flag == 0)//��������ģʽ�ж�
				{
					Seg_Buf[0] = 12;
					Seg_Buf[5] = (unsigned char)Voltage2 % 10 ;	
					Seg_Buf[6] = (unsigned char)(Voltage2 * 10) % 10 ;	
					Seg_Buf[7] = (unsigned int)(Voltage2 * 100) % 10 ;
					Seg_Point[5] = 1;
				}
				else
				{
					Seg_Buf[0] = 12;
					Seg_Buf[5] = (unsigned char)Voltage1 % 10 ;	
					Seg_Buf[6] = (unsigned char)(Voltage1 * 10) % 10 ;	
					Seg_Buf[7] = (unsigned int)(Voltage1 * 100) % 10 ;
					Seg_Point[5] = 1;				
				}
					
			break;
		}
    }
	else
	{
		Seg_Buf[0] = 10;
		Seg_Buf[5] = 10 ;	
		Seg_Buf[6] = 10 ;	
		Seg_Buf[7] = 10 ;
		Seg_Point[5] = 0;		
	}
	
}

/*������ʾ����*/
void Led_Proc()
{
	unsigned char i;//ѭ������
	
	if(Voltage1 < 1.5 || Voltage1 >= 2.5 && Voltage1 < 3.5)//ָʾ��3ʹ���ж�
		Led3_Flag = 0;
	else
		Led3_Flag = 1;
	
	if(Led_Flag == 0)//Led��ʹ���ж�
	{
		ucLed[0] = ~(bit)Seg_Mode;
		ucLed[1] = (bit)Seg_Mode;
		ucLed[2] = Led3_Flag;
		ucLed[3] = Set_Mode_Flag;
	}
	else
	{
		for(i = 0; i < 4;i++)
		    ucLed[i] = 0;
	}
}

/*��ʱ��0�жϳ�ʼ������*/
void Timer0Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x18;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
	EA = 1;
}

/*��ʱ��0������*/
void Timer0Siver() interrupt 1
{
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	
	if(++Pos == 8) Pos = 0;
	Seg_Disp(Pos,Seg_Buf[Pos],Seg_Point[Pos]);
	Led_Disp(Pos,ucLed[Pos]);
	
}

/*������*/
void main()
{
	Init();
	Timer0Init();
	while(1)
	{
		Key_Porc();
		Seg_Proc();
		Led_Proc();
	}
}

