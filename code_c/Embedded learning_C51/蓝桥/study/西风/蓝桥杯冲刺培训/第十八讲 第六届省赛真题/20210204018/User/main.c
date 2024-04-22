/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include "ds1302.h"  
#include "onewire.h"

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char Seg_Disp_Mode;//数码管显示模式变量 0-参数设置 1-时钟显示 2-温度显示
unsigned char AcqInvl_Disp = 1;//采集间隔显示变量
unsigned char AcqInvl_Ctrol = 1;//采集间隔控制变量
unsigned char ucRtc[3] = {0x23,0x59,0x50};//系统时钟数据储存变量
unsigned char Time_Data[4] = {1,5,30,60};//时间间隔数据储存数组
unsigned char Time_Data_Index;
bit Seg_Star_Flag;//数码管闪烁标志位
unsigned char Temperature_Data[10];//温度采集数据储存数组
unsigned char Temperature_Data_Index;
unsigned int Sys_Tick;//系统计时器
unsigned char Time_Sec;
unsigned char Timer_200Ms;
bit Led_Star_Flag;
bit Led_Enable_Flag;

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
			if(Seg_Disp_Mode == 0)
			{
				if(++Time_Data_Index == 4)
					Time_Data_Index = 0;
				AcqInvl_Disp = Time_Data[Time_Data_Index];
			}
		break;
		case 5:
			if(Seg_Disp_Mode == 0)
			{
				AcqInvl_Ctrol = AcqInvl_Disp;
				Temperature_Data[Temperature_Data_Index] = rd_temperature();
				Temperature_Data_Index++;
				Seg_Disp_Mode = 1;
			}
		break;
		case 6:
			if(Seg_Disp_Mode == 2)
			{
				Led_Enable_Flag = 0;
				if(++Temperature_Data_Index == 10)
					Temperature_Data_Index = 0;
			}
		break;
		case 7:
			if(Seg_Disp_Mode == 2)
			{
				Temperature_Data_Index = 0;
				Seg_Disp_Mode = 0;
			}
		break;
	}

}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i;
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序

	/* 数据读取区域 */
	Read_Rtc(ucRtc);
	if(Time_Sec == AcqInvl_Ctrol)
	{
		Time_Sec = 0;
		Temperature_Data[Temperature_Data_Index] = rd_temperature();
		if(++Temperature_Data_Index == 10)
		{
			Temperature_Data_Index = 0;
			Led_Enable_Flag = 1;
			Seg_Disp_Mode = 2;
		}
	}
	
	/* 信息显示区域 */
	switch(Seg_Disp_Mode)
	{
		case 0://参数设置
			for(i=0;i<3;i++)
				Seg_Buf[i] = 10;
			Seg_Buf[5] = 11;
			Seg_Buf[6] = AcqInvl_Disp / 10;
			Seg_Buf[7] = AcqInvl_Disp % 10;
		break;
		case 1:
			for(i=0;i<3;i++)
			{
				Seg_Buf[3*i] = ucRtc[i] / 16;
				Seg_Buf[3*i+1] = ucRtc[i] % 16;
			}
			Seg_Star_Flag = Seg_Buf[7] % 2;
			Seg_Buf[2] = Seg_Buf[5] = Seg_Star_Flag?10:11;
		break;
		case 2:
			Seg_Buf[0] = Seg_Buf[5] = 11;
			Seg_Buf[1] = 0;
			Seg_Buf[2] = Temperature_Data_Index;
			Seg_Buf[3] = Seg_Buf[4] = 10;
			Seg_Buf[6] = Temperature_Data[Temperature_Data_Index] / 10;
			Seg_Buf[7] = Temperature_Data[Temperature_Data_Index] % 10;
		break;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	ucLed[0] = Led_Enable_Flag & Led_Star_Flag;
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

/* 定时器0中断服务函数 */
void Timer0Server() interrupt 1
{  
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	if(Seg_Disp_Mode == 1)
	{
		if(++Sys_Tick == 1000)
		{
			Sys_Tick = 0;
			Time_Sec++;
		}
	}
	if(++Timer_200Ms == 200)
	{
		Timer_200Ms = 0;
		Led_Star_Flag ^= 1;
	}
}

/* Main */
void main()
{
	rd_temperature();
	Set_Rtc(ucRtc);
	System_Init();
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}