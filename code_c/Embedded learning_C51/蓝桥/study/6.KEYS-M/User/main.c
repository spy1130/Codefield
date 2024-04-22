# include "STC15F2K60S2.H"
# include "init.h"
# include "timer.h"
# include "stdio.h"
# include "Key.h"
#include "SEG.h"

//-----------------------------------------------
/* �������� */
void Key_Proc(void);//���������ײ����ݱ��
void Seg_Proc(void);//��ʾ������ʾ��Ϣ����

//-----------------------------------------------
/* ȫ�ֱ������� */

//��ʾר�ã���������Զ����
unsigned char seg_buf[8];//�����ַ���ת����Ķ��뵽����
unsigned char seg_string[18];//�����ַ���
unsigned char	pos = 0;//�ж���ʾר��

//����ר�ã���������Զ����
unsigned char Key_Value;//��ȡ��������ֵ�洢����
unsigned char Key_Down, Key_Old;//��ȡ��������ֵ�洢����	

//��������ʾ��������ר�ã�������Զ����
unsigned int Key_Slow_Down;//��������
unsigned int Seg_Slow_Down;//��������



//���ݴ����������仯
unsigned char Seg_Show[1];//׼����ʾ��������ֵ	


/* main */
void main()
{
	//���ݴ����������仯
	init_close();//�ر�����
	Timer1_Init();//��ʱ��1��ʼ��������ʹ�ܶ�ʱ��1�жϣ�1ms����һ��
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
	NumShow(seg_buf, 0);//��ʾˢ��
	if( ++pos == 8 ) 	pos = 0;
	
}

void Key_Proc(void)//���������ײ����ݱ��
{
	//��Զ����
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//���ٳ���
	//����3��������
	Key_Value = Key_Read();//��ȡ�������µı�ţ���ʱ����ֵ��
	Key_Down = Key_Value & (Key_Old ^ Key_Value);//����Ϊ����ֵ��������δ���£������ȶ��ڣ�̧������У�Ϊ0	
	Key_Old = Key_Value;
	
	//���ݴ����������仯
	if(Key_Down)//�����׽���½�������
	{	switch(Key_Down){
		case 7 :Seg_Show[0] = '0';break;
		case 11 :Seg_Show[0] = '1';break;
		case 15 :Seg_Show[0] = '2';break;
		case 19 :Seg_Show[0] = '3';break;
		case 6 :Seg_Show[0] = '4';break;
		case 10 :Seg_Show[0] = '5';break;
		case 14 :Seg_Show[0] = '6';break;
		case 18 :Seg_Show[0] = '7';break;
		case 5 :Seg_Show[0] = '8';break;
		case 9 :Seg_Show[0] = '9';break;
		case 13 :Seg_Show[0] = 'A';break;
		case 17 :Seg_Show[0] = 'b';break;
		case 4 :Seg_Show[0] = 'C';break;
		case 8 :Seg_Show[0] = 'd';break;
		case 12 :Seg_Show[0] = 'E';break;
		case 16 :Seg_Show[0] = 'F';break;
		
		default :Seg_Show[0]=' ';break;
		
				}
	}
	
}

/*seg_proc */
void Seg_Proc(void)//��ʾ������ʾ��Ϣ����
{
	//��Զ����
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//���ٳ���

	//���ݴ����������仯
	seg_string[0]=Seg_Show[0];//��Ҫ��ʾ����ֵ��ӡ���ַ�������
	//��Զ����
	NumTran(seg_string, seg_buf);
}