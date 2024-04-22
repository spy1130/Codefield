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
bit Seg_Disp_Mode;//数码管显示模式变量 0-时间相关 1-温度相关
unsigned char ucRtc[3] = {0x23,0x59,0x50};
unsigned char ucRtc_Set[3] = {0x23,0x59,0x50};
unsigned char Alarm[3] = {0x00,0x00,0x00};
unsigned char Alarm_Set[3] = {0x00,0x00,0x00};
unsigned char ucRtc_Index;
unsigned int Timer_500Ms;
bit Seg_Star_Flag;
bit Beep_Flag;
unsigned char Timer_100Ms;
unsigned char Temperature;
bit Led_Star_Flag;
unsigned char* Data_Flag[3] = {ucRtc,ucRtc_Set,Alarm_Set};
unsigned char Seg_Index;//时间相关显示内容 0-时钟显示 1-时钟设置 2-闹钟设置

/* 键盘处理函数 */
void Key_Proc()
{
	unsigned char i;
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上降沿
	Key_Old = Key_Val;//辅助扫描变量

	if(Beep_Flag == 1)
	{
		if(Key_Down != 0)
			Beep_Flag = 0;
		return;
	}
	
	if(Seg_Index == 0)
	{
		if(Key_Old == 4)
			Seg_Disp_Mode = 1;
		else
			Seg_Disp_Mode = 0;
	}
	switch(Key_Down)
	{
		case 7:
			if(Seg_Disp_Mode == 0) //时钟相关
			{
				if(Seg_Index == 0)
				{
					for(i=0;i<3;i++)
						ucRtc_Set[i] = ucRtc[i];
					Seg_Index = 1;
				}
				else if(Seg_Index == 1)
				{
					if(++ucRtc_Index == 3)
					{
						Set_Rtc(ucRtc_Set);
						ucRtc_Index = 0;
						Seg_Index = 0;
					}
				}
			}
		break;
		case 5:
			if(Seg_Disp_Mode == 0) //时钟相关
			{
//				if(Seg_Index == 1) //设置时钟
//				{
//					ucRtc_Set[ucRtc_Index]++;
//					if(ucRtc_Set[ucRtc_Index] % 16 == 0x0a)
//						ucRtc_Set[ucRtc_Index] += 6;
//					if(ucRtc_Set[ucRtc_Index] == (ucRtc_Index?0x60:0x24))
//						ucRtc_Set[ucRtc_Index] = (ucRtc_Index?0x59:0x23);
//				}
				Data_Flag[Seg_Index][ucRtc_Index]++;
				if(Data_Flag[Seg_Index][ucRtc_Index] % 16 == 0x0a)
					Data_Flag[Seg_Index][ucRtc_Index] += 6;
				if(Data_Flag[Seg_Index][ucRtc_Index] == (ucRtc_Index?0x60:0x24))
					Data_Flag[Seg_Index][ucRtc_Index] = (ucRtc_Index?0x59:0x23);	
			}
		break;
		case 4:
			if(Seg_Disp_Mode == 0) //时钟相关
			{
//				if(Seg_Index == 1) //设置时钟
//				{
//					ucRtc_Set[ucRtc_Index]--;
//					if(ucRtc_Set[ucRtc_Index] % 16 == 0x0f)
//						ucRtc_Set[ucRtc_Index] -= 6;
//					if(ucRtc_Set[ucRtc_Index] == 0xf9)
//						ucRtc_Set[ucRtc_Index] = 0;
//				}
				Data_Flag[Seg_Index][ucRtc_Index]--;
				if(Data_Flag[Seg_Index][ucRtc_Index] % 16 == 0x0f)
					Data_Flag[Seg_Index][ucRtc_Index] -= 6;
				if(Data_Flag[Seg_Index][ucRtc_Index] == 0xf9)
					Data_Flag[Seg_Index][ucRtc_Index] = 0;				
			}			
		break;
		case 6:
			if(Seg_Disp_Mode == 0) //时钟相关
			{
				if(Seg_Index == 0)
					Seg_Index = 2;
				else if(Seg_Index == 2)
				{
					if(++ucRtc_Index == 3)
					{
						for(i=0;i<3;i++)
							Alarm[i] = Alarm_Set[i];
						ucRtc_Index = 0;
						Seg_Index = 0;
					}
				}
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

	
	/* 信息读取区域 */
	Read_Rtc(ucRtc);
	Temperature = rd_temperature();
	
	/* 数据显示区域 */
	if(Seg_Disp_Mode == 0) //时钟
	{
//		switch(Seg_Index)
//		{
//			case 0://时钟显示
//				Seg_Buf[2] = Seg_Buf[5] = 11;
//				for(i=0;i<3;i++)
//				{
//					Seg_Buf[3*i] = ucRtc[i] / 16;
//					Seg_Buf[3*i+1] = ucRtc[i] % 16;
//				}
//			break;
//			case 1://时钟设置
//				Seg_Buf[2] = Seg_Buf[5] = 11;
//				for(i=0;i<3;i++)
//				{
//					Seg_Buf[3*i] = ucRtc_Set[i] / 16;
//					Seg_Buf[3*i+1] = ucRtc_Set[i] % 16;
//				}
//				Seg_Buf[3*ucRtc_Index] = Seg_Star_Flag?10:ucRtc_Set[ucRtc_Index] / 16;
//				Seg_Buf[3*ucRtc_Index+1] = Seg_Star_Flag?10:ucRtc_Set[ucRtc_Index] % 16;
//			break;
				Seg_Buf[2] = Seg_Buf[5] = 11;
				for(i=0;i<3;i++)
				{
					Seg_Buf[3*i] = Data_Flag[Seg_Index][i] / 16;
					Seg_Buf[3*i+1] = Data_Flag[Seg_Index][i] % 16;
				}		
				if(Seg_Index > 0)
				{
					Seg_Buf[3*ucRtc_Index] = Seg_Star_Flag?10:Data_Flag[Seg_Index][ucRtc_Index] / 16;
					Seg_Buf[3*ucRtc_Index+1] = Seg_Star_Flag?10:Data_Flag[Seg_Index][ucRtc_Index] % 16;
				}
		}
	else
	{
		for(i=0;i<5;i++)
			Seg_Buf[i] = 10;
		Seg_Buf[5] = Temperature / 10;
		Seg_Buf[6] = Temperature % 10;
		Seg_Buf[7] = 12;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	if(Alarm[0] == ucRtc[0] && Alarm[1] == ucRtc[1] && Alarm[2] == ucRtc[2])
		Beep_Flag = 1;
	if((ucRtc[2] % 16) == (Alarm[2] % 16 + 5))
		Beep_Flag = 0;
	ucLed[0] = Led_Star_Flag & Beep_Flag;
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
	if(++Seg_Slow_Down == 200) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	
	if(++Timer_500Ms == 500)
	{
		Timer_500Ms = 0;
		Seg_Star_Flag ^= 1;
	}
	
	if(++Timer_100Ms == 100)
	{
		Timer_100Ms = 0;
		Led_Star_Flag ^= 1;
	}
}

/* Main */
void main()
{
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