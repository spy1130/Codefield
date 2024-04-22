# include "bsp_init.h"
# include "bsp_seg.h"
# include "timer.h"
# include "bsp_key.h"
# include "stdio.h"
#include "bsp_ds1302.h"
# include "bsp_led.h"
#include "bsp_onewire.h"

//-----------------------------------------------
/* 函数声明 */
//三个主体循环，基本上永远不变
void Key_Proc(void);//按键处理，底层数据变更
void Seg_Proc(void);//显示处理，显示信息生成
void Led_Proc(void);//LED处理，LED状态信息表示


//-----------------------------------------------
/* 全局变量声明 */

//数码管显示专用，基本上永远不变
unsigned char seg_buf[8];//放置字符串转换后的段码到数组
unsigned char seg_string[10];//放置字符串
unsigned char	pos = 0;//中断显示专用

//LED显示专用，基本上永远不变
unsigned char ucLed;//记录LED的状态信息

//按键专用，基本上永远不变
unsigned char Key_Value;//读取按键的数值存储变量
unsigned char Key_Down, Key_Old;//读取按键的数值存储变量	

//按键和显示函数减速专用，基本永远不变
unsigned int Key_Slow_Down;//按键减速
unsigned int Seg_Slow_Down;//按键减速

//ds1302专用，当使用DS1302时，基本永远不变
unsigned char ucRtc[3] = {23,59,55};//数组初始值，里边放的是时分秒的数值


//根据代码需求发生变化
unsigned int ms_count;//记录毫秒数据
unsigned char s_count;//记录秒数据
	
unsigned char Running_State;//记录运行状态

	
//-----------------------------------------------
/* main */
void main()
{
	//根据代码需求发生变化
	Cls_Peripheral();//关闭外设
	Timer1Init();//定时器1初始化，并且使能定时器1中断，1ms进入一次
	EA = 1;//打开总中断

	Set_Rtc(ucRtc);//设置RTC时间，23-59-55

	
	while(1)
	{
		//永远不变
		Key_Proc();//按键处理，底层数据变更
		Seg_Proc();//显示处理，显示信息生成
    Led_Proc();//LED处理，LED状态信息表示		
	}


}



//-----------------------------------------------
/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{	
	
	//永远不变
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	
	if(++ms_count == 1000) //记录运行时间
	{
		s_count++;
		ms_count = 0;
	}
	

	//永远不变
	Seg_Disp(seg_buf, pos);//数码管显示刷新
	if( ++pos == 8 ) 	pos = 0;
	
	Led_Disp(ucLed);//LED显示
	
	
}



//-----------------------------------------------
/*key_proc */

void Key_Proc(void)//按键处理，底层数据变更
{
	//永远不变
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//减速程序
	
	Key_Value = Key_Read();//读取按键按下的编号
	Key_Down = Key_Value & (Key_Old ^ Key_Value);// (0000^0101) = 0101     0101 & 0101 =0101  ,如果按键发生了下降沿的变化，输出结果和本次按键数值相同
																					// (0101^0101) = 0000       0101&0000 = 0000  如果按键一直保持同样的状态，输出结果为0
	Key_Old = Key_Value;
	
	
	
	
	
	//根据代码需求发生变化
	if(Key_Down)//如果捕捉到下降沿跳变
	{
			if(++Running_State == 3)  Running_State = 0;//保证Running_State在0-2之间翻滚
	}
	
}



//----------------------------------------------
/*seg_proc */
void Seg_Proc(void)//显示处理，显示信息生成
{
	//永远不变
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//减速程序

	
	

	
	//根据代码需求发生变化
	switch(Running_State)
	{
		case 0:
				sprintf(seg_string,"----%04.2f",rd_temperature()/16.0);//读取18b20的温度数值,将要显示的数值打印到字符串当中
				break;		
		
		case 1:
				Read_Rtc(ucRtc);//读取1302内部时分秒的数据，放到预定义的数组空间中	
				sprintf(seg_string,"%02d-%02d-%02d",(unsigned int)ucRtc[0],(unsigned int)ucRtc[1],(unsigned int)ucRtc[2]);//将要显示的数值打印到字符串当中
				break;
	
		case 2:
				sprintf(seg_string,"-----%03d",(unsigned int)s_count);//将要显示的数值打印到字符串当中	
				break;		
	}	
	

	
	//永远不变
	Seg_Tran(seg_string, seg_buf);
}

//----------------------------------------------
/*led_proc */
void Led_Proc(void)//显示处理，显示信息生成
{
	
	//根据代码需求发生变化
	switch(Running_State)
	{
		case 0:
				ucLed = 0x03;//让L1 L2两个亮
				break;
	
		case 1:
				ucLed = 0x0C;//让L3 L4亮
				break;		
		
		case 2:
				ucLed = 0x30;//让L5 L6亮
				break;				
		
	}
	
	
}


