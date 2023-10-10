#include <reg52.h>

sbit Wave=P3^3;
unsigned char T0RH,T0RL;
unsigned int count=0;
bit flag1s=0;//��־λ����int char����һ����ֻ��1��0



//1<=max<=71
void ConfigTimer0(unsigned char ms)
{
	unsigned long tmp;
	
	tmp=11059200/12;		//����Ƶ��
	tmp=(tmp*ms)/1000;
	tmp=65536-tmp;			//��ֵ
	T0RH=(unsigned char)(tmp>>8);
	T0RL=(unsigned char)(tmp);
	
	ET0=1;
	TMOD=0x01;	//���ù���ģʽ
	TH0=T0RH;	//���ö�ʱʱ��
	TL0=T0RL;
	TR0=1;		//������ʱ��
}
void ConfigINT0(void)
{
	IT0=1; //�½��ش���
	EX0=1;
	
}

void main()
{	
	EA=1;
	P1=0x0E; //ѡ��8��LED
	ConfigTimer0(10);	//�趨10ms
	ConfigINT0();
	

	while(1)
	{		
		
//			if(tmp!=Wave){
//				count++;
//				tmp=Wave;//����״̬
//			}
//			if(flag1s)
//			{
//				//count=1000  =0000 0011 1110 1000
//			count/=2; �������»������
//				count>>=4;  0000 0000 0011 1110 ���8λ�ɶ�
//				P0=(count>>4);
//				count=0
//			}
	}
}

//T0��ʱ���жϺ���
void InterruptTimer0() interrupt 1
{	
	static unsigned int cnt=0;

	TH0=T0RH;
	TL0=T0RL;

	cnt++;
	if(cnt>=100)
	{
		cnt=0;
		P0=(count>>4); 
		count=0;
	}
}
//�������½��ص�ʱ��Ƭ���Զ���ת���жϺ���
void InterruptINT0() interrupt 0
{
	count++;
}