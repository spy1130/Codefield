#include "uart.h"

void UartInit(void)					//4800bps@12.000MHz
{ 
  SCON = 0x50;						// 8λ����,�ɱ䲨����
  AUXR |= 0x01;						// ����1ѡ��ʱ��2Ϊ�����ʷ�����
  AUXR |= 0x04;						// ��ʱ��2ʱ��ΪFosc, ��1T
  T2L = 0x8F;						// �趨��ʱ��ֵ
  T2H = 0xFD; 						// �趨��ʱ��ֵ
  AUXR |= 0x10;						// ������ʱ��2
  ES = 1;									// �������ж�
}

void Uart_Sendstring(unsigned char *pucStr)
{
  while(*pucStr != '\0')
  {
    SBUF = *pucStr;
    while(TI == 0);
    TI = 0;
    pucStr++;
  }
}
