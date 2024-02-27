#include <STC15F2K60S2.h>
#include <delay.h>
#include <iic.h>
#include <ds1302.h>

#define uchar unsigned char  
#define uint unsigned int 

uchar code tab[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar shizhong_flag=0,dianya_flag=0,gongneng_flag=0,jia_flag=0,jian_flag=0;
uchar count_1=0,count_2=0,count_3=0,count_4=0,count_5=0;
uchar hour=0,minute=0,second=0,moshi_flag=0;
uchar discount = 0,k=0,dma_liang=0,dmi_liang=0;
uchar s_liang=0,f_liang=0,m_liang=0;
uchar dianya_max=20,dianya_min=10;
uchar dsbuff[8]={1,2,3,4,5,6,7,8};
uchar vol_max=0,vol_min=0;
uint voltage=0;
uint kk=0;

void allinit()
{
	P2=0x80;P0=0xff;
	P2=0xa0;P0=0x00;
	P2=0xc0;P0=0xff;
	P2=0xe0;P0=0xff;
}

void display()
{
	P2=0xef;
	P0=0xff;
	P2=0x1f;

	P2=0xc0;
	P0=1<<discount;
	P2=0x1f;

	P2=0xef;
  P0=tab[dsbuff[discount]];
	P2=0x1f;

	if(++discount==8)
	 discount=0;
}

void Timer0Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x80;					//定时器时钟1T模式
	TMOD &= 0xF0;					//设置定时器模式
	TL0 = 0x9A;						//设置定时初值
	TH0 = 0xA9;						//设置定时初值
	TF0 = 0;							//清除TF0标志
	TR0 = 1;							//定时器0开始计时
	ET0=1;
	EA=1;
}

void Timer1Init(void)		//5毫秒@11.0592MHz
{
	AUXR |= 0x40;					//定时器时钟1T模式
	TMOD &= 0x0F;					//设置定时器模式
	TL1 = 0x00;						//设置定时初值
	TH1 = 0x28;						//设置定时初值
	TF1 = 0;							//清除TF1标志
	TR1 = 1;							//定时器1开始计时
	EA=1;
	ET1=1;
}

void time_chuli()
{
	if(gongneng_flag==0)
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
			else 
			{
				switch(moshi_flag)// 进入闪烁状态 不能在主函数里写数码管显示 而应都放在中断中显示 
				{
					case 1:hour=1;minute=0;second=0;break; 
					case 2:minute=1;hour=0;second=0;break;
					case 3:second=1;hour=0;minute=0;break;
					case 4:moshi_flag=1;break;
				}
					if(hour==1)
					{
						if(jia_flag==1)
						{
							if(shijian[2]<=23)
								{ 
									shijian[2]=shijian[2]+1;
								}
								jia_flag=0;
								dswrite();
						}
						if(jian_flag==1)
						{
							if(shijian[2]>=1)
								{
									shijian[2]-=1;
								}
								jian_flag=0;
								dswrite();
						}	
					}
					else if(minute==1)
					{
						if(jia_flag==1)
						{
							if(shijian[1]<=58)
							shijian[1]+=1;
							jia_flag=0;
							dswrite();
						}
						if(jian_flag==1)
						{
							if(shijian[1]>=1)
							shijian[1]-=1;
							jian_flag=0;
							dswrite();
						}	
					}
					else if(second==1)
					{
						if(jia_flag==1)
						{
							if(shijian[0]<=58)
							shijian[0]+=1;
							jia_flag=0;
							dswrite();
						}
						if(jian_flag==1)
						{
							if(shijian[0]>=1)
							shijian[0]-=1;
							jian_flag=0;
							dswrite();
						}
					}
			}	
}
void time_shan()
{
		if(hour==1)
		{
			count_1++;
			if(count_1==200)
			{
				count_1=0; 
				if(s_liang==0)
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
				else 
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
			}
		}
		else if(minute==1)
		{
			count_2++;
			if(count_2==200)
			{
				count_2=0;
				if(f_liang==0)
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
				else 
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
			}
		}
		else if(second==1)
		{
			count_3++;
			if(count_3==200)
			{
				count_3=0;
				if(m_liang==0)
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
				else 
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
			}
		}	
}

void dianya_chuli()
{
	if(gongneng_flag==0) 
	{
		dsbuff[0]=10;
		dsbuff[1]=1;
		dsbuff[2]=10;
		dsbuff[3]=11;
		dsbuff[4]=voltage/1000;
		dsbuff[5]=voltage%1000/100;
		dsbuff[6]=voltage%100/10;
		dsbuff[7]=voltage%10;
	} 
	else   //对于要求闪烁的直接放中断显示数码管
	{
		switch(moshi_flag)
		{
			case 1:vol_max=1; vol_min=0;break;
			case 2:vol_max=0; vol_min=1;break;
			case 3:moshi_flag=1;break;
		}
		if(jia_flag==1)
		 {
				if(vol_max==1)
				{
					dianya_max+=5;
					jia_flag=0;
				}
				if(vol_min==1)
				{
					dianya_min+=5;
					jia_flag=0;
				}	
		 }
		 if(jian_flag==1)
		 {
				if(vol_max==1)
				{
					dianya_max-=5;
					jian_flag=0;
				}
				if(vol_min==1)
				{
					dianya_min-=5;
					jian_flag=0;
				}	
		 }
		
	} 
}

void dianya_shan()
{
	if(vol_max==1)
	{
		count_4++;
		if(count_4==200)
		{
			count_4=0;
			if(dma_liang==0)
			{
				dma_liang=1;
				dsbuff[0]=11;
				dsbuff[1]=11;
				dsbuff[2]=0;
				dsbuff[3]=0;
				dsbuff[4]=dianya_min/10;
				dsbuff[5]=dianya_min%10;
				dsbuff[6]=0;
				dsbuff[7]=0;
			}
			else 
			{
				dma_liang=0;
				dsbuff[0]=dianya_max/10;
				dsbuff[1]=dianya_max%10;
				dsbuff[2]=0;
				dsbuff[3]=0;
				dsbuff[4]=dianya_min/10;
				dsbuff[5]=dianya_min%10;
				dsbuff[6]=0;
				dsbuff[7]=0;
			}
		}
	}
	if(vol_min==1)
	{
		count_5++;
		if(count_5==200)
		{
			count_5=0;
			if(dmi_liang==0)
			{
				dmi_liang=1;
				dsbuff[0]=dianya_max/10;
				dsbuff[1]=dianya_max%10;
				dsbuff[2]=0;
				dsbuff[3]=0;
				dsbuff[4]=11;
				dsbuff[5]=11;
				dsbuff[6]=0;
				dsbuff[7]=0;
			}
			else 
			{
				dmi_liang=0;
				dsbuff[0]=dianya_max/10;
				dsbuff[1]=dianya_max%10;
				dsbuff[2]=0;
				dsbuff[3]=0;
				dsbuff[4]=dianya_min/10;
				dsbuff[5]=dianya_min%10;
				dsbuff[6]=0;
				dsbuff[7]=0;
			}
		}
	}
}

void keyscan()
{
	uchar temp;
	P3=0x7f;P44=0;P42=1;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay5ms();
		if(temp!=0x0f)
		{
			temp=P3;
			temp=P3&0x0f;
			switch(temp)
			{
				case 0x0e: 
					if(gongneng_flag==1)
					gongneng_flag=0;   
					shizhong_flag=1;
					moshi_flag=0;
					dianya_flag=0;
				break;
				
				case 0x0d:
					if(gongneng_flag==1)
					gongneng_flag=0;
					dianya_flag=1;
					shizhong_flag=0;
					moshi_flag=0;
				break;
				case 0x0b: 														 break;
				case 0x07:gongneng_flag=1;moshi_flag++;break;
			} 
			while(P3!=0x7f);
		}
	}
	
	P3=0xbf;P44=1;P42=0;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay5ms();
		if(temp!=0x0f)
		{
			temp=P3;
			temp=P3&0x0f;
			switch(temp)
			{
				case 0x0e: jia_flag=1; break;
				case 0x0d: jian_flag=1;break;
				case 0x0b: 						 break;
				case 0x07:						 break;
			}
		}
		while(P3!=0xbf);
	}
}

void main()
{
	allinit();
	Timer0Init();
	Timer1Init();
	dswrite();
	while(1)
	{
		keyscan();
		dsread();
		if(shizhong_flag==1)
		{
			time_chuli();	
		}
		if(dianya_flag==1)
		{
			kk++;
			if(kk==1000)   //使电位器AD不是时刻采样 抑制其跳动
			{
				kk=0;
				voltage=adread(0x03)*19.61;
			}
			dianya_chuli();
		}
	}
}

void time0() interrupt 1
{
	display();
}

void time1() interrupt 3
{
	if((shizhong_flag==1)&&(gongneng_flag==1))
	{
		time_shan();
	}
	else if((dianya_flag==1)&&(gongneng_flag==1))
	{
		dianya_shan();
	}
}


