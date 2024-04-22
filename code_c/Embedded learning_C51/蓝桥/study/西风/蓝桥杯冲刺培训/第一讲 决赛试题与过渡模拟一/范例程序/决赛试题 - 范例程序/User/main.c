/* 头文件声明区 */
#include <REGX52.H>//单片机寄存器专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include <intrins.h>

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[6] = {10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[6] = {0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char Seg_Disp_Mode;//数码管显示模式变量 0-运行状态界面 1-流转时间设置界面 2-数据显示界面
bit System_Flag;//系统使能标志位 0-暂停 1-启动 上电默认处于暂停状态
unsigned char Led_Mode;//Led模式标志位
unsigned int Led_Time_Data[4] = {400,400,400,400};//Led流转时间数据存放数组
unsigned int Sys_Tick;//系统计时器专用变量
unsigned char ucLed = 0xfe;//Led数据存放数组
unsigned char ucLed_Data[4] = {0x7e,0xbd,0xdb,0xe7};//模式3、4数据存放数组
unsigned char ucLed_Data_Index;//模式3、4数组指针
bit Set_Flag;//设置标志位 0-模式编号 1-流转间隔
unsigned int Led_Time_Set[4];//Led流转数据设置数组
unsigned char Led_Time_Set_Index;//Led流转数据设置数组指针
bit Seg_Star_Flag;//数码管闪烁标志位
unsigned int Timer_400Ms;//400毫秒变量
bit Data_Disp_Flag;//数据显示标志位 0-不显示 1-显示
unsigned char Led_Data;//Led数据储存变量

/* 键盘处理函数 */
void Key_Proc()
{
	unsigned char i;//For循环专用变量
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Old = Key_Val;//辅助扫描变量
	
	if(System_Flag == 0)//系统处于暂停状态
	{
		if(Key_Old == 4 && Seg_Disp_Mode == 0)//在显示界面下长按S4
			Data_Disp_Flag = 1;//显示数据
		else
			Data_Disp_Flag = 0;//显示状态
	}
	else
		Data_Disp_Flag = 0;
	
	switch(Key_Down)
	{
		case 7://启动暂停按键
			System_Flag ^= 1;
		break;
		case 6://设置切换按键
			if(Seg_Disp_Mode == 0)//若在显示界面下使能此按键
			{
				for(i=0;i<4;i++)//读取当前设置数据
					Led_Time_Set[i] = Led_Time_Data[i];
				Seg_Disp_Mode = 1;//跳转到参数设置界面
			}
			else if(Seg_Disp_Mode == 1)//else-为了避免第一次按下后直接执行下列程序 所以需要另外加else使得在执行完if语句后跳出判断主体
			{
				Set_Flag ^= 1;//用于判断设置处于哪个状态的变量                        
				if(Set_Flag == 0)//第二次跳转到模式设置状态
				{
					for(i=0;i<4;i++)
						Led_Time_Data[i] = Led_Time_Set[i];//保存当前设置数据
					Seg_Disp_Mode = 0;
				}
			}
		break;
		case 5://参数+
			if(Seg_Disp_Mode == 1)//处于参数设置界面
			{
				if(Set_Flag == 0)//编号设置
				{
					if(++Led_Time_Set_Index == 4)
						Led_Time_Set_Index = 0;
				}
				else
				{
					Led_Time_Set[Led_Time_Set_Index] = Led_Time_Set[Led_Time_Set_Index]+100;
					if(Led_Time_Set[Led_Time_Set_Index] >= 1300)
						Led_Time_Set[Led_Time_Set_Index] = 400;
				}
			}
		break;
		case 4://参数-
			if(Seg_Disp_Mode == 1)//处于参数设置界面
			{
				if(Set_Flag == 0)//编号设置
				{
					if(--Led_Time_Set_Index == 255)
						Led_Time_Set_Index = 3;
				}
				else
				{
					Led_Time_Set[Led_Time_Set_Index] = Led_Time_Set[Led_Time_Set_Index]-100;
					if(Led_Time_Set[Led_Time_Set_Index] <= 300)
						Led_Time_Set[Led_Time_Set_Index] = 1200;
				}
			}
		break;		
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i = 2;//用于熄灭高位未启用数码管
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序

	switch(Seg_Disp_Mode)
	{
		case 0://运行状态显示界面
			if(Data_Disp_Flag == 0)
			{
				Seg_Buf[0] = System_Flag?11:12;//11-S 12-R
				Seg_Buf[1] = Led_Mode+1;
				Seg_Buf[2] = Led_Time_Data[Led_Mode] / 1000 % 10;
				Seg_Buf[3] = Led_Time_Data[Led_Mode] / 100 % 10;
				Seg_Buf[4] = Led_Time_Data[Led_Mode] / 10 % 10;
				Seg_Buf[5] = Led_Time_Data[Led_Mode] % 10;
				while(Seg_Buf[i] == 0)//使用循环体结构令高位为0的数码管熄灭 直到不为0时退出
				{
					Seg_Buf[i] = 10;
					i++;
				}
			}
			else
			{
				Seg_Buf[0] = 0;
				Seg_Buf[1] = 12;
				Seg_Buf[2] = 13;
				Seg_Buf[3] = Led_Mode+1;
				Seg_Buf[4] = Led_Data / 10;
				Seg_Buf[5] = Led_Data % 10;
			}
		break;
		case 1://流转时间设置界面
			Seg_Buf[0] = 13;
			Seg_Buf[1] = Led_Time_Set_Index+1;
			Seg_Buf[2] = Led_Time_Set[Led_Time_Set_Index] / 1000 % 10;
			Seg_Buf[3] = Led_Time_Set[Led_Time_Set_Index] / 100 % 10;
			Seg_Buf[4] = Led_Time_Set[Led_Time_Set_Index] / 10 % 10;
			Seg_Buf[5] = Led_Time_Set[Led_Time_Set_Index] % 10;
			if(Set_Flag == 0)
			{
				Seg_Buf[0] = Seg_Star_Flag?13:10;
				Seg_Buf[1] = Seg_Star_Flag?Led_Time_Set_Index+1:10;
			}
			else
			{
				Seg_Buf[2] = Seg_Star_Flag?Led_Time_Set[Led_Time_Set_Index] / 1000 % 10:10;
				Seg_Buf[3] = Seg_Star_Flag?Led_Time_Set[Led_Time_Set_Index] / 100 % 10:10;
				Seg_Buf[4] = Seg_Star_Flag?Led_Time_Set[Led_Time_Set_Index] / 10 % 10:10;
				Seg_Buf[5] = Seg_Star_Flag?Led_Time_Set[Led_Time_Set_Index] % 10:10;				
			}
			
			while(Seg_Buf[i] == 0)//使用循环体结构令高位为0的数码管熄灭 直到不为0时退出
			{
				Seg_Buf[i] = 10;
				i++;
			}		
		break;
	}
	Seg_Point[0] = Seg_Point[1] = Data_Disp_Flag&(!Data_Disp_Flag)?0:1;
}

/* 其他显示函数 */
void Led_Proc()
{
	unsigned char i;//用于循环判断
	P1 = ucLed;
	if(Sys_Tick == Led_Time_Data[Led_Mode])//当计时器走到当前模式设定的流转时间间隔时
	{
		Sys_Tick = 0;//计时器复位 便于下次循环
		switch(Led_Mode)//根据Led_Mode的不同 选择不同的彩灯运行模式
		{
			case 0:
				if(ucLed == 0x7e)//判断是否从模式4切换至模式1 若是，则复位ucLed数据
					ucLed = 0xfe;
				else
				{
					ucLed = _crol_(ucLed,1);//模式1 L1-L8
					if(ucLed == 0x7f) Led_Mode = 1;//当L8点亮时 跳转到模式2
				}
			break;
			case 1:
				ucLed = _cror_(ucLed,1);//模式2 L8-L1
				if(ucLed == 0xfe) Led_Mode = 2;//当L1点亮时 跳转到模式3
			break;
			case 2:
				ucLed = ucLed_Data[ucLed_Data_Index];//模式3 从外往内
				if(++ucLed_Data_Index == 4)//当指针溢出 即完成一次循环时 跳转到模式4
				{
					Led_Mode = 3;
					ucLed_Data_Index = 3;//指针复位至模式四初始数据
				}
			break;
			case 3:
				ucLed = ucLed_Data[ucLed_Data_Index];//模式4 从内往外
				if(--ucLed_Data_Index == 255)//当指针溢出 即完成一次循环时 跳转到模式1
				{	
					Led_Mode = 0;
					ucLed_Data_Index = 0;	//指针复位至模式三初始数据
				}
			break;
		}
	}
	/* 数据读取设计思路 */
	i = 0;//每次循环判断前将i置0，便于读取Led状态
	while((~ucLed & (0x01 << i)) != (0x01 << i))
		i++;
	if(Led_Mode < 2)//若处于前两种模式，则数据直接等于i+1
		Led_Data = i+1;
	else
		Led_Data = (i+1)*10+(8-(i+1));//若处于后两种模式，则高位数据等于i+1，根据对称变换，低位加高位的数值之和等于8
}

/* 定时器0中断初始化函数 */
void Timer0Init(void)		//1毫秒@12.000MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;    //定时器0中断打开
	EA = 1;     //总中断打开
}

/* 定时器0中断服务函数 */
void Timer0Server() interrupt 1
{
 	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值   
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 6) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	if(System_Flag == 1)//当系统启动时 计时器一毫秒自加一次	
		Sys_Tick++;
	if(++Timer_400Ms == 400)//400毫秒触发一次
	{
		Timer_400Ms = 0;
		Seg_Star_Flag ^= 1;
	}
}

/* Main */
void main()
{
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}