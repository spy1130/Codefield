/*头文件声明区*/
#include <STC15F2K60S2.H>
#include <Init.H>
#include <Led.H>
#include <Key.H>
#include <Seg.H>
#include <iic.h>

/*变量声明区*/
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Key_Val,Key_Old,Key_Down;
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char Pos;//扫描专用变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管段选数据存储数组
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//数码管小数点状态值存储数组
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led状态值存储数组
unsigned char Dat;//接收AD转换数据
float Voltage1;//接收AD转换数据的电压值
unsigned char Seg_Mode ;//数码管模式变量 0-电压显示界面 1-电压输出界面
unsigned char Write = 102;//DA转换写入值 默认为102 即2v
float Voltage2;//接收DA转换数据的电压值
bit Set_Mode_Flag;//输出模式标志位 0-VDAC=2 1-VDAC=VRB2
bit Led_Flag;//Led灯使能标志位 0-使能 1-关闭
bit Led3_Flag;//L3指示灯使能标志位 1-使能 0-熄灭
bit Seg_Flag;//数码管使能标志位 0-使能 1-关闭


/*按键处理函数*/
void Key_Porc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//按键减速程序
	
	Key_Val = Key_Read();//读取按键值
	Key_Down = Key_Val & (Key_Val ^ Key_Old);//检测下降沿
	Key_Old = Key_Val;//辅助变量
	
	switch(Key_Down)
	{
		case 4://显示界面切换按钮
			if(++Seg_Mode == 2)
				Seg_Mode = 0;
		break;
		case 5://输出模式切换按钮
			if(Seg_Mode == 1)
			{
				Set_Mode_Flag ^= 1;
			}
		break;
		case 6://Led灯使能控制按钮
			Led_Flag ^= 1;
		break;
		case 7://数码管使能控制按钮
			Seg_Flag ^= 1;
		break;
	}
	
	
}

/*信息处理函数*/
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序
	
	/*实时读取数据区域*/
	Dat = AD_Read(0x43);
	Voltage1 = ((Dat / 51.0)*1000+5)/1000.0;
	DA_Write(Write);
	Voltage2 = ((Write / 51.0)*1000+5)/1000.0;
	
	/*数码管显示区域*/
	if(Seg_Flag == 0)//数码管使能判断
	{
		switch(Seg_Mode)
		{
			case 0://电压显示界面 
				Seg_Buf[0] = 11;
				Seg_Buf[5] = (unsigned char)Voltage1 % 10 ;	
				Seg_Buf[6] = (unsigned char)(Voltage1 * 10) % 10 ;	
				Seg_Buf[7] = (unsigned int)(Voltage1 * 100) % 10 ;
				Seg_Point[5] = 1;
			break;
			case 1://电压输出界面 
				if(Set_Mode_Flag == 0)//数码管输出模式判断
				{
					Seg_Buf[0] = 12;
					Seg_Buf[5] = (unsigned char)Voltage2 % 10 ;	
					Seg_Buf[6] = (unsigned char)(Voltage2 * 10) % 10 ;	
					Seg_Buf[7] = (unsigned int)(Voltage2 * 100) % 10 ;
					Seg_Point[5] = 1;
				}
				else
				{
					Seg_Buf[0] = 12;
					Seg_Buf[5] = (unsigned char)Voltage1 % 10 ;	
					Seg_Buf[6] = (unsigned char)(Voltage1 * 10) % 10 ;	
					Seg_Buf[7] = (unsigned int)(Voltage1 * 100) % 10 ;
					Seg_Point[5] = 1;				
				}
					
			break;
		}
    }
	else
	{
		Seg_Buf[0] = 10;
		Seg_Buf[5] = 10 ;	
		Seg_Buf[6] = 10 ;	
		Seg_Buf[7] = 10 ;
		Seg_Point[5] = 0;		
	}
	
}

/*其他显示函数*/
void Led_Proc()
{
	unsigned char i;//循环变量
	
	if(Voltage1 < 1.5 || Voltage1 >= 2.5 && Voltage1 < 3.5)//指示灯3使能判断
		Led3_Flag = 0;
	else
		Led3_Flag = 1;
	
	if(Led_Flag == 0)//Led灯使能判断
	{
		ucLed[0] = ~(bit)Seg_Mode;
		ucLed[1] = (bit)Seg_Mode;
		ucLed[2] = Led3_Flag;
		ucLed[3] = Set_Mode_Flag;
	}
	else
	{
		for(i = 0; i < 4;i++)
		    ucLed[i] = 0;
	}
}

/*定时器0中断初始化函数*/
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
	EA = 1;
}

/*定时器0服务函数*/
void Timer0Siver() interrupt 1
{
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	
	if(++Pos == 8) Pos = 0;
	Seg_Disp(Pos,Seg_Buf[Pos],Seg_Point[Pos]);
	Led_Disp(Pos,ucLed[Pos]);
	
}

/*主函数*/
void main()
{
	Init();
	Timer0Init();
	while(1)
	{
		Key_Porc();
		Seg_Proc();
		Led_Proc();
	}
}

