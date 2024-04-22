//��Seg���٣�˳����ʱ��������ET0=1,EA=1;Segɨ�裬led+ɨ�裬Key����+����������������
#include <STC15F2K60S2.H>
#include <init.h>
#include <Led.h>
#include <Key.h>
#include <Seg.h>
#include "iic.h"
#include "ds1302.h"

unsigned char Seg_Slow_Down;
unsigned char Seg_Buf[8]={10,10,10,10,10,10,10,10};
unsigned char Seg_Point[8]={0,0,0,0,0,0,0,0};
unsigned char Seg_Pos;
unsigned char Seg_Disp_Mode;//0-ʱ����棬1-������棬2-��¼����

unsigned char ucRtc[3]={0x23,0x09,0x59};
unsigned char ucRtc_Finish_Disp[3];

unsigned char ucLed[8]={0,0,0,0,0,0,0,0};

unsigned char Key_Val,Key_Old,Key_Down,Key_Up;
unsigned char Key_Slow_Down;
unsigned char Input_Data[4];//��������
unsigned char Input_Data_Index;//����ֵ
unsigned int new_value;
unsigned int old_value;
unsigned char EEPROM_string_new[4];
unsigned char EEPROM_string_old[4];
bit data_up_flag; // ����������־
void Key_Proc()
{	unsigned char i;
	if(Key_Slow_Down)return;
	Key_Slow_Down =1;
	//�ص��
	Key_Val=Key_Read();
	Key_Down=Key_Val &(Key_Old^Key_Val);
	Key_Up=~Key_Val &(Key_Old^Key_Val);
	Key_Old =Key_Val;
	
	switch(Key_Down)
	{
		case 4:
			
			if(Seg_Disp_Mode==1)
			{
					
					new_value = Input_Data[1] * 1000 + Input_Data[2] * 100 + Input_Data[3] * 10 + Input_Data[4];
					EEPROM_string_new[0] = ucRtc_Finish_Disp[0] / 16 * 10 + ucRtc_Finish_Disp[0] % 16;
					EEPROM_string_new[1] = ucRtc_Finish_Disp[1] / 16 * 10 + ucRtc_Finish_Disp[1] % 16;
					EEPROM_string_new[2] = new_value & 0xff00;
					EEPROM_string_new[3] = new_value & 0x00ff;
					EEPROM_Write(EEPROM_string_new, 0, 4);//4���ֽ�
					data_up_flag = (new_value > old_value);
					old_value = new_value;
			}
			if(++Seg_Disp_Mode==3)Seg_Disp_Mode=0;
			for (i = 0; i < 4; i++)
				Input_Data[i] = 10; // ֱ�Ӷ�Ӧ��ÿ�ν��н����л���ʱ������һ������
			Input_Data_Index= 0;       // ����������������
		break;
		if(Seg_Disp_Mode==1){
			// �����һλ������
			if(Input_Data_Index==1)
				Read_Rtc(ucRtc_Finish_Disp);
			
			case 5:				
				for(i=0;i<4;i++)
					Input_Data[i]=10;
					Input_Data_Index=0;
			break;
			if(Input_Data_Index<4)
			{
			case 6:
				if(Input_Data_Index!=4)
					Input_Data[Input_Data_Index]=0;
				if(++Input_Data_Index==5)
					Input_Data_Index=4;
			break;
			case 10:
				if(Input_Data_Index!=4)
					Input_Data[Input_Data_Index]=1;
				if(++Input_Data_Index==5)
					Input_Data_Index=4;
			break;
			case 14:
				if(Input_Data_Index!=4)
					Input_Data[Input_Data_Index]=2;
				if(++Input_Data_Index==5)
					Input_Data_Index=4;
			break;
			case 18:
				if(Input_Data_Index!=4)
					Input_Data[Input_Data_Index]=3;
				if(++Input_Data_Index==5)
					Input_Data_Index=4;
			break;
			case 9:
				if(Input_Data_Index!=4)
					Input_Data[Input_Data_Index]=4;
				if(++Input_Data_Index==5)
					Input_Data_Index=4;
			break;
			case 13:
				if(Input_Data_Index!=4)
					Input_Data[Input_Data_Index]=5;
				if(++Input_Data_Index==5)
					Input_Data_Index=4;
			break;
			case 17:
				if(Input_Data_Index!=4)
					Input_Data[Input_Data_Index]=6;
				if(++Input_Data_Index==5)
					Input_Data_Index=4;
			break;
			case 8:
				if(Input_Data_Index!=4)
					Input_Data[Input_Data_Index]=7;
				if(++Input_Data_Index==5)
					Input_Data_Index=4;
			break;
			case 12:
				if(Input_Data_Index!=4)
					Input_Data[Input_Data_Index]=8;
				if(++Input_Data_Index==5)
					Input_Data_Index=4;
			break;
			case 16:
				if(Input_Data_Index!=4)
					Input_Data[Input_Data_Index]=9;
				if(++Input_Data_Index==5)
					Input_Data_Index=4;
			break;
			}
		}
	}
	
}


void Seg_Proc()
{	unsigned char i;
	//����0ʱ����
	if(Seg_Slow_Down) return;
	Seg_Slow_Down =1;
	switch(Seg_Disp_Mode)
	{
		case 0://ʱ�����
			Read_Rtc(ucRtc);
			Seg_Buf[0]=ucRtc[0]/16;
			Seg_Buf[1]=ucRtc[0]%16;
			Seg_Buf[3]=ucRtc[1]/16;
			Seg_Buf[4]=ucRtc[1]%16;
			Seg_Buf[6]=ucRtc[2]/16;
			Seg_Buf[7]=ucRtc[2]%16;
			Seg_Buf[5]=Seg_Buf[2]=11;//-
		break;
		case 1://�������
			
			Seg_Buf[0]=12;//C
			Seg_Buf[1]=Seg_Buf[3]=Seg_Buf[2]=10;
		if (Input_Data_Index == 0)
        {	//�����ؽ�
            Seg_Buf[4] = Seg_Buf[5] = Seg_Buf[6] = Seg_Buf[7] = 10;
        }
        else
        {	//Input_Data_Index ==1��
			//�Ҷ���
			for(i=0;i<Input_Data_Index;i++)
				Seg_Buf[7-i]=Input_Data[Input_Data_Index-i-1];//����Input_Data_Index =1������Ҫ��1���������������λ����
			//�����ص�
			for(;i<4;i++)
				Seg_Buf[7-i]=10;	
		}	
		break;
		case 2://��¼����
			Seg_Buf[0]=13;//E
			Seg_Buf[3]=ucRtc_Finish_Disp[0]/16;
			Seg_Buf[4]=ucRtc_Finish_Disp[0]%16;
			Seg_Buf[5]=11;//-
			Seg_Buf[6]=ucRtc_Finish_Disp[1]/16;
			Seg_Buf[7]=ucRtc_Finish_Disp[1]%16;	
		break;
	}

}

void Led_Proc()
{
	ucLed[0] = (Seg_Disp_Mode == 0);
    ucLed[1] = (Seg_Disp_Mode == 1);
    ucLed[2] = (Seg_Disp_Mode == 2);

    ucLed[3] = data_up_flag;
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
{	if(++Key_Slow_Down ==10) Key_Slow_Down = 0;//���̼���ר��
    if(++Seg_Slow_Down==500) Seg_Slow_Down=0;//����ܼ���ר��
	if(++Seg_Pos==8)Seg_Pos=0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
}



void main()
{	
	System_Init();
	Timer0_Init();
	Set_Rtc(ucRtc);
	EEPROM_Read(EEPROM_string_old, 0, 4);//д�뵽old��
    old_value = EEPROM_string_old[2] << 8 | EEPROM_string_old[3];
	while(1)
	{
		Seg_Proc();
		Key_Proc();
		Led_Proc();
	}
	
}