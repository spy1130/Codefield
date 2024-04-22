# include "timer.h"
# include "bsp_init.h"
# include "bsp_led.h"


//-----------------------------------------------
/* 变量定义空间 */
bit timer1_interrupt_into_flag = 0;//本变量为布尔变量，进入定时器中断变为1

unsigned int one_ms_trigger_count = 0;//每一毫秒触发一次,0-65535
unsigned char ucLed = 0;//每一秒钟，本数据会自动加1



//-----------------------------------------------
/* main program */
void main()
{
	Cls_Peripheral();//关闭系统外设
	Timer1Init();//定时器1初始化，并开启定时器1中断
	EA = 1;		//使能总中断
	while(1)
	{
		
		
		if(timer1_interrupt_into_flag)
		{
			timer1_interrupt_into_flag = 0;//如果识别到中断标志变化，复位中断标志。
			one_ms_trigger_count++;	//毫秒计数
		}
			
		if((!(one_ms_trigger_count%1000))&&(one_ms_trigger_count>0))//到达1s
		{
			one_ms_trigger_count = 0;
			ucLed ++;//让显示函数变量自动+1
		}
		
		Led_Disp(ucLed);//将要显示的内容显示出来

	}	

}




//-----------------------------------------------

/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{
   timer1_interrupt_into_flag = 1;//进入中断标志位
}


