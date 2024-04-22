/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include "ds1302.h"//DS1302底层驱动专用头文件
#include <string.h>//memcpy函数专用头文件


/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char Seg_Disp_Mode;//数码管显示模式变量 0-时钟显示 1-闹钟显示 2-日期显示 3-时钟设置 4-闹钟设置 5-日期设置
unsigned char ucRtc[3] = {0x23,0x59,0x55};//时钟数据存放数组 上电初始化时间 23:59:55
unsigned char ucDate[3] = {0x22,0x12,0x12};//日期数据存放数组 上电初始化日期 22-12-12
unsigned char ucAlarm[9] = {0x00,0x00,0x00,0x00,0x10,0x00,0xbb,0xbb,0xbb};//闹钟数据存放数组 三组闹钟 未启用0xbb表示
unsigned char Alarm_Dat_Index;//闹钟数组指针 代表当前闹钟编号
unsigned char Set_Dat[9];//设置数据存放数组 如果三个模式定义三个不同的设置数组会大量占用资源 又因为这三个模式在设置状态下并不会互相影响（互斥存在关系） 所以只需要定义一个设置数组 根据不同的界面将不同的数据赋值给这个数组
unsigned int Timer_500Ms;//五百毫秒计时变量
unsigned char* Set_Flag[3] = {ucRtc,ucAlarm,ucDate};//指针数组 简化数据赋值专用
unsigned char Input_Index;//输入位数变量 用于判断哪个单元格需要闪烁
unsigned char Beep_Index;//用于记录哪个闹钟使能的变量
unsigned char Timer_200Ms;//两百毫秒计时变量
bit Input_Flag;//输入标志位 用于判断输入十位还是个位
bit Seg_Star_Flag;//数码管闪烁标志位
bit Beep_Flag;//闹钟使能标志位
bit Beep_Enable_Flag = 1;//闹钟功能开启标志位
bit Led_Star_Flag;//Led闪烁标志位

/* 数据判断函数 */
unsigned char Dat_Judgment(unsigned char* Dat,unsigned char mode)//第一个入口参数为需要判断的数组地址 第二个入口参数为需要判断的模式（年月日、时分秒）
{
	unsigned char i;//用于For循环
	unsigned int Dat_Array[3];//十进制数据存放数组 将BCD码转换成十进制便于比较
	for(i=0;i<3;i++)
	{
		Dat_Array[i] = ((*Dat / 16) % 10 ) * 10 + ((*Dat % 16) % 10);// % 10的目的是为了让被删除的闹钟在判断时不对数据造成影响
		Dat++;
	}
	if(mode == 2)//日期判断
	{
		Dat_Array[0] += 2000;//将年变为20XX的格式便于下面的判断
		if(Dat_Array[1] < 1 || Dat_Array[1] > 12) //如果月份不在1-12之间
			return 0;//返回错误
		if(Dat_Array[2] < 1 || Dat_Array[2] > 31) //如果日期不在1-31之间
			return 0;
		if(Dat_Array[1] == 2 && Dat_Array[2] == 29)//检查2.29是否有效
		{
			if((Dat_Array[0] % 4 == 0 && Dat_Array[0] % 100 != 0) || Dat_Array[0] % 400) //闰年条件：能被4整除但是不能被100整除 或者 能被400整除
				return 1;//返回正确
			else
				return 0;//返回错误
		}
		if(Dat_Array[1] == 4 || Dat_Array[1] == 6 || Dat_Array[1] == 9 || Dat_Array[1] == 11)//检查日期是否有效
			return !(Dat_Array[2] / 30);//如果日期大于30 则返回0 反之返回1
	}
	else//时间判断
	{
		if(Dat_Array[0] > 23)//如果时钟不在0-23之间
			return 0;//返回错误
		if(Dat_Array[1] > 59 || Dat_Array[2] > 59)//如果分钟或秒钟不在0-59之间
			return 0;//返回错误
		return 1;//其他情况返回正确
	}
}

/* 键盘处理函数 */
void Key_Proc()
{
	unsigned char i;//用于For循环
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上降沿
	Key_Old = Key_Val;//辅助扫描变量

	/* 键盘输入区域 */
	if(Key_Down >= 4 && Key_Down <= 13 && Input_Index < 3) //输入完一次后取消使能 保护数据
	{
		if(Seg_Disp_Mode >= 3)//处于参数设置状态
		{
			if(Input_Flag == 0)//输入十位
				Set_Dat[Input_Index+3*Alarm_Dat_Index] = ((Key_Down - 4) << 4) | (Set_Dat[Input_Index+3*Alarm_Dat_Index] & 0x0f);//写入高位
			else//输入个位
				Set_Dat[Input_Index+3*Alarm_Dat_Index] = (Set_Dat[Input_Index+3*Alarm_Dat_Index] & 0xf0) | (Key_Down - 4);//写入低位
			Input_Flag ^= 1;//标志位取反
			if(Input_Flag == 0)//完成一个周期 代表一个单元输入完成
				Input_Index++;
//				Input_Index = (Input_Index + 1) % 3;//Input_Index在0-2之间循环 通用表达式为:（变量-循环起点+1）%（循环终点-循环起点+1） + 循环起点
		}
	}
	
	switch(Key_Down)
	{
		case 14://界面切换按键
			if(Seg_Disp_Mode < 3) //在非参数设置状态下使能
			{
				if(++Seg_Disp_Mode == 3) //在0-2之间循环切换
					Seg_Disp_Mode = 0;
			}
		break;
		case 15://参数设置按键
			if(Seg_Disp_Mode < 3) //在非参数设置状态
			{
				Seg_Disp_Mode += 3;//切换到对应的设置界面内
				Alarm_Dat_Index = 0;//闹钟指针复位
				memcpy(Set_Dat,Set_Flag[Seg_Disp_Mode-3],9);//Set_Flag为指针数组 Set_Flag[0] 代表 ucRtc 数组的地址 以此类推
//				switch(Seg_Disp_Mode)
//				{
//					case 3://切换到时钟设置界面
//						memcpy(Set_Dat,ucRtc,9);
//					break;
//					case 4://切换到闹钟设置界面
//						memcpy(Set_Dat,ucAlarm,9);
//					break;	
//					case 5://切换到日期设置界面
//						memcpy(Set_Dat,ucDate,9);
//					break;						
//				}
			}
			else//在参数设置状态下
			{
				if(Input_Index == 3)//只有输入完三次数据后才能使能保存按键
				{
					if(Dat_Judgment(&Set_Dat[0],Seg_Disp_Mode-3) && Dat_Judgment(&Set_Dat[3],Seg_Disp_Mode-3) && Dat_Judgment(&Set_Dat[6],Seg_Disp_Mode-3))
					{
						switch(Seg_Disp_Mode)
						{
							case 3:
								Set_Rtc(Set_Dat);//设置新的时钟数据
							break;
							case 4:
								memcpy(ucAlarm,Set_Dat,9);//保存闹钟数据
							break;
							case 5:
								Set_Date(Set_Dat);//设置新的时钟数据
							break;
						}
						Seg_Disp_Mode -= 3;//跳转回数据显示界面
					}
					else
					{
						memcpy(Set_Dat,Set_Flag[Seg_Disp_Mode-3],9);//清除输入数据
						Input_Index = Input_Flag = 0;//输入系列数据复位
					}
//					switch(Seg_Disp_Mode)
//					{
//						case 3://时钟设置
//							if(Dat_Judgment(Set_Dat,0))//判断时钟数据有效性
//							{
//								Set_Rtc(Set_Dat);//设置新的时钟数据
//								Seg_Disp_Mode = 0;//跳转回时钟显示界面
//							}
//							else//数据不合法
//							{
//								memcpy(Set_Dat,ucRtc,9);//清除输入数据
//								Input_Index = Input_Flag = 0;//输入系列数据复位
//							}
//						break;
//						case 4://闹钟设置
//							if(Dat_Judgment(&Set_Dat[0],1) && Dat_Judgment(&Set_Dat[3],1) && Dat_Judgment(&Set_Dat[6],1))//判断三组闹钟数据有效性
//							{
//								memcpy(ucAlarm,Set_Dat,9);//保存闹钟数据
//								Seg_Disp_Mode = 1;//跳转回闹钟显示界面
//							}
//							else//数据不合法
//							{
//								memcpy(Set_Dat,ucAlarm,9);//清除输入数据
//								Input_Index = Input_Flag = 0;//输入系列数据复位
//							}
//						break;
//						case 5://日期设置
//							if(Dat_Judgment(Set_Dat,2))//判断日期数据有效性
//							{
//								Set_Date(Set_Dat);//设置新的时钟数据
//								Seg_Disp_Mode = 2;//跳转回时钟显示界面
//							}
//							else//数据不合法
//							{
//								memcpy(Set_Dat,ucDate,9);//清除输入数据
//								Input_Index = Input_Flag = 0;//输入系列数据复位
//							}
//						break;
//					}
				}
			}
		break;
		case 17://闹钟切换按键
			if(Seg_Disp_Mode == 1 || Seg_Disp_Mode == 4) // 处于闹钟显示界面
			{
				if(++Alarm_Dat_Index == 3) //在0-2之间循环切换
					Alarm_Dat_Index = 0;				
			}
		break;
		case 18://闹钟功能按键
			if(Seg_Disp_Mode < 3)//处于非参数设置界面
				Beep_Enable_Flag ^= 1;
		break;
		case 19://闹钟删除按键
			if(Seg_Disp_Mode == 4)//处于闹钟设置界面
			{
				for(i=0;i<3;i++)
					Set_Dat[3*Alarm_Dat_Index + i] = 0xbb;//清空数据
				Input_Index = 0;
				Input_Flag = 0;
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
	
	/* 信息读取区域 */
	Read_Rtc(ucRtc);//读取实时时钟数据
	Read_Date(ucDate);//读取实时日期数据
	
	/* 数据处理区域 */
	Seg_Buf[2] = Seg_Buf[5] = 11; //这两个数码管不管在什么状态下都为 -  因此只需要写在switch语句外面即可
	
//	switch(Seg_Disp_Mode)
//	{
//		case 0://时钟显示
//			for(i=0;i<3;i++)
//			{
//				Seg_Buf[3*i] = ucRtc[i] / 16;
//				Seg_Buf[3*i+1] = ucRtc[i] % 16;
//			}
//		break;
//		case 1://闹钟显示
//			for(i=0;i<3;i++)
//			{
//				Seg_Buf[3*i] = ucAlarm[i+3*Alarm_Dat_Index] / 16;
//				Seg_Buf[3*i+1] = ucAlarm[i+3*Alarm_Dat_Index] % 16;
//			}	
//		break;
//		case 2://日期显示
//			for(i=0;i<3;i++)
//			{
//				Seg_Buf[3*i] = ucDate[i] / 16;
//				Seg_Buf[3*i+1] = ucDate[i] % 16;
//			}	
//		break;
//		default://处于参数设置状态
//			for(i=0;i<3;i++)
//			{
//				Seg_Buf[3*i] = Set_Dat[i+3*Alarm_Dat_Index] / 16;
//				Seg_Buf[3*i+1] = Set_Dat[i+3*Alarm_Dat_Index] % 16;
//			}
//	}

	if(Seg_Disp_Mode < 3)//处于参数显示界面
	{
		for(i=0;i<3;i++)
		{
			Seg_Buf[3*i] = Set_Flag[Seg_Disp_Mode][i+3*Alarm_Dat_Index] / 16;
			Seg_Buf[3*i+1] = Set_Flag[Seg_Disp_Mode][i+3*Alarm_Dat_Index] % 16;
		}			
	}
	else//处于设置参数界面
	{
		for(i=0;i<3;i++)
		{
			Seg_Buf[3*i] = Set_Dat[i+3*Alarm_Dat_Index] / 16;
			Seg_Buf[3*i+1] = Set_Dat[i+3*Alarm_Dat_Index] % 16;
		}		
		Seg_Buf[3*Input_Index] = Seg_Star_Flag?Set_Dat[Input_Index+3*Alarm_Dat_Index] / 16:10;
		Seg_Buf[3*Input_Index+1] = Seg_Star_Flag?Set_Dat[Input_Index+3*Alarm_Dat_Index] % 16:10;		
	}
}


/* 其他显示函数 */
void Led_Proc()
{
	unsigned char i;//For循环专用变量
	for(i=1;i<4;i++)//常用互斥点亮思路 利用位运算符 条件满足时默认返回1
		ucLed[i] = (i == Alarm_Dat_Index + 1) * (Seg_Disp_Mode == 1 || Seg_Disp_Mode == 4);//处于时钟界面时 点亮相应的Led指示灯
//	if(ucAlarm[0] == ucRtc[0] && ucAlarm[1] == ucRtc[1] && ucAlarm[2] == ucRtc[2])
//		Beep_Flag = 1;
//	if(ucAlarm[3] == ucRtc[0] && ucAlarm[4] == ucRtc[1] && ucAlarm[5] == ucRtc[2])
//		Beep_Flag = 1;
//	if(ucAlarm[6] == ucRtc[0] && ucAlarm[7] == ucRtc[1] && ucAlarm[8] == ucRtc[2])
//		Beep_Flag = 1;
	for(i=0;i<3;i++)//使用循环简化代码 当检测到一个闹钟使能时 直接退出循环 减少运行资源占用
	{
		if(ucAlarm[0+i*3] == ucRtc[0] && ucAlarm[1+i*3] == ucRtc[1] && ucAlarm[2+i*3] == ucRtc[2])
		{
			Beep_Flag = 1;
			Beep_Index = i;//记录闹钟编号
			break;
		}
	}
	if(Beep_Flag == 1)//闹钟处于使能状态
	{
		if((ucRtc[2] % 16) ==  ((ucAlarm[2+Beep_Index*3] % 16) + 5) % 10)//记录五秒
			Beep_Flag = 0;//关闭闹钟
	}
	ucLed[0] = Led_Star_Flag?(Beep_Flag&Beep_Enable_Flag):0;
	ucLed[7] = !(ucAlarm[3*Alarm_Dat_Index] / 0x24) * (Seg_Disp_Mode == 1 || Seg_Disp_Mode == 4);;//若时钟值比23大 则表示未启用
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
	if(++Seg_Slow_Down == 100) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	if(++Timer_500Ms == 500)
	{
		Timer_500Ms = 0;//计时值复位
		Seg_Star_Flag ^= 1;//标志位取反
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
	System_Init();
	Timer0Init();
	Set_Rtc(ucRtc);//设置上电初始化时间
	Set_Date(ucDate);//设置上电初始化日期
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}
