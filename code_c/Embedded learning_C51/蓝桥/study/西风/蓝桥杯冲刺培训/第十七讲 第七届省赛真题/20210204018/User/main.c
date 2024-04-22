/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include "onewire.h"

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char Work_Time;//工作时间变量
unsigned char Wind_Mode;//风速模式变量 0-睡眠风 1-自然风 2-常风
unsigned char Time_Data[3] = {0,60,120};//设置时间储存数组
unsigned char Time_Data_Index;//时间储存数组指针
bit Seg_Disp_Mode;//数码管显示模式变量 0-风速 1-温度
unsigned int Timer_1000Ms;//一千毫秒计时变量
unsigned char Temperature;//实时温度变量
unsigned char Pwm_Count;
unsigned char Pwm_Level;

/* 键盘处理函数 */
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上降沿
	Key_Old = Key_Val;//辅助扫描变量


	switch(Key_Down)
	{
		case 4:
			if(++Wind_Mode == 3)
				Wind_Mode = 0;
		break;
		case 5:
			if(++Time_Data_Index == 3)
				Time_Data_Index = 0;
			Work_Time = Time_Data[Time_Data_Index];
		break;
		case 6:
			Work_Time = 0;
		break;
		case 7:
			Seg_Disp_Mode ^= 1;
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序
	
	Temperature = rd_temperature();
	switch(Wind_Mode)
	{
		case 0:
			Pwm_Level = 2;
		break;
		case 1:
			Pwm_Level = 3;
		break;
		case 2:
			Pwm_Level = 7;
		break;		
	}
	
	
	if(Seg_Disp_Mode == 0)
	{
		Seg_Buf[1] = Wind_Mode + 1;
		Seg_Buf[0] = Seg_Buf[2] = 11;
		Seg_Buf[4] = 0;
		Seg_Buf[5] = Work_Time / 100 % 10;
		Seg_Buf[6] = Work_Time / 10 % 10;
		Seg_Buf[7] = Work_Time % 10;
	}
	else
	{
		Seg_Buf[1] = 4;
		Seg_Buf[4] = 10;
		Seg_Buf[5] = Temperature / 10 % 10;
		Seg_Buf[6] = Temperature % 10;
		Seg_Buf[7] = 12;		
	}

}

/* 其他显示函数 */
void Led_Proc()
{
	unsigned char i;
	for(i=0;i<3;i++)
		ucLed[i] = (i == Wind_Mode) * Work_Time;
}

/* 定时器0中断初始化函数 */
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;    //定时器中断0打开
	EA = 1;     //总中断打开
}

void Timer1Init(void)		//100微秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x9C;		//设置定时初始值
	TH1 = 0xFF;		//设置定时初始值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;
	EA = 1;
}


/* 定时器0中断服务函数 */
void Timer0Server() interrupt 1
{  
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	
	if(Work_Time != 0)
	{
		if(++Timer_1000Ms == 1000)
		{
			Timer_1000Ms = 0;
			Work_Time--;
		}
	}
}

void Timer1Server() interrupt 3
{
	if(++Pwm_Count == 10) Pwm_Count = 0;
	if(Work_Time != 0)
	{
		if(Pwm_Count >= Pwm_Count)
			P34 = 1;
		else
			P34 = 0;
	}
	else
		P34 = 0;
}

/* Main */
void main()
{
	System_Init();
	Timer1Init();
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}