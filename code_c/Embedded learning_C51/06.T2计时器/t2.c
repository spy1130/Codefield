#include "reg52.h"

sbit LED = P2 ^ 0;

unsigned char T2RH, T2RL;


// 1<=max<=277us
void ConfigTime0Mode2(unsigned int us)
{
	unsigned long temp;
	if(us>277) us=277;
	temp = 11059200 / 12; // ����Ƶ�� 65536
	temp = (temp * us) / 1000000;
	temp = 256 - temp;			   // ��ֵ
	TH0 = temp;
	TL0 = temp;//8λ�Զ���װģʽ
	TMOD &= 0XF0;					   // ����������Ѻ�����0��1111 0000��1/0->0
	TMOD |= 0X02;					   // �������⹦�ܼĴ���TMOD\ģʽ2�����úù���ģʽ,����������Է�ֹӰ�������õ�ֵ 1/0->1

	
	TF0 = 0; // ���TF0��־
	TR0 = 1; // ����(��)��ʱ��
}

int main()
{
	unsigned int cnt0;

	ConfigTime0Mode2(2000); // �趨2000usʱ��
	while (1)
	{ // �ж�T0���Ƿ���ʱ�䣨�����
		if (TF0 == 1)
		{
			cnt0++;
			if (cnt0 >= 5000)
			{
				cnt0 = 0;
				LED = !LED;
			}
			TF0 = 0; // ��ʱ����
		}
		
		}
	}
