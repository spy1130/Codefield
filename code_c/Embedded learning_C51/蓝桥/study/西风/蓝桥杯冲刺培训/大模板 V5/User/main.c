/* 
		��ģ�� V5 ����˵��
		
		1���Ż��������� ����һ���жϼ��ٱ��� ʹ�ñ�־λ�����ж� ����ĳһʱ����޷������ӳ��������
		
		2���������ݶ�ȡ���� �������ʾ����ֻ��������ʾ ��ȡ���ַ��뿪�� ���ټ������ڲ�ͬ����Դ���˷�
		
		3�������˲����ݴ����� ����ظ���ȡ������ż������������
		
		------------------------------------
		
		��������˵����
		
		Proc_Flag - �ӳ���ִ�б�־λ Ĭ�� 1 -> ���������� 2 -> �������ʾ���� 3 -> ���ڴ����� 4 -> ���ݶ�ȡ������ģ���Զ�ȡ������Ϊ����
		
		------------------------------------
		
		����������˵����		
		
		Filtering.h - �˲�������ר��ͷ�ļ� Moving_Average_Filter -> ����ƽ���˲��� Median_Filter -> ��ֵ�˲���
		
		Tpis �������˲�ֻ֧���������� �漰��С�� ���Ƚ����˲��� �ٶ����ݽ���С��������
		
*/

/* ͷ�ļ������� */
#include <STC15F2K60S2.H>//��Ƭ���Ĵ���ר��ͷ�ļ�
#include <Init.h>//��ʼ���ײ�����ר��ͷ�ļ�
#include <Led.h>//Led�ײ�����ר��ͷ�ļ�
#include <Key.h>//�����ײ�����ר��ͷ�ļ�
#include <Seg.h>//����ܵײ�����ר��ͷ�ļ�
#include <Uart.h>//���ڵײ�����ר��ͷ�ļ�
#include <stdio.h>//��׼��ײ�����ר��ͷ�ļ�
#include <Filtering.h>//�����˲�����ר��ͷ�ļ�
#include <ultrasound.h>//�������ײ�����ר��ͷ�ļ�
#include "onewire.h"


/* ���������� */
unsigned int Proc_Slow_Down;//�Ӻ������ٱ���
unsigned char Key_Val,Key_Down,Key_Old,Key_Up;//����ר�ñ���
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//�������ʾ���ݴ������
unsigned char Seg_Point[8] = {0,0,0,0,0,0,0,0};//�����С�������ݴ������
unsigned char Seg_Pos;//�����ɨ��ר�ñ���
idata unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led��ʾ���ݴ������
idata unsigned char Uart_Recv[10];//���ڽ������ݴ������� Ĭ��10���ֽ� ���������ݽϳ� �ɸ�������ֽ���
unsigned char Uart_Recv_Index;//���ڽ�������ָ��
unsigned char Proc_Flag;//�ӳ���ִ�б�־λ 
float Wave_Data;//���������ݴ������
unsigned char dat;

/* ���̴����� */
void Key_Proc()
{
	if(Proc_Flag != 1) return;
	Proc_Flag = 0;//���̼��ٳ���

	Key_Val = Key_Read();//ʵʱ��ȡ����ֵ
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//��׽�����½���
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//��׽�����Ͻ���
	Key_Old = Key_Val;//����ɨ�����

}

/* ���ݶ�ȡ���� */
void Read_Proc()
{
	unsigned int Filter_Dat[2];//�˲�ǰ���ݽ��ձ���
	switch(Proc_Flag)
	{
		case 4://��������ȡ
			Filter_Dat[0] = rd_temperature();//��ȡ����������
			Filter_Dat[1] = Ut_Wave_Data();
			Wave_Data = Moving_Average_Filter(0,Filter_Dat[0]) / 16.0;//����ƽ�����˲�����
			dat = Moving_Average_Filter(1,Filter_Dat[1]);
			Proc_Flag = 0;//�����־λ״̬
		break;
	}
}

/* ��Ϣ������ */
void Seg_Proc()
{
	if(Proc_Flag != 2) return;
	Proc_Flag = 0;//����ܼ��ٳ���

	Seg_Buf[0] = (int)Wave_Data / 10 % 10;
	Seg_Buf[1] = (int)Wave_Data % 10;
	Seg_Buf[2] = (int)(Wave_Data * 10) % 10;
	
	Seg_Buf[5] = dat / 100 % 10;
	Seg_Buf[6] = dat / 10 % 10;
	Seg_Buf[7] = dat % 10;
}

/* ������ʾ���� */
void Led_Proc()
{
	
}

/* ���ڴ����� */
void Uart_Proc()
{
	if(Proc_Flag != 3) return;
	Proc_Flag = 0;//���ڼ��ٳ���
	
}

/* ��ʱ��0�жϳ�ʼ������ */
void Timer0Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x18;		//���ö�ʱ��ʼֵ
	TH0 = 0xFC;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;    //��ʱ���ж�0��
	EA = 1;     //���жϴ�
}

/* ��ʱ��0�жϷ����� */
void Timer0Server() interrupt 1
{  
	if(++Proc_Slow_Down == 500) Proc_Slow_Down = 0;//�ӳ������ר��
	
	if(Proc_Slow_Down % 9 == 0) Proc_Flag = 1;
		
	if(Proc_Slow_Down % 49 == 0) Proc_Flag = 2;
	
	if(Proc_Slow_Down % 29 == 0) Proc_Flag = 4;
	
	
	switch(Proc_Slow_Down)
	{
		//case 50: Proc_Flag = 2; break;
		case 100: Proc_Flag = 3; break;
		//case 200: Proc_Flag = 4; break;
	}
	
	if(++Seg_Pos == 8) Seg_Pos = 0;//�������ʾר��
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
}

/* ����1�жϷ����� */
void Uart1Server() interrupt 4
{
	if(RI == 1) //���ڽ�������
	{
		Uart_Recv[Uart_Recv_Index] = SBUF;
		Uart_Recv_Index++;
		RI = 0;
	}
}

/* Main */
void main()
{
	System_Init();
	Timer0Init();
	UartInit();
	while (1)
	{
		Key_Proc();
		Read_Proc();
		Seg_Proc();
		Led_Proc();
		Uart_Proc();
	}
}