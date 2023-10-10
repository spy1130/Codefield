#include "reg52.h"

sbit LED =P2^0;
sbit LED1=P2^1;

unsigned char T3RH, T3RL;


// 1<=max<=277us
void ConfigTime0Mode3(unsigned int usl,unsigned int ush)
{
	unsigned long temp;
	if(usl>277) usl=277;
	if(ush>277) ush=277;
	
	//���õ�һ����ʱ��
	temp = 11059200 / 12; // ����Ƶ�� 65536
	temp = (temp * usl) / 1000000;
	temp = 256 - temp;			   // ��ֵ
	T3RL = temp;
	
	//���õڶ�����ʱ��
	temp = 11059200 / 12; // ����Ƶ�� 65536
	temp = (temp * ush) / 1000000;
	temp = 256 - temp;			   // ��ֵ
	T3RH = temp;
	
	TMOD &= 0XF0;					   // ����������Ѻ�����0��1111 0000��1/0->0
	TMOD |= 0X03;					   // �������⹦�ܼĴ���TMOD\ģʽ3�����úù���ģʽ,����������Է�ֹӰ�������õ�ֵ 1/0->1
	
	TL0=T3RL;
	TH0=T3RH;
	
	TF0 = 0; // ���TF0��־
	TR0 = 1; // ����(��)��һ���й�TL0�Ķ�ʱ��
	TR1 = 1; // ����(��)�ڶ����й�TH0�Ķ�ʱ��
}

int main()
{
	unsigned int cntL,cntH;

	ConfigTime0Mode3(2000,1000); // �趨2000usʱ��
	while (1)
	{ // ��һ����ʱ���Ƿ����
		if (TF0 == 1)
		{
			cntL++;
			if (cntL >= 5000)
			{
				cntL = 0;
				LED = !LED;
			}
			TF0 = 0; // ��ʱ����
			TL0=T3RL;
		}
		// �ڶ�����ʱ���Ƿ����
		if (TF1 == 1)
		{
			cntH++;
			if (cntH >= 5000)
			{
				cntH = 0;
				LED1 = !LED1;
			}
			TF1 = 0; // ��ʱ����
			TL0=T3RH;
		}
		
		}
	}
