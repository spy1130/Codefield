#include <ultrasound.h>
#include <intrins.h>

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


void Ul_Init()
{
	unsigned char i;
	EA = 0;
	for(i=0;i<8;i++)
	{
		Tx = 1;
		Delay12us();
		Tx = 0;
		Delay12us();
	}
	EA = 1;
}

unsigned char Wave_Read()
{
	unsigned int time;
	TMOD &= 0x0F;
	TH1 = TL1 = 0;//��λ����ֵ �ȴ��������źŷ���
	Ul_Init();
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