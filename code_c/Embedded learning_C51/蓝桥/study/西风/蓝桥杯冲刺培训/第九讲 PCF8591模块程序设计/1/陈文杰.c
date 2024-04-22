/*ͷ�ļ�������*/
#include <STC15F2K60S2.H>//��Ƭ���Ĵ���ר��ͷ�ļ�
#include <Init.h>//��ʼ���ײ�ר��ͷ�ļ�
#include <Key.h>//�����ײ�����ͷ�ļ�
#include <Seg.h>//����ܵײ�����ͷ�ļ�
#include <Led.h>//Led�ײ�����ͷ�ļ�
#include "iic.h"//icc����ͷ�ļ�

/*����������*/
unsigned char Key_Slow_Down;//��������ר�ñ���
unsigned int Seg_Slow_Down;//����ܼ���ר�ñ���
unsigned char Key_Val,Key_Old,Key_Down,Key_Up;//����ר�ñ���
unsigned char Seg_Pos;
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};
unsigned char Seg_Point[8] ={0,0,0,0,0,0,0,0};
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};
unsigned char dat1,dat1_output;//dat1��������  dat2����������
unsigned char Seg_Mode;//ģʽ���� 0����ѹ��ʾ���� 1����ѹ�������
bit output_Flag;//���ģʽ��־λ 0-2v  1-���
bit Led_Enable;//ledʹ�ܱ�־λ
bit Seg_Enable;//�����ʹ�ܰ���


/*����������*/
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//�������ٳ���
	
	Key_Val = Key_Read();//��ȡ����ֵ
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//��׽�����½���
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//��׽����������
	Key_Old = Key_Val;//����ɨ�躯��
	
	switch(Key_Down)
	{
		case 4://��ʾ�����л�����
			Seg_Mode ^=1;
		break;
		case 5://���ģʽ�л�����
			output_Flag ^= 1;
		break;
		case 6://Ledָʾ�ƿ���
			Led_Enable ^= 1;
		break;
		case 7://�������ʾ���ܰ���
			Seg_Enable ^= 1;
		break;

	}
}

/*��Ϣ��ʾ����*/
void Seg_Proc()
{
	unsigned char i;//����ѭ��
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//�������ٳ���	
	
	dat1 = Ad_Read(0x43);
	dat1_output = output_Flag ? dat1 : 102;
	
	if(Seg_Enable == 0)//�����ʹ��
	{
		switch(Seg_Mode)
		{
			case 0://��ѹ��ʾ����
				Seg_Buf[0] = 11;//��ʾc
			Seg_Buf[1] = Seg_Buf[2] = Seg_Buf[3] = Seg_Buf[4] = 10;
			Seg_Buf[5] = (unsigned int)(dat1 / 51.0) % 10;
			Seg_Buf[6] = (unsigned int)((dat1 / 51.0) * 100) / 10 % 10;
			Seg_Buf[7] = (unsigned int)((dat1 / 51.0) * 100) % 10;
			Seg_Point[5] = 1;//����λС������
			break;
			case 1://��ѹ�������
				Seg_Buf[0] = 12;//F
			Seg_Buf[1] = Seg_Buf[2] = Seg_Buf[3] = Seg_Buf[4] = 10;
			Seg_Buf[5] = (unsigned int)(dat1_output / 51.0) % 10;
			Seg_Buf[6] = (unsigned int)((dat1_output / 51.0) * 100) / 10 % 10;
			Seg_Buf[7] = (unsigned int)((dat1_output / 51.0) * 100) % 10;
			Seg_Point[5] = 1;//����λС������
			break;
		}
	}
	else
	{
		Seg_Point[5] = 0;
		for(i=0;i<8;i++)
		Seg_Buf[i] = 10;//Ϩ�������
	}
}


/*������ʾ����*/
void Led_Proc()
{
	unsigned char i;
	if(Led_Enable == 0)//Ledʹ��״̬
	{
//		if(Seg_Mode == 0)//���ڵ�ѹ��������
//		{
//			ucLed[0] = 1;
//			ucLed[1] = 0;
//		}
//		else//���ڵ�ѹ�������Seg_Mode == 1
//		{
//			ucLed[0] = 0;
//			ucLed[1] = 1;			
//		}
		for(i = 0;i<2;i++)
		ucLed[i] = (i == Seg_Mode);
		
		ucLed[3] = output_Flag;//2v-L4Ϩ��
		if((dat1 / 51.0) < 1.5 )
			ucLed[2] = 0;
		
		else if((dat1 / 51.0) <2.5 && (dat1 / 51.0) > 1.5)
			ucLed[2] = 1;
		
		else if((dat1 / 51.0) > 2.5 && (dat1 / 51.0) < 3.5)
			ucLed[2] = 0;
		
		else if((dat1 / 51.0) > 3.5)
			ucLed[2] = 1;
	}
	else //led������
	{
		for(i = 0;i<8;i++)//ȫ��Ϩ��
		ucLed[i] = 0;
	}
	
}

/*��ʱ��0�жϳ�ʼ������*/
void Timer0Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x18;		//���ö�ʱ��ʼֵ
	TH0 = 0xFC;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
	EA = 1;
}

/*��ʱ��0��ʼ��������*/
void Timer0Server() interrupt 1
{
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
	if(++Seg_Slow_Down == 100) Seg_Slow_Down = 0;	
	if(++Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	
}

/*main*/
int main()
{
	System_Init();
	Timer0Init();
	while(1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();		
	}
}

