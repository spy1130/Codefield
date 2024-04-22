/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include <ultrasound.h>

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned int Freq;//实时频率储存变量
unsigned int Timer_1000Ms;//一千毫秒计时变量
unsigned char Wave_Dat;//超声波测距变量
bit Seg_Disp_Mode;//数码管显示模式变量 0-频率显示 1-距离显示
bit Wave_Flag;//测距使能标志位 0-不使能 1-使能

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
			Seg_Disp_Mode ^= 1;
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i = 3;//循环专用变量
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序

	/* 信息获取区域 */
	Wave_Dat = Wave_Read();
	
	
	/* 数据显示区域 */
	Seg_Buf[0] = 11;//标识符U
	Seg_Buf[1] = 1 + (unsigned char)Seg_Disp_Mode;//界面标识符
	Seg_Buf[2] = 10;//熄灭
	Seg_Point[5] = Seg_Disp_Mode;
	
	if(Seg_Disp_Mode == 0) //处于频率显示界面
	{
		Seg_Buf[3] = Freq / 10000 % 10;
		Seg_Buf[4] = Freq / 1000 % 10;
		Seg_Buf[5] = Freq / 100 % 10;
		Seg_Buf[6] = Freq / 10 % 10;
		Seg_Buf[7] = Freq % 10;
		while(Seg_Buf[i] == 0)
		{
			Seg_Buf[i] = 10;
			if(++i == 7) break;
		}
	}
	else
	{
		Seg_Buf[3] = 10;
		Seg_Buf[4] = 10;
		Seg_Buf[5] = Wave_Dat / 100 % 10;
		Seg_Buf[6] = Wave_Dat / 10 % 10;
		Seg_Buf[7] = Wave_Dat % 10;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	
}

/* 定时器0初始化函数 */
void Timer0Init(void)		//0毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x04;		//设置计数模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x00;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

/* 定时器2初始化函数 */
void Timer2Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0x18;		//设置定时初值
	T2H = 0xFC;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2  |=  0x04;    //允许定时器2中断
	EA = 1;					//总中断打开
}


/* 定时器0中断服务函数 */
void Timer0Server() interrupt 12
{  
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	
	if(++Timer_1000Ms == 1000) //实时读取频率值
	{
		Timer_1000Ms = 0;
		TR0 = 0;
		Freq = TH0 << 8 | TL0;
		TH0 = TL0 = 0;
		TR0 = 1;
	} 
}

/* Main */
void main()
{
	System_Init();
	Timer2Init();
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}