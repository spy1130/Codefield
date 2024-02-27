/**********************************************************************************
ʵ�鹦�ܣ����߽�ʡ������������
ʱ�䣺2107.12.20
���ߣ��⿵
***********************************************************************************/
#include <STC15F2K60S2.H>
#include <onewire.h>

#define uchar unsigned char 
#define uint unsigned int	

uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF,0XC6};
uchar yi,er,san,si,wu ,liu,qi,ba;	
uchar moshi=1;										//����ģʽ����
uchar shijian=0;									//����ʱ����
uchar shiwen=0;										//�¶ȴ洢����
uint num=0,miao=0;								//��ʱ�õı�־λ

/**********************************************************************************
�������ƣ�delayms
���ܣ���ʱ����
***********************************************************************************/
void delayms(int ms)
{
	int i,j;
	for(i=ms;i>0;i--)
		for(j=940;j>0;j--);
}

/**********************************************************************************
�������ƣ�keyscan
���ܣ�����ɨ�躯��
***********************************************************************************/
void keyscan()
{
	if(P33==0)									//ģʽѡ�񰴼�
	{
		delayms(9);
		if(P33==0)
			{
				if(er==1) moshi=2;
				else if(er==2) moshi=3;
				else if(er==3) moshi=1;
			}
		while(!P33);
	}
	else if(P32==0)							//ʱ��ѡ�񰴼�
		{
			delayms(9);
			if(P32==0)
				{
					if(shijian==0) shijian=60;
					else if(shijian<=60) shijian=120;
					else if(shijian<=120) shijian=0;
				}
			while(!P32);
		}
	else if(P31==0)							//ʱ���������
		{
			delayms(9);
			if(P31==0) shijian=0;
			while(!P31);
		}
	else if(P30==0)							//�¶���ʾ����
		{
			delayms(9);
			if(P30==0)
				{	
					if(shiwen==0) shiwen=1;
					else if(shiwen==1) shiwen=0;
				}
			while(!P30);
		}
}

/**********************************************************************************
�������ƣ�display
���ܣ���ʾ����
***********************************************************************************/
void display1(uchar yi,uchar er)
{
		P2=0XC0;//��λѡ573   U8
		P0=0X01;//ѡ���һ�������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[yi];
		delayms(1);
		
		P2=0XC0;//��λѡ573   U8
		P0=0X02;//ѡ��ڶ��������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[er];
		delayms(1);
}	

void display2(uchar san,uchar si)
{
		P2=0XC0;//��λѡ573   U8
		P0=0X04;//ѡ������������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[san];
		delayms(1);
		
		P2=0XC0;//��λѡ573   U8
		P0=0X08;//ѡ����ĸ������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[si];
		delayms(1);
}

void display3(uchar wu,uchar liu)
{
		P2=0XC0;//��λѡ573   U8
		P0=0X10;//ѡ���һ�������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[wu];
		delayms(1);
		
		P2=0XC0;//��λѡ573   U8
		P0=0X20;//ѡ���һ�������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[liu];
		delayms(1);
}

void display4(uchar qi,uchar ba)
{
		P2=0XC0;//��λѡ573   U8
		P0=0X40;//ѡ���һ�������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[qi];
		delayms(1);
		
		P2=0XC0;//��λѡ573   U8
		P0=0X80;//ѡ���һ�������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[ba];
		delayms(1);
}

/**********************************************************************************
�������ƣ�Timer0Init
���ܣ���ʱ��0��ʼ������
***********************************************************************************/
void Timer0Init(void)		//100΢��@11.0592MHz
{
	AUXR |= 0x80;					//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;					//���ö�ʱ��ģʽ
	TL0 = 0xAE;						//���ö�ʱ��ֵ
	TH0 = 0xFB;						//���ö�ʱ��ֵ
	TF0 = 0;							//���TF0��־
	TR0 = 1;							//��ʱ��0��ʼ��ʱ
	EA=1;
	ET0=1;
}

/**********************************************************************************
�������ƣ�main
���ܣ�������
***********************************************************************************/
void main()
{
	allinit();													//���������ʼ��
	Timer0Init();												//��ʱ��0��ʼ��
	yi=10;er=1;san=10;si=11;wu=0;liu=0;qi=0;ba=0;
	while(1)
	{
		keyscan();												//����ɨ�躯��
		if(shiwen==0)											//��ʼ������ʾ
			{
				yi=10;
				san=10;
				si=11;
				er=moshi;
				wu=shijian/1000;
				liu=shijian%1000/100;
				qi=shijian%100/10;
				ba=shijian%10;
			}
		else if(shiwen==1)								//�����¶���ʾ����
			{
				yi=10;
				er=4;
				san=10;
				si=11;
				wu=11;
				liu=temget()/10;
				qi=temget()%10;
				ba=12;
				delayms(1);
			}		
		if(shijian>0)											//�ж����Ǹ�ģʽѡ�����Ǹ���
		{
			if(moshi==1)
				{	
					P2=0X80;
					P0=0XFE;
				}
			else if(moshi==2)
				{	
					P2=0X80;
					P0=0XFD;
				}
			else if(moshi==3)
				{	
					P2=0X80;
					P0=0XFB;
				}
		}
		else 
			{	
				P2=0X80;
				P0=0XFF;
			}
			display1(yi,er);									//��ʾ����
			display2(san,si);
			display3(wu,liu);
			display4(qi,ba);
		}
}

/**********************************************************************************
�������ƣ�Timer0
���ܣ���ʱ��0���жϷ�����
***********************************************************************************/
void Timer0() interrupt 1
{
	num++;miao++;
	if(num>10) num=1;												//��������
	if(miao==10000)													//��ʱʱ��1s
		{
			miao=0;
			if(shijian>0) shijian--;
		}
	if(shijian>0)														//�жϵ���ʱ�Ƿ�����������������PWM��
		{
			if(moshi==1)
				{
					if(num<3) P34=1;
					else P34=0;
				}
			else if(moshi==2)
				{
					if(num<4) P34=1;
					else P34=0;
				}
			else if(moshi==3)
				{
					if(num<8) P34=1;
					else P34=0;
				}
		}
}


