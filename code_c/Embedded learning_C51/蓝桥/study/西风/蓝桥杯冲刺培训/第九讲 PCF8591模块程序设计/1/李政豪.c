//ͷ�ļ�
#include <STC15F2K60S2.H>
#include <IIC.h>
#include <Init.h>
#include <Key.h>
#include <Seg.h>
#include <Led.h>
/*----------------��������-------------------*/
unsigned char Seg_Disp_Mobe;
unsigned char Key_Val,Key_Old,Key_Down,Key_Up;
unsigned char Key_Slow_Down;
unsigned int  Seg_Slow_Down;
unsigned char Seg_Pos;
unsigned char Seg_Buf[]={10,10,10,10,10,10,10,10};
unsigned char ucLed[]={0,0,0,0,0,0,0,0};
unsigned char Seg_Point[]={0,0,0,0,0,0,0,0};
/*---------------IICЭ�����-----------------*/
float         Date;                                			//ADת������
/*---------------��ѹ��ʾ����----------------*/
unsigned char Seg_Show[]={11,10,10,10,10,10,10,10};			//��ѹ��ʾ����ר��
/*---------------��ѹ���ģʽ----------------*/
unsigned char Seg_Output[]={12,10,10,10,10,10,10,10};   //��ѹ�������ר��
/*---------------�������ܱ���----------------*/
unsigned char Output_Solid;                             //�̶������������
unsigned char Seg_Switch=1;                             //����Seg������(������ʼֵ)
unsigned char Led_Function=1;                           //����Led������(������ʼֵ)
//key����
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down=1;
	
	Key_Val=Key_Read();
	Key_Down=Key_Val & (Key_Old ^ Key_Val);
	Key_Up=~Key_Val & (Key_Old ^ Key_Val);
	Key_Old=Key_Val;
	
	switch(Key_Down)
	{
		case 4 :
//		switch(Seg_Disp_Mobe)
//		{
//			case 0 :
//			Seg_Disp_Mobe=1;
//			break;
//			
//			case 1 :
//			Seg_Disp_Mobe=0;
//			break;
//		}
//		break;
		if(++Seg_Disp_Mobe==2) Seg_Disp_Mobe=0;
		break;
		
		case 5 :
		Output_Solid=~Output_Solid;
		break;
		
		case 7 :
		Seg_Switch=~Seg_Switch;
		break;
		
		case 6 :
		Led_Function=~Led_Function;
	}
}

//Seg����
void Seg_Proc()
{
	unsigned char Cycle;                            //ѭ������
	
	if(Seg_Slow_Down) return;
	Seg_Slow_Down=1;
	
	Date=Ad_Read(0x43);                             //��ȡ�����������ĵ�ѹֵ
	
	switch(Seg_Disp_Mobe)
	{
		case 0 :                                      //��ѹ��ʾ����
		Seg_Show[5]=(unsigned char)Date/51.0;
		Seg_Show[6]=(unsigned char)(Date/51.0*10)%10;
		Seg_Show[7]=(unsigned char)(Date/51.0*100)%10;
		for(Cycle=0;Cycle<8;Cycle++)
		Seg_Buf[Cycle]=Seg_Show[Cycle];
		Seg_Point[5]=1;
		break;
		
		case 1 :                                      //��ѹ�������
		Seg_Output[5]=Output_Solid?(unsigned char)Date/51.0:2;
		Seg_Output[6]=Output_Solid?(unsigned char)(Date/51.0*10)%10:0;
		Seg_Output[7]=Output_Solid?(unsigned char)(Date/51.0*100)%10:0;
		for(Cycle=0;Cycle<8;Cycle++)
		Seg_Buf[Cycle]=Seg_Output[Cycle];
		Seg_Point[5]=1;
		break;
	}
	
	if(Seg_Switch!=1)
	{
		for(Cycle=0;Cycle<8;Cycle++)
		Seg_Buf[Cycle]=10;
		Seg_Point[5]=0;
	}
}

//Led����
void Led_Proc()
{
	unsigned char Move;                       //�ƶ�����
	/*-----------Led�ܿ��ش�-----------*/	
	if(Led_Function==1)
	{
	/*-------��1���͵ڶ���Led��---------*/
	if(Seg_Disp_Mobe==0) 
	{ucLed[0]=1;ucLed[1]=0;}
	if(Seg_Disp_Mobe==1)
	{ucLed[0]=0;ucLed[1]=1;}
	
	/*----------������Led��------------*/
	if(Seg_Show[5]+Seg_Show[6]*0.1<1.5)
	ucLed[2]=0;
	if(Seg_Show[5]+Seg_Show[6]*0.1>=1.5 && Seg_Show[5]+Seg_Show[6]*0.1<2.5)
	ucLed[2]=1;
	if(Seg_Show[5]+Seg_Show[6]*0.1>=2.5 && Seg_Show[5]+Seg_Show[6]*0.1<3.5)
	ucLed[2]=0;
	if(Seg_Show[5]+Seg_Show[6]*0.1>=3.5)
	ucLed[2]=1;
	
	/*-----------���ĸ�Led��-----------*/
	if(Output_Solid==0)
	ucLed[3]=0;
	else
	ucLed[3]=1;
	}
	/*-------Led�ܿ��عر�-------------*/
	else
	{
		for(Move=0;Move<4;Move++)
		ucLed[Move]=0;
	}
}

//��ʱ��0����
void Timer0Init(void)		
{
	AUXR &= 0x7F;		
	TMOD &= 0xF0;		
	TL0 = 0x18;		
	TH0 = 0xFC;		
	TF0 = 0;		
	TR0 = 1;
	ET0=1;
	EA=1;
}
//��ʱ��0������ 
void Time0Server() interrupt 1
{
	if(++Seg_Slow_Down==500) Seg_Slow_Down=0;
	if(++Key_Slow_Down==10)  Key_Slow_Down=0;
	
	if(++Seg_Pos==8) Seg_Pos=0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	
}

//main����
void main()
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