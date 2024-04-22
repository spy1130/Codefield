/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include <stdio.h>//标准库底层驱动专用头文件
#include "iic.h"//数模转换底层驱动专用头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char Seg_Disp_Mode;//0-频率显示界面，1-电压显示界面
unsigned int fre;//频率变量
float Vol;//电压变量
bit Vol_Output_Mode;//0-电位器输出模式，1-固定输出模式
float Vol_Output;//真实电压输出
bit Led_Enable_Flag = 1;//Led使能标志位 默认开启
bit Seg_Enable_Flag = 1;//数码管
unsigned int  Timer_1000Ms;
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
		case 4:
			Seg_Disp_Mode ^=1;
		break;
		case 5:
			Vol_Output_Mode ^=1;
		break;
		case 6:
			Led_Enable_Flag ^=1;
		break;
		case 7://数码管显示功能控制
			Seg_Enable_Flag ^= 1;
		break;
			
		
	}
}

/* 信息处理函数 */
void Seg_Proc()
{	
	unsigned char i=2;
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序
	Vol = Ad_Read(0x43) / 51.0;//实时获取电位器电压值
	if(Vol_Output_Mode==0)
		Vol_Output=2;
	else
		Vol_Output=Vol;
	if(Seg_Disp_Mode==0){//处于频率显示
	Seg_Point[5]=0;
	Seg_Buf[0]=11;//F
	Seg_Buf[2]=fre/100000%10;
	Seg_Buf[3]=fre/10000%10;
	Seg_Buf[4]=fre/1000%10;
	Seg_Buf[5]=fre/100%10;
	Seg_Buf[6]=fre/10%10;
	Seg_Buf[7]=fre%10;
		
	while(Seg_Buf[i]==0)
	{
		Seg_Buf[i]=10;
		if(++i==7)break;
	}
	}
	else//处于电压显示
	{
		Seg_Buf[0]=12;//U
		Seg_Buf[4]=Seg_Buf[3]=10;
		Seg_Buf[5]=(unsigned char)Vol_Output;
		Seg_Buf[6]=(unsigned int)(Vol_Output * 100)/10%10;
		Seg_Buf[7]=(unsigned int)(Vol_Output * 100)%10;
		Seg_Point[5]=1;
	}
}

/* 其他显示函数 */
void Led_Proc()
{	unsigned char i;
	/*DAC相关*/
	Da_Write(Vol_Output*51);//实时输出电压值
	for(i=0;i<2;i++)
	{
		ucLed[i]=(i=Seg_Disp_Mode);
		ucLed[2] = ((Vol >= 1.5 && Vol< 2.5) || (Vol >= 3.5));
		ucLed[3] = ((fre >= 1000 && fre< 5000) || (fre >= 10000));
		ucLed[4] = Vol_Output_Mode;
	}
}


/* 计数器0中断初始化函数 */

void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x05;			//设置16位不重载计数模式
	TL0 = 0;				//设置定时初始值
	TH0 = 0;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
}
/* 定时器1中断初始化函数 */
void Timer1_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xBF;			//定时器时钟12T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x18;				//设置定时初始值
	TH1 = 0xFC;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1=1;					//定时器中断1打开
	EA=1;					//总中断打开		
}

/* 定时器0中断服务函数 */
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
		fre = TH0 << 8 | TL0;
		TH0 = TL0 = 0;
	}

}



/* Main */
void main()
{
	System_Init();
	Timer0_Init();	
	Timer1_Init();	
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
		
	}
}