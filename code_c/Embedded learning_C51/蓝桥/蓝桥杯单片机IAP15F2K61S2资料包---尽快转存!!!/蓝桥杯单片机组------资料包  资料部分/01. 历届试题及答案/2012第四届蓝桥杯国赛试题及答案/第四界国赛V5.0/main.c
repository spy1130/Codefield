#include <STC15F2K60S2.h>
#include <delay.h>
#include <iic.h>
#include <ds1302.h>

#define TX P10
#define RX P11
#define uchar unsigned char
#define uint unsigned int
#define somenop1 {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}

//超声波不宜用延时数码管显示
uint ss1,distance;
uchar yi,er,san,si,wu,liu,qi,ba;
uchar discount,open=0;
uchar code tab[13]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff,0x00};
uchar dsbuff[8]={11,11,11,11,11,11,11,11};
uchar s4,s5,s6,s7;
uchar shi_shan,fen_shan,miao_shan;
uchar ss2,ss3,ss4,ceju_flag,s_liang,f_liang,m_liang;
uchar dis_flag,baocun_flag,beep_distance;
uchar led_shan_flag,l_liang;
uchar flag_1=1;

void Timer0Init(void);
void Timer1Init(void);
void display();
void keyscan();
void ledctrl(uchar lednum,uchar demand);
void fengming(uchar demand);
void shizhong_shan();
void shizhong_handle();
void distance_handle();
uint dis_get();
void sendwave();
void display1(uchar yi,uchar er);
void display2(uchar san,uchar si);
void display3(uchar wu,uchar liu);
void display4(uchar qi,uchar ba);

void allinit()
{
	P2=0x80;P0=0xff;
	P2=0xa0;P0=0x00;
	P2=0xc0;P0=0xff;
	P2=0xe0;P0=0xff;
}

void main()
{ 	
	Timer0Init(); 
	Timer1Init();
	EA=1;
	ET0=1;	
	beep_distance=iicread(0x00);
	dswrite(); 
	while(1)
	{
		if(open==0)
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
		else if(open==1)
		{
			if(flag_1==1)
			{
				allinit();
				flag_1=0;
			}	
			keyscan();
			dsread();
			if(s7==0)
			{
				shizhong_handle();
			}
			if(s7==1)
			{
				distance_handle();
			}
		}
	}			
}

void distance_handle()
{
	switch(s6)
	{
		case 1:dis_flag=1;break;
		case 2:dis_flag=0;baocun_flag=1;s6=0;break;
	}
	if(ceju_flag==1)
	{
		distance=dis_get();
		if(distance<beep_distance)
		{
			fengming(1);
			led_shan_flag=0;
		}
		else if((distance<1.2*beep_distance)&&(distance>=beep_distance))
		{
			fengming(0);
			led_shan_flag=1;
		}
		else if(distance>=1.2*beep_distance)   //注意条件！！
		{
			fengming(0);
			ledctrl(1,0);
			led_shan_flag=0;
		}
		ceju_flag=0;
	}
	if(s6==0)
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
	else 
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
			if(s5==1)
			{
				s5=0;
				beep_distance+=1;
			}
			else if(s4==1)
			{
				s4=0;
				beep_distance-=1;
			}	
		}
	}	
	if(baocun_flag==1)
	{
		baocun_flag=0;
		iicwrite(0x00,beep_distance);
	}
}

uint dis_get()
{
	uint dis;
	sendwave();
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
	TH1=TL1=0;
	return dis;
}

void sendwave()
{
	TX=1;
	somenop1;somenop1;somenop1;somenop1;somenop1;
	somenop1;somenop1;somenop1;somenop1;somenop1;
	TX=0;									  
}

void time_0() interrupt 1
{
	display();
	if(ss1<500)
	++ss1;
	if(ss1==500)
	{
		open=1;
		ss1=501;
	}
	shizhong_shan();
	if(++ss3==200)
	{	
		ss3=0;
		ceju_flag=1;
	}
	if(led_shan_flag==1)
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
			if(s7==0) {s7=1;s6=0;}
			else {s7=0;s6=0;}
		}
		while(!P30);
	}
}
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
void Timer0Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x80;					//定时器时钟1T模式
	TMOD &= 0xF0;					//设置定时器模式
	TL0 = 0x9A;						//设置定时初值
	TH0 = 0xA9;						//设置定时初值
	TF0 = 0;							//清除TF0标志
	TR0 = 1;							//定时器0开始计时
}
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
void shizhong_shan()
{
	if(shi_shan==1)
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

void Timer1Init(void)		//0微秒@11.0592MHz
{
	AUXR &= 0xBF;					//定时器时钟12T模式
	TMOD &= 0x0F;					//设置定时器模式
	TL1 = 0x00;						//设置定时初值
	TH1 = 0x00;						//设置定时初值
	TF1 = 0;							//清除TF1标志
	TR1 = 0;							//定时器1开始计时
}
	