//先Seg减速，顺带定时器别忘了ET0=1,EA=1;Seg扫描，led+扫描，Key减速+三行消抖，主程序
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
u16 Time_1000ms;//频率延时函数
u16 Freq;//频率

void Key_Proc()
{
	if(Key_Slow_Down)return;
	Key_Slow_Down =1;
	//重点记
	Key_Val=Key_Read();
	Key_Down=Key_Val &(Key_Old^Key_Val);
	Key_Up=~Key_Val &(Key_Old^Key_Val);
	Key_Old =Key_Val;
	
	
}


void Seg_Proc()
{	//等于0时进入
	if(Seg_Slow_Down) return;
	Seg_Slow_Down =1;
	

}

void Led_Proc()
{
	
}
void Timer0_Init(void)		//计数器@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |=0x05;            //设计计数模式
	TL0 = 0x00;				//设置定时初始值
	TH0 = 0x00;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
}

void Timer1_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xBF;			//定时器时钟12T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x18;				//设置定时初始值
	TH1 = 0xFC;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1=1;					//中断
	EA=1;
}

/* Timer0 interrupt routine */
void tm1_isr() interrupt 3
{	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
    if(++Seg_Slow_Down==500) Seg_Slow_Down=0;//数码管减速专用
	if(++Seg_Pos==8)Seg_Pos=0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	if(++Time_1000ms==1000)
	{
		TR0=0;//关闭定时器
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