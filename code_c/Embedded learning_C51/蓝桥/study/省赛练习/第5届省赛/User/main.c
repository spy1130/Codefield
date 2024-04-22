//先Seg减速，顺带定时器别忘了ET0 =1;EA=1;，，Seg扫描，led+扫描，Key减速+三行消抖，主程序
#include <STC15F2K60S2.H>
#include <init.h>
#include <Led.h>
#include <Key.h>
#include <Seg.h>
#include "ds1302.h" 
#include "iic.h"

unsigned char Seg_Slow_Down;
unsigned char Seg_Buf[8]={10,10,10,10,10,10,10,10};
unsigned char Seg_Point[8]={0,0,0,0,0,0,0,0};
unsigned char Seg_Pos;
bit Seg_Disp_Mode;//数码管显示模式变量 0-数据显示界面 1-参数设置界面
unsigned char ucLed[8]={0,0,0,0,0,0,0,0};

unsigned char Key_Val,Key_Old,Key_Down,Key_Up;
unsigned char Key_Slow_Down;
unsigned char ucRtc[3]={0x08,0x30,0x00};
unsigned char wet;//湿度百分比
unsigned char wetc_Disp=50;//温度阀值显示
unsigned char wetc_Save=50;//温度阀值保存
bit Work_Mode;//工作状态标志位 0-自动工作模式 1-手动工作模式
bit Beep_Enable_Flag;//闹钟使能标志位
bit Relay_Flag;//继电器工作标志位
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
		case 7:
			Work_Mode ^= 1;
		
		break;
		case 6:
			if(Work_Mode == 0)//自动模式
			{
				Seg_Disp_Mode ^= 1;
				if(Seg_Disp_Mode == 0)
				{
					wetc_Save=wetc_Disp;
					EEPROM_Write(&wetc_Save,0,1);
				}
				
			}
			else//手动模式
				Beep_Enable_Flag ^= 1;
		break;
		case 5:
			if(Work_Mode == 0)
			{
				if(++wetc_Disp == 100)
					wetc_Disp = 99;
			}
			else
				Relay_Flag = 1;
		break;
		case 4:
			if(Work_Mode == 0)
			{
				if(--wetc_Disp == 255)
					wetc_Disp = 0;
			}
			else
				Relay_Flag = 0;
		break;
	}
}


void Seg_Proc()
{	//等于0时进入
	unsigned char i;
	Read_Rtc(ucRtc);
	wet =Ad_Read(0x03)/2.56;//0-255一共256个数，0%-99%，对应下来是256/100=2.56
	if(Seg_Slow_Down) return;
	Seg_Slow_Down =1;
	if(Seg_Disp_Mode == 0)
	{
		for(i=0;i<2;i++)
		{
			Seg_Buf[i*3]=ucRtc[i]/16;
			Seg_Buf[i*3+1]=ucRtc[i]%16;
		}
		Seg_Buf[2] = 11;
		Seg_Buf[6] = wet / 10;
		Seg_Buf[7] = wet % 10;
	}
	else
	{
		Seg_Buf[0] = 11;
		Seg_Buf[1] = 11;		
		Seg_Buf[2] = Seg_Buf[3] = Seg_Buf[4] = 10;
		Seg_Buf[6] = wetc_Disp / 10;
		Seg_Buf[7] = wetc_Disp % 10;		
		
	}

}

void Led_Proc()
{
	ucLed[0] = !Work_Mode;
	ucLed[1] = Work_Mode;
}

void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 =1;
	EA=1;
}
/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
    if(++Seg_Slow_Down==500) Seg_Slow_Down=0;//数码管减速专用
	if(++Seg_Pos==8)Seg_Pos=0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	Relay(Relay_Flag);
	Beep((wetc_Save / wet) & Beep_Enable_Flag);
}



void main()
{	Set_Rtc(ucRtc);
	wet = Ad_Read(0x03) / 2.56;//上电读取湿度 避免出现无效数据
	EEPROM_Read(&wetc_Save,0,1);
	wetc_Disp = wetc_Save;
	System_Init();
	Timer0_Init();
	while(1)
	{
		Seg_Proc();
		Key_Proc();
		Led_Proc();
		
	}
	
}