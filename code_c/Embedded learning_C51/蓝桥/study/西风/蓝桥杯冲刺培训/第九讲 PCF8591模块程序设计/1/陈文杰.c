/*头文件声明区*/
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层专用头文件
#include <Key.h>//按键底层驱动头文件
#include <Seg.h>//数码管底层驱动头文件
#include <Led.h>//Led底层驱动头文件
#include "iic.h"//icc驱动头文件

/*变量声明区*/
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char Key_Val,Key_Old,Key_Down,Key_Up;//按键专用变量
unsigned char Seg_Pos;
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};
unsigned char Seg_Point[8] ={0,0,0,0,0,0,0,0};
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};
unsigned char dat1,dat1_output;//dat1光敏电阻  dat2滑动变阻器
unsigned char Seg_Mode;//模式变量 0—电压显示界面 1—电压输出界面
bit output_Flag;//输出模式标志位 0-2v  1-随机
bit Led_Enable;//led使能标志位
bit Seg_Enable;//数码管使能按键


/*按键处理函数*/
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//按键减速程序
	
	Key_Val = Key_Read();//读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上升沿
	Key_Old = Key_Val;//辅助扫描函数
	
	switch(Key_Down)
	{
		case 4://显示界面切换按键
			Seg_Mode ^=1;
		break;
		case 5://输出模式切换按键
			output_Flag ^= 1;
		break;
		case 6://Led指示灯控制
			Led_Enable ^= 1;
		break;
		case 7://数码管显示功能按键
			Seg_Enable ^= 1;
		break;

	}
}

/*信息显示函数*/
void Seg_Proc()
{
	unsigned char i;//用于循环
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//按键减速程序	
	
	dat1 = Ad_Read(0x43);
	dat1_output = output_Flag ? dat1 : 102;
	
	if(Seg_Enable == 0)//数码管使能
	{
		switch(Seg_Mode)
		{
			case 0://电压显示界面
				Seg_Buf[0] = 11;//显示c
			Seg_Buf[1] = Seg_Buf[2] = Seg_Buf[3] = Seg_Buf[4] = 10;
			Seg_Buf[5] = (unsigned int)(dat1 / 51.0) % 10;
			Seg_Buf[6] = (unsigned int)((dat1 / 51.0) * 100) / 10 % 10;
			Seg_Buf[7] = (unsigned int)((dat1 / 51.0) * 100) % 10;
			Seg_Point[5] = 1;//第五位小数点亮
			break;
			case 1://电压输出界面
				Seg_Buf[0] = 12;//F
			Seg_Buf[1] = Seg_Buf[2] = Seg_Buf[3] = Seg_Buf[4] = 10;
			Seg_Buf[5] = (unsigned int)(dat1_output / 51.0) % 10;
			Seg_Buf[6] = (unsigned int)((dat1_output / 51.0) * 100) / 10 % 10;
			Seg_Buf[7] = (unsigned int)((dat1_output / 51.0) * 100) % 10;
			Seg_Point[5] = 1;//第五位小数点亮
			break;
		}
	}
	else
	{
		Seg_Point[5] = 0;
		for(i=0;i<8;i++)
		Seg_Buf[i] = 10;//熄灭数码管
	}
}


/*其他显示函数*/
void Led_Proc()
{
	unsigned char i;
	if(Led_Enable == 0)//Led使能状态
	{
//		if(Seg_Mode == 0)//处于电压测量功能
//		{
//			ucLed[0] = 1;
//			ucLed[1] = 0;
//		}
//		else//处于电压输出功能Seg_Mode == 1
//		{
//			ucLed[0] = 0;
//			ucLed[1] = 1;			
//		}
		for(i = 0;i<2;i++)
		ucLed[i] = (i == Seg_Mode);
		
		ucLed[3] = output_Flag;//2v-L4熄灭
		if((dat1 / 51.0) < 1.5 )
			ucLed[2] = 0;
		
		else if((dat1 / 51.0) <2.5 && (dat1 / 51.0) > 1.5)
			ucLed[2] = 1;
		
		else if((dat1 / 51.0) > 2.5 && (dat1 / 51.0) < 3.5)
			ucLed[2] = 0;
		
		else if((dat1 / 51.0) > 3.5)
			ucLed[2] = 1;
	}
	else //led不是能
	{
		for(i = 0;i<8;i++)//全部熄灭
		ucLed[i] = 0;
	}
	
}

/*定时器0中断初始化函数*/
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

/*定时器0初始化服务函数*/
void Timer0Server() interrupt 1
{
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
	if(++Seg_Slow_Down == 100) Seg_Slow_Down = 0;	
	if(++Seg_Pos == 8) Seg_Pos = 0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	
}

/*main*/
int main()
{
	System_Init();
	Timer0Init();
	while(1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();		
	}
}

