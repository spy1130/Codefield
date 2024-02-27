#include <STC15F2K60S2.h>
#include <delay.h>
#include <ds1302.h>

#define uchar unsigned char  
#define uint unsigned int 

uchar code tab[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar shizhong_flag=0,gongneng_flag=0,jia_flag=0,jian_flag=0;
uchar hour=0,minute=0,second=0,moshi_flag=0;	
uchar s_liang=0,f_liang=0,m_liang=0;
uchar count_1=0,count_2=0,count_3=0;
uchar dsbuff[8]={1,2,3,4,5,6,7,8};
uchar discount=0;

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

	if(++discount == 8)
	 discount = 0;
}

void Timer1Init(void)		//5����@11.0592MHz
{
	AUXR |= 0x40;					//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;					//���ö�ʱ��ģʽ
	TL1 = 0x00;						//���ö�ʱ��ֵ
	TH1 = 0x28;						//���ö�ʱ��ֵ
	TF1 = 0;							//���TF1��־
	TR1 = 1;							//��ʱ��1��ʼ��ʱ
	EA=1;
	ET1=1;
}

void Timer0Init(void)		//2����@11.0592MHz
{
	AUXR |= 0x80;					//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;					//���ö�ʱ��ģʽ
	TL0 = 0x9A;						//���ö�ʱ��ֵ
	TH0 = 0xA9;						//���ö�ʱ��ֵ
	TF0 = 0;							//���TF0��־
	TR0 = 1;							//��ʱ��0��ʼ��ʱ
	ET0=1;
	EA=1;
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
				{
					if(gongneng_flag==1) gongneng_flag=0;   
					shizhong_flag=1;moshi_flag=0;
				}
				break;
				case 0x0d:    break;
				case 0x0b:    break;
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
				case 0x0e: jia_flag=1; 	break;
				case 0x0d: jian_flag=1; break;
				case 0x0b: 							break;
				case 0x07: 							break;
			}
		}
		while(P3!=0xbf);
	}

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
				switch(moshi_flag)// ������˸״̬ ��������������д�������ʾ ��Ӧ�������ж�����ʾ 
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
}



