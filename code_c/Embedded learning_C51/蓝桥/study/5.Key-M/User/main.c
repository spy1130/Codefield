# include "STC15F2K60S2.H"
# include "init.h"
# include "timer.h"
# include "LED.h"
# include "stdio.h"
# include "Key.h"

/* 函数声明 */
void Key_Proc(void);//按键处理，底层数据变更

//按键专用，基本上永远不变
unsigned char Key_Value;//读取按键的数值存储变量
unsigned char Key_Down, Key_Old;//读取按键的数值存储变量	

//按键和显示函数减速专用，基本永远不变
unsigned int Key_Slow_Down;//按键减速
unsigned int Seg_Slow_Down;//按键减速

//unsigned char LED_Show;
unsigned char stat_k = 0;//状态机
void main()
{
	init_close();
	Timer1_Init();//定时器1初始化，并且使能定时器1中断，1ms进入一次
	EA = 1;//打开总中断
	
	while(1){
		Key_Proc();
	}
}
/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{	
	
	//永远不变
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//每到10ms
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	
	
}
void Key_Proc(void)//按键处理，底层数据变更
{
	//永远不变
	if(Key_Slow_Down) return;//每到10ms Seg_Slow_Down = 0
	Key_Slow_Down = 1;//减速程序
	
	Key_Value = Key_BIN();//读取按键按下的编号

//	Key_Down = Key_Value & (Key_Old ^ Key_Value);// (0000^0101) = 0101     0101 & 0101 =0101  ,如果按键发生了下降沿的变化，输出结果和本次按键数值相同																			// (0101^0101) = 0000       0101&0000 = 0000  如果按键一直保持同样的状态，输出结果为0
//	Key_Old = Key_Value;
	
	//根据代码需求发生变化
	switch(Key_Value)
	{	
		case 7:
			if(stat_k==0){
			LED(0x01);
			stat_k=1;}
			else if(stat_k==1){
			LED(0x00);
			stat_k=0;
			}
			while(P30 ==0);
			break;
		case 6:
			if(stat_k == 0)
			{
				LED(0x02);
				stat_k = 2;
			}
			else if(stat_k == 2)
			{
				LED(0x00);
				stat_k = 0;
			}
			while(P31 ==0);
			break;
		case 5:
			if(stat_k == 1)
			{	
				LED(0x04|0x01);
				while(P32 ==0);
				LED(0x01);
			}
			else if(stat_k == 2)
			{	LED(0x10|0x02);
				while(P32 ==0);
				LED(0x02);
			}
			break;
		case 4:
			if(stat_k == 1)
			{	
				LED(0x08|0x01);
				while(P33 ==0);
				LED(0x01);
			}
			else if(stat_k == 2)
			{
				LED(0x20|0x02);
				while(P32 ==0);
				LED(0x02);
			}
			break;
		case 0:break;
			
	}
}