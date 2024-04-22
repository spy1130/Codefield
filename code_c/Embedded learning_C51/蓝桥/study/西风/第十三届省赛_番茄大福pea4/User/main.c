/*Created By pea4-*/

/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include <ds1302.h>//ds1302底层驱动专用头文件
#include <onewire.h>//ds18b20底层驱动专用头文件

#define u8 unsigned char
#define u16 unsigned int

/* 变量声明区 */
u8 Key_Slow_Down;//按键减速变量
u16 Seg_Slow_Down;//数码管减速变量
u8 Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
u8 Seg_Buf[8] = {16,16,16,16,16,16,16,16};//数码管显示数据存放数组
u8 Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
u8 Seg_Pos;//数码管扫描专用变量
u8 ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组

u16 Temperature=0;//温度变量
u8 Relay_Buf=0;//继电器变量
u8 Display_Mode = 0; // 0-温度显示 1-时间显示 2- 参数设置
u16 Timer_5000ms=0;//5秒计时变量
u8 Timer_100ms=0;//0.1秒计时变量
u8 Temp_Thres=23;//温度阈值
u8 Temp_Thres_Buf=0;//温度阈值显示缓存

bit Relay_Mode = 1;//继电器
bit Alarm_Flag=0;
bit Led_Flash_Flag=0;
bit Flash=0;//闪烁标志位

u8 ucRtc[3]={23,59,30};//ds1302时间寄存器

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
		case 12:
			switch(Display_Mode)
			{
				case 0:
					Display_Mode = 1;
				break;
				case 1:
					Temp_Thres_Buf=Temp_Thres;
					Display_Mode = 2;
				break;
				case 2:
					Temp_Thres=Temp_Thres_Buf;
					Display_Mode = 0;
				break;
			}
			break;
		case 17:
			if(Display_Mode == 1)
				Display_Mode = 3;
			else if(Display_Mode == 2)
			{
				if(Temp_Thres_Buf>10)
					Temp_Thres_Buf--;
			}
			break;
		case 16:
			if(Display_Mode == 2)
			{
				if(Temp_Thres_Buf<99)
					Temp_Thres_Buf++;
			}
			break;
		case 13:
			ucLed[2]=0;
			Relay_Buf=0;
			Relay_Mode ^= 1;
		break;
	}
	
	switch(Key_Up)
	{
		case 17:
			if(Display_Mode == 3)
				Display_Mode = 1;
		break;
	}
}

//

/* 信息处理函数 */
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序
	
	Read_Rtc(ucRtc);
	Temperature=rd_temperature();
	
	switch(Display_Mode)
	{
		case 0:
			Seg_Buf[0]=25;
			Seg_Buf[1]=1;
			Seg_Buf[2]=Seg_Buf[3]=Seg_Buf[4]=16;
			if(Temperature/100 == 0) Seg_Buf[5]=16;
			else Seg_Buf[5]=Temperature/100;
			Seg_Buf[6]=32 + (Temperature/10 % 10);
			Seg_Buf[7]=Temperature%10;
			break;
		case 1:
			Seg_Buf[0]=25;
			Seg_Buf[1]=2;
			Seg_Buf[2]=16;
			Seg_Buf[3]=ucRtc[0]/10;
			Seg_Buf[4]=ucRtc[0]%10;
			Seg_Buf[5]=17;
			Seg_Buf[6]=ucRtc[1]/10;
			Seg_Buf[7]=ucRtc[1]%10;
			break;
		case 3:
			Seg_Buf[0]=25;
			Seg_Buf[1]=2;
			Seg_Buf[2]=16;
			Seg_Buf[3]=ucRtc[1]/10;
			Seg_Buf[4]=ucRtc[1]%10;
			Seg_Buf[5]=17;
			Seg_Buf[6]=ucRtc[2]/10;
			Seg_Buf[7]=ucRtc[2]%10;
			break;
		case 2:
			Seg_Buf[0]=25;
			Seg_Buf[1]=3;
			Seg_Buf[2]=Seg_Buf[3]=Seg_Buf[4]=Seg_Buf[5]=16;
			if(Temp_Thres_Buf/10 == 0) Seg_Buf[6]=16;
			else Seg_Buf[6]=Temp_Thres_Buf/10;
			Seg_Buf[7]=Temp_Thres_Buf%10;
			break;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	//如果整点并且Alarm_Flag还没有被置1
	if(ucRtc[1]==0x00 && ucRtc[2]==0x00 && Alarm_Flag==0)
		Alarm_Flag=1;
	
	ucLed[0]=Alarm_Flag;//L1在整点也亮5秒
	ucLed[1]=Relay_Mode;//L2显示控制模式
	
	//继电器吸合就闪烁
	ucLed[2]=Relay_Buf?Flash:0;
	
	if(Relay_Mode)//温度控制模式
		Relay_Buf=(Temperature > (Temp_Thres * 10));
	else//时间控制模式
		Relay_Buf=Alarm_Flag;
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
	if(++Seg_Slow_Down == 50) Seg_Slow_Down = 0;//数码管减速专用

	//整点亮五秒
	if(Alarm_Flag)Timer_5000ms++;
	else Timer_5000ms=0;
	if(Timer_5000ms==5000)
	{
		Timer_5000ms=0;
		Alarm_Flag=0;
	}
	
	//继电器吸合闪烁
	if(Relay_Buf)Timer_100ms++;
	else Timer_100ms=0; //不吸合就清空计时变量
	if(Timer_100ms==100)
	{
		Timer_100ms=0;
		Flash^=1;
	}
	
	
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	Relay(Relay_Buf);
}

/* Main */
void main()
{
	System_Init();
	Set_Rtc(ucRtc);
	Temperature=rd_temperature();
	while(Temperature==850)Temperature=rd_temperature();
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}