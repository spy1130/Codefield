/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include "onewire.h"//温度底层驱动专用头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char Wind_Mode;//风速模式 0-睡眠风 1-自然风 2-常风
unsigned char Work_Time;//剩余工作时间变量
unsigned char Time_Data[3] = {0,60,120};//时间设置数据储存数组
unsigned char Time_Data_Index;//时间设置数组指针
unsigned int Timer_1000Ms;//一千毫秒计时变量
unsigned char Temperature;//实时温度变量
unsigned char PWM_Count;//PWM计数变量
unsigned char PWM_Level;//PWM等级变量
bit Seg_Disp_Mode;//数码管显示模式变量 0-工作状态 1-温度模式

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
		case 4://工作模式切换按键
			if(Seg_Disp_Mode == 0) //处于工作模式状态
			{
				if(++Wind_Mode == 3) //循环切换三种风速模式
					Wind_Mode = 0;
			}
		break;
		case 5://定时设置按键
			if(Seg_Disp_Mode == 0) //处于工作模式状态
			{
				if(++Time_Data_Index == 3) //切换索引值
					Time_Data_Index = 0;
				Work_Time = Time_Data[Time_Data_Index];//重新设置时间
			}
		break;
		case 6://系统停止按键
			if(Seg_Disp_Mode == 0) //处于工作模式状态
				Work_Time = 0;//复位计时值
		break;
		case 7://室温功能按键
			Seg_Disp_Mode ^= 1;//切换数码管显示模式
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序

	/* 信息获取区域 */
	Temperature = rd_temperature();//实时读取温度
	
	/* 数据显示区域 */
	if(Seg_Disp_Mode == 0) //处于工作状态
	{
		Seg_Buf[0] = Seg_Buf[2] = 11;// -
		Seg_Buf[1] = Wind_Mode + 1;
		Seg_Buf[4] = 0;
		Seg_Buf[5] = Work_Time / 100 % 10;
		Seg_Buf[6] = Work_Time / 10 % 10;
		Seg_Buf[7] = Work_Time % 10;
	}
	else //处于温度模式
	{
		Seg_Buf[0] = Seg_Buf[2] = 11;// -
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
	unsigned char i;//For循环专用变量
	for(i=0;i<3;i++) //互斥点亮
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

/* 定时器1中断初始化函数 */
void Timer1Init(void)		//100微秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x9C;		//设置定时初始值
	TH1 = 0xFF;		//设置定时初始值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;
}

/* 定时器0中断服务函数 */
void Timer0Server() interrupt 1
{  
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	
	if(Work_Time != 0) //当计时值不为0时
	{
		if(++Timer_1000Ms == 1000) //1000毫秒触发一次
		{
			Timer_1000Ms = 0;
			Work_Time--;
		}
	}
}

/* 定时器1中断服务函数 */
void Timer1Server() interrupt 3
{
	//信号频率为1KMhz 所对应周期为1毫秒 故需要生成更短时间的定时器中断实现PWM
	if(Work_Time != 0) //当计时值不为0时
	{
		if(++PWM_Count == 10) PWM_Count = 0;//定义一个周期为一毫秒
		PWM_Level = (3 * Wind_Mode) + ((Wind_Mode + 1) % 2);//根据风速调整占空比
		if(PWM_Count >= PWM_Level)
			P34 = 1;
		else
			P34 = 0;
	}
}

/* Main */
void main()
{
	System_Init();
	Timer0Init();
	Timer1Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}