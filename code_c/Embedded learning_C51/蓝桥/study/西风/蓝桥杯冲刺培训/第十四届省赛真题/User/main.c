/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include "ds1302.h"//时钟底层驱动专用头文件								
#include "onewire.h"//温度底层驱动专用头文件
#include "iic.h"//单总线底层驱动专用头文件


/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
idata unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char ucRtc[3] = {13,03,05};//时钟数据存放数组 上电默认时间 13:03:05
unsigned char Seg_Disp_Mode;//数码管显示模式变量 0-时间界面 1-回显界面 2-参数界面 3-温度界面
unsigned char Val_Max[2];//最大值储存数组 0-温度 1-湿度
float Val_Average[2];//平均数储存数组 0-温度 1-湿度
unsigned int Average_Temperature_Num;//平均值计数变量(温度)
unsigned int Average_Humidity_Num;//平均值计数变量(湿度)
unsigned char Val_Index;//回显数据指针 0-温度 1-湿度 2-时间
unsigned char Temperature_Val = 30;//温度参数显示变量 上电默认 30
unsigned char Temperature_Ctrl = 30;//温度参数控制变量 上电默认 30
unsigned char Message[2];//信息数据存放数组 0-温度 1-湿度
unsigned char Message_Old[2];//上次信息数据存放数组 0-温度 1-湿度
unsigned int Freq;//实时频率参数
unsigned int Timer_1000Ms;//一千毫秒计时变量
unsigned char Vollage_Val;//实时电压采集变量
unsigned char Time_Val[3];//采集时间记录变量
unsigned char Temperature;//实时温度采集变量
unsigned char Seg_Disp_Mode_Old;//上一界面记录变量 采集后返回原状态
unsigned char Message_Num;//采集次数变量
unsigned int Timer_2000Ms;//两千毫秒计时变量
unsigned int Timer_100Ms;//一百毫秒计时变量
bit Led_Star_Flag;//Led闪烁标志位
bit Message_Flag = 1;//采集允许标志位 0-不允许采集 1-允许采集
bit Vollage_Flag;//电压变量标志位
bit Key_Flag;//长短按标志位

/* 键盘处理函数 */
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上降沿
	Key_Old = Key_Val;//辅助扫描变量

	if(Key_Down == 9) //产生下降沿
		Key_Flag = 1; //开始计时
	if(Key_Up == 9) //产生上升沿
	{
		if(Timer_2000Ms < 2000) //短按操作
		{
			if(Seg_Disp_Mode == 2) //处于参数界面
			{
				if(--Temperature_Val == 255) //限制下限值
					Temperature_Val = 0;
			}
		}
		else //长按操作
			Message_Num = 0;//复位计数值
		Timer_2000Ms = Key_Flag = 0;//复位标志位
	}
	
	switch(Key_Down * Message_Flag)//采集等待过程中 按键失效
	{
		case 4://界面按键
			if(++Seg_Disp_Mode == 3) //循环切换界面
				Seg_Disp_Mode = 0;
			if(Seg_Disp_Mode == 1) //切换到回显界面时 指针初始化
				Val_Index = 0;
			if(Seg_Disp_Mode == 0) //切换到时间显示界面 保存设置值
				Temperature_Ctrl = Temperature_Val;
		break;
			
		case 5://切换按键
			if(Seg_Disp_Mode == 1) //处于回显界面
			{
				if(++Val_Index == 3) //循环切换回显参数
					Val_Index = 0;
			}
		break;
			
		case 8://自加按键
			if(Seg_Disp_Mode == 2) //处于参数界面
			{
				if(++Temperature_Val == 100) //限制上限值
					Temperature_Val = 99;
			}
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i;//For循环专用变量
	/* 数据读取区域 */
	switch(Seg_Slow_Down) //轮询读取芯片数据
	{
		case 200:
			Seg_Slow_Down += 1;//避免一毫秒内执行多次
			Read_Rtc(ucRtc);//实时读取时间
			if(Message_Flag == 0) //不允许采集
			{
				if((ucRtc[2] % 10) == ((Time_Val[2] + 3) % 10)) //超过三秒
				{
					Seg_Disp_Mode = Seg_Disp_Mode_Old;//返回原状态
					Message_Flag = 1;//允许采集
				}
			}
		break;
		case 400:
			Seg_Slow_Down += 1;//避免一毫秒内执行多次
			Temperature = rd_temperature();//实时读取温度
		break;
		case 300:
			Seg_Slow_Down += 1;//避免一毫秒内执行多次
			Vollage_Val = Ad_Read(0x01);//实时读取电压值
			if(Vollage_Val > 100) //光照强度为亮等级
				Vollage_Flag = 1;//拉高标志位
			else if(Vollage_Flag == 1 && Message_Flag == 1) //光照强度为暗等级
			{
				Vollage_Flag = 0;//拉低标志位
				Message_Flag = 0;//拉低采集标志位 避免三秒内重复采集
				
				Read_Rtc(Time_Val);//记录采集时间
				
				Seg_Disp_Mode_Old = Seg_Disp_Mode;//记录原状态界面
				Seg_Disp_Mode = 3;//进入温度界面
				
				Message_Num++;//采集次数自加
				
				Message_Old[0] = Message[0];//保存上次数据
				Message[0] = Temperature;//记录有效温度
				Val_Average[0] = ((Val_Average[0] * Average_Temperature_Num) + Message[0]) / (float)(Average_Temperature_Num + 1);//更新温度平均值
				Average_Temperature_Num++;
				
				if(Freq >= 200 && Freq <= 2000) //有效范围
				{
					Message_Old[1] = Message[1];//保存上次数据
					Message[1] = (unsigned char)(10 + (Freq - 200) * (80.0 / 1800.0)); //记录有效湿度
					Val_Average[1] = ((Val_Average[1] * Average_Humidity_Num) + Message[1]) / (float)(Average_Humidity_Num + 1);//更新湿度平均值
					Average_Humidity_Num++;
				}
				else //无效范围
					Message[1] = 0; //湿度为0
				
				for(i=0;i<2;i++) //更新最大值
				{
					if(Message[i] > Val_Max[i])
						Val_Max[i] = Message[i];
				}
			}
		break;
	}
	
	/* 信息显示区域 */
	if(!(Seg_Slow_Down % 30))//数码管显示数据30毫秒刷新一次
	{
		Seg_Slow_Down += 1;//避免一毫秒内执行多次
		switch(Seg_Disp_Mode)
		{
			case 0://时间界面
				Seg_Point[6] = 0;
				Seg_Buf[2] = Seg_Buf[5] = 11;//标识符 -
				for(i=0;i<3;i++)
				{
					Seg_Buf[3*i] = ucRtc[i] / 10;
					Seg_Buf[3*i+1] = ucRtc[i] % 10;
				}
			break;
			case 1://回显界面
				Seg_Buf[0] = 12 + Val_Index;//标识符
				if(Val_Index < 2) //处于非时间回显界面
				{
					Seg_Point[6] = 1;
					Seg_Buf[1] = 10;
					Seg_Buf[2] = Val_Max[Val_Index] / 10;
					Seg_Buf[3] = Val_Max[Val_Index] % 10;
					Seg_Buf[4] = 11;
					Seg_Buf[5] = (unsigned char)Val_Average[Val_Index] / 10;
					Seg_Buf[6] = (unsigned char)Val_Average[Val_Index] % 10;
					Seg_Buf[7] = (unsigned int)(Val_Average[Val_Index] * 10) % 10;
				}
				else //处于时间回显界面
				{
					Seg_Point[6] = 0;
					Seg_Buf[1] = Message_Num / 10;
					Seg_Buf[2] = Message_Num % 10;
					Seg_Buf[3] = Time_Val[0] / 10;
					Seg_Buf[4] = Time_Val[0] % 10;
					Seg_Buf[5] = 11;
					Seg_Buf[6] = Time_Val[1] / 10;
					Seg_Buf[7] = Time_Val[1] % 10;
				}
				if(Message_Num == 0)
				{
					Seg_Point[6] = 0;
					for(i=2+(Val_Index == 2);i<8;i++)
						Seg_Buf[i] = 10;
				}
			break;
			case 2://参数界面
				Seg_Point[6] = 0;
				for(i=0;i<5;i++)
					Seg_Buf[1+i] = 10;
				Seg_Buf[0] = 15;
				Seg_Buf[6] = Temperature_Val / 10;
				Seg_Buf[7] = Temperature_Val % 10;
			break;
			case 3://温度界面
				Seg_Point[6] = 0;
				Seg_Buf[0] = 16;
				Seg_Buf[1] = Seg_Buf[2] = 10; 
				Seg_Buf[3] = Message[0] / 10;
				Seg_Buf[4] = Message[0] % 10;
				Seg_Buf[5] = 11;
				if(Message[1] > 0) //有效数据
				{
					Seg_Buf[6] = Message[1] / 10;
					Seg_Buf[7] = Message[1] % 10;
				}
				else //无效数据
					Seg_Buf[6] = Seg_Buf[7] = 17;
			break;
		}
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	ucLed[0] = (Seg_Disp_Mode == 0);
	ucLed[1] = (Seg_Disp_Mode == 1);
	ucLed[2] = (Seg_Disp_Mode == 3);	
	ucLed[3] = (Message[0] > Temperature_Ctrl) * Led_Star_Flag;
	ucLed[4] = !Message[1];//采集指示灯
	ucLed[5] = ((Message[0] > Message_Old[0]) && (Message[1] > Message_Old[1])) * (Message_Num / 2);
}


/* 定时器0中断初始化函数 */
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x05;		//配置计数模式
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
	TL1 = 0x18;		//设置定时初值
	TH1 = 0xFC;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;		//定时器1中断打开
	EA = 1;			//总中断打开
}

/* 定时器1中断服务函数 */
void Timer1Server() interrupt 3
{  
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	
	if(Key_Flag == 1)
	{
		if(++Timer_2000Ms > 2000)
			Timer_2000Ms = 3000;
	}
	
	if(++Timer_100Ms == 100)
	{
		Timer_100Ms = 0;
		Led_Star_Flag ^= 1;
	}
	
	if(++Timer_1000Ms == 1000)
	{
		Timer_1000Ms = 0;
		TR0 = 0;
		Freq = TH0 << 8 | TL0;
		TH0 = TL0 = 0;
		TR0 = 1;
	}
}


/* Main */
void main()
{
	System_Init();
	Set_Rtc(ucRtc);//初始化时钟数据
	Read_Rtc(ucRtc);//读取初始化时钟数据
	while(rd_temperature() == 85);//避免上电温度数据85.00
	Timer1Init();
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}