/*头文件区*/
#include <STC15F2K60S2.H>
#include <Init.H>//单片机初始化底层
#include <Led.H>//Led灯驱动底层
#include <Key.H>//按键驱动底层
#include <Seg.H>//数码管驱动底层
#include <iic.H>//EEPROM、数模转换驱动底层

/*自定义变量*/
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Key_Val,Key_Down,Key_Old;
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char Pos;//扫描变量
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//数码管段选值存储数组
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led灯状态值存储数组
unsigned char Led_Mode;//Led灯模式变量 0-模式1 1-模式2 2-模式3 3-模式4
bit Led_Flag ;//Led灯流转标志位 0-暂停 1-启动
unsigned int STC_Timer;//系统计时变量
unsigned int Led_Timer_Sto[4] = {400,400,400,400};//流转时间存储数组 400-1200
unsigned int Led_Timer_Set[4];//流转时间设置数组
unsigned char Timer_Set_Index;//流转间隔设置指针
unsigned int Timer_400ms;//400毫秒时间变量
bit Seg_Star_Flag;//数码管闪烁标志位
unsigned char EEPROM_String[4] = {4,4,4,4};//EEPROM数据存放数组
unsigned char dat;//接受AD转换数值
unsigned char Led_Rank;//亮度等级
unsigned char Led_Count;//亮度统计
bit Key_Flag;//长按标志位


/*按键扫描函数*/
void Key_Proc()
{
	unsigned char k;
	
	if(Key_Slow_Down) return;
	else Key_Slow_Down = 1;//按键减速程序
	
	Key_Val = Key_Read();//读取按键值
	Key_Down = Key_Val ^ (Key_Val & Key_Old);//检测下降沿
	Key_Old = Key_Val;//辅助变量
	
	if(Timer_Set_Index == 0 && Key_Old == 4)
	{
		Key_Flag = 1;
	}
	else
		Key_Flag = 0;
	
	switch(Key_Down)
	{
		case 7://启动/停止按钮
			Led_Flag ^= 1;
		break;
		case 6://设置
			if(++Timer_Set_Index == 3)
				Timer_Set_Index = 0;
			if(Timer_Set_Index == 1)//进入流转间隔设置界面
			{
				for( k = 0; k < 4; k++)
				   Led_Timer_Set[k] = Led_Timer_Sto[k];
			}
			else if(Timer_Set_Index == 0)//退出流转间隔设置界面
			{
				for( k = 0; k < 4; k++)
				{
				    Led_Timer_Sto[k] = Led_Timer_Set[k];
					EEPROM_String[0] = Led_Timer_Set[0] / 100;
				}
				EEPROM_Write(EEPROM_String,0,4);				
			}		
		break;
		case 5://加
			if(Timer_Set_Index)//设置界面
			{
				if(Timer_Set_Index == 1)
				{
					if(++Led_Mode == 4)
						Led_Mode = 3;
				}
				else if(Timer_Set_Index == 2)
				{	
					if(Led_Timer_Set[Led_Mode] == 1200)
						Led_Timer_Set[Led_Mode] = 1200;
					else
						Led_Timer_Set[Led_Mode] += 100;					
				}
		   }
		break;
		case 4://减
			if(Timer_Set_Index)//设置界面
			{				
				if(Timer_Set_Index == 1)
				{
					if(--Led_Mode == 255)
						Led_Mode = 0;
				}
				else if(Timer_Set_Index == 2)
				{
					if(Led_Timer_Set[Led_Mode] == 400)
						Led_Timer_Set[Led_Mode] = 400;
					else
						Led_Timer_Set[Led_Mode] -= 100;
				}
		    }			
		break;
	}
}

/*信息处理函数*/
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	else Seg_Slow_Down = 1;//数码管减速程序
	
	/*实时读取AD值*/
	dat = AD_Read(0x43);	
	
    /*数码管显示区域*/
	if(Key_Flag == 0)
	{
		Seg_Buf[0] = Seg_Buf[2] = 11;//-
		Seg_Buf[1] = Led_Mode + 1;
		Seg_Buf[4] = Led_Timer_Set[Led_Mode] / 1000 ? Led_Timer_Set[Led_Mode] / 1000 : 10;
		Seg_Buf[5] = Led_Timer_Set[Led_Mode] /100 % 10;
		Seg_Buf[6] = Led_Timer_Set[Led_Mode] /10 % 10;	
		Seg_Buf[7] = Led_Timer_Set[Led_Mode] % 10;
		
		if(Timer_Set_Index == 1)
			Seg_Buf[1] = Seg_Star_Flag ? Led_Mode + 1 : 10;	
		else if(Timer_Set_Index == 2)
		{
			Seg_Buf[4] = Seg_Star_Flag ? (Led_Timer_Set[Led_Mode] / 1000 ? Led_Timer_Set[Led_Mode] / 1000 : 10) : 10;
			Seg_Buf[5] = Seg_Star_Flag ? (Led_Timer_Set[Led_Mode] /100 % 10) : 10;
			Seg_Buf[6] = Seg_Star_Flag ? (Led_Timer_Set[Led_Mode] /10 % 10) : 10;	
			Seg_Buf[7] = Seg_Star_Flag ? (Led_Timer_Set[Led_Mode] % 10) : 10;
		}
	}	
	else
	{
		Seg_Buf[0] = 10;
		Seg_Buf[1] = 10;
		Seg_Buf[2] = 10;
		Seg_Buf[4] = 10;
		Seg_Buf[5] = 10;		
		Seg_Buf[6] = 11;	
		Seg_Buf[7] = Led_Rank / 3;
		
	}		
	
}

/*其他显示函数*/
void Led_Proc()
{
	unsigned char i;
	static unsigned char j = 0;
	
	if((float)dat / 63.75 >= 3) Led_Rank = 12;
	if((float)dat / 63.75 >= 2 && (float)dat / 63.75 < 3) Led_Rank = 9;
	if((float)dat / 63.75 >= 1 && (float)dat / 63.75 < 2) Led_Rank = 6;
	if((float)dat / 63.75 >= 0 && (float)dat / 63.75 < 1) Led_Rank = 3;	
		
	if(STC_Timer == Led_Timer_Sto[Led_Mode])//系统计时等于对应流转模式的流转时间
	{
		STC_Timer = 0;
		switch(Led_Mode)
		{
			case 0://模式1
				for(i = 0; i < 8; i++)
				{
					ucLed[i] = (i == j);
				}
				j++;				
				if(j == 8)
				{
					j--;					
					Led_Mode = 1;
				}				
			break;
			case 1://模式2				
				for(i = 0; i < 8; i++)
				{
					ucLed[i] = (i == j);
				}
				j--;
				if(j == 255)
				{
					j = 0;
					Led_Mode = 2;
				}
			break;
			case 2://模式3
				for(i = 0; i < 4; i++)
				{
					ucLed[i] = (i == j);
					ucLed[7-i] = (i == j);
				}
				j++;				
				if(j == 4)
				{
					j--;					
					Led_Mode = 3;
				}					
			break;
			case 3://模式4
				for(i = 0; i < 4; i++)
				{
					ucLed[i] = (i == j);
					ucLed[7-i] = (i == j);					
				}
				j--;
				if(j == 255)
				{
					j = 0;
					Led_Mode = 0;
				}			
			break;
		}
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

/*定时器中断服务函数*/
void Timer_Seriver() interrupt 1
{
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	
	if(++Pos == 8) Pos = 0;
	if(Timer_Set_Index || Key_Flag == 1)
	   Seg_Disp(Pos,Seg_Buf[Pos]);
	else
	   Seg_Disp(Pos,10);
	
	if(Led_Flag == 1)
		STC_Timer++;
	
	if(++Timer_400ms == 400)
	{
		Timer_400ms = 0;
		Seg_Star_Flag ^= 1;
	}
	
	if(++Led_Count == 12)
		Led_Count = 0;
	if(Led_Count < Led_Rank)
	    Led_Disp(Pos,ucLed[Pos]);
	else
	    Led_Disp(Pos,0);
	
}

/*主函数*/
void main()
{
	unsigned char k;	
	EEPROM_Read(EEPROM_String,0,4);
	for( k = 0; k < 4; k++)
	    Led_Timer_Sto[k] = EEPROM_String[k] * 100;
	Init_STC();
	Timer0Init();
	while(1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}
	
