/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include "ds1302.h" //时钟底层驱动专用头文件
#include "onewire.h" //温度底层驱动专用头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char ucRtc[3] = {0x23,0x59,0x55};//实时时钟数据数组 上电默认时间23:59:55
unsigned char Alarm[3] = {0x00,0x00,0x00};//闹钟数据储存数组
unsigned char Alarm_Set[3] = {0x00,0x00,0x00};//闹钟数据设置数组
unsigned char ucRtc_Set[3];//时钟数据设置数组
unsigned char ucRtc_Set_Index;//时钟设置数组指针
unsigned int Timer_1000Ms;//一千毫秒计时变量
unsigned char* Set_Index[3] = {ucRtc,ucRtc_Set,Alarm_Set};//简化程序专用指针数组
unsigned char Set_Flag;//时钟设置标志位 0-显示 1-设置 2-闹钟
unsigned char Timer_200Ms;//两百毫秒计时变量
unsigned char Temperature;//实时温度储存变量
bit Led_Star_Flag;//Led闪烁标志位
bit Seg_Star_Flag;//数码管闪烁标志位
bit Seg_Disp_Mode;//数码管显示模式变量 0-时间 1-温度
bit Beep_Flag;//闹钟使能标志位 0-不使能 1-使能


/* 键盘处理函数 */
void Key_Proc()
{
	unsigned char i;//For循环专用变量
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上降沿
	Key_Old = Key_Val;//辅助扫描变量

	
	if(Beep_Flag == 1) //闹钟使能状态
	{
		if(Key_Down != 0) //按下任意按键
			Beep_Flag = 0; //关闭闹钟
		return; //跳出按键子程序 避免执行下面的语句
	}
	
	if(Set_Flag == 0) //处于非设置界面
	{
		if(Key_Old == 4) 
			Seg_Disp_Mode = 1;
		else
			Seg_Disp_Mode = 0;
	}
	
	switch(Key_Down)
	{
		case 7://时钟设置
			if(Seg_Disp_Mode == 0) //处于时钟相关界面
			{
				if(Set_Flag == 0) //处于时钟显示界面
				{
					for(i=0;i<3;i++)
						ucRtc_Set[i] = ucRtc[i];//读取实时数据
					Set_Flag = 1;//切换到时钟设置界面
				}
				else if(Set_Flag == 1) //处于时钟设置界面
				{
					if(++ucRtc_Set_Index == 3)
					{
						ucRtc_Set_Index = 0;//指针复位
						Set_Rtc(ucRtc_Set);//保存时钟数据
						Set_Flag = 0;//切换到时钟显示界面
					}
				}
			}
		break;
		case 6://闹钟设置
			if(Seg_Disp_Mode == 0) //处于时钟相关界面
			{
				if(Set_Flag == 0) //处于时钟显示界面
					Set_Flag = 2;//切换到闹钟设置界面
				else if(Set_Flag == 2) //处于闹钟设置界面
				{
					if(++ucRtc_Set_Index == 3)
					{
						ucRtc_Set_Index = 0;//指针复位
						for(i=0;i<3;i++)
							Alarm[i] = Alarm_Set[i];//保存闹钟数据
						Set_Flag = 0;//切换到时钟显示界面
					}
				}
			}			
		break;
		case 5://参数自加
			if(Seg_Disp_Mode == 0) //处于时钟相关界面)
			{
//				if(Set_Flag == 1) //处于时钟设置
//				{
//					ucRtc_Set[ucRtc_Set_Index]++;
//					if(ucRtc_Set[ucRtc_Set_Index] % 16 == 0x0a) //BCD码需要手动进行十进制进位
//						ucRtc_Set[ucRtc_Set_Index] += 6;
//					if(ucRtc_Set[ucRtc_Set_Index] == (ucRtc_Set_Index?0x60:0x24)) //设置上限
//						ucRtc_Set[ucRtc_Set_Index] = ucRtc_Set_Index?0x59:0x23;
//				}
				if(Set_Flag != 0) //处于设置状态
				{
					Set_Index[Set_Flag][ucRtc_Set_Index]++;
					if(Set_Index[Set_Flag][ucRtc_Set_Index] % 16 == 0x0a) //BCD码需要手动进行十进制进位
						Set_Index[Set_Flag][ucRtc_Set_Index] += 6;
					if(Set_Index[Set_Flag][ucRtc_Set_Index] == (ucRtc_Set_Index?0x60:0x24)) //设置上限
						Set_Index[Set_Flag][ucRtc_Set_Index] = ucRtc_Set_Index?0x59:0x23;
				}
			}
		break;
		case 4://参数自减
			if(Seg_Disp_Mode == 0) //处于时钟相关界面)
			{
//				if(Set_Flag == 1) //处于时钟设置
//				{
//					ucRtc_Set[ucRtc_Set_Index]--;
//					if(ucRtc_Set[ucRtc_Set_Index] % 16 == 0x0f) //BCD码需要手动进行十进制进位
//						ucRtc_Set[ucRtc_Set_Index] -= 6;
//					if(ucRtc_Set[ucRtc_Set_Index] == 0xf9) //设置下限
//						ucRtc_Set[ucRtc_Set_Index] = 0;
//				}
				if(Set_Flag != 0) //处于设置状态
				{
					Set_Index[Set_Flag][ucRtc_Set_Index]--;
					if(Set_Index[Set_Flag][ucRtc_Set_Index] % 16 == 0x0f) //BCD码需要手动进行十进制进位
						Set_Index[Set_Flag][ucRtc_Set_Index] -= 6;
					if(Set_Index[Set_Flag][ucRtc_Set_Index] == 0xf9) //设置下限
						Set_Index[Set_Flag][ucRtc_Set_Index] = 0;
				}
			}			
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i;//For循环专用变量
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序

	/* 信息获取区域 */
	Read_Rtc(ucRtc);//实时读取时钟数据
	Temperature = rd_temperature();//实时读取温度数据
	
	/* 数据显示区域 */
	
//	if(Seg_Disp_Mode == 0) //时钟
//	{
//		if(Set_Flag == 0) //时钟显示
//		{
//			Seg_Buf[2] = Seg_Buf[5] = 11;
//			for(i=0;i<3;i++)
//			{
//				Seg_Buf[3*i] = ucRtc[i] / 16;
//				Seg_Buf[3*i+1] = ucRtc[i] % 16;
//			}
//		}
//		else if(Set_Flag == 1) //时钟设置
//		{
//			for(i=0;i<3;i++)
//			{
//				Seg_Buf[3*i] = ucRtc_Set[i] / 16;
//				Seg_Buf[3*i+1] = ucRtc_Set[i] % 16;
//			}			
//			Seg_Buf[3*ucRtc_Set_Index] = Seg_Star_Flag?10:ucRtc_Set[ucRtc_Set_Index] / 16;
//			Seg_Buf[3*ucRtc_Set_Index+1] = Seg_Star_Flag?10:ucRtc_Set[ucRtc_Set_Index] % 16;
//		}
//		else //闹钟设置
//		{
//			for(i=0;i<3;i++)
//			{
//				Seg_Buf[3*i] = Alarm[i] / 16;
//				Seg_Buf[3*i+1] = Alarm[i] % 16;
//			}			
//			Seg_Buf[3*ucRtc_Set_Index] = Seg_Star_Flag?10:Alarm[ucRtc_Set_Index] / 16;
//			Seg_Buf[3*ucRtc_Set_Index+1] = Seg_Star_Flag?10:Alarm[ucRtc_Set_Index] % 16;			
//		}
		
		/* 
			遇到在某个状态下显示格式不变 
			但是显示数据数组需要发生改变时
			可以将这些数组按照顺序放入一个指针数组内
			然后通过访问指针数组达到简化程序的目的
		*/
	if(Seg_Disp_Mode == 0) //时钟
	{
		Seg_Buf[2] = Seg_Buf[5] = 11;
		for(i=0;i<3;i++)
		{
			Seg_Buf[3*i] = Set_Index[Set_Flag][i] / 16;
			Seg_Buf[3*i+1] = Set_Index[Set_Flag][i] % 16;		
		}
		if(Set_Flag != 0) //闪烁使能
		{
			Seg_Buf[3*ucRtc_Set_Index] = Seg_Star_Flag?10:Set_Index[Set_Flag][ucRtc_Set_Index] / 16;
			Seg_Buf[3*ucRtc_Set_Index+1] = Seg_Star_Flag?10:Set_Index[Set_Flag][ucRtc_Set_Index] % 16;			
		}
	}
	else //温度
	{
		for(i=0;i<5;i++)
			Seg_Buf[i] = 10;
		Seg_Buf[5] = Temperature / 10;
		Seg_Buf[6] = Temperature % 10;
		Seg_Buf[7] = 12;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	if(ucRtc[0] == Alarm[0] && ucRtc[1] == Alarm[1] && ucRtc[2] == Alarm[2]) //闹钟使能
		Beep_Flag = 1;
	if((Alarm[2] % 16 + 5) == (ucRtc[2] % 16)) //过五秒后
		Beep_Flag = 0;
	ucLed[0] = (Led_Star_Flag?1:0) * Beep_Flag;//只有在闹钟使能条件下闪烁
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
	
	if(++Timer_1000Ms == 1000)
	{
		Timer_1000Ms = 0;
		Seg_Star_Flag ^= 1;
	}
	
	if(++Timer_200Ms == 200)
	{
		Timer_200Ms = 0;
		Led_Star_Flag ^= 1;
	}
}

/* Main */
void main()
{
	Set_Rtc(ucRtc);//上电初始化实时时钟
	System_Init();
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}