# include "bsp_init.h"
# include "bsp_seg.h"
# include "timer.h"
# include "bsp_key.h"
# include "stdio.h"

//-----------------------------------------------
/* 函数声明 */
void Key_Proc(void);//按键处理，底层数据变更
void Seg_Proc(void);//显示处理，显示信息生成

//-----------------------------------------------
/* 全局变量声明 */

//显示专用，基本上永远不变
unsigned char seg_buf[8];//放置字符串转换后的段码到数组
unsigned char seg_string[10];//放置字符串
unsigned char	pos = 0;//中断显示专用

//按键专用，基本上永远不变
unsigned char Key_Value;//读取按键的数值存储变量
unsigned char Key_Down, Key_Old;//读取按键的数值存储变量	

//按键和显示函数减速专用，基本永远不变
unsigned int Key_Slow_Down;//按键减速
unsigned int Seg_Slow_Down;//按键减速



//根据代码需求发生变化
unsigned char Seg_Show_Num;//准备显示出来的数值	
	

	
	
//-----------------------------------------------
/* main */
void main()
{
	//根据代码需求发生变化
	Cls_Peripheral();//关闭外设
	Timer1Init();//定时器1初始化，并且使能定时器1中断，1ms进入一次
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
	Seg_Disp(seg_buf, pos);//显示刷新
	if( ++pos == 8 ) 	pos = 0;
	
}



//-----------------------------------------------
/*key_proc */

void Key_Proc(void)//按键处理，底层数据变更
{
	//永远不变
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//减速程序
	
	Key_Value = Key_Read_BTN();//读取按键按下的编号
	Key_Down = Key_Value & (Key_Old ^ Key_Value);// (0000^0101) = 0101     0101 & 0101 =0101  ,如果按键发生了下降沿的变化，输出结果和本次按键数值相同
																					// (0101^0101) = 0000       0101&0000 = 0000  如果按键一直保持同样的状态，输出结果为0
	Key_Old = Key_Value;
	
	
	
	
	
	//根据代码需求发生变化
	if(Key_Down)//如果捕捉到下降沿跳变
	{
		Seg_Show_Num = Key_Down;
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
	sprintf(seg_string,"      %02d",(unsigned int)Seg_Show_Num);//将要显示的数值打印到字符串当中
	
	//永远不变
	Seg_Tran(seg_string, seg_buf);
}



