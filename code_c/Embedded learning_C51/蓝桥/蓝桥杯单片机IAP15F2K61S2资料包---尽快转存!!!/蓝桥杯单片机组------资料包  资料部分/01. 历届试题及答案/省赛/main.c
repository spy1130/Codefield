#include <STC15F2K60S2.h>
#include "ds1302.h"
#include "onewire.h"
#include "DS.h"

sbit S7 = P3^0;    //进入采集时间设置页面
sbit S6 = P3^1;		 //
sbit S5 = P3^2;		 //确认采集时间，退出采集时间设置页面
sbit S4 = P3^3;    //切换温度采集间隔时间

bit s7_flag = 0;
bit s6_flag = 0;
bit s5_flag = 0;
bit s4_flag = 0;

bit L1_flag = 0; //0时L1不闪烁， 1闪烁
bit tt3_flag = 0;
bit tt2_flag = 0;

bit f;
unsigned char ChaTime = 0, tempCha = 0; 
unsigned char Dsplaywei = 1; 	
unsigned char tt = 0, tt1 = 0, tt2 = 0, tt3 = 0;
unsigned char Temperature[10];
unsigned char cha[4] = {1, 5, 30, 60};
void Delay5ms();
void Keypress();
void Timer0Init(void);
void Close_BUZZ();
void Timer1Init(void);		//10毫秒@11.0592MHz
void main()
{
   //选择温度检测时间间隔
 //1为温度设置页面，2为时钟页面，3为显示页面

	unsigned char tmperwei = 0;
	Close_BUZZ();
	Timer0Init();
	Init_DS1302();
	Timer1Init();

	while(1)
	{
		Keypress();

		if ((L1_flag == 1) && (tt3_flag))
		{
			P0 = 0xFF;
			P2 = 0x80;
			if (f == 0)
			{
				P00 = 1;
				f = ~f;
			}
			else
			{
				P00 = 0;	
				f = ~f;				
			}	
			P2 = 0x00;
		}
		
		
		if (s7_flag == 1)  
		{
			s7_flag = 0;
			
			if (Dsplaywei == 3)
			{
				Dsplaywei = 1;
			}		
		}
		
		if (s6_flag == 1)
		{
			s6_flag = 0;
			if (Dsplaywei == 3)
			{
				if (L1_flag == 1)
				{
					L1_flag = 0;
				}
				
				tmperwei++; 
				if (tmperwei == 10)
				{
					tmperwei = 0;
				}
			}
		}
		
		if (s5_flag == 1)
		{
			s5_flag = 0;
			if (Dsplaywei == 1)
			{
				Dsplaywei = 2;	
				ChaTime = tempCha;
				TR0 = 1;
				Temperature[tt2] = rd_temperature();
			}
		}

		if (s4_flag == 1)
		{
			s4_flag = 0;
			
			if (Dsplaywei == 1)
			{
				tempCha++;	
				if (tempCha == 4)
				{
					tempCha = 0;
				}	
			}	
		}	
		
		if (tt2_flag == 1)
		{
			tt2_flag = 0;
			Temperature[tt2] = rd_temperature();
			Dsplaywei = 3;
			L1_flag = 1;
		}
		
		if (Dsplaywei == 1)
		{
			DS1(10,10);
			DS2(10);
			DS3(10,10);
			DS4(11);
			DS5(cha[tempCha]/10,cha[tempCha]%10);
		}
		
		if (Dsplaywei == 2)
		{
			Read_DS1302();
			DS1(DisplayTime[0]>>4,DisplayTime[0]&0x0F);
			if (tt3_flag)
			{
				DS2(11);
				DS4(11);
			}
			else
			{
				DS2(10);
				DS4(10);
			}
			DS3(DisplayTime[1]>>4,DisplayTime[1]&0x0F);	
			DS5(DisplayTime[2]>>4,DisplayTime[2]&0x0F);
		}
		
		if (Dsplaywei == 3)
		{
			DS1(11,0);
			DS2(tmperwei);
			DS3(10,10);
			DS4(11);
			DS5(Temperature[tmperwei]/10,Temperature[tmperwei]%10);
		}		
	}
}

void Close_BUZZ()
{
	P2 = 0xA0;
	P0 = 0x00;
	P2 = 0x80;
	P0 = 0xFF;
	P2 = 0x00;
}

void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 54;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}

void Keypress()
{
	if (S7 == 0)
	{
		Delay5ms();
		if (S7 == 0)
		{
			s7_flag = 1;
		}
		while(S7 == 0);
	}
	
	if (S6 == 0)
	{
		Delay5ms();
		if (S6 == 0)
		{
			s6_flag = 1;
		}
		while(S6 == 0);
	}
	
	if (S5 == 0)
	{
		Delay5ms();
		if (S5 == 0)
		{
			s5_flag = 1;
		}
		while(S5 == 0);
	}	
		
	if (S4 == 0)
	{
		Delay5ms();
		if (S4 == 0)
		{
			s4_flag = 1;
		}
		while(S4 == 0);
	}
}

void Timer0Init(void)		//10毫秒@11.0592MHz 
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	
	ET0 = 1;
	EA = 1;
}

void Timer0interrupt() interrupt 1
{
	if (++tt == 100) //1秒
	{
		tt = 0;
		if (++tt1 == cha[ChaTime])
		{
			tt1 = 0;
			tt2_flag = 1;
			tt2++;
			if (tt2 == 9)
			{
				TL0 = 0x00;		//设置定时初值
				TH0 = 0xDC;		//设置定时初值
				TR0 = 0;
				tt2 = 0;
			}
			else
			{
				TL0 = 0x00;		//设置定时初值
				TH0 = 0xDC;		//设置定时初值				
			}
		}
	}
	TF0 = 0;
}


void Timer1Init(void)		//10毫秒@11.0592MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x00;		//设置定时初值
	TH1 = 0xDC;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	
	ET1 = 1;
	EA = 1;
}

void Timer1interrupt() interrupt 3
{
	if (++tt3 == 100) //1秒
	{

		
		tt3_flag = ~tt3_flag;
		
	}
}

