# include "bsp_init.h"
# include "bsp_seg.h"
# include "timer.h"
# include "bsp_key.h"
# include "stdio.h"

//-----------------------------------------------
/* �������� */
void Key_Proc(void);//���������ײ����ݱ��
void Seg_Proc(void);//��ʾ������ʾ��Ϣ����

//-----------------------------------------------
/* ȫ�ֱ������� */

//��ʾר�ã���������Զ����
unsigned char seg_buf[8];//�����ַ���ת����Ķ��뵽����
unsigned char seg_string[10];//�����ַ���
unsigned char	pos = 0;//�ж���ʾר��

//����ר�ã���������Զ����
unsigned char Key_Value;//��ȡ��������ֵ�洢����
unsigned char Key_Down, Key_Old;//��ȡ��������ֵ�洢����	

//��������ʾ��������ר�ã�������Զ����
unsigned int Key_Slow_Down;//��������
unsigned int Seg_Slow_Down;//��������



//���ݴ����������仯
unsigned char Seg_Show_Num;//׼����ʾ��������ֵ	
	

	
	
//-----------------------------------------------
/* main */
void main()
{
	//���ݴ����������仯
	Cls_Peripheral();//�ر�����
	Timer1Init();//��ʱ��1��ʼ��������ʹ�ܶ�ʱ��1�жϣ�1ms����һ��
	EA = 1;//�����ж�

	while(1)
	{
		//��Զ����
		Key_Proc();//���������ײ����ݱ��
		Seg_Proc();//��ʾ������ʾ��Ϣ����
	}


}



//-----------------------------------------------
/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{	
	
	//��Զ����
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	
	//��Զ����
	Seg_Disp(seg_buf, pos);//��ʾˢ��
	if( ++pos == 8 ) 	pos = 0;
	
}



//-----------------------------------------------
/*key_proc */

void Key_Proc(void)//���������ײ����ݱ��
{
	//��Զ����
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//���ٳ���
	
	Key_Value = Key_Read_BTN();//��ȡ�������µı��
	Key_Down = Key_Value & (Key_Old ^ Key_Value);// (0000^0101) = 0101     0101 & 0101 =0101  ,��������������½��صı仯���������ͱ��ΰ�����ֵ��ͬ
																					// (0101^0101) = 0000       0101&0000 = 0000  �������һֱ����ͬ����״̬��������Ϊ0
	Key_Old = Key_Value;
	
	
	
	
	
	//���ݴ����������仯
	if(Key_Down)//�����׽���½�������
	{
		Seg_Show_Num = Key_Down;
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
	sprintf(seg_string,"      %02d",(unsigned int)Seg_Show_Num);//��Ҫ��ʾ����ֵ��ӡ���ַ�������
	
	//��Զ����
	Seg_Tran(seg_string, seg_buf);
}



