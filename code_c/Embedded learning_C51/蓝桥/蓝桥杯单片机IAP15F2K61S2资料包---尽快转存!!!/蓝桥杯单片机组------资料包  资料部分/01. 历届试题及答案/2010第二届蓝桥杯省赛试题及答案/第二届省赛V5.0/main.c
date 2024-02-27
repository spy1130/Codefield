/*******************************************************************************
实验功能：第二届省赛试题程序设计
时间：2017.12.23
作者：吴康
*******************************************************************************/
#include <STC15F2K60S2.h>
#include <24c02.h>
#include <ds18b20.h>

#define uint unsigned int
#define uchar unsigned char

uchar code tab[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar dsbuff[8]={11,11,11,11,11,11,11,11};
uchar DQTH,DQTL,temp;//设定温度上下限变量
uchar discount;			 //数码管显示变量
uchar flag;					 //读取温度的标志位

/*******************************************************************************
函数名称：delayms
功能：延时函数
*******************************************************************************/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
	for(j=110;j>0;j--);
}

/*******************************************************************************
函数名称：display
功能：显示函数
*******************************************************************************/
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

/*******************************************************************************
函数名称：warning
功能：超限输出PWM函数
*******************************************************************************/
void warning()
{
	static bit relay,pwmkey;
	if(DQTH<temp&&pwmkey==0)
		{
			pwmkey=1;
			TR1=1;	
		}
	if(DQTH>=temp&&pwmkey==1)
		{
			pwmkey=0;
			TR1=0;
			P34=1;
		}
	if(DQTL<=temp)
		{
			if(relay)
				{
					relay=0;
					P0=0x00;
					P2&=0x1f;
					P2|=0xa0;
					P2&=0x1f;	
				}
		}
	if(DQTL>temp)
		{
			if(relay==0)
				{
					relay=1;
					P0=0x00;
					P2&=0x1f;
					P2|=0xa0;
					P04=1;
					P2&=0x1f;
				}		
		}
}

/*******************************************************************************
函数名称：key
功能：按键函数
*******************************************************************************/
void key()
{
	if(P33==0)
		{
			delayms(10);
			if(P33==0)
				{
					DQTL--;
					if(DQTL>100) DQTL=0;
					at24c02_wirte(0x01,DQTL);
				}
				while(!P33);
		}
	
	if(P32==0)
		{
			delayms(10);
			if(P32==0)
				{
					DQTH--;
					if(DQTH>100) DQTH=0;
					if(DQTL>DQTH) DQTH=DQTL;
					at24c02_wirte(0x00,DQTH);
				}
			while(!P32);
		}
	
	if(P31==0)
		{
			delayms(10);
			if(P31==0)
				{
					DQTL++;
					if(DQTL>=100) DQTL=99;
					if(DQTL>DQTH) DQTL=DQTH;
					at24c02_wirte(0x01,DQTL);
				}
				while(!P31);
		}
	
	if(P30==0)
		{
			delayms(10);
			if(P30==0)
				{
					DQTH++;
					if(DQTH>=100) DQTH=99;
					at24c02_wirte(0x00,DQTH);
				}
				while(!P30);
		}
}
/*******************************************************************************
函数名称：Timer0Init
功能：定时器0初始化函数
*******************************************************************************/
void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;					//定时器时钟1T模式
	TMOD &= 0xF0;					//设置定时器模式
	TL0 = 0xCD;						//设置定时初值
	TH0 = 0xD4;						//设置定时初值
	TF0 = 0;							//清除TF0标志
	TR0 = 1;							//定时器0开始计时
	EA=1;
	ET0=1;
}

/*******************************************************************************
函数名称：Timer1Init
功能：定时器1初始化函数
*******************************************************************************/
void Timer1Init(void)		//100微秒@11.0592MHz
{
	AUXR |= 0x40;					//定时器时钟1T模式
	TMOD &= 0x0F;					//设置定时器模式
	TL1 = 0xAE;						//设置定时初值
	TH1 = 0xFB;						//设置定时初值
	EA=1;
	ET1=1;
}

/*******************************************************************************
函数名称：allinit
功能：板子初始化函数
*******************************************************************************/
void allinit()
{
		P2=0X80;P0=0XFF;
		P2=0XA0;P0=0X00;
		P2=0XC0;P0=0XFF;
		P2=0XE0;P0=0XFF;
}

/*******************************************************************************
函数名称：main
功能：主函数
*******************************************************************************/
void main()
{
	allinit();							//板子整体初始化
	DQTH=at24c02_read(0x00);//读0x00地址里的数据
	DQTL=at24c02_read(0x01);//读0x01地址里的数据
	Timer0Init();						//定时器0初始化
	Timer1Init();						//定时器1初始化
	while(1)
	{
			if(flag==1)					//定时读取温度
				{
					flag=0;
					temp=read_ds18b20();
				}
			warning();					//报警函数
			dsbuff[7]=temp%10;
			dsbuff[6]=temp/10;
			dsbuff[5]=11;
			dsbuff[4]=11;
			dsbuff[3]=DQTL%10;
			dsbuff[2]=DQTL/10;
			dsbuff[1]=DQTH%10;
			dsbuff[0]=DQTH/10;
	}
}

/*******************************************************************************
函数名称：Timer0
功能：定时器0中断服务函数
*******************************************************************************/
void Timer0() interrupt 1
{
	uint i;
	display();
	key();
	i++;
	if(i==4)
		{
			flag=1;
			i=0;
		}
}

/*******************************************************************************
函数名称：Timer1
功能：定时器1中断服务函数
*******************************************************************************/
void Timer1() interrupt 3
{
	static unsigned char i;
	if(i<3&&P34==0)
	P34=1;
	else if(i>3&&i<10&&P34==1)
	P34=0;
	i++;
	if(i==10)
	i=0;
}
