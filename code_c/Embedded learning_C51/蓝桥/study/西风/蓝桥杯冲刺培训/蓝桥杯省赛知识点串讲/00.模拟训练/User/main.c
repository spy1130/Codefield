/* 头文件声明区 */
#include <REGX52.H>//单片机寄存器专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include <Led.h>//Led底层驱动专用头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[6] = {10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned char Seg_Point[6] = {0,0,0,0,0,0};//数码管小数点存放数组
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char Led_Pos;//Led扫描专用变量
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led显示数据存放数组
unsigned char Seg_Disp_Mode;//数码管显示模式 0-温度采集界面 1-数据显示界面 2-参数设置界面
unsigned char Seg_Input[3] = {11,11,11};//数码管输入数据存放数组
unsigned char Seg_Input_Index;//数码管输入数组指针
unsigned int Timer_500Ms;//五百毫秒计时变量
unsigned char Point_Wela;//小数点输入位数记录变量
unsigned char Parameter[2] = {30,20};//参数设置数组
unsigned char Parameter_Ctrol[2] = {30,20};//实际参数控制数组
unsigned int Count_500Ms;//五百毫秒计数变量
unsigned char Led_Num;//Led亮度等级计数变量
unsigned char Led_Pwm;//Led亮度等级
float Temperature;//实际温度变量
bit Seg_Star_Flag;//数码管闪烁标志位
bit Point_Flag;//小数点使能标志位
bit Parameter_Index;//参数设置数组指针
bit Time_Flag;//计时启动标志位
bit Error_Flag;//错误数据标志位

/* 键盘处理函数 */
void Key_Proc()
{
	unsigned char i;//For循环专用变量
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上升沿
	Key_Old = Key_Val;//辅助扫描变量
	
	if(Key_Down >= 1 && Key_Down <= 10 && Seg_Input_Index <= 2)//矩阵输入键盘使能
	{
		Seg_Input[Seg_Input_Index] = Key_Down - 1;//将输入数值给输入数组
		Seg_Input_Index++;//数组指针加一
	}
	
	if(Seg_Disp_Mode == 2)//处于参数设置界面
	{
		if(Key_Down == 14)//S14按下
			Time_Flag = 1;//计时开始
	}
	if(Count_500Ms < 500)//短按
	{
		if(Key_Up == 14)//S14抬起
		{
			Time_Flag = Count_500Ms = 0;//状态复位
			if(++Parameter[Parameter_Index] > 70)//超过上限值
				Parameter[Parameter_Index] = 10;
		}
	}
	else//长按
	{
		if(Key_Old == 14)//S14长按
		{
			if(++Parameter[Parameter_Index] > 70)//超过上限值
				Parameter[Parameter_Index] = 10;
		}
		if(Key_Up == 14)//S14抬起
			Time_Flag = Count_500Ms = 0;//状态复位
	}
	
	
	if(Seg_Disp_Mode == 2)//处于参数设置界面
	{
		if(Key_Down == 15)//S15按下
			Time_Flag = 1;//计时开始
	}
	if(Count_500Ms < 500)//短按
	{
		if(Key_Up == 15)//S15抬起
		{
			Time_Flag = Count_500Ms = 0;//状态复位
			if(--Parameter[Parameter_Index] == 255)//超过下限值
				Parameter[Parameter_Index] = 70;
		}
	}
	else//长按
	{
		if(Key_Old == 15)//S15长按
		{
			if(--Parameter[Parameter_Index] == 255)//超过下限值
				Parameter[Parameter_Index] = 70;
		}
		if(Key_Up == 15)//S15抬起
			Time_Flag = Count_500Ms = 0;//状态复位
	}
	
	switch(Key_Down)
	{
		case 11://小数点输入
			if(Seg_Disp_Mode == 0 && Point_Flag == 0 && Seg_Input[0] != 11)//处于温度采集界面、标志位为0、输入数组第一位有数据
			{
				Seg_Point[2+Seg_Input_Index] = 1;
				Point_Wela = Seg_Input_Index;//记录此时指针状态 便于后期数据处理
				Point_Flag = 1;//拉高标志位 保证一次输入周期小数点只能使能一次
			}
		break;
		case 16://温度采集
			if(Seg_Disp_Mode != 0)//处于非温度采集界面
			{
				Seg_Input_Index = 0;//指针复位
				Point_Flag = 0;//小数点输入标志位复位
				for(i = 0;i < 3;i++)//数组复位
					Seg_Input[i] = 11;
				Seg_Disp_Mode = 0;
			}
			else//处于温度采集界面
			{
				if(Point_Flag == 0 || Seg_Input_Index < 3)//若小数点没有使能 或者指针不为3 则一定为无效数据
				{
					Seg_Input_Index = 0;//指针复位
					Seg_Point[3] = Seg_Point[4] = 0;//小数点复位
					Point_Flag = 0;//小数点输入标志位复位
					for(i = 0;i < 3;i++)//数组复位
						Seg_Input[i] = 11;
				}
				else//小数点使能 进行范围判断
				{
					Temperature = Seg_Input[0] * 100 + Seg_Input[1] * 10 + Seg_Input[2] + 5;
					while(3 - Point_Wela)
					{
						Temperature /= 10.0;
						Point_Wela++;
					}
					if((unsigned char)Temperature > 85)//超出采集上限值
					{
						Seg_Input_Index = 0;//指针复位
						Seg_Point[3] = Seg_Point[4] = 0;//小数点复位
						Point_Flag = 0;//小数点输入标志位复位
						for(i = 0;i < 3;i++)//数组复位
							Seg_Input[i] = 11;						
					}
					else//数据合理
					{
						Seg_Input_Index = 0;//指针复位
						Seg_Point[3] = Seg_Point[4] = 0;//小数点复位
						Point_Flag = 0;//小数点输入标志位复位
						Seg_Disp_Mode = 1;//跳转到数据显示界面
					}
				}
			}
		break;
		case 12://界面切换
			if(Seg_Disp_Mode != 0)//处于非温度采集界面
				Seg_Disp_Mode = Seg_Disp_Mode==1?2:1;
			if(Seg_Disp_Mode == 2)//切换到参数设置界面时
			{
				Parameter_Index = 0;//指针复位
				Parameter[0] = Parameter_Ctrol[0];//数据复位
				Parameter[1] = Parameter_Ctrol[1];
			}
			else//切换到数据显示界面
			{
				if(Parameter[0] >= Parameter[1] && Parameter[0] <= 70 && Parameter[1] >= 10)//合理设置参数
				{
					Error_Flag = 0;
					Parameter_Ctrol[0] = Parameter[0];//设置生效
					Parameter_Ctrol[1] = Parameter[1];
				}
				else
					Error_Flag = 1;
			}
		break;
		case 13://参数切换
			if(Seg_Disp_Mode == 2)//处于参数设置界面
				Parameter_Index ^= 1;//指针取反
		break;
//		case 14://参数+1
//			if(Seg_Disp_Mode == 2)//处于参数设置界面
//			{
//				if(++Parameter[Parameter_Index] > 70)//超过上限值
//					Parameter[Parameter_Index] = 10;
//			}
//		break;
//		case 15://参数-1
//			if(Seg_Disp_Mode == 2)//处于参数设置界面
//			{
//				if(--Parameter[Parameter_Index] == 255)//超过下限值
//					Parameter[Parameter_Index] = 70;
//			}			
//		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i;//For循环专用变量
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序

	switch(Seg_Disp_Mode)
	{
		case 0://温度采集界面
			Seg_Buf[0] = 12;//A
			Seg_Buf[1] = Seg_Buf[2] = 10;//熄灭
			for(i = 0;i < 3;i++)//输入数据
				Seg_Buf[3+i] = Seg_Input[i];
			if(Seg_Input[2] == 11)//当最后一位数码管未输入数据时
				Seg_Buf[3+Seg_Input_Index] = Seg_Star_Flag?Seg_Input[Seg_Input_Index]:10;//当前输入数码管闪烁
		break;
		case 1://数据显示界面
			Seg_Buf[0] = 13;//C
			for(i = 0;i < 3;i++)//熄灭
				Seg_Buf[1+i] = 10;
			Seg_Buf[4] = (unsigned char)Temperature / 10;
			Seg_Buf[5] = (unsigned char)Temperature % 10;
		break;
		case 2://参数设置界面
			Seg_Buf[0] = 14;//P
			Seg_Buf[1] = 10;//熄灭
			Seg_Buf[2] = Parameter[0] / 10;
			Seg_Buf[3] = Parameter[0] % 10;
			Seg_Buf[4] = Parameter[1] / 10;
			Seg_Buf[5] = Parameter[1] % 10;
			Seg_Buf[2+2*Parameter_Index] = Seg_Star_Flag?Parameter[Parameter_Index] / 10:10;//当前选中数码管闪烁
			Seg_Buf[3+2*Parameter_Index] = Seg_Star_Flag?Parameter[Parameter_Index] % 10:10;
		break;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
//	Led_Pwm = 3 * (1 * ((int)Temperature / Parameter_Ctrol[0]) + 2 * ((!((int)Temperature / Parameter_Ctrol[0])) & ((int)Temperature / Parameter_Ctrol[1])) + 3 *(!((int)Temperature / Parameter_Ctrol[1])));
	if(Temperature > Parameter_Ctrol[0])
		Led_Pwm = 3;
	else if(Temperature < Parameter_Ctrol[0] && Temperature > Parameter_Ctrol[1])
		Led_Pwm = 6;
	else
		Led_Pwm = 9;
	ucLed[0] = (int)Temperature / Parameter_Ctrol[0];
	ucLed[1] = (!((int)Temperature / Parameter_Ctrol[0])) & ((int)Temperature / Parameter_Ctrol[1]);
	ucLed[2] = !((int)Temperature / Parameter_Ctrol[1]);
	ucLed[3] = Error_Flag;
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
	ET0 = 1;        //定时器0中断打开
	EA = 1;         //总中断打开
}

/* 定时器0中断服务函数 */
void Timer0Server() interrupt 1
{
 	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值   
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 10) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 6) Seg_Pos = 0;//数码管显示专用
	if(++Led_Pos == 8) Led_Pos = 0;//Led显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	if(++Timer_500Ms == 500)//每五百毫秒触发一次
	{
		Timer_500Ms = 0;//计数值复位 便于下次使用
		Seg_Star_Flag ^= 1;//闪烁标志位取反
	}
	if(Time_Flag == 1)
	{
		if(++Count_500Ms == 600)//自加到五百毫秒以上就可以限制住 防止一直按下自加的时候数值越界
			Count_500Ms = 600;
	}
	if(++Led_Num == 10) Led_Num = 0;
	if(Led_Num < Led_Pwm)
		Led_Disp(Led_Pos,ucLed[Led_Pos]);
	else
		Led_Disp(Led_Pos,0);
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