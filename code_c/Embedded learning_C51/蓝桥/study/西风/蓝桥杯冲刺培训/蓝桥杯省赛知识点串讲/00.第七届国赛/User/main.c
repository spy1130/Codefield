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
unsigned char Seg_Disp_Mode = 8;//数码管显示模式变量 0-时钟显示 1-时钟设置 2-电压测量 3-阈值设置 4-频率测量 5-周期显示 6-事件类型 7-事件时间
unsigned char ucRtc[3] = {23,59,55};//时钟数据储存数组 上电默认时间23:59:55
unsigned char ucRtc_Set[3];//时钟数据设置数组
unsigned char ucRtc_Set_Index;//时钟数据设置数组指针
unsigned int Voltage;//实时电压采集变量
unsigned char Voltage_Val_Disp[2] = {20,10};//电压阈值上下限显示数组
unsigned char Voltage_Val_Ctrol[2] = {20,10};//电压阈值上下限控制数组
bit Voltage_Val_Index;//电压阈值控制数组指针
unsigned int Timer_1000Ms;//1000毫秒计时变量
unsigned int Freq;//实时频率采集变量
unsigned int Cycle;//实时周期采集变量
bit Seg_Star_Flag;//数码管闪烁标志位
unsigned char Event_Type;//事件类型 0-低于下限事件 1-高于上限事件
bit Event_Flag = 1;//事件异常标志位 0-正常 1-异常
unsigned char Event_Time[3];//异常触发事件记录数组

/* 延时函数 */
void Delay200ms()		//@12.000MHz
{
	unsigned char i, j, k;
	i = 10;
	j = 31;
	k = 147;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


/* 键盘处理函数 */
void Key_Proc()
{
	unsigned char i;//循环专用变量
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上降沿
	Key_Old = Key_Val;//辅助扫描变量

	switch(Key_Down)
	{
		case 7://时钟显示按键
			if(Seg_Disp_Mode != 1) //处于非时钟设置界面
				Seg_Disp_Mode = 0;//切换到时钟显示界面
			else
			{
				Set_Rtc(ucRtc_Set);//重新设置时钟数据
				Read_Rtc(ucRtc);//读取一次更新时钟值
				Seg_Disp_Mode = 0;//切换到时钟显示界面
			}
		break;
		case 6://电压显示按键
			if(Seg_Disp_Mode != 3) //处于非阈值设置界面
				Seg_Disp_Mode = 2;//切换到电压显示界面
			else
			{
				if(Voltage_Val_Disp[0] > Voltage_Val_Disp[1])//判断数据有效性
				{
					EEPROM_Write(Voltage_Val_Disp,0,2);//将阈值写入芯片
					Voltage_Val_Ctrol[0] = Voltage_Val_Disp[0];//保存设置阈值
					Voltage_Val_Ctrol[1] = Voltage_Val_Disp[1];
				}
				else
				{
					Voltage_Val_Disp[0] = Voltage_Val_Ctrol[0];//还原设置阈值
					Voltage_Val_Disp[1] = Voltage_Val_Ctrol[1];					
				}
				Seg_Disp_Mode = 2;//切换到电压显示界面
			}
		break;
		case 5://频率显示按键
			if(Seg_Disp_Mode != 5) //处于非周期设置界面
				Seg_Disp_Mode = 4;//切换到频率显示界面
		break;
		case 9://事件时间按键
			if(Seg_Disp_Mode != 7) //处于非事件时间界面
				Seg_Disp_Mode = 6;//切换到事件类型界面
		break;
		case 4://功能切换按键
			switch(Seg_Disp_Mode)
			{
				case 0://处于时钟显示界面
					for(i=0;i<3;i++) //记录设置记录时间
						ucRtc_Set[i] = ucRtc[i];
					ucRtc_Set_Index = 0;//指针值复位
					Seg_Disp_Mode = 1;//切换到时钟设置界面
				break;
				case 1://处于时钟设置界面
					if(++ucRtc_Set_Index == 3) ucRtc_Set_Index = 0;//时分秒循环切换
				break;
				case 2://处于电压显示界面
					Voltage_Val_Index = 0;//指针值复位
					Seg_Disp_Mode = 3;//切换到阈值设置界面
				break;
				case 3://处于阈值设置界面
					Voltage_Val_Index ^= 1;//上下限循环切换
				break;
				case 4://处于频率显示界面
					Seg_Disp_Mode = 5;//切换到周期显示界面
				break;
				case 5://处于周期显示界面
					Seg_Disp_Mode = 4;//切换到频率显示界面
				break;
				case 6://处于事件类型界面
					Seg_Disp_Mode = 7;//切换到事件时间界面
				break;
				case 7://处于事件时间界面
					Seg_Disp_Mode = 6;//切换到事件类型界面
				break;
			}
		break;
		case 11://参数自加按键
			if(Seg_Disp_Mode == 1) //处于时钟设置界面
			{
				if(++ucRtc_Set[ucRtc_Set_Index] == (ucRtc_Set_Index?60:24))
					ucRtc_Set[ucRtc_Set_Index] = ucRtc_Set_Index?59:23;
			}
			else if(Seg_Disp_Mode == 3) //处于阈值设置界面
			{
				Voltage_Val_Disp[Voltage_Val_Index] += 5;
				if(Voltage_Val_Disp[Voltage_Val_Index] > 50)
					Voltage_Val_Disp[Voltage_Val_Index] = 50;
			}
		break;
		case 10://参数自减按键
			if(Seg_Disp_Mode == 1) //处于时钟设置界面
			{
				if(--ucRtc_Set[ucRtc_Set_Index] == 255)
					ucRtc_Set[ucRtc_Set_Index] = 0;
			}
			else if(Seg_Disp_Mode == 3) //处于阈值设置界面
			{
				Voltage_Val_Disp[Voltage_Val_Index] -= 5;
				if(Voltage_Val_Disp[Voltage_Val_Index] > 200)
					Voltage_Val_Disp[Voltage_Val_Index] = 0;
			}
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i;//循环专用变量
	
	/* 
		减速的实际意义是避免程序循环执行过快导致的芯片数据读取出错
		在题目中出现多个芯片时 可设计不同的减速执行周期 达到轮询读取的效果
		避免单片机在某一时刻读取多个芯片所导致的效率较慢或数据错误的问题
	*/
	
	switch(Seg_Slow_Down)
	{
		case 200:
			Seg_Slow_Down += 1;
			Read_Rtc(ucRtc);//实时读取时钟值
			Seg_Star_Flag = ucRtc[2] % 2;//使用秒钟界定闪烁时间
		break;
		case 300:
			Seg_Slow_Down += 1;
			Voltage = (unsigned int)((Ad_Read(0x43) / 51.0) * 1000);//实时读取电压值
		break;
		case 400:
			if(Voltage >= (Voltage_Val_Ctrol[1] * 100) && Voltage < (Voltage_Val_Ctrol[0] * 100)) //正常事件
				Event_Flag = 0;
			else if(Event_Flag == 0)
			{
				Event_Flag = 1;//异常事件
				Read_Rtc(Event_Time);//读取异常事件
				Event_Type = (bit)(Voltage / (Voltage_Val_Ctrol[0] * 100));
				EEPROM_Write(Event_Time,8,3);//保存事件变量
				Delay200ms();
				EEPROM_Write(&Event_Type,3,1);//保存类型变量
			}
			
			switch(Seg_Disp_Mode)
			{
				case 0://时钟显示
					Seg_Buf[2] = Seg_Buf[5] = 11;
					for(i=0;i<3;i++)
					{
						Seg_Buf[3*i] = ucRtc[i] / 10;
						Seg_Buf[3*i+1] = ucRtc[i] % 10;
					}
				break;
				case 1://时钟设置
					Seg_Buf[2] = Seg_Buf[5] = 11;
					for(i=0;i<3;i++)
					{
						Seg_Buf[3*i] = ucRtc_Set[i] / 10;
						Seg_Buf[3*i+1] = ucRtc_Set[i] % 10;
					}
					Seg_Buf[3*ucRtc_Set_Index] = Seg_Star_Flag?ucRtc_Set[ucRtc_Set_Index] / 10:10;
					Seg_Buf[3*ucRtc_Set_Index+1] = Seg_Star_Flag?ucRtc_Set[ucRtc_Set_Index] % 10:10;
				break;
				case 2://电压显示
					Seg_Buf[0] = Seg_Buf[2] = 11;
					Seg_Buf[1] = 1;
					Seg_Buf[3] = 10;
					Seg_Buf[4] = Voltage / 1000 % 10;
					Seg_Buf[5] = Voltage / 100 % 10;
					Seg_Buf[6] = Voltage / 10 % 10;
					Seg_Buf[7] = Voltage % 10;
				break;
				case 3://阈值显示
					Seg_Buf[2] = Seg_Buf[3] = Voltage_Val_Index?0:(Seg_Star_Flag?10:0);
					Seg_Buf[6] = Seg_Buf[7] = Voltage_Val_Index?(Seg_Star_Flag?10:0):0;
					for(i=0;i<2;i++)
					{
						Seg_Buf[4*i] =  Voltage_Val_Disp[i] / 10;
						Seg_Buf[4*i+1] =  Voltage_Val_Disp[i] % 10;
					}
					Seg_Buf[4*Voltage_Val_Index] =  Seg_Star_Flag?10:Voltage_Val_Disp[Voltage_Val_Index] / 10;
					Seg_Buf[4*Voltage_Val_Index+1] = Seg_Star_Flag?10:Voltage_Val_Disp[Voltage_Val_Index] % 10;
				break;
				case 4://频率显示
					Seg_Buf[0] = Seg_Buf[2] = 11;
					Seg_Buf[1] = 2;
					Seg_Buf[3] = Freq / 10000 % 10;
					Seg_Buf[4] = Freq / 1000 % 10;
					Seg_Buf[5] = Freq / 100 % 10;
					Seg_Buf[6] = Freq / 10 % 10;
					Seg_Buf[7] = Freq % 10;
				break;
				case 5://周期显示
					Cycle = 10000 / Freq;
					Seg_Buf[0] = Seg_Buf[2] = 11;
					Seg_Buf[1] = 2;
					Seg_Buf[3] = Cycle / 10000 % 10;
					Seg_Buf[4] = Cycle / 1000 % 10;
					Seg_Buf[5] = Cycle / 100 % 10;
					Seg_Buf[6] = Cycle / 10 % 10;
					Seg_Buf[7] = Cycle % 10;				
				break;
				case 6://事件类型
					for(i=0;i<6;i++)
						Seg_Buf[i] = 10;
					Seg_Buf[6] = 0;
					Seg_Buf[7] = Event_Type;
				break;
				case 7://事件事件
					Seg_Buf[2] = Seg_Buf[5] = 11;
					for(i=0;i<3;i++)
					{
						Seg_Buf[3*i] = Event_Time[i] / 10;
						Seg_Buf[3*i+1] = Event_Time[i] % 10;
					}					
				break;
			}
		break;
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

/* 定时器2中断服务函数 */
void Timer2Server() interrupt 12
{  
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	
	if(++Timer_1000Ms == 1000)
	{
		TR0 = 0;
		Timer_1000Ms = 0;
		Freq = TH0 << 8 | TL0;
		TH0 = TL0 = 0;
		TR0 = 1;
	}
}

/* Main */
void main()
{
	System_Init();
	EEPROM_Read(Voltage_Val_Disp,0,2);//上电读取初始阈值
	Voltage_Val_Ctrol[0] = Voltage_Val_Disp[0];
	Voltage_Val_Ctrol[1] = Voltage_Val_Disp[1];	
	EEPROM_Read(&Event_Type,3,1);//读取类型变量
	EEPROM_Read(Event_Time,8,3);//读取事件变量
	Set_Rtc(ucRtc);//上电设置初始时钟数据
	Timer2Init();
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}