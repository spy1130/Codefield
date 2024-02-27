#include <STC15F2K60S2.H>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int	
	
sbit scl=P2^0;
sbit sda=P2^1;

uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XFF};
uchar code dot[]={0X40,0x79,0x24,0x30,0x19,0x12,0x20,0x78,0x00,0x10};

uchar yi,er,san,si,wu ,liu,qi,ba;
uchar tt=0;
uint ML=0;
uint money=0;
uchar guan=0;

void delayms(int ms)
{
	int i,j;
	for(i=ms;i>0;i--)
		for(j=845;j>0;j--);
}

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
		P0=dot[er];
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
		P0=dot[liu];
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

void Timer0Init(void)		//5毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x28;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

void rel(uchar dong)
{
	if(dong==0)
	{
		P2=0XA0;P0=0X00;//全关
	}
	else if(dong==1)
	{
		P2=0XA0;P0=0X40;//开蜂鸣器 
	}
	else if(dong==2)
	{
		P2=0XA0;P0=0X10;//开继电器
	}
	else if(dong==3)
	{
		P2=0XA0;P0=0Xff;//全开
	}
}

void keyscan()
{
	if(P30==0)
	{
		delayms(5);
		if(P30==0)
		{
			rel(2);
			wu=0;
			liu=0;
			qi=0;
			ba=0;
			EA=1;
			ET0=1;
		}
		while(!P30);
	}
	else if(P31==0)
	{
		delayms(5);
		if(P31==0)
		{
			EA=0;
			ET0=0;
			rel(0);
			money=ML*5;
			wu=money/100000;
			liu=money%100000/10000;
			qi=money%10000/1000;
			ba=money%1000/100;
			ML=0;	
		}
		while(!P31);
	}
}

void iicdelay(uchar m)
{
	do
	{
		_nop_();
	}
	while(m--);
}

void iicstrat()
{
	sda=1;
	_nop_();
	scl=1;
	_nop_();
	sda=0;
	_nop_();
	scl=0;
	_nop_();
}


void iicstop()
{
	sda=0;
	_nop_();
	scl=1;
	_nop_();
	sda=1;
	_nop_();
}

void writebyte(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		scl=0;
		sda=dat&0x80;
		scl=1;
		dat<<=1;
	}
	scl=0;
}


uchar iicreadbyte()
{
	uchar dat;
	uchar i;
	for(i=0;i<8;i++)
	{
		scl=1;
		iicdelay(5);
		dat<<=1;
		if(sda)
		{
			dat|=0x01;
		}
		scl=0;
	}
	
	return dat;
}

uchar ack()
{
	scl=1;
	iicdelay(5);
	if(sda==1)
	{
		scl=0;
		iicstop();
		return 0;
	}
	else 
	{
		scl=0;
		return 1;
	}
}

uchar iicread(uchar add)
{
	uchar temp;
	
	iicstrat();
	writebyte(0x90);
	ack();
	writebyte(add);
	ack();
	iicstop();
	
	iicstrat();
	writebyte(0x91);
	ack();
	temp=iicreadbyte();
	iicstop();
	
	temp=0.39*temp;
	
	return temp;
}

void adrun()
{
	uchar num;
	num=iicread(0x01);
	if(num<25)
	{
		P2=0X80;P0=0XFE;
	}
	else 
	{
		P2=0X80;P0=0XFF;
	}
}

void main()
{
	allinit();
	Timer0Init();
	
	yi=10;er=0;san=5;si=0;wu=0;liu=0;qi=0;ba=0;
	while(1)
	{
		adrun();
		keyscan();
		display1(yi,er);
		display2(san,si);
		display3(wu,liu);
		display4(qi,ba);
	}
}


void time0() interrupt 1
{
	tt++;
	if(tt==20)
	{
		tt=0;
		ML=ML+10;
		wu=ML/10000;
		liu=ML%10000/1000;
		qi=ML%1000/100;
		ba=ML%100/10;//qiu=ML%10;
		if(ML>99990)
		{
			EA=0;ET0=0;
			rel(0);
			money=ML*5;
			wu=money/100000;
			liu=money%100000/10000;
			qi=money%10000/1000;
			ba=money%1000/100;
			ML=0;	
		}
	}
}











