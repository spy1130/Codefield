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
unsigned char h=0,m=0,s=0;//׼����ʾ��������ֵ	
unsigned char ms=0;//0.05ms �ļ�����λ

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
	NumShow(seg_buf, pos);//��ʾˢ��
	if( ++pos == 8 ) 	pos = 0;
	
}
//-----------------------------------------------

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{	ms++;
    if(ms==20) {s+=1;ms=0;}                   //50ms
	if(s==60) {m+=1;s=0;}
	if(m==60) {h+=1;m=0;}
	if(h==24) h=0;
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
		switch(Key_Down)
		{
			case 4:TR0=~TR0;break;
			case 5:  h=0;m=0;s=0;break;
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
	sprintf(seg_string,"%02d-%02d-%02d",(unsigned int)h,(unsigned int)m,(unsigned int)s);//��Ҫ��ʾ����ֵ��ӡ���ַ�������
	//��Զ����
	NumTran(seg_string, seg_buf);
}