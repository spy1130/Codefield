#include "reg52.h"

/**
  * @brief ���ڳ�ʼ����4800bps@12.000MHz
  * @param  ��
  * @retval ��
  */
void UART_Init() //4800bps@11.0592MHz
{
	SCON=0x50;          //����ͨ�Žӿ�
	PCON &= 0x7F;		//�����ʲ�����
	TMOD &= 0x0F;       //��ն�ʱ��1ģʽλ
	TMOD |= 0x20;       //�趨��ʱ��1Ϊ8λ�Զ���װ
	TL1 = 0xFA;			//���ö�ʱ��ʼֵ
	TH1 = 0xFA;			//���ö�ʱ����ֵ
	ET1 = 0;			//��ֹ��ʱ���ж�
	TR1 = 1;			//��ʱ��1��ʼ��ʱ
	EA=1; 				//�������ж�
	ES=1; 				//���������ж�
}
/**
  * @brief ���ڷ�����
  * @param  Byte��16��������
  * @retval ��
  */
void UART_SendByte(unsigned	char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0; //���¸�λ
}