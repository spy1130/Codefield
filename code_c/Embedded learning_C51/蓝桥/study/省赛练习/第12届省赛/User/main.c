/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include <stdio.h>//标准库底层驱动专用头文件
#include <onewire.h>
#include <iic.h>

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char Seg_Disp_Mode;//显示模式参数 0-温度显示 1-参数设置 2-DAC输出
float temperature;
unsigned char temp_disp=25;//暂存显示
unsigned char temp_ctrl=25;//暂存控制
float Voltage_Output;//输出电压
bit Output_Mode;//电压输出模式切换
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
		case 4:
			if(++Seg_Disp_Mode==3) Seg_Disp_Mode=0;
			if(Seg_Disp_Mode==1) temp_disp=temp_ctrl;//初始显示
			if(Seg_Disp_Mode==2) temp_ctrl=temp_disp;//保存显示
		break;
		case 8:
			if(Seg_Disp_Mode==1){
			if(--temp_disp==255) temp_disp=0;
			}
		break;
		case 9:
			if(Seg_Disp_Mode==1){
			if(++temp_disp==100) temp_disp=99;
			}
		break;
		case 5:
			Output_Mode ^=1;
		break;		
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序
	temperature=rd_temperature();
	switch(Seg_Disp_Mode)
	{
		case 0://温度
			Seg_Buf[0]=12;//C
			Seg_Buf[4]=(unsigned char)temperature /10 %10;
			Seg_Buf[5]=(unsigned char)temperature %10;
			Seg_Buf[6]=(unsigned int)(temperature*100) /10%10;
			Seg_Buf[7]=(unsigned int)(temperature*100) %10;
			Seg_Point[5]=1;
		break;
		case 1://设置
			Seg_Buf[0]=13;//P
			Seg_Buf[4]=10;
			Seg_Buf[5]=10;
			Seg_Point[5]=0;
			Seg_Buf[6]=temp_disp /10%10;
			Seg_Buf[7]=temp_disp %10;
			
		break;
		case 2://DAC
			Seg_Buf[0]=11;//A
			Seg_Buf[5]=(unsigned char)Voltage_Output %10;
			Seg_Buf[6]=(unsigned int)(Voltage_Output*100) /10%10;
			Seg_Buf[7]=(unsigned int)(Voltage_Output*100) %10;
			Seg_Point[5]=1;
		break;
	}
}

/* 其他显示函数 */
void Led_Proc()
{	unsigned char i;
	/*--DAC--*/
	if(Output_Mode==0)
	{
		if(temperature<temp_ctrl)
		{
			Voltage_Output=0;
		}
		else{
			Voltage_Output=5;
		}
	}
	if(Output_Mode==1)
	{
		if(temperature<20)
		{
			Voltage_Output=1;
		}else if(temperature>40)
		{
			Voltage_Output=4;
		}
		else
		{
			Voltage_Output=temperature*0.15-2;
		}
	}
	Da_Write(Voltage_Output*51);
	
	/*--Led--*/
	ucLed[0]=!Output_Mode;
	for(i=0;i<3;i++)
	{
		ucLed[i+1]=(i==Seg_Disp_Mode);
	}
	
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
}

void Delay750ms(void)	//@12.000MHz
{
	unsigned char data i, j, k;

	i = 35;
	j = 51;
	k = 182;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

/* Main */
void main()
{	rd_temperature();
	Delay750ms();
	System_Init();
	Timer0Init();
	
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
		
	}
}