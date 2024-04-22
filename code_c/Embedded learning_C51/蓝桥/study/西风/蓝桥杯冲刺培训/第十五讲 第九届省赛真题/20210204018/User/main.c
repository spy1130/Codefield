/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include "iic.h"


/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned int Led_Time_Disp[4] = {400,400,400,400};//流转时间间隔数据显示数组
unsigned int Led_Time_Ctrol[4] = {400,400,400,400};//流转时间间隔数据控制数组
unsigned char Led_Time_Index;//流转时间间隔指针 - 运行模式编号
unsigned int Timer_400Ms;//400毫秒计时变量
unsigned char Seg_Index;//数码管索引值 0-熄灭 1-模式编号 2-流转间隔闪
unsigned char EEPROM_Dat[4];
unsigned char Led_Mode;
unsigned int Ms_Tick;
unsigned char Led_Pos;
unsigned char Led_Level;
unsigned char Led_Count;
bit System_Flag;
bit Seg_Star_Flag;//数码管闪烁标志位
bit Seg_Disp_Mode;//数码管显示模式专用变量 0-参数界面 1-亮度等级界面

/* 键盘处理函数 */
void Key_Proc()
{
	unsigned char i;
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上降沿
	Key_Old = Key_Val;//辅助扫描变量
	
	if(Seg_Index == 0)
	{
		if(Key_Old == 4)
			Seg_Disp_Mode = 1;
		else
			Seg_Disp_Mode = 0;
	}
	
	switch(Key_Down)
	{
		case 6:
			if(++Seg_Index == 3) Seg_Index = 0;
			if(Seg_Index == 0)
			{
				Led_Time_Index = 0;
				for(i=0;i<4;i++)
				{
					Led_Time_Ctrol[i] = Led_Time_Disp[i];
					EEPROM_Dat[i] = Led_Time_Ctrol[i] / 100;
				}
				EEPROM_Write(EEPROM_Dat,0,4);
			}
		break;
		case 5:
			if(Seg_Index == 1) 
			{
				if(++Led_Time_Index == 4) 
					Led_Time_Index = 0;
			}
			else if(Seg_Index == 2)
			{
				Led_Time_Disp[Led_Time_Index] += 100;
				if(Led_Time_Disp[Led_Time_Index] > 1200)
					Led_Time_Disp[Led_Time_Index] = 1200;
			}
		break;
		case 4:
			if(Seg_Index == 1) 
			{
				if(--Led_Time_Index == 255) 
					Led_Time_Index = 3;
			}
			else if(Seg_Index == 2)
			{
				Led_Time_Disp[Led_Time_Index] -= 100;
				if(Led_Time_Disp[Led_Time_Index] < 400)
					Led_Time_Disp[Led_Time_Index] = 400;
			}
		break;
		case 7:
			System_Flag ^= 1;
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i;
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序

	Led_Level =	Ad_Read(0x03) / 64;
	if(Seg_Disp_Mode == 0) //处于参数界面
	{
		Seg_Buf[0] = Seg_Buf[2] = 11;//-
		Seg_Buf[1] = Led_Time_Index + 1;
		Seg_Buf[4] = Led_Time_Disp[Led_Time_Index] / 1000 % 10;
		Seg_Buf[5] = Led_Time_Disp[Led_Time_Index] / 100 % 10;
		Seg_Buf[6] = Led_Time_Disp[Led_Time_Index] / 10 % 10;
		Seg_Buf[7] = Led_Time_Disp[Led_Time_Index] % 10;
		if(Seg_Index == 1) //模式
		{
			Seg_Buf[0] = Seg_Buf[2] = Seg_Star_Flag?10:11;//-
			Seg_Buf[1] = Seg_Star_Flag?10:Led_Time_Index + 1;
		}
		else if(Seg_Index == 2) //流转时间
		{
			Seg_Buf[4] = Seg_Star_Flag?10:Led_Time_Disp[Led_Time_Index] / 1000 % 10;
			Seg_Buf[5] = Seg_Star_Flag?10:Led_Time_Disp[Led_Time_Index] / 100 % 10;
			Seg_Buf[6] = Seg_Star_Flag?10:Led_Time_Disp[Led_Time_Index] / 10 % 10;
			Seg_Buf[7] = Seg_Star_Flag?10:Led_Time_Disp[Led_Time_Index] % 10;			
		}
		if(Seg_Buf[4] == 0) Seg_Buf[4] = 10;
	}
	else
	{
		for(i=0;i<6;i++)
			Seg_Buf[i] = 10;
		Seg_Buf[6] = 11;
		Seg_Buf[7] = Led_Level+1;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	unsigned char i;
	if(System_Flag == 1)
	{
		if(Ms_Tick == Led_Time_Ctrol[Led_Mode])
		{
			Ms_Tick = 0;
			switch(Led_Mode)
			{
				case 0:
					if(++Led_Pos == 8)
					{
						Led_Pos = 7;
						Led_Mode = 1;
					}
				break;
				case 1:
					if(--Led_Pos == 255)
					{
						Led_Pos = 7;
						Led_Mode = 2;
					}
				break;
				case 2:
					Led_Pos += 9;
					if(Led_Pos > 34)
					{
						Led_Pos = 34;
						Led_Mode = 3;
					}
				break;
				case 3:
					Led_Pos -= 9;
					if(Led_Pos > 200)
					{
						Led_Pos = 0;
						Led_Mode = 0;
					}
				break;
			}
		}
	}
	
	if(Led_Mode < 2)
	{
		for(i=0;i<8;i++)
			ucLed[i] = (i == Led_Pos);
	}
	else
	{
		for(i=0;i<8;i++)
			ucLed[i] = (i == (Led_Pos / 10) || i == (Led_Pos % 10));
	}
}

/* 定时器0中断初始化函数 */
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;    //定时器中断0打开
	EA = 1;     //总中断打开
}

/* 定时器0中断服务函数 */
void Timer0Server() interrupt 1
{  
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 10) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	if(++Led_Count == 12) Led_Count = 0;
	if(Seg_Index != 0 || Seg_Disp_Mode == 1)
		Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	else
		Seg_Disp(Seg_Pos,10,0);
	if(Led_Count <= ((Led_Level+1) * 3))
		Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	else
		Led_Disp(Seg_Pos,0);
	
	if(++Timer_400Ms == 400)
	{
		Timer_400Ms = 0;
		Seg_Star_Flag ^= 1;
	}
	
	if(System_Flag == 1)
		Ms_Tick++;
}

/* Main */
void main()
{
	unsigned char i;
	System_Init();
	EEPROM_Read(EEPROM_Dat,0,4);
	for(i=0;i<4;i++)
		Led_Time_Disp[i] = Led_Time_Ctrol[i] = EEPROM_Dat[i] * 100;
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}