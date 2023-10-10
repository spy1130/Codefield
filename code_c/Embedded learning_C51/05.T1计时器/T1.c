#include "reg52.h"

sbit LED = P2 ^ 0;
sbit LED2 = P2 ^ 1;

unsigned char T0RH, T0RL;
unsigned char T1RH, T1RL;

// 1<=max<=71
void ConfigTime0(unsigned int ms)
{
	unsigned long temp;
	temp = 11059200 / 12; // ����Ƶ�� 65536
	temp = (temp * ms) / 1000;
	temp = 65536 - temp;			   // ��ֵ
	T0RH = (unsigned char)(temp >> 8); // �����߰�λ
	T0RL = (unsigned char)(temp);	   // �����Ͱ�λ
	TMOD &= 0XF0;					   // ����������Ѻ�����0��1111 0000��1/0->0
	TMOD |= 0X01;					   // �������⹦�ܼĴ���TMOD\T0�����úù���ģʽ,����������Է�ֹӰ�������õ�ֵ 1/0->1

	TH0 = T0RH;
	TL0 = T0RL;
	TF0 = 0; // ���TF0��־
	TR0 = 1; // ����(��)��ʱ��
}
void ConfigTime1(unsigned int ms)
{
	unsigned long temp;
	temp = 11059200 / 12; // ����Ƶ�� 65536
	temp = (temp * ms) / 1000;
	temp = 65536 - temp;			   // ��ֵ
	T1RH = (unsigned char)(temp >> 8); // �����߰�λ
	T1RL = (unsigned char)(temp);	   // �����Ͱ�λ
	TMOD &= 0X0F;					   // �����������ǰ����0��0000 1111��1/0->0
	TMOD |= 0X10;					   // �������⹦�ܼĴ���TMOD\T1�����úù���ģʽ

	TH1 = T1RH;
	TL1 = T1RL;
	TF1 = 0; // ���TF1��־
	TR1 = 1; // ����(��)��ʱ��
}
int main()
{
	unsigned int cnt0, cnt1;

	ConfigTime0(2); // �趨2msʱ��
	ConfigTime1(4); // �趨4msʱ��
	while (1)
	{ // ��ѯģʽ
		// �ж�T0���Ƿ���ʱ�䣨�����
		if (TF0 == 1)
		{
			TF0 = 0; // ��ʱ����
			TH0 = T0RH;
			TL0 = T0RL; // ���¼�ʱ
			cnt0++;
			if (cnt0 >= 500)
			{
				cnt0 = 0;
				LED = !LED;
			}
		}
		// �ж�T1���Ƿ���ʱ�䣨�����
		if (TF1 == 1)
		{
			TF1 = 0; // ��ʱ����
			TH1 = T1RH;
			TL1 = T1RL; // ���¼�ʱ
			cnt1++;
			if (cnt1 >= 500)
			{
				cnt1 = 0;
				LED2 = !LED2;
			}
		}
	}
}