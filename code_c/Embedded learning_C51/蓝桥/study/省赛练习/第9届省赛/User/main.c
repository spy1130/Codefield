/* ͷ�ļ������� */
#include <STC15F2K60S2.H>//��Ƭ���Ĵ���ר��ͷ�ļ�
#include <Init.h>//��ʼ���ײ�����ר��ͷ�ļ�
#include <Led.h>//Led�ײ�����ר��ͷ�ļ�
#include <Key.h>//�����ײ�����ר��ͷ�ļ�
#include <Seg.h>//����ܵײ�����ר��ͷ�ļ�
#include <stdio.h>//��׼��ײ�����ר��ͷ�ļ�
#include "iic.h"

/* ���������� */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//����ר�ñ���
unsigned char Key_Slow_Down;//��������ר�ñ���
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//�������ʾ���ݴ������
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//�����С�������ݴ������
unsigned char Seg_Pos;//�����ɨ��ר�ñ���
unsigned int Seg_Slow_Down;//����ܼ���ר�ñ���
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led��ʾ���ݴ������
unsigned int T_Disp[4]={400,400,400,400};//��ת���������ʾ����400~1200ms
unsigned int T_Ctrl[4]={400,400,400,400};//��ת������ݿ�������400~1200ms(�������б���)
unsigned char T_Index;//��ת���ָ��-����ģʽ���
bit Seg_Disp_Mode;//0-���ã�1-������ʾ
unsigned int Ms_Tick;//ϵͳ��ʱ����
bit Start_Flag;//��ת������־λ 0-��ͣ 1-����
unsigned char Led_Mode;//ϵͳ��תģʽ����
unsigned char eeprom_dat[4];//�洢����
unsigned int time_400ms;//�������˸ʱ��λ
bit time_flag;//�������˸��־λ
unsigned char Seg_Index;//���������ֵ 0-Ϩ�� 1-ģʽ 2-��ת���
unsigned char Led_Pos;//led����
unsigned char Led_Level;//Led���ȵȼ�����
unsigned char Led_Count;//Led���ȼ���ֵ

/* ���̴����� */
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//���̼��ٳ���

	Key_Val = Key_Read();//ʵʱ��ȡ����ֵ
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//��׽�����½���
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//��׽�����Ͻ���
	Key_Old = Key_Val;//����ɨ�����
	if(Seg_Index==0)//���Ϩ��
	{
		if(Key_Old==4){
			Seg_Disp_Mode=1; Seg_Slow_Down=0;}
		else
		{
			Seg_Disp_Mode=0;
		}
	}
	switch(Key_Down)
	{	unsigned char i;
		case 6:
			if(++Seg_Index==3)Seg_Index =0;
		
			if(Seg_Index==0)
			{	T_Index=0;//��λ����֪���ò�����
				for(i=0;i<4;i++){
				T_Ctrl[i]=T_Disp[i];//����
				eeprom_dat[i]=T_Ctrl[i]/100;//��100�������Ͷ�С��255��
				}
				EEPROM_Write(eeprom_dat,0,4);
			}
			
		break;
		case 7://��������
		Start_Flag ^= 1;
		break;
		case 5://+
			if(Seg_Index==1)
			{
				if(++T_Index==4)
				{
					T_Index=0;
				}
			}
			else if(Seg_Index==2)
			{
				T_Disp[T_Index]+=100;
				if(T_Disp[T_Index]>1200)
					T_Disp[T_Index]=1200;
			}
		break;
		case 4:
			if(Seg_Index==1)
			{
				if(--T_Index==255)
				{
					T_Index=3;
				}
			}
			else if(Seg_Index==2)
			{
				T_Disp[T_Index]-=100;
				if(T_Disp[T_Index]<400)
					T_Disp[T_Index]=400;
			}
		break;
	}

}

/* ��Ϣ������ */
void Seg_Proc()
{	unsigned char i;
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//����ܼ��ٳ���
	
	Led_Level=Ad_Read(0x03)/64;//0~255,256����/64=4,������255/4=3��ģʽΪ0��1��2��3
	if(Seg_Disp_Mode==0)//�������ý���
	{
		Seg_Buf[0]=11;
		Seg_Buf[1]=T_Index+1;
		Seg_Buf[2]=11;
		Seg_Buf[4]=T_Disp[T_Index]/1000%10?T_Disp[T_Index]:10;
		Seg_Buf[5]=T_Disp[T_Index]/100%10;
		Seg_Buf[6]=T_Disp[T_Index]/10%10;
		Seg_Buf[7]=T_Disp[T_Index]%10;
		if(Seg_Index==1)
		{
			Seg_Buf[0]=Seg_Buf[2]=time_flag?10:11;
			Seg_Buf[1]=time_flag?10:T_Index+1;	
		}
		else if(Seg_Index ==2)
		{
			Seg_Buf[4]=time_flag?10:T_Disp[T_Index] /1000 % 10;
			Seg_Buf[5]=time_flag?10:T_Disp[T_Index]/100%10;
			Seg_Buf[6]=time_flag?10:T_Disp[T_Index]/10%10;
			Seg_Buf[7]=time_flag?10:T_Disp[T_Index]%10;
		}
		if(Seg_Buf[4] == 0) Seg_Buf[4] = 10;
	}
	else//���Ƶȼ�����
	{
		for(i=0;i<6;i++)
			Seg_Buf[i]=10;
			Seg_Buf[6]=11;
			Seg_Buf[7]=Led_Level+1;
	}

}

/* ������ʾ���� */
void Led_Proc()
{	unsigned char i;
	if(Start_Flag == 1) //ϵͳ��������״̬
	{
		if(Ms_Tick ==T_Ctrl[T_Index]) //ϵͳ��ʱʱ��ﵽ��תʱ����
		{                         
			Ms_Tick = 0;//��λ��ʱ �����´ν���
			switch(Led_Mode)
			{
				case 0:
					if(++Led_Pos ==8)
					{
						Led_Pos=7;
						Led_Mode=1;
					}
				break;
				case 1:
					if(--Led_Pos ==255)
					{	
						Led_Pos = 7;//ģʽ3��ʼֵ
						Led_Mode=2;
					}
				break;
				case 2://07+9=16+9=25+9=34
					Led_Pos+=9;
					if(Led_Pos >34)
					{
						Led_Pos =34;
						Led_Mode=3;
					}
				break;
				case 3:
					Led_Pos-=9;
					if(Led_Pos >200)//07-9=-2��u8��˵��255-2=253
					{
						Led_Pos =0;
						Led_Mode=0;
					}
				break;
			}
		}
	}
	if(Led_Mode<2)
	{
	for(i=0;i<8;i++)
	{
		ucLed[i]=(i==Led_Pos);//�������
	}
	}
	else
	{	for(i=0;i<8;i++)
		ucLed[i]=((i==(Led_Pos/10))||(i==(Led_Pos%10)));//�����07��16.....
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
	if(++Led_Count==12)Led_Count=0;//
	if(Seg_Index !=0||Seg_Disp_Mode==1)
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	else Seg_Disp(Seg_Pos,10,0);
	if(Led_Count <=((Led_Level+1)*3))
		Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	else Led_Disp(Seg_Pos,0);
	if(++time_400ms==400)
	{
		time_400ms=0;
		time_flag ^=1;
	}
	if(Start_Flag == 1) //ϵͳ����ʱ��ʼ��ʱ
		Ms_Tick++;
}



/* Main */
void main()
{	unsigned char i;
	System_Init();
	Timer0Init();
	EEPROM_Read(eeprom_dat,0,4);
	for(i=0;i<4;i++)
	{
		T_Disp[i]=T_Ctrl[i]=eeprom_dat[i]*100;//��100�������Ͷ�С��255��
	}
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
		
	}
}