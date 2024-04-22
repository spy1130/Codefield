/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include <iic.h>//数模转换底层驱动专属头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
bit Seg_Disp_Mode;//数码管显示模式变量 0-电压显示界面 1-电压输出界面
bit Output_Mode;//电压输出模式变量 0-固定2V 1-随读取值变化
bit Led_Enable_Flag = 1;//Led使能标志位 0-不使能 1-使能
bit Seg_Enable_Flag = 1;//数码管使能标志位 0-不使能 1-使能
float Voltage_Output;//实时输出电压值
float Voltage;//实时读取电压值

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
		case 4://界面切换按键
			Seg_Disp_Mode ^= 1;
		break;
		case 5://输出模式切换按键
			Output_Mode ^= 1;
		break;
		case 6://Led指示灯功能控制按键
			Led_Enable_Flag ^= 1;
		break;
		case 7://数码管显示功能控制按键
			Seg_Enable_Flag ^= 1;
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i;//用于For循环
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序

	/* 数据读取区域 */
	Voltage = AD_Read(0x43) / 51.0;//前面获取的只是一个0~255的数据，模拟输出就是以5V为参考电压,公式为模拟量=数字量*5/255读取实时电压值
	if(Output_Mode == 0)//固定输出2V
		Voltage_Output = 2;
	else
		Voltage_Output = Voltage;
	
	/* 信息显示区域 */
	Seg_Point[5] = 1;//使能第六位数码管小数点
	if(Seg_Disp_Mode == 0)//处于电压显示界面
	{
		Seg_Buf[0] = 11;//U
		Seg_Buf[5] = (unsigned char)Voltage;
		Seg_Buf[6] = (unsigned int)(Voltage * 100) / 10 % 10;
		Seg_Buf[7] = (unsigned int)(Voltage * 100) % 10;
	}
	else//处于电压输出界面
	{
		Seg_Buf[0] = 12;//F
		Seg_Buf[5] = (unsigned char)Voltage_Output;
		Seg_Buf[6] = (unsigned int)(Voltage_Output * 100) / 10 % 10;//这里用int是因为char只到255
		Seg_Buf[7] = (unsigned int)(Voltage_Output * 100) % 10;		
	}
	
}

/* 其他显示函数 */
void Led_Proc()
{
	unsigned char i;//用于For循环
	DA_Write(Voltage_Output*51);//实时输出电压值
	for(i=0;i<2;i++)//互斥点亮
		ucLed[i] = (i==Seg_Disp_Mode);
	if(Voltage < 1.5 || (Voltage >= 2.5 && Voltage < 3.5))
		ucLed[2] = 0;
	else
		ucLed[2] = 1;
	ucLed[3] = Output_Mode;
	if(Seg_Enable_Flag == 0) //Led不使能
	{
		for(i=0;i<8;i++)//熄灭所有Led
			ucLed[i] = 0;
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
	if(Seg_Enable_Flag == 1)
		Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	else
		Seg_Disp(Seg_Pos,10,0);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
}

/* Main */
void main()
{
	System_Init();
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}