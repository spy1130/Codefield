# include "STC15F2K60S2.H"
# include "init.h"
# include "timer.h"
# include "Key.h"
#include "LED.h"

//-----------------------------------------------
/* 函数声明 */
void Key_Proc(void);//按键处理，底层数据变更


//-----------------------------------------------
/* 全局变量声明 */


//按键专用，基本上永远不变
unsigned char Key_Value;//读取按键的数值存储变量
unsigned char Key_Down, Key_Old;//读取按键的数值存储变量	

//按键和显示函数减速专用，基本永远不变
unsigned int Key_Slow_Down;//按键减速


//根据代码需求发生变化
unsigned char count = 0;
unsigned char pwm_duty = 0;
unsigned char stat = 0;//状态

/* main */
void main()
{
	//根据代码需求发生变化
	init_close();//关闭外设
	Timer1_Init();//定时器1初始化，并且使能定时器1中断，1ms进入一次
	Timer0_Init();
	EA = 1;//打开总中断

	while(1)
	{
		//永远不变
		Key_Proc();//按键处理，底层数据变更
		
	}


}

//-----------------------------------------------
/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{	
	
	//永远不变
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;

	
}
//-----------------------------------------------

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1 //1/100=0.01s=10ms=1000*10us=100*100us,刚好分成100份，所以用100us
{	                   
	count++;
	if(count == pwm_duty)
	{
		LED(0x00);
	}
	else if(count == 100)
	{
		LED(0x01);
		count = 0;
		
	}		
	
}

//-----------------------------------------------
void Key_Proc(void)//按键处理，底层数据变更
{
	//永远不变
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//减速程序
	//下面3行是消抖
	Key_Value = Key_BIN();//读取按键按下的编号（临时按键值）
	Key_Down = Key_Value & (Key_Old ^ Key_Value);//按下为按键值，其它（未按下，按下稳定期，抬起过程中）为0	
	Key_Old = Key_Value;
	
	//根据代码需求发生变化
	if(Key_Down)//如果捕捉到下降沿跳变
	{	
		if(Key_Down==7)
		{
			switch(stat)
			{
				case 0:
					LED(0x00);	
					TR0 = 0;
					stat = 1;
					
				break;
				
				case 1:
					LED(0x01);
					TR0=1;
					pwm_duty = 10;
					stat = 2;
					
				break;
				
				case 2:
					pwm_duty = 50;
					stat = 3;
					
				break;
				
				case 3:
					pwm_duty = 90;
					stat = 0;
				break;		
			}
		}
	}
}

