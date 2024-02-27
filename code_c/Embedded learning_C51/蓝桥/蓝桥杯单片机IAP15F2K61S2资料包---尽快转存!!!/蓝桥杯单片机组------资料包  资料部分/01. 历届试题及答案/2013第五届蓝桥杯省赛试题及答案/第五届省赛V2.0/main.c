/****************************************************************************************
实验功能：第五届省赛试题练习
时间：2017.12.17
作者：吴康
****************************************************************************************/
#include <STC15F2K60S2.h>
#include <delay.h>
#include <onewire.h>
#define uchar unsigned char 

uchar dsbuff[8];
unsigned char code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
uchar discount = 0;										//数码管移位控制变量
uchar count_1,count_2=0;							//定时扫描用的变量
uchar tem,flag_1,flag_2=0,flag_3=0;		//定时扫描用的变量
uchar tem_max=30,tem_min=20;					//初始化温度上限下限
uchar gongzuo;												//工作号变量
uchar num[4];													//存储矩阵键值的变量
uchar cishu=0;												//判断按键按的次数，如果按了四次则设置好了
uchar clear=0,shezhi=0;								//设置按键和清除按键初值
uchar qingchu=0;											//清除标志位
uchar tt=0,fan=0;											//灯亮时间的控制变量
uchar liang_1=1,liang_2=0;						//亮哪一个灯控制变量

/****************************************************************************************
函数名称：keyscan
功能：按键处理函数
****************************************************************************************/
void keyscan()
{	
	uchar temp;
	P3=0x7f;
	P44=0;P42=1;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay5ms();
		Delay5ms();
		temp=P3;
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			if(shezhi==1)
			{
				switch(temp)
				{
					case 0x0e:num[cishu]=0;cishu++;break;
					case 0x0d:num[cishu]=3;cishu++;break; 
					case 0x0b:num[cishu]=6;cishu++;break; 
					case 0x07:num[cishu]=9;cishu++;break;  
				}
			}
		}
		while(P3!=0x7f);
	}
	P3=0xbf;
	P44=1;P42=0;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay5ms();
		Delay5ms();
		temp=P3;
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			switch(temp)
			{
				case 0x0e:
				if(shezhi==1){num[cishu]=1;cishu++;}break;
				case 0x0d:
				if(shezhi==1){num[cishu]=4;cishu++;}break; 
				case 0x0b:
				if(shezhi==1){num[cishu]=7;cishu++;}break; 
				case 0x07:
				{
					if(shezhi==0) {shezhi=1;cishu=0;}
					else if((shezhi==1)&&(flag_3==0)) shezhi=0;
				}
				break;  
			}	
		}
		while(P3!=0xbf);
	}

	P3=0xdf;
	P42=1;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay5ms();
		Delay5ms();
		temp=P3;
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			if(shezhi==1)
			{
				switch(temp)
				{
					
					case 0x0e:num[cishu]=2;cishu++;break;
					case 0x0d:num[cishu]=5;cishu++;break; 
					case 0x0b:num[cishu]=8;cishu++;break; 
					case 0x07:{clear=1;cishu=0;qingchu=1;}break;  
				}
			}	
		}
		while(P3!=0xdf);
	}
}

/****************************************************************************************
函数名称：display
功能：数码管显示函数
****************************************************************************************/
void display()
{
	P2 = (P2&0x1f)|0xe0;		//打开段选锁存器
	P0 = 0xff;							//关闭所有数码管
	P2 = P2&0x1f;
		
	P2 = (P2&0x1f)|0xc0;		//打开位选锁存器
	P0 = 1<<discount;
	P2 = P2&0x1f;
	
	P2 = (P2&0x1f)|0xe0;		//打开段选锁存器
	P0 = tab[dsbuff[discount]];
	P2 = P2&0x1f;

	if(++discount==8)
	 discount = 0;
}

/****************************************************************************************
函数名称：LED
功能：LED亮灭控制函数
****************************************************************************************/
void LED(uchar liang)
{
	P2=P2&0x1f;
	P2=P2|0x80;
	P0=liang;
	P2=P2&0x1f;
}

/****************************************************************************************
函数名称：JIDIANQI
功能：继电器通断控制函数
****************************************************************************************/
void JIDIANQI(uchar xiang)
{
	P2=P2&0x1f;
	P2=P2|0xa0;
	P0=xiang;
	P2=P2&0x1f;
}

/****************************************************************************************
函数名称：allinit
功能：板子初始化函数
****************************************************************************************/
void allinit()
{
	P2=(P2&0x1f)|0x80;P0=0xff;
	P2=(P2&0x1f)|0xa0;P0=0x00;
	P2=(P2&0x1f)|0xc0;P0=0xff;
	P2=(P2&0x1f)|0xe0;P0=0xff;
}

/****************************************************************************************
函数名称：Timer1Init
功能：定时器1初始化函数
****************************************************************************************/
void Timer1Init(void)		//5毫秒@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x00;			//设置定时初值
	TH1 = 0x28;			//设置定时初值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
}

/****************************************************************************************
函数名称：Timer0Init
功能：定时器0初始化函数
****************************************************************************************/
void Timer0Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x9A;			//设置定时初值
	TH0 = 0xA9;			//设置定时初值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
}

/****************************************************************************************
函数名称：main
功能：主函数
****************************************************************************************/
void main()
{
	allinit();
	Timer0Init();
	Timer1Init(); 
	EA=1;
	ET0=1;
	ET1=1;
	while(1)
	{
		if(flag_2==1)
		{
			keyscan();
			flag_2=0;
		}	
	   if(shezhi==0)		//初始显示温度工作状态界面
			{
				dsbuff[7]=tem%10;
				dsbuff[6]=tem/10;
				dsbuff[5]=11;
				dsbuff[0]=10;
				dsbuff[1]=gongzuo;
				dsbuff[2]=10;
				dsbuff[3]=11;
				dsbuff[4]=11;

				if(flag_1==1)
					{
						tem=wendu();
						flag_1=0;	
					}
				if((tem<=tem_max)&&(tem>=tem_min))
					{
						gongzuo=1;
						JIDIANQI(0x00);
					}
				else if	(tem>tem_max)
					{
						gongzuo=2;
						JIDIANQI(0x10);
					}
				else 
				{
					gongzuo=0;
					JIDIANQI(0x00);
				}
			}
		else 					   //进入设置温度上下限界面
			{
				dsbuff[7]=num[3];
				dsbuff[6]=num[2];
				dsbuff[5]=10;
				dsbuff[0]=10;
				dsbuff[1]=num[0];
				dsbuff[2]=num[1];
				dsbuff[3]=11;
				dsbuff[4]=11;
				if(clear==1)								//清除按键按下后全部清零
				 {
						clear=0;
						num[0]=0;
						num[1]=0;
						num[2]=0;
						num[3]=0;
				 }
				 if((cishu==4)||(qingchu==1))//当按键按了四次时进行判断
				 {
							qingchu=0;
							tem_max=num[0]*10+num[1];//数据处理
							tem_min=num[2]*10+num[3];
							if(tem_max<tem_min)
							{
										flag_3=1;
										LED(0xfd);
										liang_1=0;
										liang_2=1;
							}
						else 
							{
										flag_3=0;
										LED(0xff);
										liang_1=1;
										liang_2=0;
							}
					}	
			}
	}
}
/****************************************************************************************
函数名称：time_1
功能：定时器1的中断服务函数
****************************************************************************************/
void time_1() interrupt 3
{
	++tt;
	if(gongzuo==0)
	{
		if(tt==160)
		{
			tt=0;
			if(fan==0)						 
			{
				fan=1;
				LED(0xfe);
			}
			else 
			{
				fan=0;
				LED(0xff);
			}
		}
	}
	else if(gongzuo==1)
	{

		if(tt==80)
		{
			tt=0;
			if(fan==0)
			{
				fan=1;
				LED(0xfe);
			}
			else 
			{
				fan=0;
				LED(0xff);
			}
		}	
	}
	else if(gongzuo==2)
	{ 
		if(tt==40)
		{
			tt=0;
			if(fan==0)
			{
				fan=1;
				LED(0xfe);
			}
			else 
			{
				fan=0;
				LED(0xff);
			}
		}
	}
}

/****************************************************************************************
函数名称：time_0
功能：定时器0的中断服务函数
****************************************************************************************/
void time_0() interrupt 1
{
	display();			//显示函数定时显示
	if(++count_1==100)
	{
		flag_1=1;
		count_1=0;	
	}
	if(++count_2==1)
	{
		count_2=0;
		flag_2=1;
	}
}

