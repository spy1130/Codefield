/***************************************************************************************************
实验功能：第三届国赛试题程序设计
时间：
作者：吴康
****************************************************************************************************/
#include <STC15F2K60S2.h>
#include <delay.h>
#include <ds1302.h>
#include <iic.h>
#define uchar unsigned char
#define uint unsigned int

#define TX P10															//发送
#define RX P11															//接收
#define somenop1 {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}

uchar code tab[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar dsbuff[8]={11,11,11,11,11,11,11,11};
uchar num[6]={11,11,11,11,11,11};
uchar mima[6]={6,5,4,3,2,1};								//初始密码设置
uchar discount = 0;													//数码管显示移位变量
uchar ceju_flag=0;													//定时测距标志变量
uchar mima_flag;														//密码模式标志位
uchar set=0;																//设置按键标志位
uchar admit=0;															//确认按键标志位
uchar reset=0;															//复位按键标志位
uchar exit=0;																//退出按键标志位
uchar i=0;																	//变量
uchar now;																	//现在的时间存储变量
uchar false;																//错误的次数标志位
uchar xiang_flag;														//蜂鸣器响的时间标志位
uchar l_count=4;														//LED灯闪烁标志位
uchar temp=0;																//变量
uint distance;															//距离变量
uchar k;																		//变量

/***************************************************************************************************
																						函数声明
****************************************************************************************************/
void Timer0Init();
void Timer1Init();
void display();
void keyscan();
void beep(uchar demand);
void relay(uchar demand);
uint get_distance();
void zidongmen();
void mimamen();
void ledctrl(uchar lednum,uchar demand);
void baocunmima();
void readmima();

/***************************************************************************************************
函数名称：clear
功能：清除密码函数
****************************************************************************************************/
void clear()
{
	for(k=0;k<=5;k++)
	num[k]=11;
	i=0;		//特别注意
}

/***************************************************************************************************
函数名称：allinit
功能：板子初始化函数
****************************************************************************************************/
void allinit()
{
	P2=0x80;P0=0xff;
	P2=0xa0;P0=0x00;
	P2=0xc0;P0=0xff;
	P2=0xe0;P0=0xff;
}

/***************************************************************************************************
函数名称：main
功能：主函数
****************************************************************************************************/
void main()
{
	allinit();														//板子初始化
	Timer0Init();													//定时器0初始化
	Timer1Init();													//定时器1初始化
	dswrite();														//时钟初始化
	while(1)
	{
		keyscan();													//按键扫描函数
		dsread();														//读取时间
		if((shijian[2]>=7)&&(shijian[2]<22))//判断时间
		{
			mima_flag=0;											//密码门状态标志位
			zidongmen();											//自动门控制函数
		}
		else 																//在密码门状态下
		{
			mima_flag=1;
			mimamen();												//密码门处理函数
		}

		if(mima_flag==0)  									//非密码门下显示
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
	}
}

/***************************************************************************************************
函数名称：mimamen
功能：密码门处理函数函数
****************************************************************************************************/
void mimamen()
{
	static uchar wancheng_flag=0,j=0,true=0;
	static uchar true_2=0,false_2=0,flag1=0;
	if(set==0)											//没有按下设置键时，显示— —密码
	{	
		dsbuff[0]=10;
		dsbuff[1]=10;
		dsbuff[2]=num[0];
		dsbuff[3]=num[1];	
		dsbuff[4]=num[2];
		dsbuff[5]=num[3];
		dsbuff[6]=num[4];
		dsbuff[7]=num[5];
		if(admit==1)									//按下确认键后进行对比，看输入的密码是否正确
		{
			admit=0;
			for(j=0;j<=5;j++)
			{
				if(num[j]==mima[j])
					true=1;									//正确的标志位
				else 
				{
					true=2;									//不正确的标志位
				}
			}
		}
		if(true==1)										//如果正确
		{
			relay(1);										//继电器打开
			clear();										//验证通过清除数组里存储的密码
			if((shijian[0]-now==5)||(60+shijian[0]-now==5))//让继电器通5秒
			{
				true=0;										//清除标志位
				relay(0);									//关闭数码管
			}
		}
		else if (true==2)							//如果密码不正确
		{
			false++;										//错误的次数加一，到三次时，报警
			clear();										//没输入完成一次都要把数组里存储的数据给清零，不要忘了把i也清零
			true=0;											//清除标志位
		}
		if(false==3)									//如果输入密码错误了三次
		{
			l_count=0;
			ledctrl(1,1);
			beep(1);
			false=0;
			admit=0;
			xiang_flag=1;								//蜂鸣器报警三秒标志位
//			if((shijian[0]-now==3)||(60+shijian[0]-now==3))//发蜂鸣器报警三秒
//			{
//				ledctrl(1,0);
//				beep(0);
//				false=0;
//				admit=0;
//			}
		}
	}
	else //按下设置按键
	{
		
		
		if(exit==1)										//退出键功能
		{
			exit=0;											//推出标志位清零
			true_2=0;
			set=0;	
			clear(); 										//清空输入密码		
		}
		if(true_2==0)									//设密码时的标志位
		{
			dsbuff[0]=11;
			dsbuff[1]=10;
			dsbuff[2]=num[0];
			dsbuff[3]=num[1];	
			dsbuff[4]=num[2];
			dsbuff[5]=num[3];
			dsbuff[6]=num[4];
			dsbuff[7]=num[5];
		}
		if(true_2==1)									//这里的admit与后面的admit相冲突 应该把状态判断放在后面 
		{
			if(flag1==1) 
			{
				clear();
				flag1=0;
			}
			dsbuff[0]=10;
			dsbuff[1]=11;
			dsbuff[2]=num[0];
			dsbuff[3]=num[1];
			dsbuff[4]=num[2];
			dsbuff[5]=num[3];
			dsbuff[6]=num[4];
			dsbuff[7]=num[5];
			
			if(admit==1)
			{
				admit=0; 
				ledctrl(2,1);
				for(k=0;k<=5;k++)
				{
					mima[k]=num[k];
				}
				wancheng_flag=1;
			}
			if(wancheng_flag==1)
			{
				wancheng_flag=0;
				set=0;
				true_2=0;
				ledctrl(1,1);
				clear();
			}//设置完后清空标志位		
		}		
		if(admit==1)
		{
			admit=0;
			for(k=0;k<=5;k++)
			{
				if(num[k]==mima[k])
					{
						true_2=1;
						flag1=1;
					}
				else 
					{
						true_2=2;
						clear();
					} //这个地方改变了true2的状态 
			}	
		}
		if(true_2==2)
		{
			false_2++;
			clear();
			true_2=0;
		}
		if(false_2==3)//在设置时错误了三次
		{
			set=0;
			ledctrl(2,1);
			beep(1);
			xiang_flag=1;
			false_2=0;	
		}
	}

	if(reset==1)//按下复位按键
	{
		reset=0;
		for(k=0;k<=5;k++)
		{
			mima[k]=6-k;
		}
	}
}

/***************************************************************************************************
函数名称：zidongmen
功能：自动门处理函数函数
****************************************************************************************************/
void zidongmen()
{
		static bit kai_flag=0;
		if(ceju_flag==1)
		{
			ceju_flag=0;
			distance=get_distance();
		}	
		if((distance<30)&&(kai_flag==0))
		{
			kai_flag=1;//开门的标志位
			temp=shijian[0];//将此时的时间赋值给temp
		}
		if(kai_flag==1)
		{
			relay(1);//开门打开继电器
			if((shijian[0]-temp==5)||(60+shijian[0]-temp==5))
			{
				kai_flag=0;
				relay(0);//5秒后关闭继电器
			}	
		}
}

/***************************************************************************************************
函数名称：keyscan
功能：按键处理函数函数
****************************************************************************************************/
void keyscan()
{
	uchar temp;
	P3=0x7f;P44=0;P42=1;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay5ms();
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			switch(temp)
			{
				case 0x0e:if(i<=5){num[i]=0;i++;}break;
				case 0x0d:if(i<=5){num[i]=4;i++;}break;
				case 0x0b:if(i<=5){num[i]=8;i++;}break;
				case 0x07:break;
			}
		}
		while(temp!=0x0f)
		{
			temp=P3;
			temp=P3&0x0f;
		}
	}

	P3=0xbf;P44=1;P42=0;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay5ms();
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			switch(temp)
			{
				case 0x0e:if(i<=5){num[i]=1;i++;}break;
				case 0x0d:if(i<=5){num[i]=5;i++;}break;
				case 0x0b:if(i<=5){num[i]=9;i++;}break;
				case 0x07:
				if(admit==0)
				{
					if(i>=1)
					{
						i=i-1;
						num[i]=11;
					}
				}
				break;
			}
		}
	   	while(P3!=0xbf);
	}	

	P3=0xdf;P44=1;P42=1;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay5ms();
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			temp=P3&0x0f;
			switch(temp)
			{
				case 0x0e:if(i<=5){num[i]=2;i++;}break;
				case 0x0d:if(i<=5){num[i]=6;i++;}break;
				case 0x0b:if(set==0) clear(); set=1;admit=0;break;
				case 0x07:if(i==6){admit=1;now=shijian[0];}break;
			}
		}
		while(P3!=0xdf);
	}

	P3=0xef;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay5ms();
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			switch(temp)
			{
				case 0x0e:if(i<=5){num[i]=3;i++;}break;
				case 0x0d:if(i<=5){num[i]=7;i++;}break;
				case 0x0b:reset=1;break;
				case 0x07:exit=1;break;
			}
		}
		while(P3!=0xef);
	}
}

/***************************************************************************************************
函数名称：Timer0Init
功能：定时器0初始化函数
****************************************************************************************************/
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

/***************************************************************************************************
函数名称：display
功能：数码管显示函数
****************************************************************************************************/
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

/***************************************************************************************************
函数名称：relay
功能：继电器控制函数
****************************************************************************************************/
void relay(uchar demand)
{
	static uchar relay_state=0;
	if((demand==1)&&(relay_state==0))
	{
		relay_state=1;
		P2=(P2&0x1f)|0xa0;
		P0=0x10;
		P2=P2&0x1f;
	}
	else if((demand==0)&&(relay_state==1))
	{
		relay_state=0;
		P2=(P2&0x1f)|0xa0;
		P0=0x00;
		P2=P2&0x1f;
	}
}

/***************************************************************************************************
函数名称：beep
功能：蜂鸣器控制函数
****************************************************************************************************/
void beep(uchar demand)
{
	static uchar beep_state=0;
	if((demand==1)&&(beep_state==0))
	{
		beep_state=1;
		P2=(P2&0x1f)|0xa0;
		P0=0x40;
		P2=P2&0x1f;
	}
	else if((demand==0)&&(beep_state==1))
	{
		beep_state=0;
		P2=(P2&0x1f)|0xa0;
		P0=0x00;
		P2=P2&0x1f;
	}
}

/***************************************************************************************************
函数名称：ledctrl
功能：led控制函数
****************************************************************************************************/
void ledctrl(uchar lednum,uchar demand)
{
	static uchar led_state=0;
	uchar n;
	lednum--;
	n=led_state&(0x01<<lednum);
	if((n==0)&&(demand==1))
	{
		led_state=0x01<<lednum;
		P2=(P2&0x1f)|0x80;
		P0=~(0x01<<lednum);
		P2=P2&0x1f;
	}
	else if((n>0)&&(demand==0))
	{
		led_state=0;
		P2=(P2&0x1f)|0x80;
		P0=0xff;
		P2=P2&0x1f;
	}
}

/***************************************************************************************************
函数名称：sendwave
功能：产生波形函数
****************************************************************************************************/
void sendwave()
{
	TX=1;
	somenop1;somenop1;somenop1;somenop1;somenop1;
	somenop1;somenop1;somenop1;somenop1;somenop1;
	TX=0;
}

/***************************************************************************************************
函数名称：get_distance
功能：距离计算函数
****************************************************************************************************/
uint get_distance()
{
	uint dis;
	sendwave();
	TR1=1;
	while((RX==1)&&(TF1==0));
	TR1=0;
	if(TF1)
	{
		TF1=0;
		dis=999;
	}
	else 
	{
		dis=(TH1<<8)|TL1;
		dis=dis*0.017;
	}
	TH1=TL1=0;
	return dis;	
}

/***************************************************************************************************
函数名称：Timer1Init
功能：定时器1初始化函数
****************************************************************************************************/
void Timer1Init(void)		//0微秒@11.0592MHz
{
	AUXR &= 0xBF;					//定时器时钟12T模式
	TMOD &= 0x0F;					//设置定时器模式
	TL1 = 0x00;						//设置定时初值
	TH1 = 0x00;						//设置定时初值
	TF1 = 0;							//清除TF1标志
	TR1 = 0;							//定时器1开始计时
}

/***************************************************************************************************
函数名称：Timer0
功能：定时器0中断服务函数
****************************************************************************************************/
void time_0() interrupt 1
{
	static uchar ss1=0;
	static uint ss2=0;
	static uchar ss3,s_flag;
	display();
	if(++ss1==100)
	{
		ss1=0;
		ceju_flag=1;
	}
	if(xiang_flag==1)
	{
		if(++ss2==1500)				
		{
			ss2=0;
			ledctrl(2,0);	
			beep(0);
			xiang_flag=0;
		}
	}
	if(l_count==0)
	{
		if(++ss3==200)
		{
			ss3=0;
			ledctrl(1,0);
			l_count=4;
		}
	}
	if(l_count<=2)//闪烁三次
	{
			if(++ss3==200)
			{
				ss3=0;
				if(s_flag==0)
				{
					s_flag=1;
					ledctrl(1,1);
				}
				else
				{
					s_flag=0;
					ledctrl(1,0);
					l_count++;
				}
			}
	}
}



