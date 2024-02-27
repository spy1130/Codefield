/******************************************************************************************
实验功能：第七届国赛试题程序设计
时间：2017.12.28
作者：吴康
*******************************************************************************************/
#include <STC15F2K60S2.H>
#include <delay.h>
#include <iic.h>
#include <ds1302.h>

#define uchar unsigned char  
#define uint unsigned int 

uchar code tab[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar count_1=0,count_2=0,count_3=0,count_4=0,count_5=0;//定时中间变量
uchar dsbuff[8]={1,2,3,4,5,6,7,8};											//初始化界面显示
uchar dma_liang=0,dmi_liang=0;													//控制设置电压上下限时数码管闪烁标志位
uchar p_qiehuan=1,c_qiehuan=1;													//频率，周期切换显示标志位,切换显示事件，以及发生事件是的时间
uchar shi,fen,miao;																			//存储电压突变时的时间
uchar shizhong_flag=0;																	//时钟显示标志位
uchar dianya_flag=0;																		//电压显示标志位
uchar pinlv_flag=0;																			//频率显示标志位
uchar gongneng_flag=0;																	//功能切换按键标志位
uchar jia_flag=0,jian_flag=0;														//加减按键标志位
uchar chaxun_flag=0;																		//查询按键标志位
uchar discount=0;																				//数码管扫描显示变量
uchar last_max=0,last_min=0;														//电压比较时的标志位
uchar s_keep,f_keep,m_keep;															//存储当时的时间
uchar thing;																						//事件显示变量
uchar hour=0,minute=0,second=0;													//标志位
uchar moshi_flag=0;																			//模式选择标志位
uchar s_liang=0,f_liang=0,m_liang=0;										//时分秒闪烁标志位
uchar dianya_max=20,dianya_min=10;											//初始化电压上限下限
uchar vol_max=0,vol_min=0;															//设置电压上限下限变量
uchar jieshu_flag=0;																		//开始测频率时的标志位
uint zhouqi=0,fre=0,jishu=0;														//周期，频率
uint voltage=0;																					//采集的电压
uint count=0;																						//计数
uint kk=0;																							//计数

/******************************************************************************************
																			函数声明
*******************************************************************************************/
void display();
void time_chuli();
void chaxun_chuli();
void keyscan();
void Timer0Init();
void Timer1Init();
void Timer2Init();
void dianya_chuli();
void time_shan();
void dianya_shan();
void pinlv_chuli();

/******************************************************************************************
函数名称：allinit
功能：板子初始化
*******************************************************************************************/
void allinit()
{
	P2=0x80;P0=0xff;
	P2=0xa0;P0=0x00;
	P2=0xc0;P0=0xff;
	P2=0xe0;P0=0xff;
}

/******************************************************************************************
函数名称：main
功能：主函数
*******************************************************************************************/
void main()
{
	allinit();									//板子整体初始化
	Timer0Init();								//定时器0初始化
	Timer1Init();								//定时器1初始化
	Timer2Init();								//定时器2初始化
	EA=1;												//打开总中断
	ET1=1;											//打开定时器1的中断
	IE2|=0x04;									//打开定时器2的中断
	dswrite();									//时钟初始化
	dianya_max=iicread(0x01);		//读取地址0x01里的数据
	Delay2ms();
	dianya_min=iicread(0x02);		//读取地址0x02里的数据
	Delay2ms();
	while(1)
	{
		keyscan();								//按键处理函数
		dsread();									//读取时间
		if(shizhong_flag==1)			
			{
				time_chuli();					//时间处理函数
			}
		if(dianya_flag==1)
			{
				kk++;
				if(kk==1000)   				//使电位器AD不是时刻采样 抑制其跳动
					{
						kk=0;
						voltage=adread(0x03)*19.6;//读取电位器的电压
					}
				dianya_chuli();					//电压处理函数
			}
		if(pinlv_flag==1)
			{  	
				pinlv_chuli();					//频率处理函数
			}
		if(chaxun_flag==1)
			{
				chaxun_chuli();					//查询处理函数
			}
	}
}

/******************************************************************************************
函数名称：chaxun_chuli
功能：查询按键处理函数
*******************************************************************************************/
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
			dsbuff[6]=thing/10;
			dsbuff[7]=thing%10;	
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

/******************************************************************************************
函数名称：pinlv_chuli
功能：频率计算处理函数
*******************************************************************************************/
void pinlv_chuli()
{
	if(jieshu_flag==1)						//当按键s5按下时开始计算频率
		{
			jieshu_flag=0;
			count=TH0*256+TL0;
			TH0=TL0=0;								//需要软件清零
		}
	if(count>0)
	zhouqi=count*100;							//计算周期
	fre=1000000/zhouqi;						//计算频率
	if(p_qiehuan==1)							//判断按键s4是否按下按下则进行频率周期切换显示
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

/******************************************************************************************
函数名称：dianya_chuli
功能：电压计算处理函数
*******************************************************************************************/
void dianya_chuli()
{
	if(gongneng_flag==0) 											//当s4按键没有按下时则显示此时的电压
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
	else  																		//若按键s4按下了，判断在那个模式下
		{
			switch(moshi_flag)
				{
					case 1:vol_max=1; vol_min=0;break;//在设置上限模式下
					case 2:vol_max=0; vol_min=1;break;//在设置下限模式下
					case 3:moshi_flag=1;break;				//回到原来电压显示界面
				}
			if(jia_flag==1)												//若按键s11是按下
				{
						if(vol_max==1)									//在设置上限模式，进行加
							{
								dianya_max+=5;
								jia_flag=0;
							}
						if(vol_min==1)									//在设置下限模式，进行加
							{
								dianya_min+=5;
								jia_flag=0;
							}	
				 }
			 if(jian_flag==1)											//若按键s10是按下
				 {
						if(vol_max==1)									//在设置上限模式，进行减
							{
								dianya_max-=5;
								jian_flag=0;
							}
						if(vol_min==1)									//在设置下限模式，进行减
							{
								dianya_min-=5;
								jian_flag=0;
							}	
			 }
			
		} 
	if((voltage>(dianya_max*100))&&(last_max==0))//进行比较
		 {
				s_keep=shijian[2];
				f_keep=shijian[1];
				m_keep=shijian[0];
				last_max=1;
				last_min=0;
				iicwrite(0x03,1);											//往地址里写1
				Delay2ms();
				iicwrite(0x04,s_keep);								//将刚刚的秒时间保存在地址0x04里
				Delay2ms();
				iicwrite(0x05,f_keep);								//将刚刚的分时间保存在地址0x05里
				Delay2ms();
				iicwrite(0x06,m_keep);								//将刚刚的时时间保存在地址0x06里
		 }
	else if((voltage<(dianya_min*100))&&(last_min==0))//进行比较
		 {
				s_keep=shijian[2];
				f_keep=shijian[1];
				m_keep=shijian[0];
				last_max=0;
				last_min=1;
				iicwrite(0x03,0);											//往地址里写1
				Delay2ms();
				iicwrite(0x04,s_keep);								//将刚刚的秒时间保存在地址0x04里
				Delay2ms();
				iicwrite(0x05,f_keep);								//将刚刚的分时间保存在地址0x05里
				Delay2ms();
				iicwrite(0x06,m_keep);								//将刚刚的时时间保存在地址0x06里
		 }
}

/******************************************************************************************
函数名称：keyscan
功能：按键处理函数
*******************************************************************************************/
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
							case 0x0e: //时钟显示按键
								if(gongneng_flag==1) gongneng_flag=0; 
								shizhong_flag=1;
								moshi_flag=0;
								dianya_flag=0;
								pinlv_flag=0;
								chaxun_flag=0;
							break;
							case 0x0d://电压显示按键
								if(gongneng_flag==1)
									{
										gongneng_flag=0;
										iicwrite(0x01,dianya_max);//将设置的电压上限保存在0x01地址里
										Delay2ms();
										iicwrite(0x02,dianya_min);//将设置的电压下限保存在0x02地址里
									}
								dianya_flag=1;
								shizhong_flag=0;
								pinlv_flag=0;
								moshi_flag=0;
								chaxun_flag=0;
							break;
							case 0x0b://频率显示按键
								pinlv_flag=1;
								shizhong_flag=0;
								dianya_flag=0;
								chaxun_flag=0;
								TH0=TL0=0;
							break;
							case 0x07://功能按键切换
								if((pinlv_flag!=1)&&(chaxun_flag!=1))
									{
										gongneng_flag=1;
										moshi_flag++;
									}
								else if((pinlv_flag==1)&&(chaxun_flag==0))
									{
										if(p_qiehuan==1) p_qiehuan=0;
										else p_qiehuan=1;
									}	
								else if((pinlv_flag==0)&&(chaxun_flag==1))
									{
										if(c_qiehuan==1) c_qiehuan=0;
										else c_qiehuan=1;
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
							case 0x0e: jia_flag=1;  break;//按键加
							case 0x0d: jian_flag=1; break;//按键减
							case 0x0b: 										//查询按键
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

/******************************************************************************************
函数名称：time1
功能：定时器1的中断服务函数
*******************************************************************************************/
void time1() interrupt 3
{
	if((shizhong_flag==1)&&(gongneng_flag==1))//设置标志位在设置时间时，数码管以一秒间隔再跳动
		{
			time_shan();
		}
	else if((dianya_flag==1)&&(gongneng_flag==1))//设置标志位在设置电压时，数码管以一秒间隔再跳动
		{
			dianya_shan();
		}
	
}

/******************************************************************************************
函数名称：time2
功能：定时器2的中断服务函数
*******************************************************************************************/
void time2() interrupt 12
{
	uchar start;
	display();
	if(pinlv_flag==1)//开始测频率的标志位
	{
		jishu++;
		if(jishu==495)
		{
			TR0=1;
			start=1;
		}
		if((start==1)&&(jishu==500))
			{
				TR0=0;
				start=0;
				jieshu_flag=1;
				jishu=0;
			}
	}
}

/******************************************************************************************
函数名称：dianya_shan
功能：设置电压上下限时闪烁函数
*******************************************************************************************/
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

/******************************************************************************************
函数名称：display
功能：显示处理函数
*******************************************************************************************/
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

/******************************************************************************************
函数名称：Timer0Init
功能：定时器0初始化函数
*******************************************************************************************/
void Timer0Init(void)		//2微秒@11.0592MHz
{
	AUXR|=0x80;					//定时器时钟1T模式
	TMOD&=0xF0;					//设置定时器模式
	TMOD|=0x04;
	TL0=0x00;						//设置定时初值
	TH0=0x00;						//设置定时初值
	TF0=0;							//清除TF0标志
}

/******************************************************************************************
函数名称：Timer1Init
功能：定时器1初始化函数
*******************************************************************************************/
void Timer1Init(void)		//5毫秒@11.0592MHz
{
	AUXR|=0x40;					//定时器时钟1T模式
	TMOD&=0x0F;					//设置定时器模式
	TL1=0x00;						//设置定时初值
	TH1=0x28;						//设置定时初值
	TF1=0;							//清除TF1标志
	TR1=1;							//定时器1开始计时
}

/******************************************************************************************
函数名称：Timer2Init
功能：定时器2初始化函数
*******************************************************************************************/
void Timer2Init(void)		//2毫秒@11.0592MHz
{
	AUXR|=0x04;					//定时器时钟1T模式
	T2L=0x9A;						//设置定时初值
	T2H=0xA9;						//设置定时初值
	AUXR|=0x10;					//定时器2开始计时
}

/******************************************************************************************
函数名称：time_chuli
功能：时间处理函数
*******************************************************************************************/
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
				switch(moshi_flag)
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

/******************************************************************************************
函数名称：time_shan
功能：设置时间时闪烁函数
*******************************************************************************************/
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

