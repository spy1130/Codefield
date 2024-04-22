# include "STC15F2K60S2.H"
# include "init.h"
# include "timer.h"
# include "stdio.h"
# include "Key.h"
#include "SEG.h"

//-----------------------------------------------
/* 函数声明 */
void Key_Proc(void);//按键处理，底层数据变更
void Seg_Proc(void);//显示处理，显示信息生成

//-----------------------------------------------
/* 全局变量声明 */

//显示专用，基本上永远不变
unsigned char seg_buf[8];//放置字符串转换后的段码到数组
unsigned char seg_string[18];//放置字符串
unsigned char	pos = 0;//中断显示专用

//按键专用，基本上永远不变
unsigned char Key_Value;//读取按键的数值存储变量
unsigned char Key_Down, Key_Old;//读取按键的数值存储变量	

//按键和显示函数减速专用，基本永远不变
unsigned int Key_Slow_Down;//按键减速
unsigned int Seg_Slow_Down;//按键减速



//根据代码需求发生变化
unsigned char Seg_Show[1];//准备显示出来的数值	


/* main */
void main()
{
	//根据代码需求发生变化
	init_close();//关闭外设
	Timer1_Init();//定时器1初始化，并且使能定时器1中断，1ms进入一次
	EA = 1;//打开总中断

	while(1)
	{
		//永远不变
		Key_Proc();//按键处理，底层数据变更
		Seg_Proc();//显示处理，显示信息生成
	}


}

//-----------------------------------------------
/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{	
	
	//永远不变
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	
	//永远不变
	NumShow(seg_buf, 0);//显示刷新
	if( ++pos == 8 ) 	pos = 0;
	
}

void Key_Proc(void)//按键处理，底层数据变更
{
	//永远不变
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//减速程序
	//下面3行是消抖
	Key_Value = Key_Read();//读取按键按下的编号（临时按键值）
	Key_Down = Key_Value & (Key_Old ^ Key_Value);//按下为按键值，其它（未按下，按下稳定期，抬起过程中）为0	
	Key_Old = Key_Value;
	
	//根据代码需求发生变化
	if(Key_Down)//如果捕捉到下降沿跳变
	{	switch(Key_Down){
		case 7 :Seg_Show[0] = '0';break;
		case 11 :Seg_Show[0] = '1';break;
		case 15 :Seg_Show[0] = '2';break;
		case 19 :Seg_Show[0] = '3';break;
		case 6 :Seg_Show[0] = '4';break;
		case 10 :Seg_Show[0] = '5';break;
		case 14 :Seg_Show[0] = '6';break;
		case 18 :Seg_Show[0] = '7';break;
		case 5 :Seg_Show[0] = '8';break;
		case 9 :Seg_Show[0] = '9';break;
		case 13 :Seg_Show[0] = 'A';break;
		case 17 :Seg_Show[0] = 'b';break;
		case 4 :Seg_Show[0] = 'C';break;
		case 8 :Seg_Show[0] = 'd';break;
		case 12 :Seg_Show[0] = 'E';break;
		case 16 :Seg_Show[0] = 'F';break;
		
		default :Seg_Show[0]=' ';break;
		
				}
	}
	
}

/*seg_proc */
void Seg_Proc(void)//显示处理，显示信息生成
{
	//永远不变
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//减速程序

	//根据代码需求发生变化
	seg_string[0]=Seg_Show[0];//将要显示的数值打印到字符串当中
	//永远不变
	NumTran(seg_string, seg_buf);
}