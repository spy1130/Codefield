/* 头文件声明区 */
#include <REGX52.H>//单片机寄存器专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[6] = {10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char Seg_Disp_Mode;//数码管显示模式标志位 0-电压采集 1-数据显示 2-参数设置 3-计数统计
unsigned char Seg_Input[4] = {11,11,11,11};//数码管输入数据储存数组
unsigned char Seg_Input_Index;//数码管输入数据储存数组指针
unsigned int Timer_500Ms;//五百毫秒计时变量
unsigned char Seg_Point[6] = {0,0,0,0,0,0};//数码管小数点显示数组
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char Led_Pos;
unsigned int Count;// 计数值专用变量
unsigned int Sys_Tick;//系统计时器
unsigned char Key_Error_Count;//无效按键统计变量
float Voltage;//实际电压变量
float Voltage_Parameter = 3.0;//电压设置参数
float Voltage_Parameter_Ctrol = 3.0;//实际电压设置参数  默认值3.0V
bit Seg_Flag;//数码管闪烁标志位
bit Voltage_Flag ;//电压参考标志位

/* 键盘处理函数 */
void Key_Proc()
{
	unsigned char i;//用于For循环
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Old = Key_Val;//辅助扫描变量
	
	if(Key_Down >= 1 && Key_Down <= 10)//键盘使能条件
	{
		if(Seg_Disp_Mode == 0 && Seg_Input_Index < 4)
		{
			Seg_Input[Seg_Input_Index] = Key_Down - 1;
			Seg_Input_Index++;
			Key_Error_Count = 0;
		}
		else
			Key_Error_Count++;
	}
	
	switch(Key_Down)
	{
		case 11:
			Key_Error_Count = 0;
			if(Seg_Disp_Mode == 0)//处于电压采集界面
			{
				if(Seg_Input_Index >= 4)//判断数据有效性
				{
					Voltage = (Seg_Input[0] * 1000 + Seg_Input[1] * 100 + Seg_Input[2] * 10 + Seg_Input[3] + 5) / 1000.0;
					if(Voltage >= 0.01)
						Seg_Disp_Mode = 1;//跳转到数据显示界面
					else
					{
						Seg_Input_Index = 0;//输入指针复位
						for(i = 0;i < 4;i++)//输入数组复位
							Seg_Input[i] = 11;
					}
				}
				else
				{
					Seg_Input_Index = 0;//输入指针复位
					for(i = 0;i < 4;i++)//输入数组复位
						Seg_Input[i] = 11;
				}
			}
			else//处于非电压采集界面
			{
				Seg_Disp_Mode = 0;//跳转到电压采集界面
				Seg_Input_Index = 0;//输入指针复位
				for(i = 0;i < 4;i++)//输入数组复位
					Seg_Input[i] = 11;
			}
		break;
		case 12:
			if(Seg_Disp_Mode != 0)//处于非电压采集界面
			{
				Key_Error_Count = 0;
				if(Seg_Disp_Mode == 2)
					Voltage_Parameter = Voltage_Parameter_Ctrol;//保存当前设置参数
				if(++Seg_Disp_Mode == 4)
					Seg_Disp_Mode = 1;
			}
			else
				Key_Error_Count++;
		break;
		case 15:
			if(Seg_Disp_Mode == 2)//处于参数设置界面
			{
				Key_Error_Count = 0;
				Voltage_Parameter += 0.5;
				if(Voltage_Parameter > 6)
					Voltage_Parameter = 1;
			}
			else
				Key_Error_Count++;
		break;
		case 16:
			if(Seg_Disp_Mode == 2)//处于参数设置界面
			{
				Key_Error_Count = 0;
				Voltage_Parameter -=0.5;
				if(Voltage_Parameter < 1)
					Voltage_Parameter = 6;
			}
			else
				Key_Error_Count++;
		break;
		case 14:
			if(Seg_Disp_Mode == 0)
			{
				Key_Error_Count = 0;
				Seg_Input_Index = 0;//输入指针复位
				for(i = 0;i < 4;i++)//输入数组复位
					Seg_Input[i] = 11;
			}
			else if(Seg_Disp_Mode == 3)
			{
				Key_Error_Count = 0;
				Count = 0;
			}
			else
				Key_Error_Count++;
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i;//用于For循环
	unsigned char j = 1;//用于While循环
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序
	
	if(Voltage > Voltage_Parameter_Ctrol)//当实际电压大于参考电压时
		Voltage_Flag = 1;//拉高标志位
	else if(Voltage_Flag == 1)
	{
		Voltage_Flag = 0;//标志位复位
		Count++;//计数值+1
	}
	switch(Seg_Disp_Mode)
	{
		case 0://电压采集界面
			Seg_Point[3+(int)Voltage/10] = 0;
			Seg_Buf[0] = Seg_Buf[1] = 10;
			for(i = 0;i< 4;i++)
				Seg_Buf[2+i] = Seg_Input[i];
			if(Seg_Buf[5] == 11)//只有当最后一位为-时 才实现数码管闪烁功能
				Seg_Buf[2+Seg_Input_Index] = Seg_Flag?Seg_Input[Seg_Input_Index]:10;
		break;
		case 1://数据显示界面
			Seg_Point[3+(int)Voltage/10] = 1;
			Seg_Buf[0] = 12;
			Seg_Buf[1] = Seg_Buf[2] = 10;
			Seg_Buf[3] = (int)Voltage/10?1:(unsigned char)Voltage % 10;
			Seg_Buf[4] = (unsigned int)(Voltage * 100) / 10 % 10;
			Seg_Buf[5] = (unsigned int)(Voltage * 100) % 10;
		break;
		case 2://参数设置界面
			Seg_Point[3+(int)Voltage/10] = 1;
			Seg_Buf[0] = 13;
			Seg_Buf[1] = Seg_Buf[2] = 10;
			Seg_Buf[3] = (unsigned char)Voltage_Parameter % 10;
			Seg_Buf[4] = (unsigned int)(Voltage_Parameter * 100) / 10 % 10;
			Seg_Buf[5] = (unsigned int)(Voltage_Parameter * 100) % 10;			
		break;
		case 3://计数统计界面
			Seg_Point[3+(int)Voltage/10] = 0;
			Seg_Buf[0] = 14;
			Seg_Buf[1] = Count / 10000 % 10;
		  Seg_Buf[2] = Count / 1000 % 10;
			Seg_Buf[3] = Count / 100 % 10;
			Seg_Buf[4] = Count / 10 % 10;
			Seg_Buf[5] = Count % 10;
			while(Seg_Buf[j] == 0)
			{
				Seg_Buf[j] = 10;
				if(++j == 5) break;
			}
		break;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	if(Voltage < Voltage_Parameter_Ctrol)
	{
		if(Sys_Tick >= 5000)
			ucLed[0] = 1;
	}
	else
	{
		Sys_Tick = ucLed[0] = 0;
	}
	ucLed[1] = Count % 2;
	ucLed[2] = Key_Error_Count / 3;
}

/* 定时器0中断初始化函数 */
void Timer0Init(void)		//1毫秒@12.000MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;    //定时器0中断打开
	EA = 1;     //总中断打开
}

/* 定时器0中断服务函数 */
void Timer0Server() interrupt 1
{
 	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值   
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 6) Seg_Pos = 0;//数码管显示专用
	if(++Led_Pos == 8) Led_Pos = 0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Led_Pos,ucLed[Led_Pos]);
	if(Voltage < Voltage_Parameter_Ctrol)
		Sys_Tick++;
	if(++Timer_500Ms == 500)
	{
		Timer_500Ms = 0;
		Seg_Flag ^= 1;
	}
}

/* Main */
void main()
{
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}