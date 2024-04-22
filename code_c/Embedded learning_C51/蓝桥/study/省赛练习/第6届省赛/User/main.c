//先Seg减速，顺带定时器记得写ET0=1;EA=1开定时器和中断，Seg扫描，led+扫描，Key减速+三行消抖，主程序
#include <STC15F2K60S2.H>
#include <init.h>
#include <Led.h>
#include <Key.h>
#include <Seg.h>
#include "onewire.h"
#include "ds1302.h" 

unsigned char Seg_Slow_Down;
unsigned char Seg_Buf[8]={10,10,10,10,10,10,10,10};
unsigned char Seg_Point[8]={0,0,0,0,0,0,0,0};
unsigned char Seg_Pos;
unsigned char Seg_Disp_Mode=0;//0-参数设置 1-时钟显示 2-温度采集显示
unsigned char Col_Int[]={1,5,30,60};//采集间隔数组
unsigned char Col_Index=0;//采集间隔数组索引
unsigned char Col_Save=1;//采集间隔保存函数
unsigned char Col_Disp=1;//采集间隔显示函数
unsigned char ucLed[8]={0,0,0,0,0,0,0,0};
unsigned char ucRTC[3]={0x23,0x59,0x50};
unsigned char Key_Val,Key_Old,Key_Down,Key_Up;
unsigned char Key_Slow_Down;
unsigned int Time_1000ms;//闪烁时间
bit Seg_Star_Flag;//闪烁标识位
unsigned char Temperature_Data[10];//温度数组
unsigned char Temperature_Data_Index;//温度索引
unsigned char Col_Time;//采集计时
bit Led_Enable_Flag;//LED开标志位
unsigned char Timer_200Ms;
bit Led_Star_Flag;

void Key_Proc()
{
	if(Key_Slow_Down)return;
	Key_Slow_Down =1;
	//重点记
	Key_Val=Key_Read();
	Key_Down=Key_Val &(Key_Old^Key_Val);
	Key_Up=~Key_Val &(Key_Old^Key_Val);
	Key_Old =Key_Val;
	switch(Key_Down)
	{
		case 4:
			if(Seg_Disp_Mode==0)
			{
			if(++Col_Index==4)
				Col_Index=0;
				Col_Disp=Col_Int[Col_Index];
			}
		break;
		case 5:
			if(Seg_Disp_Mode==0)
			{
				Col_Save=Col_Disp;
				Temperature_Data[Temperature_Data_Index]=rd_temperature();
				Temperature_Data_Index++;
				Seg_Disp_Mode =1;
			}
		break;
		case 6:
			if(Seg_Disp_Mode == 2)
			{
				Led_Enable_Flag = 0;
				if(++Temperature_Data_Index == 10)
					Temperature_Data_Index = 0;
			}
		break;
		case 7:
			if(Seg_Disp_Mode == 2)
			{
				Temperature_Data_Index = 0;
				Seg_Disp_Mode = 0;
			}
		break;
	}
	
}


void Seg_Proc()
{	unsigned char i;
	//等于0时进入
	if(Seg_Slow_Down) return;
	Seg_Slow_Down =1;
	Read_Rtc(ucRTC);
	if(Col_Time==Col_Save)
	{	Col_Time=0;
		Temperature_Data[Temperature_Data_Index]=rd_temperature();
		
		if(++Temperature_Data_Index==10)
		{	
			Temperature_Data_Index=0;
			Led_Enable_Flag=1;
			Seg_Disp_Mode=2;
			
		}
	}
	if(Seg_Disp_Mode==0)//参数设置
	{	for(i=0;i<3;i++)
			Seg_Buf[i]=10;
		Seg_Buf[5]=11;
		Seg_Buf[6]=Col_Disp/10;
		Seg_Buf[7]=Col_Disp%10;
	}
	else if(Seg_Disp_Mode==1)//时钟显示
	{
		for(i=0;i<3;i++)
		{
			Seg_Buf[3*i]=ucRTC[i]/16;
			Seg_Buf[3*i+1]=ucRTC[i]%16;
		}
//		Seg_Star_Flag = Seg_Buf[7] % 2;
		Seg_Buf[5]=Seg_Buf[2]=Seg_Star_Flag?10:11;
	}
	else if(Seg_Disp_Mode==2)
	{
		Seg_Buf[0]=Seg_Buf[5]=11;
		Seg_Buf[1] = 0;
		Seg_Buf[2] = Temperature_Data_Index;
		Seg_Buf[3] = Seg_Buf[4] = 10;
		Seg_Buf[6] = Temperature_Data[Temperature_Data_Index] / 10;
		Seg_Buf[7] = Temperature_Data[Temperature_Data_Index] % 10;
		
	}
	/* 信息显示区域 */
//	switch(Seg_Disp_Mode)
//	{
//		case 0://参数设置界面
//			Seg_Buf[0] = Seg_Buf[1] = Seg_Buf[2] = 10;
//			Seg_Buf[5] = 11;
//			Seg_Buf[6] = Col_Disp / 10;
//			Seg_Buf[7] = Col_Disp % 10;
//		break;
//		case 1://时钟显示界面
//			for(i=0;i<3;i++)
//			{
//				Seg_Buf[3*i] = ucRTC[i] / 16;
//				Seg_Buf[3*i+1] = ucRTC[i] % 16;
//			}
//			Seg_Star_Flag = Seg_Buf[7] % 2;//使用时钟芯片记录闪烁时间
//			Seg_Buf[2] = Seg_Buf[5] = Seg_Star_Flag?10:11;//10：熄灭 11：-
//		break;
//		case 2://温度显示界面
//			Seg_Buf[0] = 11;
//			Seg_Buf[1] = 0;
//			Seg_Buf[2] = Temperature_Data_Index;
//			Seg_Buf[3] = Seg_Buf[4] = 10;
//			Seg_Buf[5] = 11;
//			Seg_Buf[6] = Temperature_Data[Temperature_Data_Index] / 10;
//			Seg_Buf[7] = Temperature_Data[Temperature_Data_Index] % 10;
//		break; 
//	}

}

void Led_Proc()
{
	ucLed[0]=Led_Enable_Flag& Led_Star_Flag;
}

void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0=1;
	EA=1;
}
/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
    if(++Seg_Slow_Down==500) Seg_Slow_Down=0;//数码管减速专用
	if(++Seg_Pos==8)Seg_Pos=0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	if(Seg_Disp_Mode==1)
	{
		if(++Time_1000ms==1000)
		{
			Time_1000ms=0;
			Seg_Star_Flag ^=1;
			Col_Time++;
		}
	}
	if(++Timer_200Ms == 200)
	{
		Timer_200Ms = 0;
		Led_Star_Flag ^= 1;
	}
}




void main()
{	Set_Rtc(ucRTC);
	rd_temperature();
	System_Init();
	Timer0_Init();
	while(1)
	{
		Seg_Proc();
		Key_Proc();
		Led_Proc();
		
	}
	
}