// ���г���ʱ����J13����ΪMMģʽ��1-2�Ŷ̽ӣ�
#include "tim.h"
#include "seg.h"
#include "stdio.h"

unsigned char ucSec, ucLed;
unsigned char pucSeg_Buf[9], pucSeg_Code[8], ucSeg_Pos;
unsigned long ulms;
// ע�⣺sprintf()�����ַ���������ӡ�\0��������pucSeg_Buf[]�ĳ���ӦΪ9��
// ����ַ����а���С���㣬pucSeg_Buf[]�ĳ���ӦΪ10��
void main(void)
{
  Cls_Peripheral();
  Timer1Init();

  while(1)
  {
    sprintf(pucSeg_Buf, "    %04u", (unsigned int)ucSec);
    Seg_Tran(pucSeg_Buf, pucSeg_Code);
  }
}

void Time_1(void) interrupt 3
{
  ulms++;
  if(!(ulms % 1000))
  {
    ucSec++;

    ucLed ^= 1;
    Led_Disp(ucLed);
  }
  Seg_Disp(pucSeg_Code, ucSeg_Pos);	
  if(++ucSeg_Pos == 8) ucSeg_Pos = 0;
}
