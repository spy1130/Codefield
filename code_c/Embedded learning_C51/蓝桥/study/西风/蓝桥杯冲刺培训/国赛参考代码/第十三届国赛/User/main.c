/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include <stdio.h>//标准库底层驱动专用头文件
#include "iic.h"//单总线底层驱动专用头文件
#include <ultrasound.h>//超声波底层驱动专用头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char Seg_Disp_Mode;//数码管显示模式变量 0-频率界面 1-湿度界面 2-测距界面 3-参数界面
unsigned int Freq;//实时频率值
unsigned int Timer_1000Ms;//一千毫秒计时变量
bit Hz_Unit_Flag;//变量单位标志位 0-HZ 1-KHZ
bit Cm_Unit_Flag;//变量单位标志位 0-CM 1-M
unsigned char Humidity_Val;//实时湿度变量
unsigned char Distance_Val;//实时距离变量
unsigned char Param_Disp[3] = {90,40,6};//参数数据显示数组
unsigned char Param_Ctrl[3] = {90,40,6};//参数数据控制数组
unsigned char Param_Index;//参数数据指针
code unsigned char Change_Val[3] = {5,10,1};//参数修改步进值
code unsigned char Param_Max[3] = {120,60,12};//参数修改上限值
code unsigned char Param_Min[3] = {10,10,1};//参数修改下限值
float Output_Val;//输出电压变量
bit Relay_Flag;//继电器使能标志位
unsigned char Relay_Count;//继电器使能计数变量
unsigned char PWM_Count;//脉宽频率计数值 1KHZ = 1MS
unsigned char PWM_Level;//占空比数据储存变量
unsigned char Timer_100Ms;//一百毫秒计时变量
bit Led_Star_Flag;//Led闪烁标志位
unsigned int Timer_Key;//按键计时变量
bit Key_Flag;//按键长短按标志位

/* 键盘处理函数 */
void Key_Proc()
{
	unsigned char i;//循环专用变量
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上降沿
	Key_Old = Key_Val;//辅助扫描变量

	
	if(Seg_Disp_Mode == 1) //处于湿度界面
	{
		if(Key_Down == 7) //产生下降沿
			Key_Flag = 1;//使能标志位
		if(Key_Up == 7) //产生上升沿
		{
			Key_Flag = 0;//拉低标志位
			if(Timer_Key > 1000) //执行长按操作
			{
				Relay_Count = 0;//计数值清零
				EEPROM_Write(&Relay_Count,0,1);//储存计数值
			}
			Timer_Key = 0;//复位计时变量
		}
	}
	
	switch(Key_Down)
	{
		case 4://界面按键
			if(++Seg_Disp_Mode == 4) 
			{
				Seg_Disp_Mode = 0;
				Param_Index = 0;//复位数据指针
				for(i=0;i<3;i++) //保存参数变量
					Param_Ctrl[i] = Param_Disp[i];
			}
		break;
		case 5://参数按键
			if(Seg_Disp_Mode == 3) //处于参数设置界面
			{
				if(++Param_Index == 3)
					Param_Index = 0;
			}
		break;
		case 6://自加按键
			if(Seg_Disp_Mode == 3) //处于参数设置界面
			{
				Param_Disp[Param_Index] += Change_Val[Param_Index];
				if(Param_Disp[Param_Index] > Param_Max[Param_Index]) //限制参数修改上限
					Param_Disp[Param_Index] = Param_Min[Param_Index];
			}
			else if(Seg_Disp_Mode == 2) //处于距离显示界面
				Hz_Unit_Flag ^= 1;
		break;
		case 7://自减按键
		{
			if(Seg_Disp_Mode == 3) //处于参数设置界面
			{
				Param_Disp[Param_Index] -= Change_Val[Param_Index];
				if(Param_Disp[Param_Index] < Param_Min[Param_Index]) //限制参数修改下限
					Param_Disp[Param_Index] = Param_Max[Param_Index];
			}
			else if(Seg_Disp_Mode == 0) //处于频率显示界面
				Cm_Unit_Flag ^= 1;				
		}
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char Pos;//循环专用变量
//	if(Seg_Slow_Down) return;
//	Seg_Slow_Down = 1;//数码管减速程序

	/* 数据读取区域 */
	switch(Seg_Slow_Down) //轮询读取芯片数据
	{
		case 100:
			Seg_Slow_Down += 1;//执行一次限制语句
			Humidity_Val = Ad_Read(0x43) / 51.0 * 20;//实时采集湿度
			if(Humidity_Val > 99) Humidity_Val = 99;//限制湿度上限
			if(Humidity_Val < Param_Ctrl[1]) //湿度小于参数
				Output_Val = 1;
			else if(Humidity_Val > 80) //湿度大于80%
				Output_Val = 5;
			else
				Output_Val = (4.0 / (80 - Param_Ctrl[1])) * (Humidity_Val - Param_Ctrl[1]) + 1;
		break;
		case 200:
			Seg_Slow_Down += 1;//执行一次限制语句
			Distance_Val = Ut_Wave_Data();//实时读取距离值
		break;
	}
	
	/* 信息显示区域 */
	if(!(Seg_Slow_Down % 10)) //避免数码管闪烁
	{
		Seg_Slow_Down += 1;//执行一次限制语句
		Seg_Buf[0] = 11 + Seg_Disp_Mode;//模式标识符
		switch(Seg_Disp_Mode)
		{
			case 0://频率界面
				Seg_Buf[3] = (Freq / (1000 * (int)Hz_Unit_Flag)) / 10000 % 10;
				Seg_Buf[4] = (Freq / (1000 * (int)Hz_Unit_Flag)) / 1000 % 10;
				Seg_Buf[5] = (Freq / (1000 * (int)Hz_Unit_Flag)) / 100 % 10;
				Seg_Buf[6] = (Freq / (1000 * (int)Hz_Unit_Flag)) / 10 % 10;
				Seg_Buf[7] = (Freq / (1000 * (int)Hz_Unit_Flag)) % 10;
				Seg_Point[6] = Hz_Unit_Flag;
				Pos = 3;//设置数码管扫描起始位
				while(Seg_Buf[Pos] == 0) //高位未启用数码管熄灭
				{
					Seg_Buf[Pos] = 10;
					if(++Pos == (7 - (int)Hz_Unit_Flag)) break;
				}
			break;
			case 1://湿度界面
				Seg_Buf[3] = Seg_Buf[4] = Seg_Buf[5] = 10;
				Seg_Buf[6] = Humidity_Val / 10;
				Seg_Buf[7] = Humidity_Val % 10;
				Seg_Point[6] = 0;
			break;
			case 2://距离界面
				Seg_Buf[5] = Distance_Val / 100 % 10;
				Seg_Buf[6] = Distance_Val / 10 % 10;
				Seg_Buf[7] = Distance_Val % 10;
				Seg_Point[5] = Cm_Unit_Flag;
				if(Cm_Unit_Flag == 0)
				{
					Pos = 5;//设置数码管扫描起始位
					while(Seg_Buf[Pos] == 0) //高位未启用数码管熄灭
					{
						Seg_Buf[Pos] = 10;
						if(++Pos == 7) break;
					}
				}
			break;
			case 3://参数界面
				Seg_Buf[1] = Param_Index + 1;
				Seg_Buf[5] = Param_Disp[Param_Index] / 100 % 10?Param_Disp[Param_Index] / 100 % 10:10;
				Seg_Buf[6] = Param_Disp[Param_Index] / 10 % 10;
				Seg_Buf[7] = Param_Disp[Param_Index] % 10;
				Seg_Point[6] = (Param_Index != 1);
			break;
		}
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	unsigned char i;//循环专用变量
	/* Led显示功能 */
	for(i=0;i<3;i++) //互斥点亮
		ucLed[i] = Seg_Disp_Mode < 3?(i == Seg_Disp_Mode):(i == Param_Index) & Led_Star_Flag;
	ucLed[3] = Freq > (Param_Ctrl[0] * 100);//频率比较指示灯
	ucLed[4] = Humidity_Val > Param_Ctrl[1];//湿度比较指示灯
	ucLed[5] = Distance_Val > (Param_Ctrl[2] * 10);//测距比较指示灯
	
	/* DAC输出功能 */
	Da_Write(Output_Val * 51);
	
	/* 继电器功能 */
	if(ucLed[5] != 1) //测距结果小于参数值
	{
		Relay_Flag = 1;//拉高标志位
		Relay(0);//关闭继电器
	}
	else if(Relay_Flag == 1)
	{
		Relay_Flag = 0;//拉低标志位
		Relay(1);//打开继电器
		Relay_Count++;//更新计数值
		EEPROM_Write(&Relay_Count,0,1);//储存计数值
	}
	
	/* 脉冲输出功能 */
	PWM_Level = ucLed[3]?8:2;
	
}


/* 定时器0中断初始化函数 */
void Timer0Init(void)		//@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x05;		//设置定时器模式
	TL0 = 0;		//设置定时初始值
	TH0 = 0;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

/* 定时器1中断初始化函数 */
void Timer1Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x18;		//设置定时初始值
	TH1 = 0xFC;		//设置定时初始值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;    //定时器中断1打开
	EA = 1;     //总中断打开	
}

/* 定时器2中断初始化函数 */
void Timer2Init(void)		//100微秒@12.000MHz
{
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0x9C;		//设置定时初始值
	T2H = 0xFF;		//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 |= (1<<2);   //定时器中断2打开
}

/* 定时器0中断服务函数 */
void Timer0Server() interrupt 3
{  
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	
	if(++Timer_1000Ms == 1000)
	{
		TR0 = 0;
		Timer_1000Ms = 0;
		Freq = TH0 << 8 | TL0;
		TH0 = TL0 = 0;
		TR0 = 1;
	}
	
	if(++Timer_100Ms == 100)
	{
		Timer_100Ms = 0;
		Led_Star_Flag ^= 1;	
	}
	
	if(Key_Flag == 1)
	{
		if(++Timer_Key > 1200) //防止一直长按计时变量溢出
			Timer_Key = 1200;
	}
}

/* 定时器2中断服务函数 */
void Timer2Server() interrupt 12
{
	if(++PWM_Count == 10) PWM_Count = 0;//脉冲周期为1MS
	Pulse(PWM_Count < PWM_Level);//PWM输出
}

/* Main */
void main()
{
	System_Init();
	EEPROM_Read(&Relay_Count,0,1);//读取计数值
	Timer0Init();
	Timer2Init();
	Timer1Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}