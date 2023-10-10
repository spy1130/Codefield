#include <reg52.h>


unsigned char T0RH,T0RL;
unsigned long count=0; //��¼��λʱ������������
unsigned long spill=0 //��¼�������
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
	TMOD &= 0xF0;
	Tmod |= 0x01;
	TMOD=0x01;	//���ù���ģʽ
	TH0=T0RH;	//���ö�ʱʱ��
	TL0=T0RL;
	TR0=1;		//������ʱ��
}

void Configcount1(void)
{	
	ET1=1; //��֤���������ж�
	TMOD &= 0xF0;
	TMOD |= 0x50;
	TH1=0;
	TL1=0��
	TR1=1;		//����������
}

void main()
{	
	EA=1;
	P1=0x0E; //ѡ��8��LED
	ConfigTimer0(10);	//�趨10ms
	Configcount1();
	while(1)
	{		

	}
}

//T0��ʱ���жϺ���
void InterruptTimer0() interrupt 1
{	
	static unsigned int cnt=0;
	unsigned int tmp=0;
	
	TH0=T0RH;
	TL0=T0RL;

	cnt++;
	if(cnt>=100)
	{
		cnt=0;
		count=spill*65535;
		tmp=(TH1<<8)|TL1 //�����
		count+=tmp; //���յ��������
		TH1=0;
	  TL1=0��
		spill=0;
		P0=(count>>4); 
	}
}
void InterruptCount1() interrupt 3
{
	//������65535
	spill++;
}