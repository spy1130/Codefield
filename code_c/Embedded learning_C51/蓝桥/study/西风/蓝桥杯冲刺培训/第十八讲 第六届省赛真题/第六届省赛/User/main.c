/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include "ds1302.h"//时钟底层驱动专用头文件
#include "onewire.h"//温度底层驱动专用头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char Seg_Disp_Mode;//数码管显示模式变量 0-参数设置界面 1-时钟显示界面 2-温度显示界面
unsigned char AcqInvl_Disp = 1;//系统采集间隔变量(显示值)
unsigned char AcqInvl_Ctrol = 1;//系统采集间隔变量(控制值)
unsigned char Time_Data[4] = {1,5,30,60};//时间参数数据储存数组
unsigned char Time_Data_Index;//时间参数数组指针
unsigned char ucRtc[3] = {0x23,0x59,0x50};//时钟数据储存数组 上电默认时间 23：59：50
unsigned char Temperature_Data[10];//温度数据储存数组
unsigned char Temperature_Data_Index;//温度数据储存数组指针
unsigned int Sys_Tick;//系统计时变量
unsigned char Sec_Time;//系统秒数计时变量
unsigned int Timer_500Ms;//五百毫秒计时变量
bit Led_Enable_Flag;//Led使能标志位
bit Led_Star_Flag;//Led闪烁标志位
bit Seg_Star_Flag;//数码管闪烁标志位

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
		case 4://参数切换按键 
			if(Seg_Disp_Mode == 0) //当前处于参数设置界面
			{
				if(++Time_Data_Index == 4) //指针值在0-3之间循环切换 所对应数组的第1-4个数据
					Time_Data_Index = 0;
				AcqInvl_Disp = Time_Data[Time_Data_Index];//将所选中的数据赋值给显示变量
			}
		break;
		case 5://参数确认按键
			if(Seg_Disp_Mode == 0) //当前处于参数设置界面
			{
				AcqInvl_Ctrol = AcqInvl_Disp;//保存设置参数
				Temperature_Data[Temperature_Data_Index] = rd_temperature();//读取一次实时温度数据
				Temperature_Data_Index++;
				Seg_Disp_Mode = 1;//切换到时钟显示界面
			}
		break;
		case 6://数据切换按键
			if(Seg_Disp_Mode == 2) //当前处于温度显示界面
			{
				Led_Enable_Flag = 0;//关闭Led使能状态
				if(++Temperature_Data_Index == 10) //循环切换显示数据
					Temperature_Data_Index = 0;
			}
		break;
		case 7://参数设置按键
			if(Seg_Disp_Mode == 2) //当前处于温度显示界面
			{
				Temperature_Data_Index = 0;//指针值复位
				Seg_Disp_Mode = 0;//切换到参数设置界面
			}
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i;//For循环专用变量
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序
	
	/* 数据读取区域 */
	Read_Rtc(ucRtc);//实时读取时钟数据
	if(Sec_Time == AcqInvl_Ctrol) //达到一次采集间隔
	{
		Sec_Time = 0;//计时复位 便于下次进入
		Temperature_Data[Temperature_Data_Index] = rd_temperature();//读取一次实时温度数据
		if(++Temperature_Data_Index == 10) //一轮温度读取完成
		{
			Temperature_Data_Index = 0;//指针值复位
			Led_Enable_Flag = 1;//使能Led闪烁
			Seg_Disp_Mode = 2;//跳转到数据显示界面
		}
	}
	
	/* 信息显示区域 */
	switch(Seg_Disp_Mode)
	{
		case 0://参数设置界面
			Seg_Buf[0] = Seg_Buf[1] = Seg_Buf[2] = 10;
			Seg_Buf[5] = 11;
			Seg_Buf[6] = AcqInvl_Disp / 10;
			Seg_Buf[7] = AcqInvl_Disp % 10;
		break;
		case 1://时钟显示界面
			for(i=0;i<3;i++)
			{
				Seg_Buf[3*i] = ucRtc[i] / 16;
				Seg_Buf[3*i+1] = ucRtc[i] % 16;
			}
			Seg_Star_Flag = Seg_Buf[7] % 2;//使用时钟芯片记录闪烁时间
			Seg_Buf[2] = Seg_Buf[5] = 10 + (unsigned char)Seg_Star_Flag;//10：熄灭 11：-
		break;
		case 2://温度显示界面
			Seg_Buf[0] = 11;
			Seg_Buf[1] = 0;
			Seg_Buf[2] = Temperature_Data_Index;
			Seg_Buf[3] = Seg_Buf[4] = 10;
			Seg_Buf[5] = 11;
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
	if(Seg_Disp_Mode == 1) //系统开始采集温度
	{
		if(++Sys_Tick == 1000)
		{
			Sys_Tick = 0;
			Sec_Time++;
		}
	}
	
	if(++Timer_500Ms == 500)
	{
		Timer_500Ms = 0;
		Led_Star_Flag ^= 1;
	}
}

/* Main */
void main()
{
	Set_Rtc(ucRtc);//上电设置时钟数据
	rd_temperature();//上电读取一次温度避免第一次温度数据出现85
	System_Init();
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}