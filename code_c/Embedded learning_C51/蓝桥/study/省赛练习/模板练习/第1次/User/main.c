#include <STC15F2K60S2.H>
#include <Led.h>
#include <init.h>
#include <Key.h>
#include <Seg.h>

#define u8 unsigned char;
#define u16 unsigned int;

u8 Seg_Slow_Down;
u8 Seg_Buf[]={10,10,10,10,10,10,10,10};
u8 Seg_Point[]={0,0,0,0,0,0,0,0};
u8 Seg_pos;

u8 Key_Val,Key_Old,Key_Down,Key_up;
u8 Key_Slow_Down;


u8 uLed[]={0,0,0,0,0,0,0,0};
void Key_Proc()
{
	if(Key_Slow_Down)return;
	 Key_Slow_Down=1;
	
	Key_Val=Key_Read();
	Key_Down=Key_Val&(Key_Val^Key_Old);
	Key_up=~Key_Val&(Key_Val^Key_Old);
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

void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0=1;
	EA=1;
}

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{	if(++ Key_Slow_Down==10) Key_Slow_Down=0;
    if(++Seg_Slow_Down==500)Seg_Slow_Down=0;
	if(++Seg_pos==8)Seg_pos=0;
	Seg_Disp(Seg_pos,Seg_Buf[Seg_pos],Seg_Point[Seg_pos]);
	Led_Disp(Seg_pos,uLed[Seg_pos]);
	
}

//-----------------------------------------------

void main()
{
	init();
	Timer0_Init();
	while(1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
		
	}
}