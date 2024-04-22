//头文件
#include <STC15F2K60S2.H>
#include <IIC.h>
#include <Init.h>
#include <Key.h>
#include <Seg.h>
#include <Led.h>
/*----------------基本变量-------------------*/
unsigned char Seg_Disp_Mobe;
unsigned char Key_Val,Key_Old,Key_Down,Key_Up;
unsigned char Key_Slow_Down;
unsigned int  Seg_Slow_Down;
unsigned char Seg_Pos;
unsigned char Seg_Buf[]={10,10,10,10,10,10,10,10};
unsigned char ucLed[]={0,0,0,0,0,0,0,0};
unsigned char Seg_Point[]={0,0,0,0,0,0,0,0};
/*---------------IIC协议变量-----------------*/
float         Date;                                			//AD转化变量
/*---------------电压显示界面----------------*/
unsigned char Seg_Show[]={11,10,10,10,10,10,10,10};			//电压显示数组专用
/*---------------电压输出模式----------------*/
unsigned char Seg_Output[]={12,10,10,10,10,10,10,10};   //电压输出数组专用
/*---------------按键功能变量----------------*/
unsigned char Output_Solid;                             //固定按键输出变量
unsigned char Seg_Switch=1;                             //控制Seg的亮灭(给定初始值)
unsigned char Led_Function=1;                           //控制Led的亮灭(给定初始值)
//key函数
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down=1;
	
	Key_Val=Key_Read();
	Key_Down=Key_Val & (Key_Old ^ Key_Val);
	Key_Up=~Key_Val & (Key_Old ^ Key_Val);
	Key_Old=Key_Val;
	
	switch(Key_Down)
	{
		case 4 :
//		switch(Seg_Disp_Mobe)
//		{
//			case 0 :
//			Seg_Disp_Mobe=1;
//			break;
//			
//			case 1 :
//			Seg_Disp_Mobe=0;
//			break;
//		}
//		break;
		if(++Seg_Disp_Mobe==2) Seg_Disp_Mobe=0;
		break;
		
		case 5 :
		Output_Solid=~Output_Solid;
		break;
		
		case 7 :
		Seg_Switch=~Seg_Switch;
		break;
		
		case 6 :
		Led_Function=~Led_Function;
	}
}

//Seg函数
void Seg_Proc()
{
	unsigned char Cycle;                            //循环变量
	
	if(Seg_Slow_Down) return;
	Seg_Slow_Down=1;
	
	Date=Ad_Read(0x43);                             //读取滑动变阻器的电压值
	
	switch(Seg_Disp_Mobe)
	{
		case 0 :                                      //电压显示界面
		Seg_Show[5]=(unsigned char)Date/51.0;
		Seg_Show[6]=(unsigned char)(Date/51.0*10)%10;
		Seg_Show[7]=(unsigned char)(Date/51.0*100)%10;
		for(Cycle=0;Cycle<8;Cycle++)
		Seg_Buf[Cycle]=Seg_Show[Cycle];
		Seg_Point[5]=1;
		break;
		
		case 1 :                                      //电压输出界面
		Seg_Output[5]=Output_Solid?(unsigned char)Date/51.0:2;
		Seg_Output[6]=Output_Solid?(unsigned char)(Date/51.0*10)%10:0;
		Seg_Output[7]=Output_Solid?(unsigned char)(Date/51.0*100)%10:0;
		for(Cycle=0;Cycle<8;Cycle++)
		Seg_Buf[Cycle]=Seg_Output[Cycle];
		Seg_Point[5]=1;
		break;
	}
	
	if(Seg_Switch!=1)
	{
		for(Cycle=0;Cycle<8;Cycle++)
		Seg_Buf[Cycle]=10;
		Seg_Point[5]=0;
	}
}

//Led函数
void Led_Proc()
{
	unsigned char Move;                       //移动变量
	/*-----------Led总开关打开-----------*/	
	if(Led_Function==1)
	{
	/*-------第1个和第二个Led灯---------*/
	if(Seg_Disp_Mobe==0) 
	{ucLed[0]=1;ucLed[1]=0;}
	if(Seg_Disp_Mobe==1)
	{ucLed[0]=0;ucLed[1]=1;}
	
	/*----------第三个Led灯------------*/
	if(Seg_Show[5]+Seg_Show[6]*0.1<1.5)
	ucLed[2]=0;
	if(Seg_Show[5]+Seg_Show[6]*0.1>=1.5 && Seg_Show[5]+Seg_Show[6]*0.1<2.5)
	ucLed[2]=1;
	if(Seg_Show[5]+Seg_Show[6]*0.1>=2.5 && Seg_Show[5]+Seg_Show[6]*0.1<3.5)
	ucLed[2]=0;
	if(Seg_Show[5]+Seg_Show[6]*0.1>=3.5)
	ucLed[2]=1;
	
	/*-----------第四个Led灯-----------*/
	if(Output_Solid==0)
	ucLed[3]=0;
	else
	ucLed[3]=1;
	}
	/*-------Led总开关关闭-------------*/
	else
	{
		for(Move=0;Move<4;Move++)
		ucLed[Move]=0;
	}
}

//定时器0函数
void Timer0Init(void)		
{
	AUXR &= 0x7F;		
	TMOD &= 0xF0;		
	TL0 = 0x18;		
	TH0 = 0xFC;		
	TF0 = 0;		
	TR0 = 1;
	ET0=1;
	EA=1;
}
//定时器0服务函数 
void Time0Server() interrupt 1
{
	if(++Seg_Slow_Down==500) Seg_Slow_Down=0;
	if(++Key_Slow_Down==10)  Key_Slow_Down=0;
	
	if(++Seg_Pos==8) Seg_Pos=0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	
}

//main函数
void main()
{
	System_Init();
	Timer0Init();
	while(1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}