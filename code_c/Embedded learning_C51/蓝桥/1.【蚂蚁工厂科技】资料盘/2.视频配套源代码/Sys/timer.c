# include "timer.h"


//函数名：定时器1初始化函数
//入口参数：无
//返回值：无
//函数功能：系统频率为 12.000MHz，设置定时时长为 1ms，\
						选择定时器 1，选择定时器模式为 16 位自动重载（ 15 系列），\
						选择定时器时钟为 12T（ FOSC/12）
						
void Timer1Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x18;		//设置定时初值
	TH1 = 0xFC;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	
	ET1 = 1;		//使能定时器1中断
}

//入口参数：无
//返回值：无
//函数功能：系统频率为 12.000MHz，设置定时时长为 10ms，\
						选择定时器 0，选择定时器模式为 16 位自动重载（ 15 系列），\
						选择定时器时钟为 12T（ FOSC/12）
void Timer0Init(void)		//10毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xF0;		//设置定时初值
	TH0 = 0xD8;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	
	ET0 = 1;		//使能定时器0中断	
}



