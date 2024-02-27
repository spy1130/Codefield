#include <STC15F2K60S2.h>
#include <delay.h>
#include <key.h>
#include <iic.h>

#define uchar unsigned char 
#define uint unsigned int

#define TX P10
#define RX P11

uchar ceju_flag=0,count_2=0,time_1,time_2;
uchar led_state=0;
uchar liang_1,liang_2;
uint count_3=0,k;	
uchar flag_1=0;
uchar count_4=0,count_5,liang4=0;
uchar discount=0,count_1=0,ceju_finish=0;
uchar s_liang;
uchar tab[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar dsbuff[8]={1,2,3,4,5,6,7,8};
uchar vol,type; 
uint distance;

void Timer0Init(void);
void display();
void pressure_handle();
void type_identify();
void Timer1Init(void); 
void sendwave();
uint get_distance();
void key_handle();
void led_shan(uchar liang);



void allinit()
{
	P2=0x80;P0=0xff;
	P2=0xa0;P0=0x00;
	P2=0xc0;P0=0xff;
	P2=0xe0;P0=0xff;
}
void led(uchar lednum,uchar demand)
{
	uchar i;
	lednum--;
	i=led_state&(0x01<<lednum);
	if((i==0)&&(demand==1))	
	{
		EA=0;
		P2=P2&0x1f;	
		P0=~(0x01<<lednum);
		P2|=0x80;
		P2=P2&0x1f;
		led_state=0x01<<lednum;
		EA=1;
	}
	else if((i>0)&&(demand==0))
	{
		EA=0;
		P2=P2&0x1f;	
		P0=0xff;
		P2|=0x80;
		P2=P2&0x1f;	
		led_state=0;
		EA=1;	
	}
}


void main()
{
	allinit();
	Timer0Init();
	Timer1Init();
	time_1=iicread(0x01);
	Delay2ms();
	time_2=iicread(0x02);
	Delay2ms();
	while(1)
	{	
		pressure_handle();
		keyscan();//按键判断 数码管冲突问题考虑
		if(normal==1)
		{
			type_identify();//货物类型识别
			if((ceju_finish==1) && (s4==0))
			{
				if(type==1) time = time_1;
				else time = time_2;			
			}
		}
		key_handle();
	}
}

void key_handle()
{
	if(s4==1)
	{  
		if(time>0)
		{ 	
			jidian(1);
			if(s5==0)
			{
				dsbuff[0]=2;
				dsbuff[1]=11;
				dsbuff[2]=11;
				dsbuff[3]=11;
				dsbuff[4]=11;
				dsbuff[5]=11;
				dsbuff[6]=time/10;
				dsbuff[7]=time%10;
			}
			else
			{
				jidian(0);
				dsbuff[0]=2;
				dsbuff[1]=11;
				dsbuff[2]=11;
				dsbuff[3]=11;
				dsbuff[4]=11;
				dsbuff[5]=11;
				dsbuff[6]=time/10;
				dsbuff[7]=time%10;
			}
		}
		else jidian(0);				
	}
	if(s6==1)
	{
		if(ss==1)
		{
			if(s7==1)
			{
				s7=0;
				time_1++;
				if(time_1>10) time_1=1;
			}
		}
		if(ss==2)
		{
			if(s7==1)
			{
				s7=0;
				time_2++;
				if(time_2>10) time_2=1;
			}	
		}
		if(ss==3)
		{
			iicwrite(0x01,time_1);
			Delay2ms();
			iicwrite(0x02,time_2);
		}
	}
}
void type_identify()
{
	if(ceju_flag==1)
		{
			distance=get_distance();
			ceju_flag=0;
		}
	if(distance<=30) type=1;
	else type=2;
	if(s4==0)
	{
		dsbuff[0]=1;
		dsbuff[1]=11;
		dsbuff[2]=11;
		dsbuff[3]=distance/10;
		dsbuff[4]=distance%10;
		dsbuff[5]=11;//这里可以显示采集的电压
  	dsbuff[6]=11;
		dsbuff[7]=type;
	}	
	ceju_finish=1;				
}
void pressure_handle() //空载过载检测函数
{
	if(++k==1000)				  //Ad采集不能连续采 用1000次采一次 比较稳定  按键会跳回 是否是供电的问题
	{
	  vol = adread(0x03);
		k=0;
	}
	if(vol<51)
	{		
		if(kongzai==0)	
		{
			led(1,1);
			fengming(0);
		}	
		kongzai=1;
		normal=0;
		guozai=0;
		if(s6==0)
	   {
			dsbuff[0]=11;dsbuff[1]=11;dsbuff[2]=11;dsbuff[3]=11;
			dsbuff[4]=11;dsbuff[5]=11;dsbuff[6]=11;dsbuff[7]=11;
		 }

	}
	else if((vol>=51)&&(vol<204))
	{	
		kongzai=0; guozai=0;
		if((normal==0)&&(s5==0))
		  {
				led(2,1);
				fengming(0);
			}
		normal=1;
	}
	else if(vol>=204)
	{	
		kongzai=0;normal=0;s4=0;s6=0;
		if((guozai==0)&&(normal==0))
		fengming(1);
		guozai=1;
	}
		
}

void time0() interrupt 1
{
	display();
	if(s6==1)
	{
		if(++count_5==250)
		{
			count_5=0;
			if(ss==1)
			{
				if(liang_1==0)
				{
					liang_1=1;
					dsbuff[0]=3;
					dsbuff[1]=11;
					dsbuff[2]=11;
					dsbuff[3]=11;
					dsbuff[4]=11;
					dsbuff[5]=11;
					dsbuff[6]=time_2/10;
					dsbuff[7]=time_2%10;
				}
				else 
				{
					liang_1=0;					
					dsbuff[0]=3;
					dsbuff[1]=11;
					dsbuff[2]=11;
					dsbuff[3]=time_1/10;
					dsbuff[4]=time_1%10;
					dsbuff[5]=11;
					dsbuff[6]=time_2/10;
					dsbuff[7]=time_2%10;
				}
			}
			if(ss==2)
			{	
				if(liang_2==0)
				{
					liang_2=1;								
					dsbuff[0]=3;
					dsbuff[1]=11;
					dsbuff[2]=11;
					dsbuff[3]=time_1/10;
					dsbuff[4]=time_1%10;
					dsbuff[5]=11;
					dsbuff[6]=11;
					dsbuff[7]=11;
	
				}
				else 
				{
					liang_2=0;					
					dsbuff[0]=3;
					dsbuff[1]=11;
					dsbuff[2]=11;
					dsbuff[3]=time_1/10;
					dsbuff[4]=time_1%10;
					dsbuff[5]=11;
					dsbuff[6]=time_2/10;
					dsbuff[7]=time_2%10;
				}
			}
			if(ss==3) //适合补ss不循环的时候 补最后一个显示即可	  //ss循环时 由于两种闪状态不亮的是互补的 则不需要补显示
			{
				if((dsbuff[6]==11)&&(dsbuff[7]==11))
				{
					dsbuff[6]=time_2/10;
					dsbuff[7]=time_2%10;
				}
			}
	  }
	}
  if((s4==1)&&(s5==0))	//倒计时程序
	{
		++count_3;
		if(count_3==500)
		{
			count_3=0;
			if(time>=1)
			time--;
		}	
	}
	if(s5==1)  //紧急停止闪烁
	{
		++count_4;
		if(count_4==250)
		{
			count_4=0;
			if(liang4==0)
			{
				liang4=1;
				led(4,1);
			}
			else {liang4=0;led(4,0);}
		}
	}
	if(normal==1)//超声波测距条件
	{
		++count_2;
		if(count_2==100)
		{
			count_2=0;
			ceju_flag=1;	
		}
	}
	if(guozai==1) //过载闪烁
	{
		++count_1;
		if(count_1==250)
		{
			count_1=0;
			if(s_liang==0)
			{
				s_liang=1;
				led(3,1);
			}
			else 
			{
				s_liang=0;
				led(3,0);
			}
		}	
	}
}
void display()
{
	P2=(P2&0x1f)|0xe0;
	P0=0xff;
	P2=(P2&0x1f);
	
	P2=(P2&0x1f)|0xc0;
	P0=1<<discount;
	P2=(P2&0x1f);

	P2=(P2&0x1f)|0xe0;
	P0=tab[dsbuff[discount]];
	P2=(P2&0x1f);
	
	if(++discount == 8 ) discount = 0;
}
void sendwave()
{
	uchar i;
	for(i=0;i<1;i++)
	{
		TX=1;
		Delay12us();
		TX=0;
	}
}

unsigned int get_distance()
{
	uint dis;
	RX=1;
	sendwave();
	TR1=1;
	while((RX)&&(!TF1));
	TR1=0;
	if(TF1==1)
	{
		TF1=0;
		dis=0;
	}
	else 
	{
		dis=(TH1<<8)|TL1;
		dis=dis*0.017;
		
	}
	TH1=TL1=0;  //不要忘记软件清零
	return dis;
}

void Timer0Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x80;					//定时器时钟1T模式
	TMOD &= 0xF0;					//设置定时器模式
	TL0 = 0x9A;						//设置定时初值
	TH0 = 0xA9;						//设置定时初值
	TF0 = 0;							//清除TF0标志
	TR0 = 1;							//定时器0开始计时
	EA=1;
	ET0=1;
}
 
void Timer1Init(void)		//0微秒@11.0592MHz
{
	AUXR &= 0xBF;					//定时器时钟12T模式
	TMOD &= 0x0F;					//设置定时器模式
	TL1 = 0x00;						//设置定时初值
	TH1 = 0x00;						//设置定时初值
	TF1 = 0;							//清除TF1标志
}

