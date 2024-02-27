/*************************************************************************************
实验功能：第六届省赛试题程序设计
时间：2017.12.20
作者：吴康
**************************************************************************************/
#include <STC15F2K60S2.h>
#include <delay.h>
#include <onewire.h> 
#include <ds13022.h>

#define uchar unsigned char 
#define uint unsigned int 

uchar discount = 0;													//数码管移位控制变量
uchar dsbuff[8] = {11,11,11,11,11,11,11,11};//初始化数组
uchar tab[12] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar key_1 = 0,key_2 = 0,key_3 = 0,key_4 = 0;//按键标志位
uchar count_1=1;															//采集时间间隔选着变量
uchar jiange=1,j=0;														//时间间隔设置变量
uchar count_2=0,count_3=0,count_4=0;					//
uchar tt_1=0,tt_2=0,flag_1=0,flag_2=0,flag_3=0;//
uchar tem[10];																 //采集温度存储数组
uchar i=0,tt_3=0,biaozhi=0,flag_4=0,flag_5=0,flag_6=0,flag_7=0;//标志位
uint tt_4=0;																	//定时器变量标志位
uchar temp;																		//DS1302秒的数据存储变量

/**************************************************************************************
函数名称：led
功能：LED亮灭控制函数
***************************************************************************************/
void led(uchar liang)
{
	P2=P2&0x1f;
	P2=0x80;
	P0=liang;
	P2=P2&0x1f;
}

/**************************************************************************************
函数名称：display
功能：数码管显示函数
***************************************************************************************/
void display()
{
	P2=0xef;
	P0=0xff;
	P2=0x1f;
	
  P2=0xcf;
	P0=1<<discount;
	P2=0x1f;
	
	P2=0xef;
	P0=tab[dsbuff[discount]];
	P2=0x1f;
	
	if(++discount==8)
	 	discount = 0;
}

/**************************************************************************************
函数名称：allinit
功能：板子初始化函数
***************************************************************************************/
void allinit()
{
	P2=0x80;P0=0xff;//关闭LED
	P2=0xa0;P0=0x00;//关闭蜂鸣器，继电器
	P2=0xc0;P0=0xff;//选中所有数码管
	P2=0xe0;P0=0xff;//关闭所有数码管
}

/**************************************************************************************
函数名称：caiji
功能：采集温度控制函数
***************************************************************************************/
void caiji()
{
	if(flag_2==0)
		{
			ds_write();			//对于一开始不显示时钟的先不让时钟初始化
			flag_2=1;				 
			temp=shijian[0];//第一次执行总程序先将temp初始化为刚开始的时钟秒数
		}
		ds_read();				//读时间
	if(flag_7==1)
		{
			flag_7=0;
			temp=clk[0];//第二次……第N次执行总程序时 由于ds1302在执行别的程序的时候仍在走 为了弥补这部分时间 将temp初始化为按S5键后的秒数 
									//则可以从这里继续去判断第二次，第三次……是否符合时间间隔
		}			
	if(i<10)
		{
			key_2=0;key_1=0;
			dsbuff[7] = clk[0]%10;
			dsbuff[6] = clk[0]/10;	
			dsbuff[4] = clk[1]%10;
			dsbuff[3] = clk[1]/10;
			dsbuff[1] = clk[2]%10;
			dsbuff[0] = clk[2]/10;
		}

/************用1302写时间间隔 要特别注意temp的一开始的初始化要对应每次按键后的时间************/
		if(clk[0]-temp==jiange)			  //用1302写时间间隔 注意temp要不断刷新
			{								 						//当记录的秒数减去初始化为间隔时
				if(i<10)
					tem[i]=wendu();					//存储这十个温度数据
				i++;
				if(i>=10) i=10;
				temp=clk[0];							//更新temp的值
			}
		else if(60+clk[0]-temp==jiange)//当进入下一分钟后clk[0]小于temp时需要加上一分钟（60）秒
			{
				if(i<10)
					tem[i]=wendu();
				i++;
				if(i>=10) i=10;
				temp=clk[0];
			}	
}

/**************************************************************************************
函数名称：Timer1Init
功能：定时器1初始化函数
***************************************************************************************/
void Timer1Init(void)		//5毫秒@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x00;			//设置定时初值
	TH1 = 0x28;			//设置定时初值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
}

/**************************************************************************************
函数名称：Timer10Init
功能：定时器10初始化函数
***************************************************************************************/
void Timer0Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x9A;			//设置定时初值
	TH0 = 0xA9;			//设置定时初值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
}

/**************************************************************************************
函数名称：keyscan
功能：按键扫描函数
***************************************************************************************/
void keyscan()
{
	if(P30==0)				//重新进入参数设置界面
	{
		Delay5ms();			//延时消抖
		if(P30==0)
			{
				key_1=1;
			}
			while(!P30);
	}
	if(P31==0)			//按键切换显示刚刚存储的十个温度数据
	{
		Delay5ms();		//延时消抖
		if(P31==0)
			{
				key_2=1;
			}
			while(!P31);
	}
	if(P32==0)			//确认采集间隔时间，并设置标志位让他退出设置界面，进入时钟显示界面
	{
		Delay5ms();		//延时消抖
		if(P32==0)
			{
				key_3 = 1;
				count_1=0;//把选择的时间隔清零为下次进入准备
				count_4=0;
			}
			while(!P32);
	}
	
	if(P33==0)			//切换温度采集间隔时间
	{
		Delay5ms();		//延时消抖
		if(P33==0)
			{
				key_4 = 1;
				count_1++;//用于选择是什么时间间隔
			}
			while(!P33);
	}
}

/**************************************************************************************
函数名称：main
功能：主函数
***************************************************************************************/
void main()
{
	allinit();				//板子整体初始化
	Timer0Init();			//定时器0初始化
	Timer1Init();			//定时器1初始化
	EA=1;							//打开中断
	ET0=1;						//打开中断0
	ET1=1;						//打开中断1
	while(1)
	{
		keyscan();			//按键扫描
		if(key_3==1)    //按键S5按下 确定温度间隔
			{ 
					caiji();	//采集温度函数初始化
					if(i==10)	//判断是否采集到十个温度数据
						{
								if(key_2==0)
									{
										dsbuff[7] = tem[1]%10;
										dsbuff[6] = tem[1]/10;
										dsbuff[5] = 10;
										dsbuff[4] = 11;
										dsbuff[3] = 11;
										dsbuff[2] = 0;
										dsbuff[1] = 0;
										dsbuff[0] = 10;
									}
								if((flag_5==1)&&(key_1==0)&&(key_2==1))
									{
										flag_5=0;  
										P2=0x80;P0=0xff;//打开LED锁存器
										if(j==10)
											{
												j=9;
											}
											dsbuff[7] = tem[j]%10;
											dsbuff[6] = tem[j]/10;
											dsbuff[5] = 10;
											dsbuff[4] = 11;
											dsbuff[3] = 11;
											dsbuff[2] = j%10;
											dsbuff[1] = j/10;
											dsbuff[0] = 10;
											j++;
									}
								if(key_1==1)
									{
										key_1=0;
										flag_6=1;
										key_3=0;
										key_2=0;
										i=0;
										j=0;
										flag_7=1;
									}		
							}
				}
		else if((key_3==0)||(flag_6==1)) //设置采集时间间隔和显示时间间隔
		{
				flag_6=0;
				switch(count_1)
					{
						case 1:jiange=1;break;
						case 2:jiange=5;break;
						case 3:jiange=30;break;
						case 4:jiange=60;count_1=0;break;
					}
					dsbuff[7] = jiange%10;
					dsbuff[6] = jiange/10;
					dsbuff[5] = 10;
					dsbuff[4] = 11;
					dsbuff[3] = 11;
					dsbuff[2] = 11;
					dsbuff[1] = 11;
					dsbuff[0] = 11;
		}
	}
}

/**************************************************************************************
函数名称：time_0
功能：中断服务函数
***************************************************************************************/
void time_0() interrupt 1
{
	display();//扫描数码管函数
	if(++tt_4==500)
	{
		tt_4=0;
		flag_5=1;//用于控制采集的温度显示时间间隔为一秒
	}
}

/**************************************************************************************
函数名称：time_1
功能：中断服务函数
***************************************************************************************/
void time_1() interrupt 3
{
		if((key_3==1)&&(i<10))//当采集的数据没有到十个时
		{
//				if(count_4<=2)
				++tt_1;
				if(tt_1==200)
					{
						tt_1=0;
						if(flag_1==0)//用于控制提示字符1提示字符2间隔一秒闪烁
							{
								flag_1=1;
								dsbuff[5]=11;
								dsbuff[2]=11;
							}
						else 
							{
								flag_1=0;
								dsbuff[5]=10;
								dsbuff[2]=10;
//								count_4++;
							}
					}
				++tt_3;
				if(tt_3==200)
					{
						biaozhi++;
						tt_3=0;
						if(biaozhi==jiange)
							{
								biaozhi=0;
								flag_4=1;
							}
					}
		}
	if((i==10)&&(key_2==0))//采集到十个数据后灯开始闪烁
		{
			++tt_2;
			if(tt_2==100)
				{
					tt_2=0;
					if(flag_3==0)
						{
							flag_3=1;
							led(0xff);
						}
					else 
						{
							flag_3=0;
							led(0xfe);
						}
				}
		}
}