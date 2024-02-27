// 运行程序时，将J13调整为MM模式（1-2脚短接）
#include "tim.h"
#include "seg.h"
#include "stdio.h"

unsigned char ucSec, ucLed;
unsigned char pucSeg_Buf[9], pucSeg_Code[8], ucSeg_Pos;
unsigned long ulms;
// 注意：sprintf()会在字符串后面添加”\0”，所以pucSeg_Buf[]的长度应为9。
// 如果字符串中包含小数点，pucSeg_Buf[]的长度应为10。
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
