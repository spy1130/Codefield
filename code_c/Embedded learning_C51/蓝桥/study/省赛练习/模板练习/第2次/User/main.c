#include <STC15F2K60S2.H>
#include <Led.h>
#include <Key.h>
#include <init.h>
#include <Seg.h>


u8 Seg_Buf[8]={10,10,10,10,10,10,10,10};
u8 Seg_Point[8]={0,0,0,0,0,0,0,0};
u8 Seg_Slow_Down;
u8 Seg_Pos;

u8 ucLed[8]={0,0,0,0,0,0,0,0};

u8 Key_Down,Key_Up,Key_Val,Key_Old;
u8 Key_Slow_Down;

void Key_Proc()
{	
	if(Key_Slow_Down)return;
	Key_Slow_Down=1;
	
	Key_Val=Key_Read();
	Key_Down=Key_Val&(Key_Val^Key_Old);
	Key_Up=~Key_Val&(Key_Val^Key_Old);
	Key_Old=Key_Val;
	
	
}

void Seg_Proc()
{
	if(Seg_Slow_Down)return;
	Seg_Slow_Down=1;
}
void Led_Proc()
{

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


//-----------------------------------------------

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{	
	if(++Key_Slow_Down==10)Key_Slow_Down=0;
    if(++Seg_Slow_Down==500)Seg_Slow_Down=0;
	if(++Seg_Pos==8)Seg_Pos=0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	
}

//-----------------------------------------------


void main()
{
	System_init();
	Timer0_Init();
	
	while(1)
	{
		Seg_Proc();
		Key_Proc();
		Led_Proc();
	}
}