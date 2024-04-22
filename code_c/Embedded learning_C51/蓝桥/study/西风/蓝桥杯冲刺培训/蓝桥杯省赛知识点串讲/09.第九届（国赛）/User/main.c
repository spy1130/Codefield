/* 头文件申明区 */
#include "key.h"
#include "init.h"
#include "seg.h"
#include "iic.h"
#include "led.h"
#include "onewire.h"
/* 变量申明区 */
unsigned long int Ms_Tick;//计时器
unsigned long int Ms_Key_Tick;//判断按键长短按
unsigned long int Ms_L8_Tick;//L8闪烁专用
unsigned int Freq;//频率测量值
unsigned int Key_Slow_Down,Seg_Slow_Down;
unsigned char Key_Val,Key_Old,Key_Down,Key_Up;
unsigned char ucLed;
unsigned char Seg_Pos;
unsigned char Seg_String[8];//数码管显示数据存放数组
unsigned char Seg_Point[8];//数码管小数点显示数组
unsigned char Seg_Disp_Mode;//数码管显示模式 0-电压 1-频率 2-温度 3-数据回显 4-阈值设置
unsigned char EEPROM_String[5];//回显数据储存数组
unsigned int Old_Data_String[3];//回显显示数据存放数组
unsigned char EEPROM_String_Index;//回显数组指针
unsigned char Voltage_Val = 1;//系统显示阈值
unsigned char Voltage_Val_Ctrl = 1;//系统控制阈值
float Voltage;//电压测量值
float Temperature;//温度测量值
/* 键盘处理函数 */
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘处理减速程序
	Key_Val = Key_Read();//读取键码值
	Key_Down = Key_Val & (Key_Val ^ Key_Old);//捕捉下降沿
	Key_Up = ~ Key_Val & (Key_Val ^ Key_Old);//捕捉上升沿
	Key_Old = Key_Val;
	switch(Key_Down)
	{
		case 4://界面切换
			if(Seg_Disp_Mode == 3)//处于回显状态
			{
				if(++EEPROM_String_Index == 3)
					EEPROM_String_Index = 0;
			}
			else
			{
				if(++Seg_Disp_Mode == 3)
					Seg_Disp_Mode = 0;
			}
		break;
		case 5://储存数据
			EEPROM_String[0] = (unsigned char)(Voltage * 10);
			EEPROM_String[1] = Freq >> 8;//储存高八位
			EEPROM_String[2] = Freq & 0x00FF;//储存低八位
			EEPROM_String[3] = (unsigned int)(Temperature * 100) >> 8;//储存温度整数部分
			EEPROM_String[4] = (unsigned int)(Temperature * 100) & 0x00FF;//储存温度小数部分
			EEPROM_Write(EEPROM_String,0,5);//将数据写入EEPROM中
		break;
		case 6://回显切换
			if(Seg_Disp_Mode != 4)//处于非阈值设置状态
			{
				EEPROM_Read(EEPROM_String,0,5);//将EEPROM内的数据读取出来
				Old_Data_String[0] = EEPROM_String[0];
				Old_Data_String[1] = EEPROM_String[1] << 8 | EEPROM_String[2];
				Old_Data_String[2] = EEPROM_String[3] << 8 | EEPROM_String[4];
				Seg_Disp_Mode = 3;//切换到回显显示界面
			}
		break;
		case 7://阈值设置界面
			if(Seg_Disp_Mode == 4)//处于设置界面
			{
				Voltage_Val_Ctrl = Voltage_Val;//将显示参数赋值给控制变量
				Seg_Disp_Mode = 0;//切换到数据显示模式
			}
			else
				Seg_Disp_Mode = 4;//进入阈值设置界面
		break;
	}
	if(Seg_Disp_Mode == 4)//处于阈值设置状态
	{
		if(Key_Down == 6)
			Ms_Key_Tick = Ms_Tick;//记录按下按键的时间
	}
	if((Ms_Tick - Ms_Key_Tick) <= 800)//按下时间小于800ms，即为短按
	{
		if(Key_Up == 6)//产生上升沿，表示松手
		{
			if(++Voltage_Val == 51)
				Voltage_Val = 1;//设置上限
		}
	}
	else//按下时间大于800ms，即为长按
	{
		if(Key_Old == 6)//判断是否处于按下状态
		{
			if(++Voltage_Val == 51)
				Voltage_Val = 1;//设置上限			
		}
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i;//循环专用变量
	unsigned char j = 3;//清空数据专用
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//信息处理减速程序	
	Voltage = Ad_Read(0x43) / 51.0;//读取电压值
	Temperature = Ds18b20_Read();//读取温度值
	Seg_String[0] = 12 + Seg_Disp_Mode;
	switch(Seg_Disp_Mode)
	{
		case 0://电压显示
			Seg_Point[6] = 1;//第七位数码管显示小数点
			Seg_Point[5] = 0;//关闭第六位小数点
			for(i=1;i<6;i++)
				Seg_String[i] = 11;//熄灭未启用数码管
			Seg_String[6] = (unsigned char)Voltage;//读取电压值个位
			Seg_String[7] = (unsigned char)(Voltage * 10) % 10;//读取电压值十分位		
		break;
		case 1://频率显示	
			Seg_Point[6] = 0;//关闭小数点显示模式
			Seg_String[3] = Freq / 10000 % 10;//将频率值每一位提取出来
			Seg_String[4] = Freq / 1000 % 10;
			Seg_String[5] = Freq / 100 % 10;
			Seg_String[6] = Freq / 10 % 10;
			Seg_String[7] = Freq % 10;			
			while(Seg_String[j] == 0)//屏蔽高位为0的数据
			{
				Seg_String[j] = 11;
				j++;
			}
		break;
		case 2://温度显示
			Seg_Point[5] = 1;//第六位数码管显示小数点
			for(i=1;i<4;i++)
				Seg_String[i] = 11;//熄灭未启用数码管	
			Seg_String[4] = (unsigned char)Temperature / 10 % 10;//取出温度十位
			Seg_String[5] = (unsigned char)Temperature % 10;//取出温度个位
			Seg_String[6] = (unsigned int)(Temperature * 100) / 10 % 10;//取出温度十分位
			Seg_String[7] = (unsigned int)(Temperature * 100) % 10;//取出温度十分位
		break;
		case 3://回显数据显示
			Seg_String[1] = 12 + EEPROM_String_Index;
			Seg_String[2] = 11;//熄灭
			switch(EEPROM_String_Index)
			{
				case 0://电压回显
					Seg_Point[6] = 1;//第七位数码管显示小数点
					Seg_Point[5] = 0;//关闭第六位小数点
					for(i=2;i<6;i++)
						Seg_String[i] = 11;//熄灭未启用数码管
					Seg_String[6] = Old_Data_String[EEPROM_String_Index] / 10 % 10;//读取电压值个位
					Seg_String[7] = Old_Data_String[EEPROM_String_Index] % 10;//读取电压值十分位							
				break;
				case 1://频率回显
					Seg_Point[6] = 0;//关闭小数点显示模式
					Seg_String[3] = Old_Data_String[EEPROM_String_Index] / 10000 % 10;//将频率值每一位提取出来
					Seg_String[4] = Old_Data_String[EEPROM_String_Index] / 1000 % 10;
					Seg_String[5] = Old_Data_String[EEPROM_String_Index] / 100 % 10;
					Seg_String[6] = Old_Data_String[EEPROM_String_Index] / 10 % 10;
					Seg_String[7] = Old_Data_String[EEPROM_String_Index] % 10;			
					while(Seg_String[j] == 0)//屏蔽高位为0的数据
					{
						Seg_String[j] = 11;
						j++;
					}					
				break;
				case 2://温度回显
					Seg_Point[5] = 1;//第六位数码管显示小数点
					for(i=2;i<4;i++)
						Seg_String[i] = 11;//熄灭未启用数码管	
					Seg_String[4] = Old_Data_String[EEPROM_String_Index] / 1000 % 10;//取出温度十位
					Seg_String[5] = Old_Data_String[EEPROM_String_Index] / 100 % 10;//取出温度个位
					Seg_String[6] = Old_Data_String[EEPROM_String_Index] / 10 % 10;//取出温度十分位
					Seg_String[7] = Old_Data_String[EEPROM_String_Index] % 10;//取出温度十分位					
				break;
			}
		break;
		case 4://阈值设置
			Seg_Point[6] = 1;
			for(i=1;i<6;i++)
				Seg_String[i] = 11;//熄灭未启用数码管
				Seg_String[6] = Voltage_Val / 10 % 10;
				Seg_String[7] = Voltage_Val % 10;
		break;	
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	if(Seg_Disp_Mode == 2)//温度测量状态
		ucLed |= 0x01;
	else
		ucLed &= ~0x07;
	if(Seg_Disp_Mode == 1)//频率测量状态
		ucLed |= 0x02;
	else
		ucLed &= ~0x07;
	if(Seg_Disp_Mode == 0)//电压测量状态
		ucLed |= 0x04;
	else
		ucLed |= 0x02;
	if(Seg_Disp_Mode >= 3)//退出数据显示界面
		ucLed &= ~0x07;
	if((Voltage*10) > Voltage_Val_Ctrl)
	{
		if((Ms_Tick - Ms_L8_Tick) >= 200)
		{
			Ms_L8_Tick = Ms_Tick;
			ucLed ^= 0x80;
		}
	}
	else
		ucLed &= ~0x80;
}

/* 定时器1初始化函数 */
void Timer1Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x18;		//设置定时初值
	TH1 = 0xFC;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;    //打开定时器1中断
	EA = 1;     //打开总中断
}

/* 定时器1中断服务函数 */
void Timer1Server() interrupt 3
{
	Ms_Tick++;
	if(++Seg_Pos == 8) Seg_Pos = 0;
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
	if(++Seg_Slow_Down == 300) Seg_Slow_Down = 0;
	if(!(Ms_Tick % 1000))//频率测量
	{
		Freq = TH0 << 8 | TL0;
		TH0 = TL0 = 0;
	}
	Seg_Disp(Seg_String[Seg_Pos],Seg_Pos,Seg_Point[Seg_Pos]);//数码管显示
	Led_Disp(ucLed);
	
}

/* 频率测量初始化函数 */
void Timer0Init(void)
{
	TMOD &= 0xf0;
	TMOD |= 0x05;
	TH0 = 0;
	TL0 = 0;
	TF0 = 0;
	TR0 = 1;
}

/* Main */
void main()
{
	Cls_Init();//外设初始化
	Ad_Read(0x43);//上电读取数据防止出现2.5V
	Timer0Init();//频率测量计数器初始化
	Timer1Init();//定时器初始化
	while(1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}
