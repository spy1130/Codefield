/*头文件声明区*//**/
#include <STC15F2K60S2.H>
#include <Init.h>
#include "iic.h"
#include <Key.h>
#include <Seg.h>
#include <Led.h>
/*用户自定义区*/
typedef unsigned char u8;/*重定义*/
typedef unsigned int u16;
u8 Key_Show_Down,Seg_Show_Down;/*减速专用*/
u8 Key_Val,Key_Down,Key_Old;/*按键专用*/
u8 Seg_Pos;/*数码管显示位*/
u8 Seg_Buf[8]={0,1,1,1,2,2,3,4};/*数码管显示数据存放数组*/
u8 Seg_Set;/*为0熄灭数码管*//*为1改变Led模式*//*为2改变间隔*//**/
u8 i;/*循环LED点亮*/
u8 Timera_Max[8]={4,4,4,4,1,1,1,1};/*流转间隔值*/
u8 Timerb_Max[4]={4,4,4,4};/*流转间隔改变值*/
u8 Led_Pwm=2;/*PWM比较值*//*Led亮度等级*/
char Led_Mode_a,Led_Mode_b;/*Led模式以及改变模式值*/
float Rb2;/*模拟电压值*/
bit Seg_Mode;/*数码管模式*/
bit Start;/*为1点亮Led*/
bit Begin;/*为1Led启动,为0停止*/
bit Glimmer;/*闪烁标志位*/
/*用户自定义结束*/

/*键盘处理函数*/
void Key_Proc()
{
	u8 j;/*循环专用*/
	if(Key_Show_Down)return;
	Key_Show_Down=1;/*按键减速*/
	Key_Val=Key_Read();/*获取键码值*/
	Key_Down=Key_Val&(Key_Val^Key_Old);/*捕捉下降沿*/
	Key_Old=Key_Val;/*辅助扫描*/
	switch(Key_Down)
	{
		case 4:
			if(Seg_Set==1)/*Led模式减小*/
				{if(--Led_Mode_b<0)Led_Mode_b=3;}
			else if(Seg_Set==2){/*流转间隔减小*/
				if(--Timerb_Max[Led_Mode_b]<4)Timerb_Max[Led_Mode_b]=12;}
		break;
		case 5:
			if(Seg_Set==1)/*Led模式增加*/
				{if(++Led_Mode_b>3)Led_Mode_b=0;}
			else if(Seg_Set==2){/*流转间隔增加*/
			  if(++Timerb_Max[Led_Mode_b]>12)Timerb_Max[Led_Mode_b]=4;}
		break;
		case 6:/*改变修改位*/
			if(++Seg_Set==3)Seg_Set=0;
			if(Seg_Set==1)/*Led模式切换*/
				{
					Led_Mode_b=Led_Mode_a;/*获取当前Led模式*/
					for(j=0;j<4;j++)
					Timerb_Max[j]=Timera_Max[j];/*获取Led流转间隔*/
				}
			else if(Seg_Set==0)
				{
					for(j=0;j<8;j++)
					{
						if(j<4)
						Timera_Max[j]=Timerb_Max[j];/*获取改变的间隔值*/
						Led_Disp(j,0);/*熄灭Led*//*便于Led显示*/
					}
					i=0;/*Led点亮位复位*/
					Led_Mode_a=Led_Mode_b;/*获取改变的模式*/
					EEPROM_Write(Timera_Max,0,8);/*将改变间隔值保存在EEPROM中*/
				}
		break;
		case 7:
			Start=1;/*点亮Led*/
			Begin=~Begin;/*启动或停止Led*/
		break;
	}
	if(Seg_Set==0&&Key_Old==4)/*长按S4*/
		Seg_Mode=1;/*显示Led亮度等级*/
	else
		Seg_Mode=0;
}
/*数码管处理函数*/
void Seg_Proc()
{
	u8 j;
	if(Seg_Show_Down)return;
	Seg_Show_Down=1;
	Rb2=Ad_Read(0x43)/51.0;/*获取模拟电压值*/
	if(Rb2>=0&&Rb2<=1.25)Led_Pwm=2;/*亮度等级1*/
	else if(Rb2>1.25&&Rb2<=2.5)Led_Pwm=4;/*亮度等级2*/
	else if(Rb2>2.5&&Rb2<=3.75)Led_Pwm=6;/*亮度等级3*/
	else if(Rb2>3.75&&Rb2<=5.0)Led_Pwm=8;/*亮度等级4*/
	if(Seg_Mode==0){
		if(Seg_Set!=0){
			Seg_Buf[0]=(Glimmer&&Seg_Set==1)?10:11;
			Seg_Buf[1]=(Glimmer&&Seg_Set==1)?10:Led_Mode_b+1;/*显示Led模式*/
			Seg_Buf[2]=(Glimmer&&Seg_Set==1)?10:11;
			Seg_Buf[3]=10;
			if(Timerb_Max[Led_Mode_b]>=10)/*如果间隔大于等于1000ms*/
				Seg_Buf[4]=(Glimmer&&Seg_Set==2)?10:Timerb_Max[Led_Mode_b]/10;/*显示流转间隔*/
			else 
				Seg_Buf[4]=10;
				Seg_Buf[5]=(Glimmer&&Seg_Set==2)?10:Timerb_Max[Led_Mode_b]%10;
				Seg_Buf[6]=(Glimmer&&Seg_Set==2)?10:0;
				Seg_Buf[7]=(Glimmer&&Seg_Set==2)?10:0;}
		else
		{
			for(j=0;j<8;j++)
			Seg_Buf[j]=10;/*熄灭数码管*/
		}
	}
	else
	{/*长按显示亮度等级*/
		for(j=0;j<6;j++)
		Seg_Buf[j]=10;
		Seg_Buf[6]=11;
		Seg_Buf[7]=Led_Pwm/2;
	}
}
/*其它处理函数*/
void Led_Proc()
{
	if(Start){
	switch(Led_Mode_a)
	{
		case 0:
			Led_Disp(i,1);
			if(i>0)
			Led_Disp(i-1,0);
		break;
		case 1:
			Led_Disp(7-i,1);
			if(i>0)
			Led_Disp(8-i,0);
		break;
		case 2:
			Led_Disp(i,1);Led_Disp(7-i,1);
			if(i>0&&i<=3)
			{Led_Disp(i-1,0);Led_Disp(8-i,0);}
			else if(i==4){Led_Mode_a=3;i=0;}
		break;
		case 3:
			Led_Disp(3-i,1);Led_Disp(i+4,1);
			if(i>0&&i<=3)
			{Led_Disp(4-i,0);Led_Disp(i+3,0);}
			else if(i==4){Led_Mode_a=0;i=0;Led_Disp(7,0);}
		break;
	}}
}
/*中断初始化函数*/
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
}
/*中断服务函数*/
void Timer0Server() interrupt 1
{
	u8 j,Led_Num;
	u16 Timer_Min,Timer400ms;
	if(++Key_Show_Down==10)Key_Show_Down=0;/*按键减速10ms*/
	if(++Seg_Show_Down==80)Seg_Show_Down=0;/*数码管减速80ms*/
	if(++Seg_Pos==8)Seg_Pos=0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos]);
	if(Begin){/*Led启动*/
	if(++Timer_Min>=(Timera_Max[Led_Mode_a]*100))/*流转间隔*/
	{
		Timer_Min=0;
		if(++i==8){i=0;Led_Mode_a++;}/*点亮位改变*/
	}}
	if(Seg_Set!=0)/*改变值时*/
	{
		if(++Timer400ms==400)/*800ms间隔闪烁*/
		{
			Timer400ms=0;
			Glimmer=~Glimmer;
		}
	}
	else
	{
		Timer400ms=0;
		Glimmer=0;/*复位*/
	}
	if(++Led_Num==10)Led_Num=0;
	if(Led_Num>Led_Pwm)/*PWM改变亮度*/
	{for(j=0;j<8;j++)Led_Disp(j,0);}
	else Led_Proc();
}
/*主函数*/
void main()
{
	EEPROM_Read(Timera_Max,0,8);
	System_Init();
	Timer0Init();
	while(1)
	{
		Key_Proc();
		Seg_Proc();
//		Led_Proc();
	}
}