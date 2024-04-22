/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include "ds1302.h"//时钟底层驱动专用头文件
#include "iic.h"//单总线底层驱动专用头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
bit Seg_Disp_Mode;//数码管显示模式变量 0-数据显示 1-参数设置
unsigned char ucRtc[3] = {0x08,0x30,0x00};//实时时钟数据存放数组 上电默认时间08:30:00
unsigned char Humidity;//实时湿度变量
unsigned char Hum_Disp = 50;//湿度阈值显示变量
unsigned char Hum_Ctrol = 50;//湿度阈值控制变量
bit Work_Mode;//系统工作模式变量 0-自动工作状态 1-手动工作状态
bit Relay_Flag;//继电器使能标志位
bit Beep_Enable_Flag;//闹钟功能使能标志位

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
		case 7://工作模式按键
			if(Seg_Disp_Mode == 0) //处于数据显示界面 避免出现程序BUG
			{
				Relay_Flag = 0;//切换模式后将浇灌系统关闭
				Work_Mode ^= 1;
			}
		break;
		case 6://阈值调整按键（自动）- 闹钟功能按键（手动）
			if(Work_Mode == 0) //处于自动工作状态
			{
				Seg_Disp_Mode ^= 1;
				if(Seg_Disp_Mode == 0) //从阈值设置界面退出
				{
					Hum_Ctrol = Hum_Disp;//阈值参数生效
					EEPROM_Write(&Hum_Ctrol,0,1);//保存阈值参数
				}
			}
			else
				Beep_Enable_Flag ^= 1;
		break;
		case 5://阈值自加按键（自动） - 系统开启按键（手动）
			if(Work_Mode == 0) //处于自动工作状态
			{
				if(Seg_Disp_Mode == 1) //处于阈值设置界面
				{
					if(++Hum_Disp == 100)
						Hum_Disp = 99;
				}
			}
			else
				Relay_Flag = 0;
		break;
		case 4://阈值自减按键（自动） - 系统关闭按键（手动）
			if(Work_Mode == 0) //处于自动工作状态
			{
				if(Seg_Disp_Mode == 1) //处于阈值设置界面
				{
					if(--Hum_Disp == 255)
						Hum_Disp = 0;
				}
			}
			else
				Relay_Flag = 1;
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序
	
	/* 信息获取区域 */
	Read_Rtc(ucRtc);//实时读取时钟数据
	Humidity = Ad_Read(0x03) / 2.56;//实时读取湿度数据
	if(Work_Mode == 0) //处于自动模式
		Relay_Flag = Hum_Ctrol / Humidity;
	
	/* 数据显示区域 */
	if(Seg_Disp_Mode == 0) //处于数据显示界面
	{
		Seg_Buf[0] = ucRtc[0] / 16;
		Seg_Buf[1] = ucRtc[0] % 16;
		Seg_Buf[3] = ucRtc[1] / 16;
		Seg_Buf[4] = ucRtc[1] % 16;
		Seg_Buf[2] = 11;
		Seg_Buf[6] = Humidity / 10;
		Seg_Buf[7] = Humidity % 10;
	}
	else //处于参数设置界面
	{
		Seg_Buf[0] = Seg_Buf[1] = 11;
		Seg_Buf[2] = Seg_Buf[3] = Seg_Buf[4] = 10;
		Seg_Buf[6] = Hum_Disp / 10;
		Seg_Buf[7] = Hum_Disp % 10;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	ucLed[0] = !Work_Mode;//自动模式指示灯
	ucLed[1] = Work_Mode;//手动模式指示灯
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
	Relay(Relay_Flag);
	Beep(Relay_Flag & Beep_Enable_Flag);	
}

/* Main */
void main()
{
	Set_Rtc(ucRtc);//上电设置时钟值为08:30:00
	Humidity = Ad_Read(0x03) / 2.56;//上电读取一次湿度 避免数据错误
	EEPROM_Read(&Hum_Ctrol,0,1);//读取阈值参数
	Hum_Disp = Hum_Ctrol;//便于设置阈值
	System_Init();
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}