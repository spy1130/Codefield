# include "STC15F2K60S2.H"
# include "init.h"
# include "timer.h"
# include "LED.h"
# include "stdio.h"
# include "Key.h"

/* �������� */
void Key_Proc(void);//���������ײ����ݱ��

//����ר�ã���������Զ����
unsigned char Key_Value;//��ȡ��������ֵ�洢����
unsigned char Key_Down, Key_Old;//��ȡ��������ֵ�洢����	

//��������ʾ��������ר�ã�������Զ����
unsigned int Key_Slow_Down;//��������
unsigned int Seg_Slow_Down;//��������

//unsigned char LED_Show;
unsigned char stat_k = 0;//״̬��
void main()
{
	init_close();
	Timer1_Init();//��ʱ��1��ʼ��������ʹ�ܶ�ʱ��1�жϣ�1ms����һ��
	EA = 1;//�����ж�
	
	while(1){
		Key_Proc();
	}
}
/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{	
	
	//��Զ����
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//ÿ��10ms
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	
	
}
void Key_Proc(void)//���������ײ����ݱ��
{
	//��Զ����
	if(Key_Slow_Down) return;//ÿ��10ms Seg_Slow_Down = 0
	Key_Slow_Down = 1;//���ٳ���
	
	Key_Value = Key_BIN();//��ȡ�������µı��

//	Key_Down = Key_Value & (Key_Old ^ Key_Value);// (0000^0101) = 0101     0101 & 0101 =0101  ,��������������½��صı仯���������ͱ��ΰ�����ֵ��ͬ																			// (0101^0101) = 0000       0101&0000 = 0000  �������һֱ����ͬ����״̬��������Ϊ0
//	Key_Old = Key_Value;
	
	//���ݴ����������仯
	switch(Key_Value)
	{	
		case 7:
			if(stat_k==0){
			LED(0x01);
			stat_k=1;}
			else if(stat_k==1){
			LED(0x00);
			stat_k=0;
			}
			while(P30 ==0);
			break;
		case 6:
			if(stat_k == 0)
			{
				LED(0x02);
				stat_k = 2;
			}
			else if(stat_k == 2)
			{
				LED(0x00);
				stat_k = 0;
			}
			while(P31 ==0);
			break;
		case 5:
			if(stat_k == 1)
			{	
				LED(0x04|0x01);
				while(P32 ==0);
				LED(0x01);
			}
			else if(stat_k == 2)
			{	LED(0x10|0x02);
				while(P32 ==0);
				LED(0x02);
			}
			break;
		case 4:
			if(stat_k == 1)
			{	
				LED(0x08|0x01);
				while(P33 ==0);
				LED(0x01);
			}
			else if(stat_k == 2)
			{
				LED(0x20|0x02);
				while(P32 ==0);
				LED(0x02);
			}
			break;
		case 0:break;
			
	}
}