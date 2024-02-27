/****************************************************************************************
ʵ�鹦�ܣ������ʡ��������ϰ
ʱ�䣺2017.12.17
���ߣ��⿵
****************************************************************************************/
#include <STC15F2K60S2.h>
#include <delay.h>
#include <onewire.h>
#define uchar unsigned char 

uchar dsbuff[8];
unsigned char code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
uchar discount = 0;										//�������λ���Ʊ���
uchar count_1,count_2=0;							//��ʱɨ���õı���
uchar tem,flag_1,flag_2=0,flag_3=0;		//��ʱɨ���õı���
uchar tem_max=30,tem_min=20;					//��ʼ���¶���������
uchar gongzuo;												//�����ű���
uchar num[4];													//�洢�����ֵ�ı���
uchar cishu=0;												//�жϰ������Ĵ�������������Ĵ������ú���
uchar clear=0,shezhi=0;								//���ð��������������ֵ
uchar qingchu=0;											//�����־λ
uchar tt=0,fan=0;											//����ʱ��Ŀ��Ʊ���
uchar liang_1=1,liang_2=0;						//����һ���ƿ��Ʊ���

/****************************************************************************************
�������ƣ�keyscan
���ܣ�����������
****************************************************************************************/
void keyscan()
{	
	uchar temp;
	P3=0x7f;
	P44=0;P42=1;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay5ms();
		Delay5ms();
		temp=P3;
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			if(shezhi==1)
			{
				switch(temp)
				{
					case 0x0e:num[cishu]=0;cishu++;break;
					case 0x0d:num[cishu]=3;cishu++;break; 
					case 0x0b:num[cishu]=6;cishu++;break; 
					case 0x07:num[cishu]=9;cishu++;break;  
				}
			}
		}
		while(P3!=0x7f);
	}
	P3=0xbf;
	P44=1;P42=0;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay5ms();
		Delay5ms();
		temp=P3;
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			switch(temp)
			{
				case 0x0e:
				if(shezhi==1){num[cishu]=1;cishu++;}break;
				case 0x0d:
				if(shezhi==1){num[cishu]=4;cishu++;}break; 
				case 0x0b:
				if(shezhi==1){num[cishu]=7;cishu++;}break; 
				case 0x07:
				{
					if(shezhi==0) {shezhi=1;cishu=0;}
					else if((shezhi==1)&&(flag_3==0)) shezhi=0;
				}
				break;  
			}	
		}
		while(P3!=0xbf);
	}

	P3=0xdf;
	P42=1;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
	{
		Delay5ms();
		Delay5ms();
		temp=P3;
		temp=P3&0x0f;
		if(temp!=0x0f)
		{
			if(shezhi==1)
			{
				switch(temp)
				{
					
					case 0x0e:num[cishu]=2;cishu++;break;
					case 0x0d:num[cishu]=5;cishu++;break; 
					case 0x0b:num[cishu]=8;cishu++;break; 
					case 0x07:{clear=1;cishu=0;qingchu=1;}break;  
				}
			}	
		}
		while(P3!=0xdf);
	}
}

/****************************************************************************************
�������ƣ�display
���ܣ��������ʾ����
****************************************************************************************/
void display()
{
	P2 = (P2&0x1f)|0xe0;		//�򿪶�ѡ������
	P0 = 0xff;							//�ر����������
	P2 = P2&0x1f;
		
	P2 = (P2&0x1f)|0xc0;		//��λѡ������
	P0 = 1<<discount;
	P2 = P2&0x1f;
	
	P2 = (P2&0x1f)|0xe0;		//�򿪶�ѡ������
	P0 = tab[dsbuff[discount]];
	P2 = P2&0x1f;

	if(++discount==8)
	 discount = 0;
}

/****************************************************************************************
�������ƣ�LED
���ܣ�LED������ƺ���
****************************************************************************************/
void LED(uchar liang)
{
	P2=P2&0x1f;
	P2=P2|0x80;
	P0=liang;
	P2=P2&0x1f;
}

/****************************************************************************************
�������ƣ�JIDIANQI
���ܣ��̵���ͨ�Ͽ��ƺ���
****************************************************************************************/
void JIDIANQI(uchar xiang)
{
	P2=P2&0x1f;
	P2=P2|0xa0;
	P0=xiang;
	P2=P2&0x1f;
}

/****************************************************************************************
�������ƣ�allinit
���ܣ����ӳ�ʼ������
****************************************************************************************/
void allinit()
{
	P2=(P2&0x1f)|0x80;P0=0xff;
	P2=(P2&0x1f)|0xa0;P0=0x00;
	P2=(P2&0x1f)|0xc0;P0=0xff;
	P2=(P2&0x1f)|0xe0;P0=0xff;
}

/****************************************************************************************
�������ƣ�Timer1Init
���ܣ���ʱ��1��ʼ������
****************************************************************************************/
void Timer1Init(void)		//5����@11.0592MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x00;			//���ö�ʱ��ֵ
	TH1 = 0x28;			//���ö�ʱ��ֵ
	TF1 = 0;				//���TF1��־
	TR1 = 1;				//��ʱ��1��ʼ��ʱ
}

/****************************************************************************************
�������ƣ�Timer0Init
���ܣ���ʱ��0��ʼ������
****************************************************************************************/
void Timer0Init(void)		//2����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x9A;			//���ö�ʱ��ֵ
	TH0 = 0xA9;			//���ö�ʱ��ֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
}

/****************************************************************************************
�������ƣ�main
���ܣ�������
****************************************************************************************/
void main()
{
	allinit();
	Timer0Init();
	Timer1Init(); 
	EA=1;
	ET0=1;
	ET1=1;
	while(1)
	{
		if(flag_2==1)
		{
			keyscan();
			flag_2=0;
		}	
	   if(shezhi==0)		//��ʼ��ʾ�¶ȹ���״̬����
			{
				dsbuff[7]=tem%10;
				dsbuff[6]=tem/10;
				dsbuff[5]=11;
				dsbuff[0]=10;
				dsbuff[1]=gongzuo;
				dsbuff[2]=10;
				dsbuff[3]=11;
				dsbuff[4]=11;

				if(flag_1==1)
					{
						tem=wendu();
						flag_1=0;	
					}
				if((tem<=tem_max)&&(tem>=tem_min))
					{
						gongzuo=1;
						JIDIANQI(0x00);
					}
				else if	(tem>tem_max)
					{
						gongzuo=2;
						JIDIANQI(0x10);
					}
				else 
				{
					gongzuo=0;
					JIDIANQI(0x00);
				}
			}
		else 					   //���������¶������޽���
			{
				dsbuff[7]=num[3];
				dsbuff[6]=num[2];
				dsbuff[5]=10;
				dsbuff[0]=10;
				dsbuff[1]=num[0];
				dsbuff[2]=num[1];
				dsbuff[3]=11;
				dsbuff[4]=11;
				if(clear==1)								//����������º�ȫ������
				 {
						clear=0;
						num[0]=0;
						num[1]=0;
						num[2]=0;
						num[3]=0;
				 }
				 if((cishu==4)||(qingchu==1))//�����������Ĵ�ʱ�����ж�
				 {
							qingchu=0;
							tem_max=num[0]*10+num[1];//���ݴ���
							tem_min=num[2]*10+num[3];
							if(tem_max<tem_min)
							{
										flag_3=1;
										LED(0xfd);
										liang_1=0;
										liang_2=1;
							}
						else 
							{
										flag_3=0;
										LED(0xff);
										liang_1=1;
										liang_2=0;
							}
					}	
			}
	}
}
/****************************************************************************************
�������ƣ�time_1
���ܣ���ʱ��1���жϷ�����
****************************************************************************************/
void time_1() interrupt 3
{
	++tt;
	if(gongzuo==0)
	{
		if(tt==160)
		{
			tt=0;
			if(fan==0)						 
			{
				fan=1;
				LED(0xfe);
			}
			else 
			{
				fan=0;
				LED(0xff);
			}
		}
	}
	else if(gongzuo==1)
	{

		if(tt==80)
		{
			tt=0;
			if(fan==0)
			{
				fan=1;
				LED(0xfe);
			}
			else 
			{
				fan=0;
				LED(0xff);
			}
		}	
	}
	else if(gongzuo==2)
	{ 
		if(tt==40)
		{
			tt=0;
			if(fan==0)
			{
				fan=1;
				LED(0xfe);
			}
			else 
			{
				fan=0;
				LED(0xff);
			}
		}
	}
}

/****************************************************************************************
�������ƣ�time_0
���ܣ���ʱ��0���жϷ�����
****************************************************************************************/
void time_0() interrupt 1
{
	display();			//��ʾ������ʱ��ʾ
	if(++count_1==100)
	{
		flag_1=1;
		count_1=0;	
	}
	if(++count_2==1)
	{
		count_2=0;
		flag_2=1;
	}
}

