/* 头文件声明区 */
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <Init.h>//初始化底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include "iic.h"//单总线底层驱动专用头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
bit Seg_Disp_Mode;//数码管显示模式变量 0-设置界面 1-亮度等级界面
unsigned int Led_Time_Disp[4] = {400,400,400,400};//Led流转时间间隔显示数组
unsigned int Led_Time_Ctrol[4] = {400,400,400,400};//Led流转时间间隔控制数组
unsigned char Led_Time_Index;//Led流转数组指针
unsigned char Seg_Star_Index;//数码管闪烁指针 0-熄灭 1-运行模式编号 2-流转时间间隔
unsigned int Timer_400Ms;//400毫秒计时变量
bit Seg_Star_Flag;//数码管闪烁标志位
unsigned int Ms_Tick;//系统计时变量
bit Start_Flag;//流转启动标志位 0-暂停 1-启动
unsigned char Led_Mode;//系统流转模式变量
unsigned char Led_Pos;//Led流转专用变量
unsigned char EEPROM_Dat[4];//EEPROM专用数组
unsigned char Led_Level;//Led亮度等级变量
unsigned char Led_Count;//Led计数变量
unsigned char Pos_125us;//125微妙扫描变量

/* 键盘处理函数 */
void Key_Proc()
{
	unsigned char i;//For循环专用变量
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上降沿
	Key_Old = Key_Val;//辅助扫描变量

	if(Seg_Star_Index == 0) //处于非设置状态
	{
		if(Key_Old == 4) //长按S4
			Seg_Disp_Mode = 1;//切换到亮度等级显示界面
		else
			Seg_Disp_Mode = 0;//切换回数据显示界面
	}
	
	switch(Key_Down)
	{
		case 6://设置按键
			if(++Seg_Star_Index == 3)
				Seg_Star_Index = 0;
			if(Seg_Star_Index == 0) //退出设置界面
			{
				Led_Time_Index = 0;//复位指针值
				for(i=0;i<4;i++)
				{
					Led_Time_Ctrol[i] = Led_Time_Disp[i];//保存设置参数
					EEPROM_Dat[i] = Led_Time_Ctrol[i] / 100;//EEPROM数据处理
				}
				EEPROM_Write(EEPROM_Dat,0,4);//将数据保存到EEPROM中
			}
		break;
		case 7://启动按键
			Start_Flag ^= 1;
		break;
		case 5://自加按键
			if(Seg_Star_Index == 1) //选中参数编号
			{
				if(++Led_Time_Index == 4) //四种模式循环切换
					Led_Time_Index = 0;
			}
			else if(Seg_Star_Index == 2) //选中流转间隔
			{
				Led_Time_Disp[Led_Time_Index] += 100;
				if(Led_Time_Disp[Led_Time_Index] > 1200) //限制上限为1200
					Led_Time_Disp[Led_Time_Index] = 1200;
			}
		break;
		case 4://自减按键
			if(Seg_Star_Index == 1) //选中参数编号
			{
				if(--Led_Time_Index == 255) //四种模式循环切换
					Led_Time_Index = 3;
			}
			else if(Seg_Star_Index == 2) //选中流转间隔
			{
				Led_Time_Disp[Led_Time_Index] -= 100;
				if(Led_Time_Disp[Led_Time_Index] < 400) //限制下限为400
					Led_Time_Disp[Led_Time_Index] = 400;
			}
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i;//For循环专用变量
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序

	Led_Level = Ad_Read(0x03) / 64;//实时获取亮度等级
	
	if(Seg_Disp_Mode == 0) //处于设置界面
	{
		Seg_Buf[0] = Seg_Buf[2] = 11;
		Seg_Buf[1] = Led_Time_Index+1;
		Seg_Buf[4] = Led_Time_Disp[Led_Time_Index] / 1000 % 10;
		Seg_Buf[5] = Led_Time_Disp[Led_Time_Index] / 100 % 10;
		Seg_Buf[6] = Led_Time_Disp[Led_Time_Index] / 10 % 10;
		Seg_Buf[7] = Led_Time_Disp[Led_Time_Index] % 10;
		if(Seg_Star_Index == 0) //熄灭
		{
			for(i=0;i<8;i++)
				Seg_Buf[i] = 10;
		}
		else if(Seg_Star_Index == 1) //运行编号闪烁
		{
			Seg_Buf[0] = Seg_Buf[2] = Seg_Star_Flag?10:11;
			Seg_Buf[1] = Seg_Star_Flag?10:Led_Time_Index+1;			
		}
		else //流转时间间隔闪烁
		{
			Seg_Buf[4] = Seg_Star_Flag?10:Led_Time_Disp[Led_Time_Index] / 1000 % 10;
			Seg_Buf[5] = Seg_Star_Flag?10:Led_Time_Disp[Led_Time_Index] / 100 % 10;
			Seg_Buf[6] = Seg_Star_Flag?10:Led_Time_Disp[Led_Time_Index] / 10 % 10;
			Seg_Buf[7] = Seg_Star_Flag?10:Led_Time_Disp[Led_Time_Index] % 10;			
		}
		if(Seg_Buf[4] == 0) Seg_Buf[4] = 10;//只有最高位需要考虑是否高位熄灭
	}
	else//处于亮度等级界面
	{
		Seg_Buf[6] = 11;
		Seg_Buf[7] = Led_Level+1;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	unsigned char i;//For循环专用变量
	if(Start_Flag == 1) //系统处于启动状态
	{
		if(Ms_Tick == Led_Time_Ctrol[Led_Mode]) //系统计时时间达到流转时间间隔
		{
			Ms_Tick = 0;//复位计时 便于下次进入
			switch(Led_Mode)
			{
				case 0://模式1-从L1到L8
					if(++Led_Pos == 8)
					{
						Led_Pos = 7;//模式2起始值
						Led_Mode = 1;//切换到模式2
					}
				break;
				case 1://模式2-从L8到L1
					if(--Led_Pos == 255)
					{
						Led_Pos = 07;//模式3起始值
						Led_Mode = 2;//切换到模式3
					}
				break;
				case 2://模式3-07 16 25 34
					Led_Pos += 9;
					if(Led_Pos > 34)
					{
						Led_Pos = 34;//模式4起始值
						Led_Mode = 3;//切换到模式4
					}
				break;
				case 3://模式4-34 25 16 07
					Led_Pos -= 9;
					if(Led_Pos > 200)
					{
						Led_Pos = 0;//模式1起始值
						Led_Mode = 0;//切换到模式1
					}
				break;
			}
		}
	}
	
//	if(Led_Mode <= 1) //系统处于前两种流转模式时
//	{
//		for(i=0;i<8;i++)
//		 ucLed[i] = (i == Led_Pos);
//	}
//	else //系统处于后两种流转模式时
//	{
//		for(i=0;i<8;i++)
//			ucLed[i] = ((i == (Led_Pos / 10)) || (i == (Led_Pos % 10))); //点亮十位跟个位所对应的Led
//	}
	
	//两种情况可合并简化成一种 只需要屏蔽掉在前两种模式0X的时候 十位也会满足逻辑为真的情况
	for(i=0;i<8;i++)
		ucLed[i] = ((i == (Led_Pos / 10) & (Led_Mode / 2)) || (i == (Led_Pos % 10)));
}

/* 定时器1中断初始化函数 */
void Timer1Init(void)		//125微秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x83;		//设置定时初始值
	TH1 = 0xFF;		//设置定时初始值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;		//定时器中断1打开
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

/* 定时器1中断服务函数 */
void Timer1Server() interrupt 3
{	
	if(++Pos_125us == 8) Pos_125us = 0;//一个完整的扫描周期为1毫秒
	if(Led_Count <= (Led_Level*4)) //12毫秒的周期中均分四个亮度等级 0 4 8 12
		Led_Disp(Pos_125us,ucLed[Pos_125us]);
	else
		Led_Disp(Pos_125us,0);
	/* 
		Led采用的动态扫描的思路 即需要扫描8次才算一个完整的显示周期
		如果继续放在定时器0中断内扫描的话 需要8毫秒才能实现一次扫描
		这样的话 会因为扫描周期较长 无法实现亮度等级的变化（会因为周期长而看起来闪烁 而不是亮度变暗）
		所以需要生成一个125微妙的定时器中断 这样实现一个完整的扫描周期需要1毫秒
	*/
}

/* 定时器0中断服务函数 */
void Timer0Server() interrupt 1
{  
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	if(++Led_Count == 13) Led_Count = 0;//Led一个显示周期为12毫秒

	if(++Timer_400Ms == 400) //四百毫秒取反一次
	{
		Timer_400Ms = 0;
		Seg_Star_Flag ^= 1;
	}
	
	if(Start_Flag == 1) //系统启动时开始计时
		Ms_Tick++;
	
	Seg_Proc();
	/* 
		如果放在While内循环扫描 定时器1中断触发频率较高
		程序没有执行完就会被中断打断 所以会出现数码管数据会出现异常闪烁的情况
		所以需要将此函数放到中断里面执行 保证函数不会被中断给打断 能完整执行
	*/
}

/* Main */
void main()
{
	unsigned char i;//For循环专用变量
	EEPROM_Read(EEPROM_Dat,0,4);//读取EEPROM数据
	for(i=0;i<4;i++)
	 Led_Time_Ctrol[i] = Led_Time_Disp[i] = EEPROM_Dat[i] * 100;//数据处理
	System_Init();
	Timer0Init();
	Timer1Init();
	while (1)
	{
		Key_Proc();
		//Seg_Proc();
		Led_Proc();
	}
}