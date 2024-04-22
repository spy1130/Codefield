/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include "ds1302.h"//DS1302底层驱动专用头文件
#include <string.h>//memcpy函数专用头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char Seg_Disp_Mode;//数码管显示模式变量 0-时钟显示 1-闹钟显示 2-日期显示 3-时钟设置 4-闹钟设置 5-日期设置
unsigned char ucRtc[3] = {0x23,0x59,0x55};//时钟数据存放数组 上电初始化时间 23:59:55
unsigned char ucDate[3] = {0x22,0x12,0x12};//日期数据存放数组 上电初始化日期 22-12-12
unsigned char ucRtc_Set[6];//时钟数据设置数组 将时分秒的十位和个位拆分成六个数字 便于直接输入后的替换
unsigned char ucDate_Set[6];//日期数据设置数组
unsigned char Alarm_Set[3][6];//闹钟数据设置数组
unsigned char Set_Index;//设置数组指针 用于判断当前输入的位数
unsigned int Timer_500Ms;//五百毫秒计时变量
bit Seg_Star_Flag;//数码管闪烁标志位


/* 方法一 结构体 */
//struct Alarm//定义闹钟结构体
//{
//	unsigned char Hour;
//	unsigned char Min;
//	unsigned char Sec;
//};
//struct Alarm Alarm_Dat[3] = //定义闹钟数据
//{
//	{0x00,0x00,0x00},
//	{0x00,0x01,0x00},
//	{0xbb,0xbb,0xbb}
//};

/* 方法二 二维数组 */
unsigned char Alarm_Dat[3][3] = 
{
	{0x00,0x00,0x00},
	{0x00,0x01,0x00},
	{0xbb,0xbb,0xbb}
};

unsigned char Alarm_Dat_Index;//闹钟数组指针 用于表示闹钟编号

/* 数组数据读取函数 */
void Dat_To_Set(unsigned char* Dat,unsigned char* Set)
{
	unsigned char i;//用于For循环
	for(i=0;i<3;i++)
	{
		Set[2*i] = Dat[i] / 16;
		Set[2*i+1] = Dat[i] % 16;
	}
}

/* 键盘处理函数 */
void Key_Proc()
{
	unsigned char i,j;//用于For循环
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上降沿
	Key_Old = Key_Val;//辅助扫描变量

	/* 键盘输入区域 */
	if(Key_Down >= 4 && Key_Down <= 13)
	{
		switch(Seg_Disp_Mode)
		{
			case 3://时钟设置
				ucRtc_Set[Set_Index] = Key_Down - 4;
			break;
			case 4://闹钟设置
				Alarm_Set[Alarm_Dat_Index][Set_Index] = Key_Down - 4;
			break;
			case 5://日期设置
				ucDate_Set[Set_Index] = Key_Down - 4;
			break;
		}
		//if(++Set_Index == 6) Set_Index = 0;//循环输入
		Set_Index = (Set_Index + 1) % 6;//循环通用表达式：（变量-循环起点+1）%（循环终点-循环起点+1） + 循环起点
	}
	
	switch(Key_Down)
	{
		case 14://界面切换按键
			if(Seg_Disp_Mode < 3) //在非参数设置状态下使能
			{
				if(++Seg_Disp_Mode == 3) //在0-2之间循环切换
					Seg_Disp_Mode = 0;
			}
		break;
		case 15://参数设置按键
			if(Seg_Disp_Mode < 3) //在非参数设置状态
			{
				Seg_Disp_Mode += 3;//切换到对应的设置界面内
				switch(Seg_Disp_Mode)
				{
					case 3://切换到时钟设置界面
//						for(i=0;i<3;i++)//将当前时钟数据给设置数组
//						{
//							ucRtc_Set[2*i] = ucRtc[i] / 16;
//							ucRtc_Set[2*i+1] = ucRtc[i] % 16;
//						}
						Dat_To_Set(ucRtc,ucRtc_Set);
					break;
					case 4://切换到闹钟设置界面
						Alarm_Dat_Index = 0;//闹钟指针复位
						for(i=0;i<3;i++)//将当前闹钟数据给设置数组
						{
							Dat_To_Set(Alarm_Dat[i],Alarm_Set[i]);
//							for(j=0;j<3;j++)
//							{
//								Alarm_Set[i][2*j] = Alarm_Dat[i][j] / 16;
//								Alarm_Set[i][2*j+1] = Alarm_Dat[i][j] % 16;
//							}
						}
					break;	
					case 5://切换到日期设置界面
						Dat_To_Set(ucDate,ucDate_Set);
//						for(i=0;i<3;i++)//将当前日期数据给设置数组
//						{
//							ucDate_Set[2*i] = ucDate[i] / 16;
//							ucDate_Set[2*i+1] = ucDate[i] % 16;
//						}
					break;						
				}
			}
		break;
		case 17://闹钟切换按键
			if(Seg_Disp_Mode == 1 || Seg_Disp_Mode == 4) // 处于闹钟显示界面
			{
				Set_Index = 0;//每次切换将输入标志位置0
				if(++Alarm_Dat_Index == 3) //在0-2之间循环切换
					Alarm_Dat_Index = 0;				
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
	
	/* 信息读取区域 */
	Read_Rtc(ucRtc);//读取实时时钟数据
	Read_Date(ucDate);//读取实时日期数据
	
	/* 数据处理区域 */
	switch(Seg_Disp_Mode)
	{
		case 0://时钟显示
//			Seg_Buf[0] = ucRtc[0] / 16;
//			Seg_Buf[1] = ucRtc[0] % 16;
//			Seg_Buf[3] = ucRtc[1] / 16;
//			Seg_Buf[4] = ucRtc[1] % 16;
//			Seg_Buf[6] = ucRtc[2] / 16;
//			Seg_Buf[7] = ucRtc[2] % 16;		
//		  Seg_Buf[2] = Seg_Buf[5] = 11;
			for(i=0;i<3;i++)
			{
				Seg_Buf[3*i] = ucRtc[i] / 16;
				Seg_Buf[3*i+1] = ucRtc[i] % 16;
			}
			Seg_Buf[2] = Seg_Buf[5] = 11;
		break;
		case 1://闹钟显示
//			Seg_Buf[0] = Alarm_Dat[Alarm_Dat_Index].Hour / 16;
//			Seg_Buf[1] = Alarm_Dat[Alarm_Dat_Index].Hour % 16;
//			Seg_Buf[3] = Alarm_Dat[Alarm_Dat_Index].Min / 16;
//			Seg_Buf[4] = Alarm_Dat[Alarm_Dat_Index].Min % 16;
//			Seg_Buf[6] = Alarm_Dat[Alarm_Dat_Index].Sec / 16;
//			Seg_Buf[7] = Alarm_Dat[Alarm_Dat_Index].Sec % 16;		
//		  Seg_Buf[2] = Seg_Buf[5] = 11;
			for(i=0;i<3;i++)
			{
				Seg_Buf[3*i] = Alarm_Dat[Alarm_Dat_Index][i] / 16;
				Seg_Buf[3*i+1] = Alarm_Dat[Alarm_Dat_Index][i] % 16;
			}
			Seg_Buf[2] = Seg_Buf[5] = 11;		
		break;
		case 2://日期显示
			for(i=0;i<3;i++)
			{
				Seg_Buf[3*i] = ucDate[i] / 16;
				Seg_Buf[3*i+1] = ucDate[i] % 16;
			}
			Seg_Buf[2] = Seg_Buf[5] = 11;			
		break;
		case 3://时钟设置
			for(i=0;i<3;i++)
			{
				Seg_Buf[3*i] = ucRtc_Set[2*i];
				Seg_Buf[3*i+1] = ucRtc_Set[2*i+1];
			}
			Seg_Buf[2] = Seg_Buf[5] = 11;
			Seg_Buf[3*(Set_Index/2)] = Seg_Star_Flag?ucRtc_Set[2*(Set_Index/2)]:10;
			Seg_Buf[3*(Set_Index/2)+1] = Seg_Star_Flag?ucRtc_Set[2*(Set_Index/2)+1]:10;
		break;
		case 4://闹钟设置
			for(i=0;i<3;i++)
			{
				Seg_Buf[3*i] = Alarm_Set[Alarm_Dat_Index][2*i];
				Seg_Buf[3*i+1] = Alarm_Set[Alarm_Dat_Index][2*i+1];
			}
			Seg_Buf[2] = Seg_Buf[5] = 11;
			Seg_Buf[3*(Set_Index/2)] = Seg_Star_Flag?Alarm_Set[Alarm_Dat_Index][2*(Set_Index/2)]:10;
			Seg_Buf[3*(Set_Index/2)+1] = Seg_Star_Flag?Alarm_Set[Alarm_Dat_Index][2*(Set_Index/2)+1]:10;
		break;
		case 5://日期设置
			for(i=0;i<3;i++)
			{
				Seg_Buf[3*i] = ucDate_Set[2*i];
				Seg_Buf[3*i+1] = ucDate_Set[2*i+1];
			}
			Seg_Buf[2] = Seg_Buf[5] = 11;		
			Seg_Buf[3*(Set_Index/2)] = Seg_Star_Flag?ucDate_Set[2*(Set_Index/2)]:10;
			Seg_Buf[3*(Set_Index/2)+1] = Seg_Star_Flag?ucDate_Set[2*(Set_Index/2)+1]:10;
		break;
	}
	
}


/* 其他显示函数 */
void Led_Proc()
{
	unsigned char i;//For循环专用变量
	for(i=1;i<4;i++)//常用互斥点亮思路 利用位运算符 条件满足时默认返回1
		ucLed[i] = (i == Alarm_Dat_Index + 1) * (Seg_Disp_Mode == 1 || Seg_Disp_Mode == 4);//处于时钟界面时 点亮相应的Led指示灯
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
	if(++Seg_Slow_Down == 100) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	if(++Timer_500Ms == 500)
	{
		Timer_500Ms = 0;//计时值复位
		Seg_Star_Flag ^= 1;//标志位取反
	}
}

/* Main */
void main()
{
	System_Init();
	Timer0Init();
	Set_Rtc(ucRtc);//设置上电初始化时间
	Set_Date(ucDate);//设置上电初始化日期
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}


//if (input == 1)
//    A = (Key_Down << 4) | (A & 0xF)
//else 
//    A = (A & 0xF0) | Key_Down
