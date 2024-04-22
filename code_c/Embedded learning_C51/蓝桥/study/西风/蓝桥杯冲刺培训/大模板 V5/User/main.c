/* 
		大模板 V5 更新说明
		
		1、优化减速问题 共用一个中断减速变量 使用标志位进行判断 避免某一时间点无法进入子程序的问题
		
		2、新增数据读取函数 数码管显示函数只做数据显示 读取部分分离开来 减少减速周期不同对资源的浪费
		
		3、新增滤波数据处理函数 解决重复读取后数据偶尔跳动的问题
		
		------------------------------------
		
		新增变量说明：
		
		Proc_Flag - 子程序执行标志位 默认 1 -> 按键处理函数 2 -> 数码管显示函数 3 -> 串口处理函数 4 -> 数据读取函数（模板以读取超声波为例）
		
		------------------------------------
		
		新增函数库说明：		
		
		Filtering.h - 滤波处理函数专用头文件 Moving_Average_Filter -> 滑动平均滤波器 Median_Filter -> 中值滤波器
		
		Tpis ：数据滤波只支持整形数据 涉及到小数 请先进行滤波后 再对数据进行小数化处理
		
*/

/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include <Uart.h>//串口底层驱动专用头文件
#include <stdio.h>//标准库底层驱动专用头文件
#include <Filtering.h>//数据滤波驱动专用头文件
#include <ultrasound.h>//超声波底层驱动专用头文件
#include "onewire.h"


/* 变量声明区 */
unsigned int Proc_Slow_Down;//子函数减速变量
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
idata unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
idata unsigned char Uart_Recv[10];//串口接收数据储存数组 默认10个字节 若接收数据较长 可更改最大字节数
unsigned char Uart_Recv_Index;//串口接收数组指针
unsigned char Proc_Flag;//子程序执行标志位 
float Wave_Data;//超声波数据储存变量
unsigned char dat;

/* 键盘处理函数 */
void Key_Proc()
{
	if(Proc_Flag != 1) return;
	Proc_Flag = 0;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上降沿
	Key_Old = Key_Val;//辅助扫描变量

}

/* 数据读取函数 */
void Read_Proc()
{
	unsigned int Filter_Dat[2];//滤波前数据接收变量
	switch(Proc_Flag)
	{
		case 4://超声波读取
			Filter_Dat[0] = rd_temperature();//读取超声波数据
			Filter_Dat[1] = Ut_Wave_Data();
			Wave_Data = Moving_Average_Filter(0,Filter_Dat[0]) / 16.0;//滑动平均数滤波处理
			dat = Moving_Average_Filter(1,Filter_Dat[1]);
			Proc_Flag = 0;//清除标志位状态
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	if(Proc_Flag != 2) return;
	Proc_Flag = 0;//数码管减速程序

	Seg_Buf[0] = (int)Wave_Data / 10 % 10;
	Seg_Buf[1] = (int)Wave_Data % 10;
	Seg_Buf[2] = (int)(Wave_Data * 10) % 10;
	
	Seg_Buf[5] = dat / 100 % 10;
	Seg_Buf[6] = dat / 10 % 10;
	Seg_Buf[7] = dat % 10;
}

/* 其他显示函数 */
void Led_Proc()
{
	
}

/* 串口处理函数 */
void Uart_Proc()
{
	if(Proc_Flag != 3) return;
	Proc_Flag = 0;//串口减速程序
	
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
	if(++Proc_Slow_Down == 500) Proc_Slow_Down = 0;//子程序减速专用
	
	if(Proc_Slow_Down % 9 == 0) Proc_Flag = 1;
		
	if(Proc_Slow_Down % 49 == 0) Proc_Flag = 2;
	
	if(Proc_Slow_Down % 29 == 0) Proc_Flag = 4;
	
	
	switch(Proc_Slow_Down)
	{
		//case 50: Proc_Flag = 2; break;
		case 100: Proc_Flag = 3; break;
		//case 200: Proc_Flag = 4; break;
	}
	
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
}

/* 串口1中断服务函数 */
void Uart1Server() interrupt 4
{
	if(RI == 1) //串口接收数据
	{
		Uart_Recv[Uart_Recv_Index] = SBUF;
		Uart_Recv_Index++;
		RI = 0;
	}
}

/* Main */
void main()
{
	System_Init();
	Timer0Init();
	UartInit();
	while (1)
	{
		Key_Proc();
		Read_Proc();
		Seg_Proc();
		Led_Proc();
		Uart_Proc();
	}
}