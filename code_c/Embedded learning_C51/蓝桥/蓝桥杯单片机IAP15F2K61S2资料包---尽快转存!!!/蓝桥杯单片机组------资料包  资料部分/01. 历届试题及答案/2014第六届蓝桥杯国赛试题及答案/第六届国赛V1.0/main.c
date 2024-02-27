#include <STC15F2K60S2.H>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int	

sbit TX=P1^0;  //发射引脚
sbit RX=P1^1;  //接收引脚

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_(); _nop_();}
uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
uchar dsbuff[8]={11,11,11,11,11,11,11,11};
uchar discount;
uint count=0;
uchar flag;
uint t=0;

void allinit()
{
	P2=0XA0;
	P0=0X00;//关闭蜂鸣器，继电器
	
	P2=0X80;
	P0=0XFF;//关闭LED灯
	
	P2=0XC0;
	P0=0XFF;//选择所有数码管
	P2=0XFF;
	P0=0XFF;//关闭所有数码管
}

void display()
{
	P2=0XEF;
	P0=0XFF;
	P2=0X1F;
	
	P2=0XCF;
	P0=1<<discount;
	P2=0X1F;
	
	P2=0XEF;
	P0=tab[dsbuff[discount]];
	P2=0X1F;
	
	if(++discount==8) discount=0;
}

void Timer0Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x80;					//定时器时钟1T模式
	TMOD &= 0xF0;					//设置定时器模式
	TL0 = 0x9A;						//设置定时初值
	TH0 = 0xA9;						//设置定时初值
	TF0 = 0;							//清除TF0标志
	TR0 = 1;							//定时器0开始计时
	EA = 1;								//打开总中断
	ET0 = 1;  						//打开定时器0中断
}

void Timer1Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x40;					//定时器时钟1T模式
	TMOD &= 0x0F;					//设置定时器模式
}

//TX引脚发送40KHz方波信号驱动超声波发送探头
//使用软件延时注意RC振荡器频率
void send_wave(void)
{
	uchar i=8;  //发送8个脉冲
	do
	{
		TX = 1;
		somenop;somenop;somenop;somenop;somenop;somenop;somenop;somenop;somenop;somenop;		
		TX = 0;
		somenop;somenop;somenop;somenop;somenop;somenop;somenop;somenop;somenop;somenop;		
	}
	while(i--);
}

void main()
{
	uint distance;
	Timer0Init();
  Timer1Init();
	allinit();
	while(1)
		{
			if(flag==1) // 200毫秒更新一次数据 
				{
					flag=0;
					send_wave();  //发送方波信号
					TR1=1;  //启动计时
					while((RX==1) && (TF1==0));  //等待收到脉冲
					TR1=0;  //关闭计时		
					if(TF1==1)//发生溢出
						{
							TF1=0;
							distance=999;  //无返回
						}
					else
						{		
							t=TH1;// 计算时间  
							t<<=8;
							t|=TL1;
							distance=(uint)(t*0.017);  //计算距离	
							distance=distance/12;
							//t*340   // t/1000000*340/2*100
						}
					TH1=0;
					TL1=0;
				}
				dsbuff[5]=distance/100;
				dsbuff[6]=distance%100/10;
				dsbuff[7]=distance%10; 
	}
}

void Timer0(void)  interrupt 1  
{
	TL0=0x9A;		
	TH0=0xA9;		
	display();
	if(++count==100)
	{
		flag=1;
		count=0;
  }
}
