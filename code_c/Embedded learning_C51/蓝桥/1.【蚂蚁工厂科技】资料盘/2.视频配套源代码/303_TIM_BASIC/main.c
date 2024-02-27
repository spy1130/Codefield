# include "timer.h"
# include "bsp_init.h"
# include "bsp_led.h"


//-----------------------------------------------
/* ��������ռ� */
bit timer1_interrupt_into_flag = 0;//������Ϊ�������������붨ʱ���жϱ�Ϊ1

unsigned int one_ms_trigger_count = 0;//ÿһ���봥��һ��,0-65535
unsigned char ucLed = 0;//ÿһ���ӣ������ݻ��Զ���1



//-----------------------------------------------
/* main program */
void main()
{
	Cls_Peripheral();//�ر�ϵͳ����
	Timer1Init();//��ʱ��1��ʼ������������ʱ��1�ж�
	EA = 1;		//ʹ�����ж�
	while(1)
	{
		
		
		if(timer1_interrupt_into_flag)
		{
			timer1_interrupt_into_flag = 0;//���ʶ���жϱ�־�仯����λ�жϱ�־��
			one_ms_trigger_count++;	//�������
		}
			
		if((!(one_ms_trigger_count%1000))&&(one_ms_trigger_count>0))//����1s
		{
			one_ms_trigger_count = 0;
			ucLed ++;//����ʾ���������Զ�+1
		}
		
		Led_Disp(ucLed);//��Ҫ��ʾ��������ʾ����

	}	

}




//-----------------------------------------------

/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{
   timer1_interrupt_into_flag = 1;//�����жϱ�־λ
}


