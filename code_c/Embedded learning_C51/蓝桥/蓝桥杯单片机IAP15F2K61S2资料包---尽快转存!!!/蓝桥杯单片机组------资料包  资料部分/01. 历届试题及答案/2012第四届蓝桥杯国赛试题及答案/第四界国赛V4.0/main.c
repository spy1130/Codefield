/**********************************************************************************************
ʵ�鹦�ܣ����Ľ�����������
ʱ�䣺2017
���ߣ��⿵
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
uint distance;														//����
uchar discount;														//�������λ��ʾ���Ʊ���
uchar open=0;															//��ʼ����־λ
uchar s4,s5,s6,s7;												//������־λ
uchar shi_shan,fen_shan,miao_shan;				//����ʱ����˸��־λ
uchar ss1,ss2,ss3,ss4;										//����
uchar ceju_flag;													//��ඨʱɨ���־λ
uchar s_liang,f_liang,m_liang;						//����������־λ
uchar dis_flag,baocun_flag,beep_distance;	//�������룬��������־λ
uchar led_shan_flag,l_liang;							//LED��˸��־λ
uchar flag_1=1;														//�ٴγ�ʼ����־λ

/**********************************************************************************************
																			��������
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
�������ƣ�allinit
���ܣ����������ʼ������
***********************************************************************************************/
void allinit()
{
	P2=0x80;P0=0xff;
	P2=0xa0;P0=0x00;
	P2=0xc0;P0=0xff;
	P2=0xe0;P0=0xff;
}

/**********************************************************************************************
�������ƣ�main
���ܣ�������
***********************************************************************************************/
void main()
{
	Timer0Init(); 
	Timer1Init();
	beep_distance=iicread(0x00);
	dswrite(); 
	while(1)
	{
		if(open==0)//��ʼ����һ����
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
		else if(open==1)//һ����������ʾ����
		{
			if(flag_1==1)//ֻ����һ�γ�ʼ��
			{
				allinit();
				flag_1=0;
			}	
			keyscan();//����ɨ��
			dsread();//��ȡds1302��ʱ��
			if(s7==0)//s7û�а���ʱ��ʾʱ��
			{
				shizhong_handle();
			}
			if(s7==1)//s7����ʱ��ʾ����
			{
				distance_handle();
			}
		}
	}			
}

/**********************************************************************************************
�������ƣ�distance_handle
���ܣ����봦����
***********************************************************************************************/
void distance_handle()
{
	switch(s6)//�ھ�����ʾ�����£�s6û�а��£�����ʾ���룬s6������ʾ���ñ����������
	{
		case 1:dis_flag=1;break;
		case 2:dis_flag=0;baocun_flag=1;s6=0;break;
	}
	if(ceju_flag==1)//��ʱɨ���ຯ��
	{
		distance=dis_get();
		if(distance<beep_distance)//�ж����ڵľ����Ƿ�С�ڱ�������
		{
			fengming(1);
			led_shan_flag=0;
		}
		else if((distance<1.2*beep_distance)&&(distance>=beep_distance))//�ж����ھ����Ƿ���ڱ������룬��С�ڱ��������1.2��
		{
			fengming(0);
			led_shan_flag=1;
		}
		else if(distance>=1.2*beep_distance)   //ע���������ڱ��������1.2��
		{
			fengming(0);
			ledctrl(1,0);
			led_shan_flag=0;
		}
		ceju_flag=0;
	}
	if(s6==0)//s6û�а�����ʾ����
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
	else //s6������ʾ���ñ�������
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
			if(s5==1)//�����ñ��������°���s5�ӱ�������
			{
				s5=0;
				beep_distance+=1;
			}
			else if(s4==1)//����s4����������
			{
				s4=0;
				beep_distance-=1;
			}	
		}
	}	
	if(baocun_flag==1)//��������������õı������뱣����eeprom
	{
		baocun_flag=0;
		iicwrite(0x00,beep_distance);
	}
}

/**********************************************************************************************
�������ƣ�dis_get
���ܣ�������㺯��
***********************************************************************************************/
uint dis_get()
{
	uint dis;
	sendwave();//����һ��40KHZ�ķ���
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
	TH1=TL1=0;//�ǵ�Ҫ�������
	return dis;
}

/**********************************************************************************************
�������ƣ�sendwave
���ܣ�����40Khz�ķ����źź���
***********************************************************************************************/
void sendwave()
{
	TX=1;
	somenop1;somenop1;somenop1;somenop1;somenop1;
	somenop1;somenop1;somenop1;somenop1;somenop1;
	TX=0;									  
}

/**********************************************************************************************
�������ƣ�timer0
���ܣ���ʱ��0�жϷ�����
***********************************************************************************************/
void time_0() interrupt 1
{
	display();
	if(ss1<500)
	++ss1;
	if(ss1==500)//������ʾ�õĶ�ʱ
	{
		open=1;
		ss1=501;
	}
	shizhong_shan();//ʱ����˸������ʱɨ��
	if(++ss3==200)//����־λ
	{	
		ss3=0;
		ceju_flag=1;
	}
	if(led_shan_flag==1)//LED��˸����
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
�������ƣ�keyscan
���ܣ�����ɨ�躯��
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
�������ƣ�display
���ܣ���ʾ����
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
�������ƣ�Timer0Init
���ܣ���ʱ��0��ʼ������
***********************************************************************************************/
void Timer0Init(void)		//2����@11.0592MHz
{
	AUXR |= 0x80;					//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;					//���ö�ʱ��ģʽ
	TL0 = 0x9A;						//���ö�ʱ��ֵ
	TH0 = 0xA9;						//���ö�ʱ��ֵ
	TF0 = 0;							//���TF0��־
	TR0 = 1;							//��ʱ��0��ʼ��ʱ
}

/**********************************************************************************************
�������ƣ�ledctrl
���ܣ�LED������ƺ���
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
�������ƣ�fengming
���ܣ��̵���ͨ�Ͽ��ƺ���
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
�������ƣ�shizhong_shan
���ܣ�ʱ����˸���ƺ���
***********************************************************************************************/
void shizhong_shan()
{
	if(shi_shan==1)//ʱ��˸��־λ
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
�������ƣ�shizhong_handle
���ܣ�ʱ�Ӵ�����
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
	else if(fen_shan==1)	  //����ѭ��ʱ������ʾ �߽��ж�
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
�������ƣ�Timer1Init
���ܣ���ʱ��1��ʼ������
***********************************************************************************************/
void Timer1Init(void)		//0΢��@11.0592MHz
{
	AUXR &= 0xBF;					//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;					//���ö�ʱ��ģʽ
	TL1 = 0x00;						//���ö�ʱ��ֵ
	TH1 = 0x00;						//���ö�ʱ��ֵ
	TF1 = 0;							//���TF1��־
	TR1 = 0;							//��ʱ��1��ʼ��ʱ
	EA=1;
	ET0=1;	
}
	