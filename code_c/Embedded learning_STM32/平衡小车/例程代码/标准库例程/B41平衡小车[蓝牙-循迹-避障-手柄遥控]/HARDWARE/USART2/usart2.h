#ifndef __USRAT2_H
#define __USRAT2_H 
#include "sys.h"	  	
/**************************************************************************
 ��  �� ���������
 �Ա���ַ��https://shop119207236.taobao.com
 ΢�Ź��ںš���������ˡ�
 ��̨�ظ���ƽ��С��������ȡƽ��С��ȫ��DIY����
 ��̨�ظ������ӿ������ߡ�����ȡ���ӹ���ʦ�ر���������
 ��̨�ظ�������������ϡ�����ȡ����������ϰ�
 ֪���������� 
**************************************************************************/
void uart2_init(u32 bound);
void USART2_IRQHandler(void);
void Uart2SendByte(char byte);   //���ڷ���һ���ֽ�
void Uart2SendBuf(char *buf, u16 len);
void Uart2SendStr(char *str);
void BluetoothCMD(int Uart_Receive);
#endif

