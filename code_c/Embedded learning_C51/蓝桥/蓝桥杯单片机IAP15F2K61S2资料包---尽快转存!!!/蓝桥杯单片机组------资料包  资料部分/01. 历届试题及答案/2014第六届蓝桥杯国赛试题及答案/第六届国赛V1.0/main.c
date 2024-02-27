#include <STC15F2K60S2.H>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int	

sbit TX=P1^0;  //��������
sbit RX=P1^1;  //��������

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_(); _nop_();}
uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
uchar dsbuff[8]={11,11,11,11,11,11,11,11};
uchar discount;
uint count=0;
uchar flag;
uint t=0;

void allinit()
{
	P2=0XA0;
	P0=0X00;//�رշ��������̵���
	
	P2=0X80;
	P0=0XFF;//�ر�LED��
	
	P2=0XC0;
	P0=0XFF;//ѡ�����������
	P2=0XFF;
	P0=0XFF;//�ر����������
}

void display()
{
	P2=0XEF;
	P0=0XFF;
	P2=0X1F;
	
	P2=0XCF;
	P0=1<<discount;
	P2=0X1F;
	
	P2=0XEF;
	P0=tab[dsbuff[discount]];
	P2=0X1F;
	
	if(++discount==8) discount=0;
}

void Timer0Init(void)		//2����@11.0592MHz
{
	AUXR |= 0x80;					//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;					//���ö�ʱ��ģʽ
	TL0 = 0x9A;						//���ö�ʱ��ֵ
	TH0 = 0xA9;						//���ö�ʱ��ֵ
	TF0 = 0;							//���TF0��־
	TR0 = 1;							//��ʱ��0��ʼ��ʱ
	EA = 1;								//�����ж�
	ET0 = 1;  						//�򿪶�ʱ��0�ж�
}

void Timer1Init(void)		//2����@11.0592MHz
{
	AUXR |= 0x40;					//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;					//���ö�ʱ��ģʽ
}

//TX���ŷ���40KHz�����ź���������������̽ͷ
//ʹ�������ʱע��RC����Ƶ��
void send_wave(void)
{
	uchar i=8;  //����8������
	do
	{
		TX = 1;
		somenop;somenop;somenop;somenop;somenop;somenop;somenop;somenop;somenop;somenop;		
		TX = 0;
		somenop;somenop;somenop;somenop;somenop;somenop;somenop;somenop;somenop;somenop;		
	}
	while(i--);
}

void main()
{
	uint distance;
	Timer0Init();
  Timer1Init();
	allinit();
	while(1)
		{
			if(flag==1) // 200�������һ������ 
				{
					flag=0;
					send_wave();  //���ͷ����ź�
					TR1=1;  //������ʱ
					while((RX==1) && (TF1==0));  //�ȴ��յ�����
					TR1=0;  //�رռ�ʱ		
					if(TF1==1)//�������
						{
							TF1=0;
							distance=999;  //�޷���
						}
					else
						{		
							t=TH1;// ����ʱ��  
							t<<=8;
							t|=TL1;
							distance=(uint)(t*0.017);  //�������	
							distance=distance/12;
							//t*340   // t/1000000*340/2*100
						}
					TH1=0;
					TL1=0;
				}
				dsbuff[5]=distance/100;
				dsbuff[6]=distance%100/10;
				dsbuff[7]=distance%10; 
	}
}

void Timer0(void)  interrupt 1  
{
	TL0=0x9A;		
	TH0=0xA9;		
	display();
	if(++count==100)
	{
		flag=1;
		count=0;
  }
}
