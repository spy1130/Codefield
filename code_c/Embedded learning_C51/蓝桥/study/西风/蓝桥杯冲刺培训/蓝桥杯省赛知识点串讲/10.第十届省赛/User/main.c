/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include "iic.h"//数模转换底层驱动专用头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned int Timer_1000Ms;//1000毫秒计时变量
unsigned int Freq;//实时频率值
float Voltage;//实时电压值
float Voltage_Output;//实时输出电压
bit Seg_Disp_Mode;//数码管显示模式变量 0-频率显示界面 1-电压显示界面
bit Output_Mode;//DAC输出模式标志位 0-固定2V 1-随AD变化
bit Led_Enable_Flag = 1;//Led使能标志位 默认开启
bit Seg_Enable_Flag = 1;//数码管使能标志位 默认开启

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
		case 4://显示界面切换
			Seg_Disp_Mode ^= 1;
		break;
		case 5://输出模式切换
			Output_Mode ^= 1;
		break;
		case 6://LED 指示灯功能控制
			Led_Enable_Flag ^= 1;
		break;
		case 7://数码管显示功能控制
			Seg_Enable_Flag ^= 1;
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i = 3;//高位熄灭专用变量
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序

	/* 信息读取区域 */
	Voltage = Ad_Read(0x03) / 51.0;//实时获取电压值
	if(Output_Mode == 0) //处于固定输出模式
		Voltage_Output = 2;
	else //处于随AD输出模式
		Voltage_Output = Voltage;
	
	//Voltage_Output = 2*(!Output_Mode) + Voltage*(Output_Mode);
	
	/* 数据显示区域 */
	Seg_Point[5] = Seg_Disp_Mode;//小数点使能
	if(Seg_Disp_Mode == 0) //处于频率显示界面
	{
		Seg_Buf[0] = 11;//标识符F
		Seg_Buf[3] = Freq / 10000 % 10;
		Seg_Buf[4] = Freq / 1000 % 10;
		Seg_Buf[5] = Freq / 100 % 10;
		Seg_Buf[6] = Freq / 10 % 10;
		Seg_Buf[7] = Freq % 10;
		while(Seg_Buf[i] == 0) //数码管高位熄灭
		{
			Seg_Buf[i] = 10;
			if(++i == 7) break; //保证最低位不熄灭 避免程序卡死
		}
	}
	else //处于电压显示界面
	{
		Seg_Buf[0] = 12;//标识符U
		Seg_Buf[3] = Seg_Buf[4] = 10;
		Seg_Buf[5] = (unsigned char)Voltage;
		Seg_Buf[6] = (unsigned int)(Voltage * 100) / 10 % 10;
		Seg_Buf[7] = (unsigned int)(Voltage * 100) % 10;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	unsigned char i;//For循环专用变量
	/* DAC相关 */
	Da_Write(Voltage_Output*51);//实时输出电压值
	
	/* Led相关 */
	for(i=0;i<2;i++) //互斥点亮
		ucLed[i] = (i == Seg_Disp_Mode);
	ucLed[2] = ((Voltage >= 1.5 && Voltage < 2.5) || (Voltage >= 3.5));
	ucLed[3] = ((Freq >= 1000 && Freq < 5000) || (Freq >= 10000));
	ucLed[4] = Output_Mode;
}

/* 定时器0中断初始化函数 */
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x05;		//设置计数模式
	TL0 = 0;		//设置定时初始值
	TH0 = 0;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

/* 定时器1中断初始化函数 */
void Timer1Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x18;		//设置定时初始值
	TH1 = 0xFC;		//设置定时初始值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;		//定时器中断1打开
	EA = 1;			//总中断打开
}



/* 定时器1中断服务函数 */
void Timer1Server() interrupt 3
{  
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	if(Seg_Enable_Flag == 1) //数码管使能
		Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	else
		Seg_Disp(Seg_Pos,10,0); //熄灭所有数码管
	if(Led_Enable_Flag == 1) //Led使能
		Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	else
		Led_Disp(Seg_Pos,0); //熄灭所有Led
	
	if(++Timer_1000Ms == 1000) //实时读取频率值
	{
		Timer_1000Ms = 0;
		Freq = TH0 << 8 | TL0;
		TH0 = TL0 = 0;
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