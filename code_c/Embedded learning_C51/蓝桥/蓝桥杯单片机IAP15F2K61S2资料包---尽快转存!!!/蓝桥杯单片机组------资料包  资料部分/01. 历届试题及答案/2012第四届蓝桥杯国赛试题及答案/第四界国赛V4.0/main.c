/**********************************************************************************************
实验功能：第四界国赛程序设计
时间：2017
作者：吴康
***********************************************************************************************/
#include <STC15F2K60S2.h>
#include <delay.h>
#include <iic.h>
#include <ds1302.h>

#define TX P10
#define RX P11
#define uchar unsigned char
#define uint unsigned int
#define somenop1 {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}

uchar code tab[13]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff,0x00};
uchar dsbuff[8]={11,11,11,11,11,11,11,11};
uint distance;														//距离
uchar discount;														//数码管移位显示控制变量
uchar open=0;															//初始化标志位
uchar s4,s5,s6,s7;												//按键标志位
uchar shi_shan,fen_shan,miao_shan;				//设置时钟闪烁标志位
uchar ss1,ss2,ss3,ss4;										//计数
uchar ceju_flag;													//测距定时扫描标志位
uchar s_liang,f_liang,m_liang;						//数码管亮灭标志位
uchar dis_flag,baocun_flag,beep_distance;	//报警距离，及报警标志位
uchar led_shan_flag,l_liang;							//LED闪烁标志位
uchar flag_1=1;														//再次初始化标志位

/**********************************************************************************************
																			函数声明
***********************************************************************************************/
void Timer0Init();
void Timer1Init();
void display();
void keyscan();
void ledctrl(uchar lednum,uchar demand);
void fengming(uchar demand);
void shizhong_shan();
void shizhong_handle();
void distance_handle();
uint dis_get();
void sendwave();

/**********************************************************************************************
函数名称：allinit
功能：板子整体初始化函数
***********************************************************************************************/
void allinit()
{
	P2=0x80;P0=0xff;
	P2=0xa0;P0=0x00;
	P2=0xc0;P0=0xff;
	P2=0xe0;P0=0xff;
}

/**********************************************************************************************
函数名称：main
功能：主函数
***********************************************************************************************/
void main()
{
	Timer0Init(); 
	Timer1Init();
	beep_distance=iicread(0x00);
	dswrite(); 
	while(1)
	{
		if(open==0)//初始化亮一秒钟
		{
			ledctrl(1,1);
			dsbuff[0]=12;
			dsbuff[1]=12;
			dsbuff[2]=12;
			dsbuff[3]=12;
			dsbuff[4]=12;
			dsbuff[5]=12;
			dsbuff[6]=12;
			dsbuff[7]=12;
			fengming(1);		
		}
		else if(open==1)//一秒后进入主显示界面
		{
			if(flag_1==1)//只进入一次初始化
			{
				allinit();
				flag_1=0;
			}	
			keyscan();//按键扫描
			dsread();//读取ds1302的时间
			if(s7==0)//s7没有按下时显示时钟
			{
				shizhong_handle();
			}
			if(s7==1)//s7按下时显示距离
			{
				distance_handle();
			}
		}
	}			
}

/**********************************************************************************************
函数名称：distance_handle
功能：距离处理函数
***********************************************************************************************/
void distance_handle()
{
	switch(s6)//在距离显示界面下，s6没有按下，则显示距离，s6按下显示设置报警距离界面
	{
		case 1:dis_flag=1;break;
		case 2:dis_flag=0;baocun_flag=1;s6=0;break;
	}
	if(ceju_flag==1)//定时扫描测距函数
	{
		distance=dis_get();
		if(distance<beep_distance)//判断现在的距离是否小于报警距离
		{
			fengming(1);
			led_shan_flag=0;
		}
		else if((distance<1.2*beep_distance)&&(distance>=beep_distance))//判断现在距离是否大于报警距离，且小于报警距离的1.2倍
		{
			fengming(0);
			led_shan_flag=1;
		}
		else if(distance>=1.2*beep_distance)   //注意条件大于报警距离的1.2倍
		{
			fengming(0);
			ledctrl(1,0);
			led_shan_flag=0;
		}
		ceju_flag=0;
	}
	if(s6==0)//s6没有按下显示距离
	{
		dsbuff[0]=11;
		dsbuff[1]=11;
		dsbuff[2]=11;
		dsbuff[3]=11;
		dsbuff[4]=11;
		dsbuff[5]=distance/100;
		dsbuff[6]=distance%100/10;
		dsbuff[7]=distance%10;
	}
	else //s6按下显示设置报警距离
	{
		if(dis_flag==1)
		{
			dsbuff[0]=11;
			dsbuff[1]=11;
			dsbuff[2]=11;
			dsbuff[3]=11;
			dsbuff[4]=11;
			dsbuff[5]=11;
			dsbuff[6]=beep_distance/10;
			dsbuff[7]=beep_distance%10;
			if(s5==1)//在设置报警距离下按下s5加报警距离
			{
				s5=0;
				beep_distance+=1;
			}
			else if(s4==1)//按下s4减报警距离
			{
				s4=0;
				beep_distance-=1;
			}	
		}
	}	
	if(baocun_flag==1)//如果报警，把设置的报警距离保存在eeprom
	{
		baocun_flag=0;
		iicwrite(0x00,beep_distance);
	}
}

/**********************************************************************************************
函数名称：dis_get
功能：距离计算函数
***********************************************************************************************/
uint dis_get()
{
	uint dis;
	sendwave();//发送一个40KHZ的方波
	TR1=1;
	while((RX)&&(TF1==0));
	TR1=0;
	if(TF1)
	{
		TF1=0;
		dis=999;
	}
	else 
	{
		dis=(TH1<<8)|(TL1);
		dis=dis*0.017; 
	}
	TH1=TL1=0;//记得要软件清零
	return dis;
}

/**********************************************************************************************
函数名称：sendwave
功能：发送40Khz的方波信号函数
***********************************************************************************************/
void sendwave()
{
	TX=1;
	somenop1;somenop1;somenop1;somenop1;somenop1;
	somenop1;somenop1;somenop1;somenop1;somenop1;
	TX=0;									  
}

/**********************************************************************************************
函数名称：timer0
功能：定时器0中断服务函数
***********************************************************************************************/
void time_0() interrupt 1
{
	display();
	if(ss1<500)
	++ss1;
	if(ss1==500)//开机显示用的定时
	{
		open=1;
		ss1=501;
	}
	shizhong_shan();//时钟闪烁函数定时扫描
	if(++ss3==200)//测距标志位
	{	
		ss3=0;
		ceju_flag=1;
	}
	if(led_shan_flag==1)//LED闪烁处理
	{
		if(++ss4==200)
		{
			ss4=0;
			if(l_liang==0)
			{
				l_liang=1;
				ledctrl(1,1);
			}
			else 
			{
				l_liang=0;
				ledctrl(1,0);
			}
		}
	}
	
}

/**********************************************************************************************
函数名称：keyscan
功能：按键扫描函数
***********************************************************************************************/
void keyscan()
{
	if(P33==0)
	{
		Delay5ms();
		if(P33==0)
		{
			if(s6>0)
			s4=1;
		}
		while(!P33);
	}
	else if(P32==0)
	{
		Delay5ms();
		if(P32==0)
		{
			if(s6>0)
			s5=1;
		}
		while(!P32);
	}
	else if(P31==0)
	{
		Delay5ms();
		if(P31==0)
		{
			s6++;
		}
		while(!P31);
	}
	else if(P30==0)
	{
		Delay5ms();
		if(P30==0)
		{
			if(s7==0) 
			{
				s7=1;
				s6=0;
			}
			else 
			{
				s7=0;
				s6=0;
			}
		}
		while(!P30);
	}
}

/**********************************************************************************************
函数名称：display
功能：显示函数
***********************************************************************************************/
void display()
{
	P2=(P2&0x1f)|0xe0;
	P0=0xff;
	P2=P2&0x1f;

  P2=(P2&0x1f)|0xc0;
	P0=1<<discount;
	P2=P2&0x1f;

	P2=(P2&0x1f)|0xe0;
	P0=tab[dsbuff[discount]];
	P2=P2&0x1f;

	if(++discount==8) discount=0;
}

/**********************************************************************************************
函数名称：Timer0Init
功能：定时器0初始化函数
***********************************************************************************************/
void Timer0Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x80;					//定时器时钟1T模式
	TMOD &= 0xF0;					//设置定时器模式
	TL0 = 0x9A;						//设置定时初值
	TH0 = 0xA9;						//设置定时初值
	TF0 = 0;							//清除TF0标志
	TR0 = 1;							//定时器0开始计时
}

/**********************************************************************************************
函数名称：ledctrl
功能：LED亮灭控制函数
***********************************************************************************************/
void ledctrl(uchar lednum,uchar demand)
{
	uchar i;
	static uchar led_state=0;
	lednum--;
	i=led_state&(0x01<<lednum);
	if((i==0)&&(demand==1))
	{
		EA=0;
		led_state=0x01<<lednum;
		P2=(P2&0x1f)|0x80;
		P0=~(0x01<<lednum);
		P2=P2&0x1f;
		EA=1;
	}
	else if((i>0)&&(demand==0))
	{
		EA=0;
		led_state=0;
		P2=(P2&0x1f)|0x80;
		P0=0xff;
		P2=P2&0x1f;
		EA=1;
	}	
}

/**********************************************************************************************
函数名称：fengming
功能：继电器通断控制函数
***********************************************************************************************/
void fengming(uchar demand)
{
	static xiang_state=0; 
	if((demand==1)&&(xiang_state==0))
	{
		P0=0x00;
		EA=0;
		P2=(P2&0x1f)|0xa0;
		P0=0x40;
		P2=P2&0x1f;
		xiang_state=1;
		EA=1;	
	}
	else if((demand==0)&&(xiang_state==1))
	{
		xiang_state=0;
		P2=(P2&0x1f)|0xa0;
		P0=0x00;
		P2=P2&0x1f;
	}
}

/**********************************************************************************************
函数名称：shizhong_shan
功能：时钟闪烁控制函数
***********************************************************************************************/
void shizhong_shan()
{
	if(shi_shan==1)//时闪烁标志位
	{
		if(++ss2==200)
		{
			ss2=0;
			if(s_liang==1)
			{
				s_liang=0;
				dsbuff[0]=shijian[2]/10;
				dsbuff[1]=shijian[2]%10;
				dsbuff[2]=10;
				dsbuff[3]=shijian[1]/10;
				dsbuff[4]=shijian[1]%10;
				dsbuff[5]=10;
				dsbuff[6]=shijian[0]/10;
				dsbuff[7]=shijian[0]%10;
			}
			else 
			{
				s_liang=1;
				dsbuff[0]=11;
				dsbuff[1]=11;
				dsbuff[2]=10;
				dsbuff[3]=shijian[1]/10;
				dsbuff[4]=shijian[1]%10;
				dsbuff[5]=10;
				dsbuff[6]=shijian[0]/10;
				dsbuff[7]=shijian[0]%10;
			}
		}
	}
	else if(fen_shan==1)
	{
		if(++ss2==200)
		{
			ss2=0;
			if(f_liang==1)
			{
				f_liang=0;
				dsbuff[0]=shijian[2]/10;
				dsbuff[1]=shijian[2]%10;
				dsbuff[2]=10;
				dsbuff[3]=shijian[1]/10;
				dsbuff[4]=shijian[1]%10;
				dsbuff[5]=10;
				dsbuff[6]=shijian[0]/10;
				dsbuff[7]=shijian[0]%10;
			}
			else 
			{
				f_liang=1;
				dsbuff[0]=shijian[2]/10;
				dsbuff[1]=shijian[2]%10;
				dsbuff[2]=10;
				dsbuff[3]=11;
				dsbuff[4]=11;
				dsbuff[5]=10;
				dsbuff[6]=shijian[0]/10;
				dsbuff[7]=shijian[0]%10;
			}
		}
	}
	else if(miao_shan==1)
	{
		if(++ss2==200)
		{
			ss2=0;
			if(m_liang==1)
			{
				m_liang=0;
				dsbuff[0]=shijian[2]/10;
				dsbuff[1]=shijian[2]%10;
				dsbuff[2]=10;
				dsbuff[3]=shijian[1]/10;
				dsbuff[4]=shijian[1]%10;
				dsbuff[5]=10;
				dsbuff[6]=shijian[0]/10;
				dsbuff[7]=shijian[0]%10;
			}
			else 
			{
				m_liang=1;
				dsbuff[0]=shijian[2]/10;
				dsbuff[1]=shijian[2]%10;
				dsbuff[2]=10;
				dsbuff[3]=shijian[1]/10;
				dsbuff[4]=shijian[1]%10;
				dsbuff[5]=10;
				dsbuff[6]=11;
				dsbuff[7]=11;
			}
		}
	}
}

/**********************************************************************************************
函数名称：shizhong_handle
功能：时钟处理函数
***********************************************************************************************/
void shizhong_handle()
{
	switch(s6)
	{
		case 1:shi_shan=1;fen_shan=0;miao_shan=0;break;
		case 2:shi_shan=0;fen_shan=1;miao_shan=0;break;
		case 3:shi_shan=0;fen_shan=0;miao_shan=1;break;
		case 4:s6=0;break;
	}
	if(s6==0)
	{
		dsbuff[0]=shijian[2]/10;
		dsbuff[1]=shijian[2]%10;
		dsbuff[2]=10;
		dsbuff[3]=shijian[1]/10;
		dsbuff[4]=shijian[1]%10;
		dsbuff[5]=10;
		dsbuff[6]=shijian[0]/10;
		dsbuff[7]=shijian[0]%10;
	}
	if(shi_shan==1)
	{
		if(s5==1)
		{
			s5=0;
			if(shijian[2]<=22)
			shijian[2]=shijian[2]+1;
			dswrite();
		}
		else if(s4==1)
		{
			s4=0;
			if(shijian[2]>0)
			shijian[2]=shijian[2]-1;
			dswrite();
		}
	}
	else if(fen_shan==1)	  //考虑循环时分秒显示 边界判断
	{
		if(s5==1)
		{
			s5=0;
			if(shijian[1]<=58)
			shijian[1]=shijian[1]+1;
			dswrite();
		}
		else if(s4==1)
		{
			s4=0;
			if(shijian[1]>0)
			shijian[1]=shijian[1]-1;
			dswrite();
		}	
	}
	else if(miao_shan==1)
	{
		if(s5==1)
		{
			s5=0;
			if(shijian[0]<=58)
			shijian[0]=shijian[0]+1;
			dswrite();
		}
		else if(s4==1)
		{
			s4=0;
			if(shijian[0]>0)
			shijian[0]=shijian[0]-1;
			dswrite();
		}	
	}	
}

/**********************************************************************************************
函数名称：Timer1Init
功能：定时器1初始化函数
***********************************************************************************************/
void Timer1Init(void)		//0微秒@11.0592MHz
{
	AUXR &= 0xBF;					//定时器时钟12T模式
	TMOD &= 0x0F;					//设置定时器模式
	TL1 = 0x00;						//设置定时初值
	TH1 = 0x00;						//设置定时初值
	TF1 = 0;							//清除TF1标志
	TR1 = 0;							//定时器1开始计时
	EA=1;
	ET0=1;	
}
	