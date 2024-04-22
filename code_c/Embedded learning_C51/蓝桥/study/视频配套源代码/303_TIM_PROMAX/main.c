
# include "bsp_init.h"
# include "bsp_led.h"


//-----------------------------------------------
/* ��������ռ� */
bit timer1_interrupt_into_flag = 0;//������Ϊ�������������붨ʱ���жϱ�Ϊ1
bit timer0_interrupt_into_flag = 0;//������Ϊ�������������붨ʱ���жϱ�Ϊ1

unsigned int one_ms_trigger_count = 0;//ÿһ���봥��һ��,0-65535
unsigned int ten_ms_trigger_count = 0;//ÿһ���봥��һ��,0-65535
unsigned char ucLed = 0;//Ҫ��ʾ����



//-----------------------------------------------
/* main program */
void main()
{
	Cls_Peripheral();//�ر�ϵͳ����
	Timer1Init();//��ʱ��1��ʼ������������ʱ��1�ж�
//	Timer0Init();//��ʱ��0��ʼ������������ʱ��1�ж�
	EA = 1;		//ʹ�����ж�
	
	while(1)
	{
		
		
		
		//��ʱ��1�ص�������ÿ1ms��������һ��������ÿ1s������ʾ����
		if(timer1_interrupt_into_flag)
		{
			timer1_interrupt_into_flag = 0;//���ʶ���жϱ�־�仯����λ�жϱ�־��
			one_ms_trigger_count++;	//�������
		}
			
		if((!(one_ms_trigger_count%100))&&(one_ms_trigger_count>0))//����1s
		{
			one_ms_trigger_count = 0;
			ucLed ^= 0XF0;//���Ƶĸ�4λ����
		}
		
		
		//��ʱ��0�ص�������ÿ10ms����������ʾ����		
//		if(timer0_interrupt_into_flag)
//		{
//			timer0_interrupt_into_flag = 0;//���ʶ���жϱ�־�仯����λ�жϱ�־��
//			ten_ms_trigger_count++;	//�������
//		}		
//		
//		if((!(ten_ms_trigger_count%10))&&(ten_ms_trigger_count>0))//����1s
//		{
//			ten_ms_trigger_count = 0;
//			ucLed ^= 0X0F;//���Ƶĵ�4λ����
//		}		
		
		
		Led_Disp(ucLed);//��Ҫ��ʾ��������ʾ����

	}	

}




//-----------------------------------------------

/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{
   timer1_interrupt_into_flag = 1;//�����жϱ�־λ
}

//-----------------------------------------------

/* Timer0_interrupt routine */
//void tm0_isr() interrupt 1
//{
//   timer0_interrupt_into_flag = 1;//�����жϱ�־λ
//}
