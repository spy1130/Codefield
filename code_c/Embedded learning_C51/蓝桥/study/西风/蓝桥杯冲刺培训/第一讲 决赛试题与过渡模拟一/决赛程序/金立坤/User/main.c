/* 第二届 创意智造·奠基未来 单片机设计与开发大赛 专用模板 */
 
/* 头文件声明区 */
#include <REGX52.H>//单片机寄存器专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include <intrins.h>//Led位移头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[6] = {10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[] = {0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量

unsigned char Mode_Disp;//模式判断专用变量“0“为运行状态界面，”1“为流转状态界面。
unsigned char Mode_Count;//设置模式专用;
bit System_Flag = 1;//运行模式暂停标志位,"1"运行，“0”暂停
bit Run_Flag = 0;//显示界面标志位;"1"为进入显示界面
unsigned char Seg_Mode1[] = {11,1,10,10,10,10};//运行界面显示专用
unsigned char Led[8] = {0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F};
unsigned char Led_Index;
unsigned int Time[9] = {400,500,600,700,800,900,1000,1100,1200};//Led流转时间：400ms-1200ms;
unsigned char Time_Index = 0;//Led加减时间标志位;
unsigned int Time_400ms;//Led流转间隔时长专用
unsigned char Mode_Index = 0;//Led模式专用指针
bit Mode2_Flag = 1;//“1”为Led模式+
unsigned int Time_800ms;//设置模式专用
bit Seg_Flag;//闪烁标志位
unsigned char Led_r = 0;//右移指针Led2
unsigned char Led_l = 7;//左移指针Led2
unsigned char Led3_r = 4;//右移指针Led3
unsigned char Led3_l = 3;//左移指针Led3
/* 键盘处理函数 */
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Old = Key_Val;//辅助扫描变量
	
	switch(Key_Down)
	{
		case 4://参数-
			if(Mode_Disp == 1&&Mode2_Flag==0)
			{
				if(--Mode_Index == 255)Mode_Index = 3;
			}else if(Mode_Disp == 1&&Mode2_Flag==1)
			{
				Time_400ms = Time[Time_Index];
				if(--Time_Index == 255)Time_Index = 8;
			}
			if(System_Flag == 0)
			{
				if(Key_Down == 4)
				{
				Run_Flag = 1;
				}else {
				System_Flag = 1;
				Mode_Disp = 0;
				Run_Flag = 0;
				}
			}
		break;
		case 5://参数+
			if(Mode_Disp == 1 && Mode2_Flag==0)
			{
				if(++Mode_Index == 4)Mode_Index = 0;
			}else if(Mode_Disp == 1 && Mode2_Flag==1)
			{
				Time_400ms = Time[Time_Index];
				if(++Time_Index == 9)Time_Index = 0;
			}
		break;
		case 6://设置
			Mode2_Flag ^=1;
		  Mode_Disp = 1;
		if(++Mode_Count == 3)
		{
			Mode_Count = 0;
			Mode_Disp = 0;
			System_Flag = 1;
		}
		break;
		case 7://暂停
			System_Flag ^= 1;
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序
	
	switch(Mode_Disp)
	{
		case 0://运行状态
			if(System_Flag == 1)
			{
		Seg_Buf[0] = 11;
		Seg_Buf[1] = Mode_Index + 1;
		if(Time[Time_Index]/1000%10 != 0)
		Seg_Buf[2] = Time[Time_Index]/1000%10;
				else
		Seg_Buf[2] = 10;
		Seg_Buf[3] = Time[Time_Index] /100%10;
		Seg_Buf[4] = Time[Time_Index] /10%10;
		Seg_Buf[5] = Time[Time_Index] %10;
			}else{//暂停
		Seg_Buf[0] = 5;
		Seg_Buf[1] = Mode_Index + 1;
				if(Time[Time_Index]/1000%10 != 0)
		Seg_Buf[2] = Time[Time_Index]/1000%10;
				else
		Seg_Buf[2] = 10;
		Seg_Buf[3] = Time[Time_Index] /100%10;
		Seg_Buf[4] = Time[Time_Index] /10%10;
		Seg_Buf[5] = Time[Time_Index] %10;
				if(Run_Flag == 1)
				{
					Seg_Point[0] = 1;
					Seg_Point[1] = 1;
					Seg_Buf[0] = 0;
					Seg_Buf[1] = 11;
					Seg_Buf[2] = 12;
					Seg_Buf[3] = Mode_Index+1;
					if(Mode_Index == 0)
					 Seg_Buf[4] = 0;
					else if(Mode_Index == 1)
						Seg_Buf[4] = 0;
					else
						Seg_Buf[4] = Led_Index + 1;
					Seg_Buf[5] = Led_Index + 1;
				}
			}
		break;
			case 1://流转设置界面
				if(Mode2_Flag == 0)
				{
		  Seg_Buf[0] = Seg_Flag?12:10;
			Seg_Buf[1] = Seg_Flag?Mode_Index + 1:10;
				}else{
		if(Time[Time_Index]/1000%10 != 0)
			Seg_Buf[2] = Seg_Flag?Time[Time_Index]/1000%10:10;
		else
		Seg_Buf[2] = 10;
		Seg_Buf[3] = Seg_Flag?Time[Time_Index] /100%10:10;
		Seg_Buf[4] = Seg_Flag?Time[Time_Index] /10%10:10;
		Seg_Buf[5] = Seg_Flag?Time[Time_Index] %10:10;
	}
			break;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	if(Mode_Disp == 0)
	{
		switch(Mode_Index)
		{
			case 0:
				P1=Led[Led_Index];
			break;
			case 1:
				P1=Led[Led_Index];
			break;
			case 2://18-27-36-45-18
				P1 = Led[Led_r]&Led[Led_l];
			break;
			case 3:
				P1 = Led[Led3_r]&Led[Led3_l];
			break;
		}
	}
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
	ET0 = 1;        //定时器0中断打开
	EA = 1;         //总中断打开
}

/* 定时器0中断服务函数 */
void Timer0Server() interrupt 1
{
 	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值   
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 6) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	if(System_Flag == 1)
	{
		switch(Mode_Index)
		{
			case 0:
			if(++Time_400ms == Time[Time_Index])
			{
			Time_400ms = 0;
			if(++Led_Index == 8)Led_Index=0;
			}
			break;
			case 1:
			if(++Time_400ms == Time[Time_Index])
			{
			Time_400ms = 0;
			if(--Led_Index == 255)Led_Index=7;
			}
			break;
			case 2:
				if(++Time_400ms == Time[Time_Index])
			{
			Time_400ms = 0;
				if(++Led_r == 4)
					Led_r = 0;
				if(--Led_l == 3)
					Led_l = 7;
			}
			break;
			case 3:
				if(++Time_400ms == Time[Time_Index])
				{
					Time_400ms = 0;
					if(++Led3_r == 8)Led3_r = 4;
					if(--Led3_l == 255)Led3_l = 3;
				}
			break;
	 }
	}
	if(Mode_Disp == 1)
	{
		if(++Time_800ms == 800)
		{
		Time_800ms = 0;
		Seg_Flag ^=1;
		}
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