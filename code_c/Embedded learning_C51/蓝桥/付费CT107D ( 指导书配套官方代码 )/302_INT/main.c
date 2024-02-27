// ʹ�ó���ǰ��������J5����ΪBTNģʽ��2��3���Ŷ̽�ʹ��һ�а���һ�˽ӵأ�
// ������J13����ΪIOģʽ��2��3���Ŷ̽�ʹU25����������ź�WR�ӵأ�
// ����P32-INT0���Ŷ�Ӧ�İ���S5��L1״̬��ת
// ����P33-INT1���Ŷ�Ӧ�İ���S4��L2״̬��ת
#include "reg52.h"

unsigned char ucLed;
// �ر�����
void Cls_Peripheral(void)
{
  P0 = 0xFF;
  P2 = P2 & 0x1F | 0x80;			// P27~P25���㣬�ٶ�λY4C
  P2 &= 0x1F;						// P27~P25����
  P0 = 0;
  P2 = P2 & 0x1F | 0xA0;			// P27~P25���㣬�ٶ�λY5C
  P2 &= 0x1F;						// P27~P25����
}
// ϵͳ��ʼ��
void Sys_Init(void)
{
  EX0 = 1;
  IT0 = 1;  							// ���ش�����ʽ(�½���)
  EX1 = 1;
  IT1 = 1;  							// ���ش�����ʽ(�½���)
  EA = 1;  							// �������ж�
}
// ������
void main(void)
{
  Cls_Peripheral();
  Sys_Init();

  while(1);
}
//�ⲿ�ж�0�жϷ�����
void isr_intr_0(void) interrupt 0
{
  ucLed ^= 1;
  P0 = ~ucLed;
  P2 = P2 & 0x1F | 0x80; 			// P27~P25���㣬�ٶ�λY4C
  P2 &= 0x1F; 						// P27~P25����
}
//�ⲿ�ж�1�жϷ�����
void isr_intr_1(void) interrupt 2
{
  ucLed ^= 2;
  P0 = ~ucLed;
  P2 = P2 & 0x1F | 0x80; 			// P27-P25���㣬�ٶ�λY4C
  P2 &= 0x1F; 						// P27-P25����
}
