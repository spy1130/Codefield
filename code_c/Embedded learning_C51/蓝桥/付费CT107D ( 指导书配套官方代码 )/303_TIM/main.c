// 使用程序前，将J13调整为IO模式（2-3脚短接）
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
