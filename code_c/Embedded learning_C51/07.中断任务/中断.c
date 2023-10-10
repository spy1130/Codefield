#include "reg52.h"

sbit LED = P2^0;
sbit LED1 = P2^1;

unsigned char T0RH, T0RL;
unsigned char T1RH, T1RL;

void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}



// 1<=max<=71
void ConfigTime0(unsigned int ms)
{
	unsigned long temp;
	temp = 11059200 / 12; // ����Ƶ�� 65536
	temp = (temp * ms) / 1000;
	temp = 65536 - temp;			   // ��ֵ
	T0RH = (unsigned char)(temp >> 8); // �����߰�λ
	T0RL = (unsigned char)(temp);		   // �����Ͱ�λ
	ET0=1//1000 0010�ж�����Ĵ��� IE=0x82  ��Ч�� EA=1 ET0=1  ET0�ǿ����ڶ�ʱ�������ʱ��ͻ�����ж�
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
	T1RL = (unsigned char)(temp);		   // �����Ͱ�λ
	
	ET1=1; //��T1�ж�
	PT1=1;//�������ȼ�1
	TMOD &= 0X0F;					   // �����������ǰ����0��0000 1111��1/0->0
	TMOD |= 0X10;					   // �������⹦�ܼĴ���TMOD\T1�����úù���ģʽ
	IE=0x82 //1000 0010�ж�����Ĵ��� ��Ч�� EA=1 ET0=1  ET0�ǿ����ڶ�ʱ�������ʱ��ͻ�����ж�
	TH1 = T1RH;
	TL1 = T1RL;
	TF1 = 0; // ���TF0��־
	TR1 = 1; // ����(��)��ʱ��
}
int main()
{
	EA=1;
	ConfigTime0(10); // ��ʱʱ��10ms
	ConfigTime1(3); // ��ʱʱ��3ms
	while (1);
}

void InterruptTimer0() interrput 1//7�η�ת���ƽ1msʱ��
{	
//	static unsigned int cnt=0; ��̬����
//	TF0 = 0;  ��ʱ���㣬��Ӧ�жϺ�Ӳ����0
	unsigned char i=7;
	TH0 = T0RH;
	TL0 = T0RL; // ���¼�ʱ
	
	while(i--){
	LED = !LED;
	Delay1ms();
	}
}
void InterruptTimer1() interrput 3//3ms��תһ��
{	
	TH1 = T1RH;
	TL1 = T1RL; // ���¼�ʱ
	LED1 = !LED1;
}