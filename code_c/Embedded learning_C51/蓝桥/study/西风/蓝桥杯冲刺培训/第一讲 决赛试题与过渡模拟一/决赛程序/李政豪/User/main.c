/* 第二届 创意智造·奠基未来 单片机设计与开发大赛 专用模板 */
 
/* 头文件声明区 */
#include <REGX52.H>//单片机寄存器专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件
#include <intrins.h>//LED移动函数库

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[6] = {10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char Seg_Disp_Mobe;//数码管的三种模式 0-运行模式 1-设置模式 2-数据显示界面
unsigned char Seg_Come[6]={12,1,10,4,0,0};//运行模式1
unsigned char Seg_Come_Index=0;//运行指针
unsigned char Seg_Setting[6]={14,1,10,4,0,0};//设置界面数组
unsigned char Seg_Setting_Index=0;//数组指针
unsigned int  Time_400ms;//时间400ms延迟
unsigned char LED_Flood;//流动变量
unsigned char LED_First=0xfe;//模式1-LED
unsigned char LED_Second=0x7f;//模式2-LED
unsigned char LED_3[4]={0x7e,0xbd,0xdb,0xe7};//模式3-LED
unsigned char LED_3x=0xff;//模式3
unsigned char Try;//次数
unsigned char LED_4[4]={0xe7,0xdb,0xbd,0x7e};//模式4-LED
unsigned char LED_4x=0xff;//模式4
unsigned int  Time_Delay;//时间延迟变量
unsigned int  Time_Arrive;//时间到达变量
bit Key_6;//按键6按下
bit Seg_Degin;//运行模式开始计时
bit Seg_Setting_Flash;//设置模式闪烁

/* 键盘处理函数 */
void Key_Proc()
{
	unsigned char Move;                                //流动变量
	
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Old = Key_Val;//辅助扫描变量

	
	switch(Key_Down)
	{
/*------------------切换按键-------------------------*/		
		case 6 :            															//按键6按下可以在运行模式和流动间隔模式切换
		LED_Flood=0;                                      //关闭LED流动
		P1=0xff;                                          //开启设置模式关闭所有LED
		Seg_Disp_Mobe=1;
		Key_6=1;
		if(LED_Flood==0)
	{ switch(Seg_Setting_Index)
		{                                									//切换到设置模式
			case 0 :                                        //运行模式
			Seg_Setting_Index=1;                            //切换到流动模式
			break;
			
			case 1 :
		  Seg_Setting_Index=0;                            //切换到运行模式		
			break;
		}
	}
		break;
/*-----------------加速按键--------------------------*/		
	   case 5 :                                         //按键5进入加速模式
			if(Seg_Disp_Mobe==1)
			{
				switch(Seg_Setting_Index)
				{
					case 0 :                                    //加速模式若选中运行模式																										 
					if(++Seg_Setting[1]==5)                     //个位加到5
						Seg_Setting[1]=1;
					break;
					
					case 1 :                                    //加速模式选中在流动间隔模式
					if(++Seg_Setting[3]==10)
					{
						Seg_Setting[2]=1;
						Seg_Setting[3]=0;
					}
					if(Seg_Setting[2]==1 && Seg_Setting[3]>2)  //固定到12
					{
						Seg_Setting[2]=1;
						Seg_Setting[3]=2;
					}
					break;
				}
			}break;
/*----------------减速按键------------------------------*/	
			case 4 :                                          //按键4进入减速模式
			if(Seg_Disp_Mobe==1)
			{
				switch(Seg_Setting_Index)
				{
					case 0 :                                      //减速模式选中运行模式
					if(--Seg_Setting[1]==0)
						Seg_Setting[1]=4;
					break;
					
					case 1 :
						if(Seg_Setting[2]==1)                       //如果十位为1
					{
						if(--Seg_Setting[3]==255)                   //将十位化为10
						{Seg_Setting[2]=10;
						Seg_Setting[3]=9;}
					}
					else
						if(--Seg_Setting[3]<4)                      //固定到4
						 Seg_Setting[3]=4;      
 					break;
				}
			}break;
/*----按键7为“启动/停止按键,按下后启动或停止LED------*/

			case 7 : 
			Seg_Disp_Mobe=0;                                 //按下后回到运行模式
			switch(LED_Flood)
			{
				case 0 :                                       //LED还没有流动
				for(Move=1;Move<4;Move++)
				Seg_Come[Move]=Seg_Setting[Move];              //赋值
//				Seg_Come[1]=Seg_Setting[1];                  
//				Seg_Come[2]=Seg_Setting[2];
//				Seg_Come[3]=Seg_Setting[3];
				LED_Flood=1;                                   //切换到流动模式
				Seg_Come[0]=11;                                //流动标志位
				break;
				
				case 1 :                                       //LED流动模式
				Seg_Come[0]=12;                                //停止标志位
				LED_Flood=0;
				break;      
			}
			  break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char Cycle;  //移动变量
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序
	
	switch(Seg_Disp_Mobe)
	{
		case 0 :         //运行模式
		for(Cycle=0;Cycle<6;Cycle++)
		Seg_Buf[Cycle]=Seg_Come[Cycle];
		break;
		
		case 1 :        //设置模式
		for(Cycle=0;Cycle<6;Cycle++)
		Seg_Buf[Cycle]=Seg_Setting[Cycle];
				            //闪烁位
		if(Seg_Setting_Index==0)
		{
		Seg_Buf[0]=Seg_Setting_Flash?10:Seg_Setting[0];
		Seg_Buf[1]=Seg_Setting_Flash?10:Seg_Setting[1];
		}
		
		if(Seg_Setting_Index==1)
		{
			for(Cycle=2;Cycle<6;Cycle++)
			Seg_Buf[Cycle]=Seg_Setting_Flash?10:Seg_Setting[Cycle];
		}
		break;
			

	}
}

/* 其他显示函数 */
void Led_Proc()
{
	                                                                //模式1
	if(Seg_Disp_Mobe==0 && Seg_Come[1]==1 && LED_Flood==1)
	P1=LED_First;
	                                                                //模式2
	if(Seg_Disp_Mobe==0 && Seg_Come[1]==2 && LED_Flood==1)
	P1=LED_Second;
	                                                                //模式3
	if(Seg_Disp_Mobe==0 && Seg_Come[1]==3 && LED_Flood==1)
	P1=LED_3x;
	                                                                //模式4
	if(Seg_Disp_Mobe==0 && Seg_Come[1]==4 && LED_Flood==1)
	P1=LED_4x;
}

/* 定时器0中断初始化函数 */
void Timer0Init(void)		//1毫秒@12.000MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x18;			//设置定时初始值
	TH0 = 0xFC;			//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;        //定时器0中断打开
	EA = 1;         //总中断打开
}

/* 定时器0中断服务函数 */
void Timer0Server() interrupt 1
{
	unsigned char Run;//移动变量
	
 	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值   
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	
	if(++Seg_Pos == 6) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos]);
/*---------------设置模式闪烁------------*/	
	if(Key_6==1)
	{
		if(++Time_400ms==400)
		{
			Time_400ms=0;
				Seg_Setting_Flash=~Seg_Setting_Flash;
			}
		}
/*---------------LED模式1------------------*/	
	if(Seg_Disp_Mobe==0 && Seg_Come[1]==1 && LED_Flood==1)
	{
		if(Seg_Come[2]==1)                           //如果为1
		Time_Delay=Seg_Come[2]*1000+Seg_Come[3]*100;
		if(Seg_Come[2]==10)
		Time_Delay=Seg_Come[3]*100;
		if(++Time_Arrive==Time_Delay)
		{
			Time_Arrive=0;
			LED_First=_crol_(LED_First,1);             //移动
		}
	}
/*--------------LED模式2--------------------*/
	if(Seg_Disp_Mobe==0 && Seg_Come[1]==2 && LED_Flood==1)
	{
		if(Seg_Come[2]==1)                           //如果为1
		Time_Delay=Seg_Come[2]*1000+Seg_Come[3]*100;
		if(Seg_Come[2]==10)
		Time_Delay=Seg_Come[3]*100;
		if(++Time_Arrive==Time_Delay)
		{
			Time_Arrive=0;
			LED_Second=_cror_(LED_Second,1);             //移动
		}			
	}
/*------------LED模式3-------------------------*/
	if(Seg_Disp_Mobe==0 && Seg_Come[1]==3 && LED_Flood==1)
	{
		if(Seg_Come[2]==1)                           //如果为1
		Time_Delay=Seg_Come[2]*1000+Seg_Come[3]*100;
		if(Seg_Come[2]==10)
		Time_Delay=Seg_Come[3]*100;
		if(++Time_Arrive==Time_Delay)
		{
			Time_Arrive=0;
			++Try;
			switch(Try)
			{
				case 1 :
				LED_3x=LED_3[0];
				break;
				
				case 2 :
				LED_3x=LED_3[1];
				break;
				
				case 3 :
				LED_3x=LED_3[2];
				break;
				
				case 4 :
				LED_3x=LED_3[3];
				break;
		}
			if(Try==4)
				Try=0;
		}
	}
/*-----------------LED模式4---------------------------*/
	if(Seg_Disp_Mobe==0 && Seg_Come[1]==4 && LED_Flood==1)
	{
		if(Seg_Come[2]==1)                           //如果为1
		Time_Delay=Seg_Come[2]*1000+Seg_Come[3]*100;
		if(Seg_Come[2]==10)
		Time_Delay=Seg_Come[3]*100;
		if(++Time_Arrive==Time_Delay)
		{
			Time_Arrive=0;
			++Try;
			switch(Try)
			{
				case 1 :
				LED_4x=LED_4[0];
				break;
				
				case 2 :
				LED_4x=LED_4[1];
				break;
				
				case 3 :
				LED_4x=LED_4[2];
				break;
				
				case 4 :
				LED_4x=LED_4[3];
				break;
			}
			if(Try==4)
				Try=0;
		}
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