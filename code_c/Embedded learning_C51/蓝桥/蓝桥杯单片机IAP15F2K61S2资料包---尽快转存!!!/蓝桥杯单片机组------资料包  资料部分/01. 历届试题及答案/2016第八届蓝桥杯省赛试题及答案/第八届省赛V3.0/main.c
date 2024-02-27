#include <STC15F2K60S2.h>
#include <onewire.h>

#define uchar unsigned char
#define uint unsigned int 

uint count0,count1,count2;					//��ʱ����Ҫ�ı���
uchar mode;													//ģʽѡ�����
uchar baojing;											//���ӱ����ı�־λ
uchar flag=0;												//LED����˸ʱ����趨�ı�־λ
uchar flash;												//�������ʾ��˸�ı�־λ
uchar hour_n=0,min_n=0,sec_n=0;			//�趨������
uchar hour=23,min=59,sec=50;				//�趨�ĳ�ʼʱ��
uchar hour_i,min_i,sec_i;						//�趨��ʱ��
uchar temp;													//�¶ȴ洢����
uchar discount;											//�������ʾɨ�����
uchar code wei_xuan[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar code duan_xuan[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff,0xc6};

void delay(uint x)
{
	uint i,j;
	for(i=x;i>0;i--)
	for(j=110;j>0;j--);
}

void allinit()
{
	P2=0XA0;P0=0X00;
	P2=0X80;P0=0XFF;
	P2=0XC0;P0=0XFF;
	P2=0XE0;P0=0XFF;
}

void key()
{
	if(P30==0)
		{
			delay(20);
			if(P30==0)
				{
					while(!P30);
					hour_i=hour;
					min_i=min;
					sec_i=sec;
					mode++;
					if(mode>=4) mode=0;
				}
		}
		
	if(P31==0)
		{
			delay(20);
			if(P31==0)
				{
					while(!P31);
					if(mode>=4) mode++;
					if(mode<4)  mode=4;
					if(mode>=7) mode=0;
				}
		}
		
	if(P32==0)
		{
			delay(20);
			if(P32==0)
				{
					while(!P32);
					switch(mode)
						{
							case 1: hour_i++;if(hour_i>=24) hour_i=0;hour = hour_i;break;
							case 2: min_i++;if(min_i>=60) min_i=0;min = min_i;break;
							case 3: sec_i++;if(sec_i>=60) sec_i=0;sec = sec_i;break;
							case 4: hour_n++;if(hour_n>=24) hour_n=0;break;
							case 5: min_n++;if(min_n>=60) min_n=0;break;
							case 6: sec_n++;if(sec_n>=60) sec_n=0;break;
						}
				}
				
		}
		
	if(P33==0)
		{
			delay(20);
			if(P33==0)
				{
					if(mode==0) mode=7;
					while(!P33);
					if(mode==7) mode=0;
					switch(mode)
						{
							case 1: hour_i--;if(hour_i<=0) hour_i=23;hour = hour_i;break;
							case 2: min_i--;if(min_i<=0) min_i=59;min = min_i;break;
							case 3: sec_i--;if(sec_i<=0) sec_i=59;sec = sec_i;break;
							case 4: hour_n--;if(hour_n<=0) hour_n=23;break;
							case 5: min_n--;if(min_n<=0) min_n=59;break;
							case 6: sec_n--;if(sec_n<=0) sec_n=59;break;
						}
				}
		}
}

void xian_shi(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[hour/10];break;
		case 1:P0 = duan_xuan[hour%10];break;
		case 2:P0 = duan_xuan[10];break;
		case 3:P0 = duan_xuan[min/10];break;
		case 4:P0 = duan_xuan[min%10];break;
		case 5:P0 = duan_xuan[10];break;
		case 6:P0 = duan_xuan[sec/10];break;
		case 7:P0 = duan_xuan[sec%10];break;
	}
	i++;
	if(i>=8) i=0;
}
void xian_shi1(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[hour_i/10] | flash;break;
		case 1:P0 = duan_xuan[hour_i%10] | flash;break;
		case 2:P0 = duan_xuan[10];break;
		case 3:P0 = duan_xuan[min/10];break;
		case 4:P0 = duan_xuan[min%10];break;
		case 5:P0 = duan_xuan[10];break;
		case 6:P0 = duan_xuan[sec/10];break;
		case 7:P0 = duan_xuan[sec%10];break;
	}
	i++;
	if(i>=8) i=0;
}
void xian_shi2(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[hour/10];break;
		case 1:P0 = duan_xuan[hour%10];break;
		case 2:P0 = duan_xuan[10];break;
		case 3:P0 = duan_xuan[min_i/10] | flash;break;
		case 4:P0 = duan_xuan[min_i%10] | flash;break;
		case 5:P0 = duan_xuan[10];break;
		case 6:P0 = duan_xuan[sec/10];break;
		case 7:P0 = duan_xuan[sec%10];break;
	}
	i++;
	if(i>=8) i=0;
}
void xian_shi3(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[hour/10];break;
		case 1:P0 = duan_xuan[hour%10];break;
		case 2:P0 = duan_xuan[10];break;
		case 3:P0 = duan_xuan[min/10];break;
		case 4:P0 = duan_xuan[min%10];break;
		case 5:P0 = duan_xuan[10];break;
		case 6:P0 = duan_xuan[sec_i/10] | flash;break;
		case 7:P0 = duan_xuan[sec_i%10] | flash;break;
	}
	i++;
	if(i>=8) i=0;
}
void xian_shi4(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[hour_n/10] | flash;break;
		case 1:P0 = duan_xuan[hour_n%10] | flash;break;
		case 2:P0 = duan_xuan[10];break;
		case 3:P0 = duan_xuan[min_n/10];break;
		case 4:P0 = duan_xuan[min_n%10];break;
		case 5:P0 = duan_xuan[10];break;
		case 6:P0 = duan_xuan[sec_n/10];break;
		case 7:P0 = duan_xuan[sec_n%10];break;
	}
	i++;
	if(i>=8) i=0;
}
void xian_shi5(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[hour_n/10];break;
		case 1:P0 = duan_xuan[hour_n%10];break;
		case 2:P0 = duan_xuan[10];break;
		case 3:P0 = duan_xuan[min_n/10] | flash;break;
		case 4:P0 = duan_xuan[min_n%10] | flash;break;
		case 5:P0 = duan_xuan[10];break;
		case 6:P0 = duan_xuan[sec_n/10];break;
		case 7:P0 = duan_xuan[sec_n%10];break;
	}
	i++;
	if(i>=8) i=0;
}

void xian_shi6(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[hour_n/10];break;
		case 1:P0 = duan_xuan[hour_n%10];break;
		case 2:P0 = duan_xuan[10];break;
		case 3:P0 = duan_xuan[min_n/10];break;
		case 4:P0 = duan_xuan[min_n%10];break;
		case 5:P0 = duan_xuan[10];break;
		case 6:P0 = duan_xuan[sec_n/10] | flash;break;
		case 7:P0 = duan_xuan[sec_n%10] | flash;break;
	}
	i++;
	if(i>=8) i=0;
}

void xian_shi7(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[11];break;
		case 1:P0 = duan_xuan[11];break;
		case 2:P0 = duan_xuan[11];break;
		case 3:P0 = duan_xuan[11];break;
		case 4:P0 = duan_xuan[11];break;
		case 5:P0 = duan_xuan[temp/10];break;
		case 6:P0 = duan_xuan[temp%10];break;
		case 7:P0 = duan_xuan[12];break;
	}
	i++;
	if(i>=8) i=0;
}

void qie_huan(void)
{
	switch(mode)
	{
		case 0: xian_shi();break; 
		case 1: xian_shi1();break;
		case 2: xian_shi2();break;
		case 3: xian_shi3();break;
		case 4: xian_shi4();break;
		case 5: xian_shi5();break;
		case 6: xian_shi6();break;
		case 7: xian_shi7();break;
	}
}
void display(void)
{

	P2 = 0xc0;
	P0 = wei_xuan[discount];
	P2 = 0x00;
	P2 = 0xe0;
	qie_huan();
	P2 = 0x00;

	discount++;
	if(discount>=8) discount=0;
}

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

void led(uchar liang)
{
	P2=P2&0x1f;
	P2=0x80;
	P0=liang;
	P2=P2&0x1f;
}

void main()
{
	allinit();
	Timer0Init();
	while(1)
	{
		temp=Read_Temperature();
		key();
		if((hour==hour_n)&&(min==min_n)&&(sec==sec_n)) baojing=1;
	}	
}

void Timer0(void) interrupt 1
{
	display();
	count0++;
	if(baojing==1)					//�õ���˸ʱ����λ0.2��
		{
			if(count0>=100) 
				{
					if(flag==0)
						{
							flag=1;
							led(0xff);
						}
					else 
						{
							flag=0;
							led(0xfe);
						}
				}
		}
	else
		{ 
			P2=0x80;
			P0=0xff;
		}	
	if(count0>=500)
		{
			count0=0;
			count1++;
			if(count1>=2)
				{
					if(baojing==1) count2++;		//�޶��ƾ���˸5��
					if(count2>5)
						{
							count2=0;
							baojing=0;
						} 
					sec++;
					flash=~flash;								//���趨ʱ���ʱ�������õ������Լ��Ϊһ����˸
					if(sec>=60)
						{
							sec=0;
							min++;
							if(min>=60)
								{
									min=0;
									hour++;
									if(hour>=24) hour=0;
								}
						}
				}
		}
}

