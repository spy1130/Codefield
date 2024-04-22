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
unsigned char Seg_Disp_Mode;//数码管显示模式标志位 0-温度显示界面 1-参数设置界面 2-DAC输出界面
unsigned char Temperature_Params;//温度参数变量（用于设置显示）
unsigned char Temperature_Params_Ctrol = 25;//温度参数控制变量（用于实际控制） 初始值：25
float Voltage_Output;//实时输出电压
float Temperature;//实时温度变量
bit Ouput_Mode;//DAC输出模式 0-DAC输出电压与温度相关 1-DAC给出的关系输出电压

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
		case 4://界面切换按键
			if(++Seg_Disp_Mode == 3) 
				Seg_Disp_Mode = 0;//数码管显示模式在0-2之间循环切换
			if(Seg_Disp_Mode == 1) //当前处于温度参数设置界面
				Temperature_Params = Temperature_Params_Ctrol;//将实际控制数据赋值给设置变量 便于数据更改
			if(Seg_Disp_Mode == 2) //当界面从参数设置切换出去后
				Temperature_Params_Ctrol = Temperature_Params;//将设置数据保存到控制变量
		break;
		case 8://参数自减按键
			if(Seg_Disp_Mode == 1) //当前处于温度参数设置界面
			{
				if(--Temperature_Params == 255) //限制温度下限为0
					Temperature_Params = 0;
			}
		break;
		case 9://参数自加按键
			if(Seg_Disp_Mode == 1) //当前处于温度参数设置界面
			{
				if(++Temperature_Params == 100) //限制温度上限为99
					Temperature_Params = 99;
			}
		break;
		case 5://模式切换按键
			Ouput_Mode ^= 1;//切换输出模式
		break;
	}

}

/* 信息处理函数 */
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序

	/* 信息采集区域 */
	Temperature = rd_temperature();//实时采集温度数据
	
	/* 数据显示区域 */
	switch(Seg_Disp_Mode)
	{
		case 0://温度显示界面
			Seg_Buf[0] = 11;//标识符C
			Seg_Buf[4] = (unsigned char)Temperature / 10 % 10;
			Seg_Buf[5] = (unsigned char)Temperature % 10;
			Seg_Buf[6] = (unsigned int)(Temperature * 100) / 10 % 10;
			Seg_Buf[7] = (unsigned int)(Temperature * 100) % 10;
			Seg_Point[5] = 1;//使能小数点
		break;
		case 1://参数设置界面
			Seg_Buf[0] = 12;//标识符P
			Seg_Buf[4] = Seg_Buf[5] = 10;//熄灭第五、第六个数码管
			Seg_Buf[6] = Temperature_Params / 10 % 10;
			Seg_Buf[7] = Temperature_Params % 10;
			Seg_Point[5] = 0;//关闭小数点
		break;
		case 2://DAC输出界面
			Seg_Buf[0] = 13;//标识符A
			Seg_Buf[5] = (unsigned char)Voltage_Output;
			Seg_Buf[6] = (unsigned int)(Voltage_Output * 100) / 10 % 10;
			Seg_Buf[7] = (unsigned int)(Voltage_Output * 100) % 10;
			Seg_Point[5] = 1;//使能小数点
		break;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	unsigned char i;//用于For循环
	/* DAC相关 */
	if(Ouput_Mode == 0)//DAC输出电压与温度相关
	{
		if(Temperature > Temperature_Params_Ctrol) //当实时温度大于温度参数时
			Voltage_Output = 5;//DAC输出5V
		else//当实时温度小于参数设置时
			Voltage_Output = 0;//DAC输出0V
		//Voltage_Output = (bit)((unsigned char)Temperature / Temperature_Params_Ctrol) * 5;
	}		
	else//DAC给出的关系输出电压
	{
		if(Temperature < 20)//当实时温度小于20度时
			Voltage_Output = 1;//DAC输出1V
		else if(Temperature > 40)//当实时温度小于40度时
			Voltage_Output = 4;//DAC输出4V
		else//当实时温度在20-40之间时
			Voltage_Output = 0.15 * (Temperature - 20) + 1;//DAC输出与实时温度为一次函数关系
	}
	Da_Write(Voltage_Output * 51);//实时输出DAC
	
	/* Led相关 */
	ucLed[0] = !Ouput_Mode;//当前处于模式 1 状态，指示灯 L1 点亮，否则熄灭
	for(i=0;i<3;i++)//模式指示灯
	 ucLed[1+i] = (i == Seg_Disp_Mode);
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