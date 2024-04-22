#include <ultrasound.h>
#include "intrins.h"

sbit Tx = P1^0;
sbit Rx = P1^1;

void Delay12us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}

void Ut_Wave_Init() //��������ʼ������ ����8��40Mhz�ķ����ź�
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		Tx = 1;
		Delay12us();
		Tx = 0;
		Delay12us();
	}
}

unsigned char Ut_Wave_Data() //�����������ȡ����
{
	unsigned int time;//ʱ�䴢�����
	TMOD &= 0x0f;//���ö�ʱ��1��ʱģʽ
	TH1 = TL1 = 0;//��λ����ֵ �ȴ��������źŷ���
	Ut_Wave_Init();//���ͳ������ź�
	TR1 = 1;//��ʼ��ʱ
	while((Rx == 1) && (TF1 == 0));//�ȴ����ܷ����źŻ��߶�ʱ�����
	TR1 = 0;//ֹͣ��ʱ
	if(TF1 == 0) //��ʱ��û�����
	{
		time = TH1 << 8 | TL1;//��ȡ��ǰʱ��
		return (time * 0.017);//���ؾ���ֵ
	}
	else
	{
		TF1 = 0;//��������־λ
		return 0;
	}
}