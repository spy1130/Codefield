// ʹ�ó���ǰ����J13����ΪIOģʽ��2-3�Ŷ̽ӣ�
#include "tim.h"

unsigned char ucSec;
unsigned long ulms;

void main(void)
{
  Cls_Peripheral();
  Timer1Init();	
  while(1);
}

void Time_1(void) interrupt 3
{
  ulms++;
  if(!(ulms % 1000))
  {
    ucSec++;
    Led_Disp(ucSec);
  }
}
