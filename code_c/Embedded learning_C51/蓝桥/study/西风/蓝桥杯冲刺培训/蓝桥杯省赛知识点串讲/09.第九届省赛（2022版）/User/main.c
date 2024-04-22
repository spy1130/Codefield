/* 头文件声明区开始 */
#include "init.h"//系统初始化

/* 三大金刚 */
#include "key.h"//键盘扫描
#include "seg.h"//数码管显示
#include "led.h"//Led显示

/* 其他外设 */
#include "iic.h"

/* 头文件声明区结束 */

/* 变量声明区开始 */
unsigned long int ms_tick;//滴答定时器专用
unsigned int Key_Slow_Down,Seg_Slow_Down,Led_Slow_Down;//三大金刚扫描减速专用
unsigned char Key_Old,Key_Val,Key_Up,Key_Down;//矩阵扫描专用
unsigned char Seg_Pos;//数码管显示专用
unsigned char Seg_String[8];//数码管数据存放专用
unsigned char ucLed;//LED显示专用

//用户自定义声明区
unsigned char Seg_Disp_Mode;//数码管显示模式 0-熄灭 1-运行模式闪烁 2-流转间隔闪烁
unsigned char Running_Mode = 1;//运行模式 1-4循环运行
unsigned int Circulation_Tm_Invl[4] = {400,400,400,400};//时间流转间隔数据存放数组
unsigned char Star[2] = {0x02,0xf0};//数码管闪烁模式数据存放数组
unsigned char EEPROM_String[4] = {4,4,4,4};//EEPROM数据存放数组
unsigned char Right_Level;//亮度等级
unsigned char Led_Disp_Flag;//LED显示标志位
unsigned char Led_Disp_Mode[4][8] = 
{
	0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,//第一种模式，从左到右
	0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,//第二种模式，从右到左
	0x81,0x42,0x24,0x18,0x81,0x42,0x24,0x18,//第三种模式，从两边到中间
	0x18,0x24,0x42,0x81,0x18,0x24,0x42,0x81 //第四种模式，从中间到两边
};
unsigned long int ms_tick_point;//打点计数专用
unsigned char Led_Pwm;//LED亮度等级控制专用
unsigned char Led_Num;//LED显示计数专用
bit Right_Level_Flag;//亮度等级显示标志位 0-不显示 1-显示
bit Star_Flag;//数码管闪烁时间标志位
bit Led_Start_Stop_Flag;//LED闪烁启动停止标志位 0-停止 1-启动
/* 变量声明区结束 */

/* 矩阵信息处理函数 */
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//减速专用
	
	Key_Val = Key_Read();
	Key_Down = Key_Val & (Key_Val ^ Key_Old);//通过下降沿检测按键
	Key_Up = ~ Key_Val & (Key_Val ^ Key_Old);//通过上升沿检测按键
	Key_Old = Key_Val;
	
	//用户自定义代码区
	if(Seg_Disp_Mode == 0)
	{
		if(Key_Old == 4)
			Right_Level_Flag = 1;
		else
			Right_Level_Flag = 0;
	}
	
	switch(Key_Down)
	{
		case 6://显示模式设置按键
			if(++Seg_Disp_Mode == 3)//显示模式在0-2之间循环切换.
			{
				EEPROM_String[0] = Circulation_Tm_Invl[0] / 100;
				EEPROM_String[1] = Circulation_Tm_Invl[1] / 100;
				EEPROM_String[2] = Circulation_Tm_Invl[2] / 100;
				EEPROM_String[3] = Circulation_Tm_Invl[3] / 100;//数据处理之后将流转时间存入EEPROM专用数组
				EEPROM_Write(EEPROM_String,16,4);
				Seg_Disp_Mode = 0;
			}
			break;
		case 5://数据+
			switch(Seg_Disp_Mode)
			{
				case 1://显示模式
					if(++Running_Mode == 5)
						Running_Mode = 4;//设置上限为4
					break;
				case 2://流转时间
					Circulation_Tm_Invl[Running_Mode - 1] += 100;
					if(Circulation_Tm_Invl[Running_Mode - 1] == 1300)
						Circulation_Tm_Invl[Running_Mode - 1] = 1200;//设置上限为1200
					break;
			}
			break;
		case 4://数据-
			switch(Seg_Disp_Mode)
			{
				case 1:
					if(--Running_Mode == 0)
						Running_Mode = 1;//设置下限为1
					break;
				case 2:
					Circulation_Tm_Invl[Running_Mode - 1] -= 100;
					if(Circulation_Tm_Invl[Running_Mode - 1] == 300)
						Circulation_Tm_Invl[Running_Mode - 1] = 400;//设置下限为400
					break;
			}
			break;
		case 7://启动停止
			if(Led_Start_Stop_Flag)
				ucLed = 0;
			else
				Running_Mode = 1;
			Led_Start_Stop_Flag ^= 1;
		  break;
	}
}

/* 数码管数据处理函数 */
void Seg_Proc()
{
	unsigned char i = 4;//清空数据专用
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//减速专用

	//用户自定义代码区
	Right_Level = (Ad_Read(0x43) / 64) + 1;//检测亮度等级
	
	if(Seg_Disp_Mode)//显示数据
	{
		Seg_String[0] = Seg_String[2] = 10;//显示 ——
		Seg_String[1] = Running_Mode;//显示运行模式
		Seg_String[3] = 11;//熄灭
		Seg_String[4] = Circulation_Tm_Invl[Running_Mode - 1] / 1000 % 10;//显示流转时间间隔
		Seg_String[5] = Circulation_Tm_Invl[Running_Mode - 1] / 100 % 10;
		Seg_String[6] = Circulation_Tm_Invl[Running_Mode - 1] / 10 % 10;
		Seg_String[7] = Circulation_Tm_Invl[Running_Mode - 1] % 10;
		while(Seg_String[i] == 0)//若流转时间为三位数 则最高位不显示
		{
			Seg_String[i] = 11;
			i--;
		}
	}
	else//不显示数据
	{
		Seg_String[0] = Seg_String[1] = Seg_String[2] = Seg_String[3] = Seg_String[4] = Seg_String[5] = 11;
		if(Right_Level_Flag)//显示亮度等级
		{
			Seg_String[6] = 10;
			Seg_String[7] = Right_Level;
		}
		else
			Seg_String[6] = Seg_String[7] = 11;
	}
}

/* Led显示函数 */
void Led_Proc()
{
//	if(Led_Slow_Down) return;
//	Led_Slow_Down = 1;//减速专用
	
	//用户自定义代码区
	if(Led_Start_Stop_Flag)
	{
		if((ms_tick - ms_tick_point) >= Circulation_Tm_Invl[Running_Mode - 1])
		{
			ms_tick_point = ms_tick;
			ucLed = Led_Disp_Mode[Running_Mode - 1][Led_Disp_Flag];
			if(++Led_Disp_Flag == 8)
			{
				Led_Disp_Flag = 0;
				if(++Running_Mode == 5)
					Running_Mode = 1;
			}
		}
	}
}

/* 定时器中断初始化函数 */
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;    //打开定时器0中断
	EA = 1;    //打开总中断
}

/* 定时器中断服务函数 */
void Timer0Sever() interrupt 1
{
	ms_tick++;//滴答定时器
	
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//10ms扫描一次
	if(++Seg_Slow_Down == 100) Seg_Slow_Down = 0;//200ms扫描一次
//	if(++Led_Slow_Down == 300) Led_Slow_Down = 0;//300ms扫描一次
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	
	if(!(ms_tick % 800))//800ms触发一次
	{
		Star_Flag ^= 1;
	}
	
//	Seg_Disp(Seg_String[Seg_Pos],Seg_Pos,0,0,0);//数码管显示
//	Led_Disp(ucLed);//LED显示
	
	//用户自定义代码区
	if(Seg_Disp_Mode)//显示数据
		Seg_Disp(Seg_String[Seg_Pos],Seg_Pos,0,Star[Seg_Disp_Mode - 1],Star_Flag);
	else//不显示数据
		Seg_Disp(Seg_String[Seg_Pos],Seg_Pos,0,0,0);
	
	switch(Right_Level)
	{
		case 1:Led_Pwm = 0;break;
		case 2:Led_Pwm = 3;break;
		case 3:Led_Pwm = 8;break;
		case 4:Led_Pwm = 11;break;
	}
	
	if(++Led_Num == 13) Led_Num = 0;//计数在0-12之间循环
	
	if(Led_Num <= Led_Pwm)
		Led_Disp(ucLed);
	else
		Led_Disp(0);
}

/* Main */
void main()
{
	Cls_Init();//关闭外设
	EEPROM_Read(EEPROM_String,16,4);//上电时读取数据
	Circulation_Tm_Invl[0] = EEPROM_String[0] * 100;
	Circulation_Tm_Invl[1] = EEPROM_String[1] * 100;
	Circulation_Tm_Invl[2] = EEPROM_String[2] * 100;
	Circulation_Tm_Invl[3] = EEPROM_String[3] * 100;	
	Timer0Init();//定时器0初始化
	while(1)
	{
		Key_Proc();//三大金刚循环扫描
		Seg_Proc();
		Led_Proc();
	}
}