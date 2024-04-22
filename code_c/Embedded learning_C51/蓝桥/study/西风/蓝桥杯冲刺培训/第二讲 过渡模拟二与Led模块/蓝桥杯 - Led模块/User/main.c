/* 头文件声明区 */
#include <Init.h>
#include <Led.h>

/* 变量声明区 */
unsigned char Led_Pos;
unsigned char Led_Buf[8] = {0,0,0,0,0,0,0,0};
unsigned char Led_Buf_Index;
unsigned int Timer_1000Ms;

/* 定时器0初始化函数 */
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
	EA = 1;
}

/* 定时器0中断服务函数 */
void Timer0Server() interrupt 1
{
	unsigned char i = 0;
	/* 动态扫描 原理参考动态数码管 */
	if(++Led_Pos == 8) Led_Pos = 0;
	Led_Disp(Led_Pos,Led_Buf[Led_Pos]);
	
	/* 先将所有Led熄灭 */
	for(i = 0;i < 8;i++)
		Led_Buf[i] = 0;
	
	/* 将当前所需要点亮的Led置高电平 */
	Led_Buf[Led_Buf_Index] = 1;
	
	/* 一秒钟切换一次当前需要点亮的Led */
	if(++Timer_1000Ms == 1000)
	{
		Timer_1000Ms = 0;
		if(++Led_Buf_Index == 8)
			Led_Buf_Index = 0;
	}
}

/* Main */
void main()
{
	System_Init();
	Timer0Init();
	while(1)
	{
		
	}
}

