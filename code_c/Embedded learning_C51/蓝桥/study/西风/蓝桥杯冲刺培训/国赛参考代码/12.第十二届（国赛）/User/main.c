/* 头文件声明区 */
#include "key.h" /* 键盘专用 */
#include "init.h" /* 初始化专用 */
#include "ds1302.h" /* 时钟专用 */							
#include "seg.h" /* 数码管专用 */
#include "ultrasound.h" /* 超声波专用 */
#include "led.h" /* LED专用 */
#include "iic.h" /* AD/DA专用 */

/* 变量声明区 */
unsigned char Num_Of_Data_Records;//数据记录次数变量
unsigned char Key_Slow_Down;//键盘减速变量 10ms
unsigned int Seg_Slow_Down;//信息处理减速变量 500ms
unsigned char Led_Slow_Down;//LED显示减速变量 200ms
unsigned char Key_Val,Key_Old,Key_Down;//键盘扫描变量
unsigned char Seg_Pos;//数码管循环扫描变量
unsigned char Seg_String[8];//数码管显示数据存放数组
unsigned char Seg_Point[8];//数码管小数点显示数据存放数组
unsigned char Seg_Disp_Mode;//数码管显示模式 0-时间数据显示 1-距离数据显示 2-数据记录显示 3-参数设置显示
unsigned char ucRtc[3] = {0x20,0x20,0x01};//时钟数据存放数组
unsigned int Data_Stg[3];//数据储存数组 0-最大值 1-平均值 2-最小值 
unsigned char Data_Stg_Index;//数据储存数组指针
unsigned char Param_Dat[2] = {02,20};//设置参数存放数组 0-采集时间 1-距离参数
unsigned char Param_Dat_Ctrl[2] = {02,20};//设置参数生效数组
unsigned char Time_Dat[5] = {2,3,5,7,9};//时间参数设置范围数组
unsigned char Time_Dat_Index;//时间参数数组指针
unsigned char Ultra_Dat;//超声波测量数据
unsigned char Photoresistance_Dat;//光敏电阻读取值
unsigned char ucLed[8];//LED显示数组
unsigned char Num_Alerts;//有效警报次数
float Avg_Val;//数据平均值
bit Param_Dat_Index;//设置参数数组指针
bit Ultra_Trigger_Mode = 1;//超声波触发模式 0-定时模式 1-触发模式  上电默认触发模式
bit Bright_Dark_Flag = 0;//亮暗标志位 0-暗 1-亮
bit Trigger_Enable_Flag;//触发使能标志位 0-不使能 1-使能
/* 键盘扫描函数 */
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序
	Key_Val = Key_Read();//读取键码值
	Key_Down = Key_Val & (Key_Val ^ Key_Old);//检测下降沿
	Key_Old = Key_Val;//扫描辅助变量
	switch(Key_Down)
	{
		case 4://数据显示-参数设置切换按键
			if(Seg_Disp_Mode == 3)//处于参数设置界面
			{
				Param_Dat_Index = 0;//指针复位
				Param_Dat_Ctrl[0] = Param_Dat[0];
				Param_Dat_Ctrl[1] = Param_Dat[1];//将设置参数赋值给控制参数
				Seg_Disp_Mode = 0;//切换到数据显示界面
			}
			else//处于数据显示界面
				Seg_Disp_Mode = 3;//切换到参数设置界面
		break;
		case 5://功能切换按键
			if(Seg_Disp_Mode != 3)//处于数据显示界面
			{
				if(Seg_Disp_Mode == 1)//进入数据显示界面
					Data_Stg_Index = 0;//指针复位
				if(++Seg_Disp_Mode == 3)//在0-2之间切换
					Seg_Disp_Mode = 0;
			}
//			else//处于参数设置界面
		break;
		case 8://模式切换按键
			if(Seg_Disp_Mode == 1)//处于距离数据显示界面
				Ultra_Trigger_Mode ^= 1;//切换模式
			if(Seg_Disp_Mode == 2)//处于数据记录显示界面
			{
				if(++Data_Stg_Index == 3)//在0-2之间切换
					Data_Stg_Index = 0;
			}
			if(Seg_Disp_Mode == 3)//处于参数设置界面
				Param_Dat_Index ^= 1;//切换通道
		break;
		case 9://参数调整按键
			if(Seg_Disp_Mode == 3)//处于参数设置界面
			{
				if(Param_Dat_Index == 0)//处于采集时间调整模式
				{
					if(++Time_Dat_Index == 5)
						Time_Dat_Index = 0;
					Param_Dat[Param_Dat_Index] = Time_Dat[Time_Dat_Index];
				}
				else//处于距离参数调整模式
				{
					Param_Dat[Param_Dat_Index] += 10;
					if(Param_Dat[Param_Dat_Index] == 90) Param_Dat[Param_Dat_Index] = 10;
				}
			}
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i = 5;//while循环专用变量
	static bit Enable_Flag = 1;//使能标志位，用于防止多次数据读取
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数据处理减速程序
	Read_Rtc(ucRtc);//将实时数据存放到ucRtc数组中
	Photoresistance_Dat = Ad_Read(0x41);//实时读取光敏电阻
	/* 超声波实时测距 */
	if(Ultra_Trigger_Mode == 0)//处于定时模式
	{
		if(((ucRtc[2]%16)%Param_Dat_Ctrl[0]) == 0)//触发一次超声波检测
		{
			if(Enable_Flag)
			{
				Ultra_Dat = Ultarsound_Read();//检测距离
				if(Ultra_Dat >= (Param_Dat_Ctrl[1]-5) && Ultra_Dat <= (Param_Dat_Ctrl[1]+5))//满足警报条件
				{
					if(Trigger_Enable_Flag == 1)//若上次处于警报状态，记录值加一
						Num_Alerts++;
					Trigger_Enable_Flag = 1;//拉高使能标志位
				}
				else
				{
					Trigger_Enable_Flag = 0;//拉低使能标志位
					Num_Alerts = 0;//记录值清零
				}
				if(Ultra_Dat > Data_Stg[0])//读取数值大于最大值
					Data_Stg[0] = Ultra_Dat;//更新最大值
				if((Ultra_Dat < Data_Stg[2]) || (Data_Stg[2] == 0))//读取数值小于最小值
					Data_Stg[2] = Ultra_Dat;//更新最小值
				Avg_Val = (Avg_Val * Num_Of_Data_Records + Ultra_Dat) / (Num_Of_Data_Records + 1);//计算平均值
				Num_Of_Data_Records++;
				Enable_Flag = 0;
			}
		}
		else
			Enable_Flag = 1;
	}
	else//处于触发模式
	{
		if(Photoresistance_Dat >= 20)//处于亮状态 拉高标志位
			Bright_Dark_Flag = 1;
		if(Photoresistance_Dat < 20 && Bright_Dark_Flag == 1)//满足触发条件
		{
			Ultra_Dat = Ultarsound_Read();//检测距离
			if(Ultra_Dat >= (Param_Dat_Ctrl[1]-5) && Ultra_Dat <= (Param_Dat_Ctrl[1]+5))//满足警报条件
			{
				if(Trigger_Enable_Flag == 1)//若上次处于警报状态，记录值加一
					Num_Alerts++;
				Trigger_Enable_Flag = 1;//拉高使能标志位
			}
			else
			{
				Trigger_Enable_Flag = 0;//拉低使能标志位
				Num_Alerts = 0;//记录值清零
			}
			if(Ultra_Dat > Data_Stg[0])//读取数值大于最大值
				Data_Stg[0] = Ultra_Dat;//更新最大值
			if((Ultra_Dat < Data_Stg[2]) || (Data_Stg[2] == 0))//读取数值小于最小值
				Data_Stg[2] = Ultra_Dat;//更新最小值
			Avg_Val = (Avg_Val * Num_Of_Data_Records + Ultra_Dat) / (Num_Of_Data_Records + 1);//计算平均值
			Num_Of_Data_Records++;
			Bright_Dark_Flag = 0;//拉低标志位			
		}
	}
	/* DA输出 */
	if(Ultra_Dat < 10)
		Da_Write(51);
	else if(Ultra_Dat > 80)
		Da_Write(255);
	else
		Da_Write((unsigned char)((Ultra_Dat - 10) * 2.91 + 51));
	switch(Seg_Disp_Mode)
	{
		case 0://时间数据显示
			Seg_Point[6] = 0;//关闭小数点显示
			/* 时间数据处理 */
			Seg_String[0] = ucRtc[0] / 16;
			Seg_String[1] = ucRtc[0] % 16;
			Seg_String[2] = 10;
			Seg_String[3] = ucRtc[1] / 16;
			Seg_String[4] = ucRtc[1] % 16;
			Seg_String[5] = 10;
			Seg_String[6] = ucRtc[2] / 16;
			Seg_String[7] = ucRtc[2] % 16;
		break;
		case 1://距离数据显示
			Seg_String[0] = 11;//L
			Seg_String[1] = 12 + (unsigned char)Ultra_Trigger_Mode;//触发模式相应标识符 0-F 1-C
			Seg_String[2] = Seg_String[3] = Seg_String[4] = 14;//熄灭
			/* 超声波测距数据处理 */
			Seg_String[5] = Ultra_Dat / 100 % 10;
			Seg_String[6] = Ultra_Dat / 10 % 10;
			Seg_String[7] = Ultra_Dat % 10;
			while(Seg_String[i] == 0)//熄灭未占用的高位数码管
			{
				Seg_String[i] = 14;
				i++;
			}
		break;
		case 2://数据记录显示
			Seg_String[0] = 15;//H
			Seg_String[1] = 16 + Data_Stg_Index;//显示格式相应标识符
			Seg_String[2] = Seg_String[3] = 14;//熄灭
			/* 显示格式数据处理 */
			if(Data_Stg_Index != 1)//最大最小值显示模式
			{
				Seg_Point[6] = 0;//关闭小数点显示
				Seg_String[4] = 14;
				Seg_String[5] = Data_Stg[Data_Stg_Index] / 100 % 10;
				Seg_String[6] = Data_Stg[Data_Stg_Index] / 10 % 10;
				Seg_String[7] = Data_Stg[Data_Stg_Index] % 10;
			}
			else//平均值显示模式
			{
				Seg_Point[6] = 1;//第七位显示小数点
				Seg_String[4] = 14;
				/* 平均值数据处理 */
				Seg_String[5] = (unsigned int)(Avg_Val * 10) / 100 % 10;
				Seg_String[6] = (unsigned int)(Avg_Val * 10) / 10 % 10;
				Seg_String[7] = (unsigned int)(Avg_Val * 10) % 10;
			}
			while(Seg_String[i] == 0)//熄灭未占用的高位数码管
			{
				Seg_String[i] = 14;
				i++;
			}
		break;
		case 3://参数设置显示
			Seg_Point[6] = 0;//关闭小数点显示
			Seg_String[0] = 19;//P
			Seg_String[1] = (unsigned char)Param_Dat_Index + 1;//通道编号
			Seg_String[2] = Seg_String[3] = Seg_String[4] = Seg_String[5] = 14;//熄灭
			/* 参数数据处理 */
			Seg_String[6] = Param_Dat[Param_Dat_Index] / 10 % 10;
			Seg_String[7] = Param_Dat[Param_Dat_Index] % 10;
		break;
	}
	
}

/* 其他显示函数 */
void Led_Proc()
{
	if(Led_Slow_Down) return;
	Led_Slow_Down = 1;//LED显示减速程序
	ucLed[0] = ucLed[1] = ucLed[2] = ucLed[3] = 0;//互斥点亮
	ucLed[Seg_Disp_Mode] = 1;//模式指示灯
	ucLed[4] = Num_Alerts / 3;//警报指示灯
	ucLed[5] = Bright_Dark_Flag;//环境亮度指示灯
}

/* 定时器1初始化函数 */
void Timer1Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x18;		//设置定时初始值
	TH1 = 0xFC;		//设置定时初始值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;    //定时器1中断打开
	EA = 1;    //总中断打开
}

/* 定时器1中断服务函数 */
void Timer1Server() interrupt 3
{
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//信息处理减速专用
	if(++Led_Slow_Down == 200) Led_Slow_Down = 0;//LED显示减速专用
	Seg_Disp(Seg_String[Seg_Pos],Seg_Pos,Seg_Point[Seg_Pos]);//数码管显示
	Led_Disp(Seg_Pos+1,ucLed[Seg_Pos]);//LED显示
}

/* Main */
void main()
{
	Cls_Init();//关闭外设
	Set_Rtc(ucRtc);//设置时钟
	Timer0Init();//定时器0初始化
	Timer1Init();//定时器1初始化
	while(1)//三大主体程序循环扫描
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}