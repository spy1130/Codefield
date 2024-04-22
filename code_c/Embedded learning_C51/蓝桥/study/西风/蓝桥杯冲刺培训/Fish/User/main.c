/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include <Uart.h>//串口底层驱动专用头文件
#include "onewire.h"//温度底层驱动专用头文件
#include "iic.h"//单总线底层驱动专用头文件
#include <stdio.h>//sprintf专用头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
idata unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
idata unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
idata unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char Uart_Slow_Down;//串口减速专用变量
unsigned char Uart_Recv[8];//串口接收数据储存数组 默认10个字节 若接收数据较长 可更改最大字节数
unsigned char Uart_Recv_Index;//串口接收数组指针
unsigned char Uart_Send[12];//串口接收数据储存数组 默认10个字节 若发送数据较长 可更改最大字节数
bit Seg_Disp_Mode;//数码管显示模式变量 0-温度显示 1-溶氧度显示
float Temperature;//实时温度采集变量
float Oxygen_Val;//实时溶氧度采集变量
bit System_Flag;//系统启动标志位 0-关闭 1-启动
bit Start_Flag;//自动喂食标志位 0-关闭 1-启动
unsigned int Timer_3000Ms;//三千毫秒计时变量

/* 键盘处理函数 */
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上降沿
	Key_Old = Key_Val;//辅助扫描变量

	if(Key_Down == 4) //系统启动暂停按键
		System_Flag ^= 1;
	
	if((Key_Down * System_Flag) == 5) //界面切换功能按键
		Seg_Disp_Mode ^= 1;
}

/* 信息处理函数 */
void Seg_Proc()
{	
	/* 信息获取区域 */
	switch(Seg_Slow_Down)
	{
		case 200://实时溶氧度读取
			Seg_Slow_Down += 1;
			Oxygen_Val = Ad_Read(0x43) / 51.0 * 2.0;
		break;
		case 400://实时温度读取
			Seg_Slow_Down += 1;
			Temperature = rd_temperature();
		break;
	}

	/* 数据显示区域 */
	Seg_Buf[0] = 11;//标识符U
	Seg_Buf[1] = (unsigned char)Seg_Disp_Mode + 1;
	if(Seg_Disp_Mode == 0) //温度显示界面
	{
		Seg_Buf[4] = 10;
		Seg_Buf[5] = (unsigned char)Temperature / 10 % 10;
		Seg_Buf[6] = (unsigned char)Temperature % 10;
		Seg_Buf[7] = (unsigned int)(Temperature * 10) % 10;
		Seg_Point[6] = 1;
		Seg_Point[5] = 0;
	}
	else //溶氧度显示界面
	{
		Seg_Buf[4] = (unsigned char)Oxygen_Val / 10 % 10?1:10;
		Seg_Buf[5] = (unsigned char)Oxygen_Val % 10;
		Seg_Buf[6] = (unsigned int)(Oxygen_Val * 100) / 10 % 10;	
		Seg_Buf[7] = (unsigned int)(Oxygen_Val * 100) % 10;			
		Seg_Point[6] = 0;
		Seg_Point[5] = 1;		
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	ucLed[0] = System_Flag;//启动功能知识灯
	ucLed[1] = !Seg_Disp_Mode;//温度显示指示灯
	ucLed[2] = Seg_Disp_Mode;//溶氧度显示指示灯
	ucLed[3] = Temperature > 26 || Temperature < 20;//温度指示灯
	ucLed[7] = Start_Flag;//喂食指示灯
	Relay(System_Flag&(Oxygen_Val<3));//供氧指示器
}

/* 串口处理函数 */
void Uart_Proc()
{
	if(Uart_Slow_Down) return;
	Uart_Slow_Down = 1;//串口减速程序	
	
	if(Uart_Recv_Index > 0) //接收到数据
	{
		if(System_Flag == 1) //系统处于启动状态
		{
			if(Uart_Recv_Index == 6) //Open\r\n
			{
				if(Uart_Recv[0] == 'O' && Uart_Recv[1] == 'p' && Uart_Recv[2] == 'e' && Uart_Recv[3] == 'n' && Uart_Recv[4] == '\r' && Uart_Recv[5] == '\n') //接受到Open命令
					Start_Flag = 1;//启动喂食
			}
			else if(Uart_Recv_Index == 1) // 1 或 2
			{
				if(Uart_Recv[0] == '1')
					sprintf(Uart_Send,"C:%.1f\r\n",Temperature);
				else if(Uart_Recv[0] == '2')
					sprintf(Uart_Send,"D:%.2fppm\r\n",Oxygen_Val);
				Uart_Send_String(Uart_Send);
			}
		}
		Uart_Recv_Index = 0; //索引值复位
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
	if(++Uart_Slow_Down == 200) Uart_Slow_Down = 0;//串口减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,System_Flag*Seg_Buf[Seg_Pos]+(!System_Flag)*10,Seg_Point[Seg_Pos]*System_Flag);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]*System_Flag);
	
	if(Start_Flag == 1) //开始喂食
	{
		if(++Timer_3000Ms == 3000) //三秒后
		{
			Timer_3000Ms = 0;
			Start_Flag = 0;//停止喂食
			Uart_Send_String("Over!\r\n");//发送串口指令
		}
	}
}

/* 串口1中断服务函数 */
void Uart1Server() interrupt 4
{
	if(RI == 1 && Uart_Recv_Index < 9) //串口接收数据
	{
		Uart_Recv[Uart_Recv_Index] = SBUF;
		Uart_Recv_Index++;
		RI = 0;
	}
	
	if(Uart_Recv_Index > 7) Uart_Recv_Index = 0;//清除读取指针
}

/* Main */
void main()
{
	System_Init();
	while(rd_temperature() == 85);//避免上电错误数据85.0
	Temperature = rd_temperature();//避免上电错误数据00.0
	Timer0Init();
	UartInit();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
		Uart_Proc();
	}
}