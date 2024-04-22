//��Seg���٣�˳����ʱ��������ET0=1,EA=1;Segɨ�裬led+ɨ�裬Key����+����������������
#include <STC15F2K60S2.H>
#include <init.h>
#include <Led.h>
#include <Key.h>
#include <Seg.h>
#define u8 unsigned char
#define u16 unsigned int
unsigned char Seg_Slow_Down;
unsigned char Seg_Buf[8]={10,10,10,10,10,10,10,10};
unsigned char Seg_Point[8]={0,0,0,0,0,0,0,0};
unsigned char Seg_Pos;

unsigned char ucLed[8]={0,0,0,0,0,0,0,0};

unsigned char Key_Val,Key_Old,Key_Down,Key_Up;
unsigned char Key_Slow_Down;
u16 Time_1000ms;//Ƶ����ʱ����
u16 Freq;//Ƶ��

void Key_Proc()
{
	if(Key_Slow_Down)return;
	Key_Slow_Down =1;
	//�ص��
	Key_Val=Key_Read();
	Key_Down=Key_Val &(Key_Old^Key_Val);
	Key_Up=~Key_Val &(Key_Old^Key_Val);
	Key_Old =Key_Val;
	
	
}


void Seg_Proc()
{	//����0ʱ����
	if(Seg_Slow_Down) return;
	Seg_Slow_Down =1;
	

}

void Led_Proc()
{
	
}
void Timer0_Init(void)		//������@12.000MHz
{
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TMOD |=0x05;            //��Ƽ���ģʽ
	TL0 = 0x00;				//���ö�ʱ��ʼֵ
	TH0 = 0x00;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
}

void Timer1_Init(void)		//1����@12.000MHz
{
	AUXR &= 0xBF;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TL1 = 0x18;				//���ö�ʱ��ʼֵ
	TH1 = 0xFC;				//���ö�ʱ��ʼֵ
	TF1 = 0;				//���TF1��־
	TR1 = 1;				//��ʱ��1��ʼ��ʱ
	ET1=1;					//�ж�
	EA=1;
}

/* Timer0 interrupt routine */
void tm1_isr() interrupt 3
{	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//���̼���ר��
    if(++Seg_Slow_Down==500) Seg_Slow_Down=0;//����ܼ���ר��
	if(++Seg_Pos==8)Seg_Pos=0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	if(++Time_1000ms==1000)
	{
		TR0=0;//�رն�ʱ��
		Time_1000ms=0;
		Freq =TH0<<8 |TL0;
		TH0=TL0=0;
		TR0=1;
	}
	
}



void main()
{
	System_Init();
	Timer0_Init();
	while(1)
	{
		Seg_Proc();
		Key_Proc();
		Led_Proc();
		
	}
	
}