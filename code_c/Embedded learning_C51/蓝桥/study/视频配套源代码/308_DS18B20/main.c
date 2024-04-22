# include "bsp_init.h"
# include "bsp_seg.h"
# include "timer.h"
# include "bsp_key.h"
# include "stdio.h"
#include "bsp_ds1302.h"
# include "bsp_led.h"
#include "bsp_onewire.h"

//-----------------------------------------------
/* �������� */
//��������ѭ������������Զ����
void Key_Proc(void);//���������ײ����ݱ��
void Seg_Proc(void);//��ʾ������ʾ��Ϣ����
void Led_Proc(void);//LED����LED״̬��Ϣ��ʾ


//-----------------------------------------------
/* ȫ�ֱ������� */

//�������ʾר�ã���������Զ����
unsigned char seg_buf[8];//�����ַ���ת����Ķ��뵽����
unsigned char seg_string[10];//�����ַ���
unsigned char	pos = 0;//�ж���ʾר��

//LED��ʾר�ã���������Զ����
unsigned char ucLed;//��¼LED��״̬��Ϣ

//����ר�ã���������Զ����
unsigned char Key_Value;//��ȡ��������ֵ�洢����
unsigned char Key_Down, Key_Old;//��ȡ��������ֵ�洢����	

//��������ʾ��������ר�ã�������Զ����
unsigned int Key_Slow_Down;//��������
unsigned int Seg_Slow_Down;//��������

//ds1302ר�ã���ʹ��DS1302ʱ��������Զ����
unsigned char ucRtc[3] = {23,59,55};//�����ʼֵ����߷ŵ���ʱ�������ֵ


//���ݴ����������仯
unsigned int ms_count;//��¼��������
unsigned char s_count;//��¼������
	
unsigned char Running_State;//��¼����״̬

	
//-----------------------------------------------
/* main */
void main()
{
	//���ݴ����������仯
	Cls_Peripheral();//�ر�����
	Timer1Init();//��ʱ��1��ʼ��������ʹ�ܶ�ʱ��1�жϣ�1ms����һ��
	EA = 1;//�����ж�

	Set_Rtc(ucRtc);//����RTCʱ�䣬23-59-55

	
	while(1)
	{
		//��Զ����
		Key_Proc();//���������ײ����ݱ��
		Seg_Proc();//��ʾ������ʾ��Ϣ����
    Led_Proc();//LED����LED״̬��Ϣ��ʾ		
	}


}



//-----------------------------------------------
/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{	
	
	//��Զ����
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	
	if(++ms_count == 1000) //��¼����ʱ��
	{
		s_count++;
		ms_count = 0;
	}
	

	//��Զ����
	Seg_Disp(seg_buf, pos);//�������ʾˢ��
	if( ++pos == 8 ) 	pos = 0;
	
	Led_Disp(ucLed);//LED��ʾ
	
	
}



//-----------------------------------------------
/*key_proc */

void Key_Proc(void)//���������ײ����ݱ��
{
	//��Զ����
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//���ٳ���
	
	Key_Value = Key_Read();//��ȡ�������µı��
	Key_Down = Key_Value & (Key_Old ^ Key_Value);// (0000^0101) = 0101     0101 & 0101 =0101  ,��������������½��صı仯���������ͱ��ΰ�����ֵ��ͬ
																					// (0101^0101) = 0000       0101&0000 = 0000  �������һֱ����ͬ����״̬��������Ϊ0
	Key_Old = Key_Value;
	
	
	
	
	
	//���ݴ����������仯
	if(Key_Down)//�����׽���½�������
	{
			if(++Running_State == 3)  Running_State = 0;//��֤Running_State��0-2֮�䷭��
	}
	
}



//----------------------------------------------
/*seg_proc */
void Seg_Proc(void)//��ʾ������ʾ��Ϣ����
{
	//��Զ����
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//���ٳ���

	
	

	
	//���ݴ����������仯
	switch(Running_State)
	{
		case 0:
				sprintf(seg_string,"----%04.2f",rd_temperature()/16.0);//��ȡ18b20���¶���ֵ,��Ҫ��ʾ����ֵ��ӡ���ַ�������
				break;		
		
		case 1:
				Read_Rtc(ucRtc);//��ȡ1302�ڲ�ʱ��������ݣ��ŵ�Ԥ���������ռ���	
				sprintf(seg_string,"%02d-%02d-%02d",(unsigned int)ucRtc[0],(unsigned int)ucRtc[1],(unsigned int)ucRtc[2]);//��Ҫ��ʾ����ֵ��ӡ���ַ�������
				break;
	
		case 2:
				sprintf(seg_string,"-----%03d",(unsigned int)s_count);//��Ҫ��ʾ����ֵ��ӡ���ַ�������	
				break;		
	}	
	

	
	//��Զ����
	Seg_Tran(seg_string, seg_buf);
}

//----------------------------------------------
/*led_proc */
void Led_Proc(void)//��ʾ������ʾ��Ϣ����
{
	
	//���ݴ����������仯
	switch(Running_State)
	{
		case 0:
				ucLed = 0x03;//��L1 L2������
				break;
	
		case 1:
				ucLed = 0x0C;//��L3 L4��
				break;		
		
		case 2:
				ucLed = 0x30;//��L5 L6��
				break;				
		
	}
	
	
}


