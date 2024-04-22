# include "STC15F2K60S2.H"
# include "init.h"
# include "timer.h"
# include "Key.h"
#include "LED.h"

//-----------------------------------------------
/* �������� */
void Key_Proc(void);//���������ײ����ݱ��


//-----------------------------------------------
/* ȫ�ֱ������� */


//����ר�ã���������Զ����
unsigned char Key_Value;//��ȡ��������ֵ�洢����
unsigned char Key_Down, Key_Old;//��ȡ��������ֵ�洢����	

//��������ʾ��������ר�ã�������Զ����
unsigned int Key_Slow_Down;//��������


//���ݴ����������仯
unsigned char count = 0;
unsigned char pwm_duty = 0;
unsigned char stat = 0;//״̬

/* main */
void main()
{
	//���ݴ����������仯
	init_close();//�ر�����
	Timer1_Init();//��ʱ��1��ʼ��������ʹ�ܶ�ʱ��1�жϣ�1ms����һ��
	Timer0_Init();
	EA = 1;//�����ж�

	while(1)
	{
		//��Զ����
		Key_Proc();//���������ײ����ݱ��
		
	}


}

//-----------------------------------------------
/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{	
	
	//��Զ����
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;

	
}
//-----------------------------------------------

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1 //1/100=0.01s=10ms=1000*10us=100*100us,�պ÷ֳ�100�ݣ�������100us
{	                   
	count++;
	if(count == pwm_duty)
	{
		LED(0x00);
	}
	else if(count == 100)
	{
		LED(0x01);
		count = 0;
		
	}		
	
}

//-----------------------------------------------
void Key_Proc(void)//���������ײ����ݱ��
{
	//��Զ����
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//���ٳ���
	//����3��������
	Key_Value = Key_BIN();//��ȡ�������µı�ţ���ʱ����ֵ��
	Key_Down = Key_Value & (Key_Old ^ Key_Value);//����Ϊ����ֵ��������δ���£������ȶ��ڣ�̧������У�Ϊ0	
	Key_Old = Key_Value;
	
	//���ݴ����������仯
	if(Key_Down)//�����׽���½�������
	{	
		if(Key_Down==7)
		{
			switch(stat)
			{
				case 0:
					LED(0x00);	
					TR0 = 0;
					stat = 1;
					
				break;
				
				case 1:
					LED(0x01);
					TR0=1;
					pwm_duty = 10;
					stat = 2;
					
				break;
				
				case 2:
					pwm_duty = 50;
					stat = 3;
					
				break;
				
				case 3:
					pwm_duty = 90;
					stat = 0;
				break;		
			}
		}
	}
}

