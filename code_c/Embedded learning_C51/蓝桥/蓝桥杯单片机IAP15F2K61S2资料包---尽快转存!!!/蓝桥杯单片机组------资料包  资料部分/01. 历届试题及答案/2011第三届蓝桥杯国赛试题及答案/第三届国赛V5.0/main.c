/***************************************************************************************************
ʵ�鹦�ܣ��������������������
ʱ�䣺
���ߣ��⿵
****************************************************************************************************/
#include <STC15F2K60S2.h>
#include <delay.h>
#include <ds1302.h>
#include <iic.h>
#define uchar unsigned char
#define uint unsigned int

#define TX P10															//����
#define RX P11															//����
#define somenop1 {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}

uchar code tab[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar dsbuff[8]={11,11,11,11,11,11,11,11};
uchar num[6]={11,11,11,11,11,11};
uchar mima[6]={6,5,4,3,2,1};								//��ʼ��������
uchar discount = 0;													//�������ʾ��λ����
uchar ceju_flag=0;													//��ʱ����־����
uchar mima_flag;														//����ģʽ��־λ
uchar set=0;																//���ð�����־λ
uchar admit=0;															//ȷ�ϰ�����־λ
uchar reset=0;															//��λ������־λ
uchar exit=0;																//�˳�������־λ
uchar i=0;																	//����
uchar now;																	//���ڵ�ʱ��洢����
uchar false;																//����Ĵ�����־λ
uchar xiang_flag;														//���������ʱ���־λ
uchar l_count=4;														//LED����˸��־λ
uchar temp=0;																//����
uint distance;															//�������
uchar k;																		//����

/***************************************************************************************************
																						��������
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
�������ƣ�clear
���ܣ�������뺯��
****************************************************************************************************/
void clear()
{
	for(k=0;k<=5;k++)
	num[k]=11;
	i=0;		//�ر�ע��
}

/***************************************************************************************************
�������ƣ�allinit
���ܣ����ӳ�ʼ������
****************************************************************************************************/
void allinit()
{
	P2=0x80;P0=0xff;
	P2=0xa0;P0=0x00;
	P2=0xc0;P0=0xff;
	P2=0xe0;P0=0xff;
}

/***************************************************************************************************
�������ƣ�main
���ܣ�������
****************************************************************************************************/
void main()
{
	allinit();														//���ӳ�ʼ��
	Timer0Init();													//��ʱ��0��ʼ��
	Timer1Init();													//��ʱ��1��ʼ��
	dswrite();														//ʱ�ӳ�ʼ��
	while(1)
	{
		keyscan();													//����ɨ�躯��
		dsread();														//��ȡʱ��
		if((shijian[2]>=7)&&(shijian[2]<22))//�ж�ʱ��
		{
			mima_flag=0;											//������״̬��־λ
			zidongmen();											//�Զ��ſ��ƺ���
		}
		else 																//��������״̬��
		{
			mima_flag=1;
			mimamen();												//�����Ŵ�����
		}

		if(mima_flag==0)  									//������������ʾ
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
�������ƣ�mimamen
���ܣ������Ŵ���������
****************************************************************************************************/
void mimamen()
{
	static uchar wancheng_flag=0,j=0,true=0;
	static uchar true_2=0,false_2=0,flag1=0;
	if(set==0)											//û�а������ü�ʱ����ʾ�� ������
	{	
		dsbuff[0]=10;
		dsbuff[1]=10;
		dsbuff[2]=num[0];
		dsbuff[3]=num[1];	
		dsbuff[4]=num[2];
		dsbuff[5]=num[3];
		dsbuff[6]=num[4];
		dsbuff[7]=num[5];
		if(admit==1)									//����ȷ�ϼ�����жԱȣ�������������Ƿ���ȷ
		{
			admit=0;
			for(j=0;j<=5;j++)
			{
				if(num[j]==mima[j])
					true=1;									//��ȷ�ı�־λ
				else 
				{
					true=2;									//����ȷ�ı�־λ
				}
			}
		}
		if(true==1)										//�����ȷ
		{
			relay(1);										//�̵�����
			clear();										//��֤ͨ�����������洢������
			if((shijian[0]-now==5)||(60+shijian[0]-now==5))//�ü̵���ͨ5��
			{
				true=0;										//�����־λ
				relay(0);									//�ر������
			}
		}
		else if (true==2)							//������벻��ȷ
		{
			false++;										//����Ĵ�����һ��������ʱ������
			clear();										//û�������һ�ζ�Ҫ��������洢�����ݸ����㣬��Ҫ���˰�iҲ����
			true=0;											//�����־λ
		}
		if(false==3)									//��������������������
		{
			l_count=0;
			ledctrl(1,1);
			beep(1);
			false=0;
			admit=0;
			xiang_flag=1;								//���������������־λ
//			if((shijian[0]-now==3)||(60+shijian[0]-now==3))//����������������
//			{
//				ledctrl(1,0);
//				beep(0);
//				false=0;
//				admit=0;
//			}
		}
	}
	else //�������ð���
	{
		
		
		if(exit==1)										//�˳�������
		{
			exit=0;											//�Ƴ���־λ����
			true_2=0;
			set=0;	
			clear(); 										//�����������		
		}
		if(true_2==0)									//������ʱ�ı�־λ
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
		if(true_2==1)									//�����admit������admit���ͻ Ӧ�ð�״̬�жϷ��ں��� 
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
			}//���������ձ�־λ		
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
					} //����ط��ı���true2��״̬ 
			}	
		}
		if(true_2==2)
		{
			false_2++;
			clear();
			true_2=0;
		}
		if(false_2==3)//������ʱ����������
		{
			set=0;
			ledctrl(2,1);
			beep(1);
			xiang_flag=1;
			false_2=0;	
		}
	}

	if(reset==1)//���¸�λ����
	{
		reset=0;
		for(k=0;k<=5;k++)
		{
			mima[k]=6-k;
		}
	}
}

/***************************************************************************************************
�������ƣ�zidongmen
���ܣ��Զ��Ŵ���������
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
			kai_flag=1;//���ŵı�־λ
			temp=shijian[0];//����ʱ��ʱ�丳ֵ��temp
		}
		if(kai_flag==1)
		{
			relay(1);//���Ŵ򿪼̵���
			if((shijian[0]-temp==5)||(60+shijian[0]-temp==5))
			{
				kai_flag=0;
				relay(0);//5���رռ̵���
			}	
		}
}

/***************************************************************************************************
�������ƣ�keyscan
���ܣ���������������
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
�������ƣ�Timer0Init
���ܣ���ʱ��0��ʼ������
****************************************************************************************************/
void Timer0Init(void)		//2����@11.0592MHz
{
	AUXR |= 0x80;					//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;					//���ö�ʱ��ģʽ
	TL0 = 0x9A;						//���ö�ʱ��ֵ
	TH0 = 0xA9;						//���ö�ʱ��ֵ
	TF0 = 0;							//���TF0��־
	TR0 = 1;							//��ʱ��0��ʼ��ʱ
	EA=1;
	ET0=1;
}

/***************************************************************************************************
�������ƣ�display
���ܣ��������ʾ����
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
�������ƣ�relay
���ܣ��̵������ƺ���
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
�������ƣ�beep
���ܣ����������ƺ���
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
�������ƣ�ledctrl
���ܣ�led���ƺ���
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
�������ƣ�sendwave
���ܣ��������κ���
****************************************************************************************************/
void sendwave()
{
	TX=1;
	somenop1;somenop1;somenop1;somenop1;somenop1;
	somenop1;somenop1;somenop1;somenop1;somenop1;
	TX=0;
}

/***************************************************************************************************
�������ƣ�get_distance
���ܣ�������㺯��
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
�������ƣ�Timer1Init
���ܣ���ʱ��1��ʼ������
****************************************************************************************************/
void Timer1Init(void)		//0΢��@11.0592MHz
{
	AUXR &= 0xBF;					//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;					//���ö�ʱ��ģʽ
	TL1 = 0x00;						//���ö�ʱ��ֵ
	TH1 = 0x00;						//���ö�ʱ��ֵ
	TF1 = 0;							//���TF1��־
	TR1 = 0;							//��ʱ��1��ʼ��ʱ
}

/***************************************************************************************************
�������ƣ�Timer0
���ܣ���ʱ��0�жϷ�����
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
	if(l_count<=2)//��˸����
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



