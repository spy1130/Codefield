#include "tim.h"

void Cls_Peripheral(void)
{
  P0 = 0xFF;
  P2 = P2 & 0x1F | 0x80;			// P27~P25���㣬�ٶ�λY4C
  P2 &= 0x1F;						// P27~P25����
  P0 = 0;
  P2 = P2 & 0x1F | 0xA0;			// P27~P25���㣬�ٶ�λY5C
  P2 &= 0x1F;						// P27~P25����
}

void Led_Disp(unsigned char ucLed)
{ // IOģʽ��J13-2��J13-3������
  P0 = ~ucLed;
  P2 = P2 & 0x1F | 0x80;			// P27~P25���㣬�ٶ�λY4C
  P2 &= 0x1F;						// P27~P25����
//XBYTE[0x8000] = ~ucLed;			// MMģʽ��J13-2��J13-1������
}

void Timer1Init(void)				// 1����@12.000MHz
{
  AUXR &= 0xBF;						// ��ʱ��ʱ��12Tģʽ
  TMOD &= 0x0F;						// ���ö�ʱ��ģʽ
  TL1 = 0x18;						// ���ö�ʱ��ֵ
  TH1 = 0xFC;						// ���ö�ʱ��ֵ
  TF1 = 0;		 					// ���TF1��־
  TR1 = 1;			  				// ��ʱ��1��ʼ��ʱ
  ET1 = 1;		  					// ����ʱ��1�ж�
  EA = 1;		  					// ����ϵͳ�ж�
}
