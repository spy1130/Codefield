/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include "onewire.h"//温度底层驱动专用头文件
#include "iic.h"//DAC底层驱动专用头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char Temperature_Params_Disp[2];//温度参数显示数组
unsigned char Temperature_Params_Ctrol[2] = {30,20};//温度参数控制数组
bit Temperature_Params_Index;//温度参数数组指针
bit Seg_Disp_Mode;//数码管显示模式变量 0-数据界面 1-参数界面
float Temperature;//实时温度变量
bit Error_Flag;//错误设置标志位

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
		case 4://界面切换
			Seg_Disp_Mode ^= 1;//循环切换数据界面和参数界面
			if(Seg_Disp_Mode == 1) //切换到参数界面
			{
				//将控制值赋值给显示数组 便于修改数据
				Temperature_Params_Disp[0] = Temperature_Params_Ctrol[0];
				Temperature_Params_Disp[1] = Temperature_Params_Ctrol[1];
				//上下限指针复位
				Temperature_Params_Index = 0;
			}
			else //切换到数据界面
			{
				if(Temperature_Params_Disp[0] > Temperature_Params_Disp[1]) //设置参数合理
				{
					//将显示值赋值给控制数组 用于保存数据
					Temperature_Params_Ctrol[0] = Temperature_Params_Disp[0];
					Temperature_Params_Ctrol[1] = Temperature_Params_Disp[1];
					Error_Flag = 0;//标志位拉低 表示设置正确
				}
				else
					Error_Flag = 1;//标志位拉高 表示设置错误
			}
		break;
		case 5://参数切换
			if(Seg_Disp_Mode == 1) //处于参数界面
				Temperature_Params_Index ^= 1;//切换参数选中上下限
		break;
		case 6://参数自加
			if(Seg_Disp_Mode == 1) //处于参数界面
			{
				if(++Temperature_Params_Disp[Temperature_Params_Index] == 100)
					Temperature_Params_Disp[Temperature_Params_Index] = 99;//限制上限到99
			}
		break;
		case 7://参数自减
			if(Seg_Disp_Mode == 1) //处于参数界面
			{
				if(--Temperature_Params_Disp[Temperature_Params_Index] == 255)
					Temperature_Params_Disp[Temperature_Params_Index] = 0;//限制下限到0
			}
		break;
	}

}

/* 信息处理函数 */
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序

	/* 信息获取区域 */
	Temperature = rd_temperature();//实时读取温度值
	
	/* 数据显示区域 */
	if(Seg_Disp_Mode == 0) //处于数据界面
	{
		Seg_Buf[0] = 11;//标识符C
		Seg_Buf[3] = 10;
		Seg_Buf[4] = 10;
		Seg_Buf[6] = (unsigned char)Temperature / 10 % 10;
		Seg_Buf[7] = (unsigned char)Temperature % 10;
	}
	else //处于参数界面
	{
		Seg_Buf[0] = 12;//标识符P
		Seg_Buf[3] = Temperature_Params_Disp[0] / 10 % 10;
		Seg_Buf[4] = Temperature_Params_Disp[0] % 10;
		Seg_Buf[6] = Temperature_Params_Disp[1] / 10 % 10;
		Seg_Buf[7] = Temperature_Params_Disp[1] % 10;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	unsigned char i;//For循环专用变量

	/* Led相关 */
	ucLed[0] = (Temperature > Temperature_Params_Ctrol[0]);
	ucLed[1] = (Temperature <= Temperature_Params_Ctrol[0] && Temperature >= Temperature_Params_Ctrol[1]);
	ucLed[2] = (Temperature < Temperature_Params_Ctrol[1]);
	ucLed[3] = Error_Flag;
	
	/* DAC相关 */
	for(i=0;i<3;i++)
	{
		if(ucLed[i] == 1)
		{
			Da_Write(51*(4-i));
			break;
		}
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
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
}

/* 延时函数 */
void Delay750ms()		//@12.000MHz
{
	unsigned char i, j, k;
	i = 35;
	j = 51;
	k = 182;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

/* Main */
void main()
{
	rd_temperature();//上电读取一次温度并且延时750MS避免数据出现85
	Delay750ms();
	System_Init();
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}