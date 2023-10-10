#include "reg52.h"

sbit LED = P2^0;

unsigned char T0RH, T0RL;

// 1<=max<=71
void ConfigTime0(unsigned int ms)
{
	unsigned long temp;
	temp = 11059200 / 12; // ����Ƶ�� 65536
	temp = (temp * ms) / 1000;
	temp = 65536 - temp;			   // ��ֵ
	T0RH = (unsigned char)(temp >> 8); // �����߰�λ
	T0RL = (unsigned char)(temp);		   // �����Ͱ�λ

	TMOD = 0X01; // �������⹦�ܼĴ���TMOD�����úù���ģʽ

	TH0 = T0RH;
	TL0 = T0RL;
	TF0 = 0; // ���TF0��־
	TR0 = 1; // ����(��)��ʱ��
}
int main()
{

	ConfigTime0(10000); // ��ʱʱ��
	while (1)
	{
		if (TF0 == 1)
		{
			TF0 = 0; // ��ʱ����
			TH0 = T0RH;
			TL0 = T0RL; // ���¼�ʱ
			LED = !LED;
		}
	}
}
