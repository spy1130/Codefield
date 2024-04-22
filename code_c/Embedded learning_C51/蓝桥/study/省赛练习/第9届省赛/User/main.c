/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include <stdio.h>//标准库底层驱动专用头文件
#include "iic.h"

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned int T_Disp[4]={400,400,400,400};//流转间隔数据显示数组400~1200ms
unsigned int T_Ctrl[4]={400,400,400,400};//流转间隔数据控制数组400~1200ms(保存运行变量)
unsigned char T_Index;//流转间隔指针-运行模式编号
bit Seg_Disp_Mode;//0-设置，1-亮度显示
unsigned int Ms_Tick;//系统计时变量
bit Start_Flag;//流转启动标志位 0-暂停 1-启动
unsigned char Led_Mode;//系统流转模式变量
unsigned char eeprom_dat[4];//存储变量
unsigned int time_400ms;//数码管闪烁时间位
bit time_flag;//数码管闪烁标志位
unsigned char Seg_Index;//数码管索引值 0-熄灭 1-模式 2-流转间隔
unsigned char Led_Pos;//led索引
unsigned char Led_Level;//Led亮度等级变量
unsigned char Led_Count;//Led亮度计数值

/* 键盘处理函数 */
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上降沿
	Key_Old = Key_Val;//辅助扫描变量
	if(Seg_Index==0)//如果熄灭
	{
		if(Key_Old==4){
			Seg_Disp_Mode=1; Seg_Slow_Down=0;}
		else
		{
			Seg_Disp_Mode=0;
		}
	}
	switch(Key_Down)
	{	unsigned char i;
		case 6:
			if(++Seg_Index==3)Seg_Index =0;
		
			if(Seg_Index==0)
			{	T_Index=0;//复位，不知道该不该有
				for(i=0;i<4;i++){
				T_Ctrl[i]=T_Disp[i];//保存
				eeprom_dat[i]=T_Ctrl[i]/100;//除100后这样就都小于255了
				}
				EEPROM_Write(eeprom_dat,0,4);
			}
			
		break;
		case 7://启动按键
		Start_Flag ^= 1;
		break;
		case 5://+
			if(Seg_Index==1)
			{
				if(++T_Index==4)
				{
					T_Index=0;
				}
			}
			else if(Seg_Index==2)
			{
				T_Disp[T_Index]+=100;
				if(T_Disp[T_Index]>1200)
					T_Disp[T_Index]=1200;
			}
		break;
		case 4:
			if(Seg_Index==1)
			{
				if(--T_Index==255)
				{
					T_Index=3;
				}
			}
			else if(Seg_Index==2)
			{
				T_Disp[T_Index]-=100;
				if(T_Disp[T_Index]<400)
					T_Disp[T_Index]=400;
			}
		break;
	}

}

/* 信息处理函数 */
void Seg_Proc()
{	unsigned char i;
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序
	
	Led_Level=Ad_Read(0x03)/64;//0~255,256个数/64=4,程序是255/4=3。模式为0，1，2，3
	if(Seg_Disp_Mode==0)//参数设置界面
	{
		Seg_Buf[0]=11;
		Seg_Buf[1]=T_Index+1;
		Seg_Buf[2]=11;
		Seg_Buf[4]=T_Disp[T_Index]/1000%10?T_Disp[T_Index]:10;
		Seg_Buf[5]=T_Disp[T_Index]/100%10;
		Seg_Buf[6]=T_Disp[T_Index]/10%10;
		Seg_Buf[7]=T_Disp[T_Index]%10;
		if(Seg_Index==1)
		{
			Seg_Buf[0]=Seg_Buf[2]=time_flag?10:11;
			Seg_Buf[1]=time_flag?10:T_Index+1;	
		}
		else if(Seg_Index ==2)
		{
			Seg_Buf[4]=time_flag?10:T_Disp[T_Index] /1000 % 10;
			Seg_Buf[5]=time_flag?10:T_Disp[T_Index]/100%10;
			Seg_Buf[6]=time_flag?10:T_Disp[T_Index]/10%10;
			Seg_Buf[7]=time_flag?10:T_Disp[T_Index]%10;
		}
		if(Seg_Buf[4] == 0) Seg_Buf[4] = 10;
	}
	else//亮灯等级界面
	{
		for(i=0;i<6;i++)
			Seg_Buf[i]=10;
			Seg_Buf[6]=11;
			Seg_Buf[7]=Led_Level+1;
	}

}

/* 其他显示函数 */
void Led_Proc()
{	unsigned char i;
	if(Start_Flag == 1) //系统处于启动状态
	{
		if(Ms_Tick ==T_Ctrl[T_Index]) //系统计时时间达到流转时间间隔
		{                         
			Ms_Tick = 0;//复位计时 便于下次进入
			switch(Led_Mode)
			{
				case 0:
					if(++Led_Pos ==8)
					{
						Led_Pos=7;
						Led_Mode=1;
					}
				break;
				case 1:
					if(--Led_Pos ==255)
					{	
						Led_Pos = 7;//模式3起始值
						Led_Mode=2;
					}
				break;
				case 2://07+9=16+9=25+9=34
					Led_Pos+=9;
					if(Led_Pos >34)
					{
						Led_Pos =34;
						Led_Mode=3;
					}
				break;
				case 3:
					Led_Pos-=9;
					if(Led_Pos >200)//07-9=-2对u8来说是255-2=253
					{
						Led_Pos =0;
						Led_Mode=0;
					}
				break;
			}
		}
	}
	if(Led_Mode<2)
	{
	for(i=0;i<8;i++)
	{
		ucLed[i]=(i==Led_Pos);//互斥输出
	}
	}
	else
	{	for(i=0;i<8;i++)
		ucLed[i]=((i==(Led_Pos/10))||(i==(Led_Pos%10)));//拆分数07，16.....
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
	if(++Led_Count==12)Led_Count=0;//
	if(Seg_Index !=0||Seg_Disp_Mode==1)
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	else Seg_Disp(Seg_Pos,10,0);
	if(Led_Count <=((Led_Level+1)*3))
		Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	else Led_Disp(Seg_Pos,0);
	if(++time_400ms==400)
	{
		time_400ms=0;
		time_flag ^=1;
	}
	if(Start_Flag == 1) //系统启动时开始计时
		Ms_Tick++;
}



/* Main */
void main()
{	unsigned char i;
	System_Init();
	Timer0Init();
	EEPROM_Read(eeprom_dat,0,4);
	for(i=0;i<4;i++)
	{
		T_Disp[i]=T_Ctrl[i]=eeprom_dat[i]*100;//除100后这样就都小于255了
	}
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
		
	}
}