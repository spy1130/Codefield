/**********************************************************************************
实验功能：第七届省赛试题程序设计
时间：2107.12.20
作者：吴康
***********************************************************************************/
#include <STC15F2K60S2.H>
#include <onewire.h>

#define uchar unsigned char 
#define uint unsigned int	

uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF,0XC6};
uchar yi,er,san,si,wu ,liu,qi,ba;	
uchar moshi=1;										//工作模式变量
uchar shijian=0;									//倒计时变量
uchar shiwen=0;										//温度存储变量
uint num=0,miao=0;								//定时用的标志位

/**********************************************************************************
函数名称：delayms
功能：延时函数
***********************************************************************************/
void delayms(int ms)
{
	int i,j;
	for(i=ms;i>0;i--)
		for(j=940;j>0;j--);
}

/**********************************************************************************
函数名称：keyscan
功能：按键扫描函数
***********************************************************************************/
void keyscan()
{
	if(P33==0)									//模式选择按键
	{
		delayms(9);
		if(P33==0)
			{
				if(er==1) moshi=2;
				else if(er==2) moshi=3;
				else if(er==3) moshi=1;
			}
		while(!P33);
	}
	else if(P32==0)							//时间选择按键
		{
			delayms(9);
			if(P32==0)
				{
					if(shijian==0) shijian=60;
					else if(shijian<=60) shijian=120;
					else if(shijian<=120) shijian=0;
				}
			while(!P32);
		}
	else if(P31==0)							//时间清除按键
		{
			delayms(9);
			if(P31==0) shijian=0;
			while(!P31);
		}
	else if(P30==0)							//温度显示按键
		{
			delayms(9);
			if(P30==0)
				{	
					if(shiwen==0) shiwen=1;
					else if(shiwen==1) shiwen=0;
				}
			while(!P30);
		}
}

/**********************************************************************************
函数名称：display
功能：显示函数
***********************************************************************************/
void display1(uchar yi,uchar er)
{
		P2=0XC0;//打开位选573   U8
		P0=0X01;//选择第一个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[yi];
		delayms(1);
		
		P2=0XC0;//打开位选573   U8
		P0=0X02;//选择第二个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[er];
		delayms(1);
}	

void display2(uchar san,uchar si)
{
		P2=0XC0;//打开位选573   U8
		P0=0X04;//选择第三个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[san];
		delayms(1);
		
		P2=0XC0;//打开位选573   U8
		P0=0X08;//选择第四个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[si];
		delayms(1);
}

void display3(uchar wu,uchar liu)
{
		P2=0XC0;//打开位选573   U8
		P0=0X10;//选择第一个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[wu];
		delayms(1);
		
		P2=0XC0;//打开位选573   U8
		P0=0X20;//选择第一个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[liu];
		delayms(1);
}

void display4(uchar qi,uchar ba)
{
		P2=0XC0;//打开位选573   U8
		P0=0X40;//选择第一个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[qi];
		delayms(1);
		
		P2=0XC0;//打开位选573   U8
		P0=0X80;//选择第一个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[ba];
		delayms(1);
}

/**********************************************************************************
函数名称：Timer0Init
功能：定时器0初始化函数
***********************************************************************************/
void Timer0Init(void)		//100微秒@11.0592MHz
{
	AUXR |= 0x80;					//定时器时钟1T模式
	TMOD &= 0xF0;					//设置定时器模式
	TL0 = 0xAE;						//设置定时初值
	TH0 = 0xFB;						//设置定时初值
	TF0 = 0;							//清除TF0标志
	TR0 = 1;							//定时器0开始计时
	EA=1;
	ET0=1;
}

/**********************************************************************************
函数名称：main
功能：主函数
***********************************************************************************/
void main()
{
	allinit();													//板子整体初始化
	Timer0Init();												//定时器0初始化
	yi=10;er=1;san=10;si=11;wu=0;liu=0;qi=0;ba=0;
	while(1)
	{
		keyscan();												//按键扫描函数
		if(shiwen==0)											//初始界面显示
			{
				yi=10;
				san=10;
				si=11;
				er=moshi;
				wu=shijian/1000;
				liu=shijian%1000/100;
				qi=shijian%100/10;
				ba=shijian%10;
			}
		else if(shiwen==1)								//进入温度显示界面
			{
				yi=10;
				er=4;
				san=10;
				si=11;
				wu=11;
				liu=temget()/10;
				qi=temget()%10;
				ba=12;
				delayms(1);
			}		
		if(shijian>0)											//判断是那个模式选择亮那个灯
		{
			if(moshi==1)
				{	
					P2=0X80;
					P0=0XFE;
				}
			else if(moshi==2)
				{	
					P2=0X80;
					P0=0XFD;
				}
			else if(moshi==3)
				{	
					P2=0X80;
					P0=0XFB;
				}
		}
		else 
			{	
				P2=0X80;
				P0=0XFF;
			}
			display1(yi,er);									//显示函数
			display2(san,si);
			display3(wu,liu);
			display4(qi,ba);
		}
}

/**********************************************************************************
函数名称：Timer0
功能：定时器0的中断服务函数
***********************************************************************************/
void Timer0() interrupt 1
{
	num++;miao++;
	if(num>10) num=1;												//设置周期
	if(miao==10000)													//定时时间1s
		{
			miao=0;
			if(shijian>0) shijian--;
		}
	if(shijian>0)														//判断倒计时是否结束，若结束则不输出PWM波
		{
			if(moshi==1)
				{
					if(num<3) P34=1;
					else P34=0;
				}
			else if(moshi==2)
				{
					if(num<4) P34=1;
					else P34=0;
				}
			else if(moshi==3)
				{
					if(num<8) P34=1;
					else P34=0;
				}
		}
}


