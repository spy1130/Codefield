#include <STC15F2K60S2.h>
#include <delay.h>
#include <iic.h>
#include <ds1302.h>

#define uchar unsigned char  
#define uint unsigned int 

uchar shizhong_flag=0,dianya_flag=0,pinlv_flag=0,gongneng_flag=0,jia_flag=0,jian_flag=0,chaxun_flag=0;
uchar code tab[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar discount = 0,k=0,dma_liang=0,dmi_liang=0,p_qiehuan=1,c_qiehuan=1;
uchar shi,fen,miao,last_max=0,last_min=0,s_keep,f_keep,m_keep,thing;
uint zheng=0,fan=0,z_zheng=0,z_fan=0,zhouqi=0,fre=0,jishu=0;
uchar count_1=0,count_2=0,count_3=0,count_4=0,count_5=0;
uchar back = 0,hour=0,minute=0,second=0,moshi_flag=0;
uchar s_liang=0,f_liang=0,m_liang=0;
uchar dianya_max=20,dianya_min=10;
uchar dsbuff[8]={1,2,3,4,5,6,7,8};
uchar vol_max=0,vol_min=0;
uchar jieshu_flag=0;
uint voltage=0;
uint count=0;
uint kk=0;


void allinit()
{
	P2=0x80;P0=0xff;
	P2=0xa0;P0=0x00;
	P2=0xc0;P0=0xff;
	P2=0xe0;P0=0xff;
}

//void led(uchar liang)
//{
//	P2 &= 0x1f;	
//	P2 |= 0x80;
//	P0 = liang;
//	P2 &= 0x1f;
//}

void chaxun_chuli()
{
	if(c_qiehuan==1)
	{
	  dsbuff[0]=11;
		dsbuff[1]=11;
		dsbuff[2]=11;
		dsbuff[3]=11;
		dsbuff[4]=11;
		dsbuff[5]=11;
		dsbuff[6]=11;
		dsbuff[7]=thing;	
	}
	else 
	{
		dsbuff[0]=s_keep/10;
		dsbuff[1]=s_keep%10;
		dsbuff[2]=10;
		dsbuff[3]=f_keep/10;
		dsbuff[4]=f_keep%10;
		dsbuff[5]=10;
		dsbuff[6]=m_keep/10;
		dsbuff[7]=m_keep%10;
	}	
}
void pinlv_chuli()
{
	if(jieshu_flag==1)
	{
		jieshu_flag=0;
		count=TH0*256+TL0;
		TH0=TL0=0;
	}
	if(count>0)
	zhouqi=count*100;
	fre=1000000/zhouqi;
	if(p_qiehuan==1)
	{
		dsbuff[0]=10;
		dsbuff[1]=2;
		dsbuff[2]=10;
		dsbuff[3]=fre/10000;
		dsbuff[4]=fre/10000%1000;
		dsbuff[5]=fre%1000/100;
		dsbuff[6]=fre%100/10;
		dsbuff[7]=fre%10;	
	}
	
	else 
	{
		dsbuff[0]=10;
		dsbuff[1]=2;
		dsbuff[2]=10;
		dsbuff[3]=zhouqi/10000;
		dsbuff[4]=zhouqi/10000%1000;
		dsbuff[5]=zhouqi%1000/100;
		dsbuff[6]=zhouqi%100/10;
		dsbuff[7]=zhouqi%10;	
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
	if((voltage>(dianya_max*100))&&(last_max==0))
		 {
		 	s_keep=shijian[2];
			f_keep=shijian[1];
			m_keep=shijian[0];
			last_max=1;
			last_min=0;
			iicwrite(0x03,1);
			Delay2ms();
			iicwrite(0x04,s_keep);
			Delay2ms();
			iicwrite(0x05,f_keep);
			Delay2ms();
			iicwrite(0x06,m_keep);
		 }
		 else if((voltage<(dianya_min*100))&&(last_min==0))
		 {
		 	s_keep=shijian[2];
			f_keep=shijian[1];
			m_keep=shijian[0];
			last_max=0;
			last_min=1;
			iicwrite(0x03,0);
			Delay2ms();
			iicwrite(0x04,s_keep);
			Delay2ms();
			iicwrite(0x05,f_keep);
			Delay2ms();
			iicwrite(0x06,m_keep);
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
					if(gongneng_flag==1) gongneng_flag=0;  
					shizhong_flag=1;
					moshi_flag=0;
					dianya_flag=0;
					pinlv_flag=0;
					chaxun_flag=0;
				break;
				case 0x0d: 
					if(gongneng_flag==1)
					{
						gongneng_flag=0;
						iicwrite(0x01,dianya_max);
						Delay2ms();
						iicwrite(0x02,dianya_min);
					}
					dianya_flag=1;
					shizhong_flag=0;
					pinlv_flag=0;
					moshi_flag=0;
					chaxun_flag=0;
				break;
				case 0x0b:
					pinlv_flag=1;
					shizhong_flag=0;
					dianya_flag=0;
					chaxun_flag=0;
					TH0=TL0=0;
				break;
				case 0x07:
					if((pinlv_flag!=1)&&(chaxun_flag!=1))
					{
					k=1;
					gongneng_flag=1;
					moshi_flag++;
					}
					else if((pinlv_flag==1)&&(chaxun_flag==0))
					{
							if(p_qiehuan==1)
							p_qiehuan=0;
								else
								{
									p_qiehuan=1;
								}
					}	
					else if((pinlv_flag==0)&&(chaxun_flag==1))
					{
						if(c_qiehuan==1)
						c_qiehuan=0;
							else 
							{
								c_qiehuan=1;
							}
					}
				break;
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
				case 0x0d: jian_flag=1; break;
				case 0x0b: 
					chaxun_flag=1;
					shizhong_flag=0;
					dianya_flag=0;
					pinlv_flag=0;
					thing=iicread(0x03);
					Delay2ms();
					shi=iicread(0x04);
					Delay2ms();
					fen=iicread(0x05);
					Delay2ms();
					miao=iicread(0x06);
				break;
				case 0x07: break;
			}
		}
		while(P3!=0xbf);
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
				dsbuff[2]=0;dsbuff[3]=0;
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

	if(++discount == 8)
	 discount = 0;
}

void Timer0Init(void)		//2微秒@11.0592MHz
{
	AUXR |= 0x80;					//定时器时钟1T模式
	TMOD &= 0xF0;					//设置定时器模式
	TMOD |= 0x04;
	TL0 = 0x00;						//设置定时初值
	TH0 = 0x00;						//设置定时初值
	TF0 = 0;							//清除TF0标志
}

void Timer1Init(void)		//5毫秒@11.0592MHz
{
	AUXR |= 0x40;					//定时器时钟1T模式
	TMOD &= 0x0F;					//设置定时器模式
	TL1 = 0x00;						//设置定时初值
	TH1 = 0x28;						//设置定时初值
	TF1 = 0;							//清除TF1标志
	TR1 = 1;							//定时器1开始计时
}
void Timer2Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x04;					//定时器时钟1T模式
	T2L = 0x9A;						//设置定时初值
	T2H = 0xA9;						//设置定时初值
	AUXR |= 0x10;					//定时器2开始计时
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
					case 1:hour=1;minute=0;second=0;break; //作为使LED只执行一次的过程 K可以在按键中初始化赋值为1
					case 2:minute=1;hour=0;second=0;break;
					case 3:second=1;hour=0;minute=0;break;
					case 4:moshi_flag=1;break;
				}
					if(hour==1)
					{
						if(jia_flag==1)
						{
							if(shijian[2]<=22)
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


void main()
{
	allinit();
	Timer0Init();
	Timer1Init();
	Timer2Init();
	EA=1;
	ET1=1;
	IE2|=0x04;
	dswrite();
	dianya_max=iicread(0x01);
	Delay2ms();
	dianya_min=iicread(0x02);
	Delay2ms();
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
				voltage=adread(0x03)*19.6;
			}
			dianya_chuli();
		}
		if(pinlv_flag==1)
		{  	
			pinlv_chuli();		
		}
		if(chaxun_flag==1)
		{
			chaxun_chuli();
		}
	}
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
void time2() interrupt 12
{
	uchar start;
	display();
	if(pinlv_flag==1)
	{
		jishu++;
		if(jishu==495)
		{
			TR0=1;
			start=1;
//			ET0=1;
		}
		if((start==1)&&(jishu==500))
			{
				TR0=0;
				start=0;
				jieshu_flag=1;
//				ET0=0;
				jishu=0;
			}
	}
}