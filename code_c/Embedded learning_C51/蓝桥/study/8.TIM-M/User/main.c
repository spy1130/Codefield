# include "STC15F2K60S2.H"
# include "init.h"
# include "timer.h"
# include "stdio.h"
#include "LED.h"


//-----------------------------------------------
/* 全局变量声明 */


//根据代码需求发生变化
unsigned char count=0;//准备显示出来的数值	


/* main */
void main()
{
	//根据代码需求发生变化
	init_close();//关闭外设
	Timer0_Init();//定时器0初始化，并且使能定时器1中断，1ms进入一次
	EA = 1;//打开总中断

	while(1)
	{
		
	}


}

//-----------------------------------------------
/* Timer1_interrupt routine */
void tm1_isr() interrupt 1
{	
	
	count++;
	if(count % 10 == 0)
	{
		LED(0X01);
	}
	else
	{
		LED(0X00);
	}
	
	if(count == 100)
	{
		LED(0X80);
		count = 0;
	}
	else
	{
		LED(0X00);
	}

	
}

