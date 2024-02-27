#include "tim.h"
#include "key.h"
#include "seg.h"
#include "stdio.h"
#include "onewire.h"

unsigned int  uiSeg_Dly, uiTemp;
unsigned char ucState=1, ucSec, ucLed;
unsigned char ucKey_Dly, ucKey_Old;
unsigned char pucSeg_Buf[9], pucSeg_Code[8], ucSeg_Pos;
unsigned long ulms;

void Key_Proc(void);
void Seg_Proc(void);

void main(void)
{ 
  Cls_Peripheral();
  Timer1Init();

  while(1)
  {
    Key_Proc();
    Seg_Proc();
  }
}

void Time_1(void) interrupt 3
{
  ulms++;
  if(++ucKey_Dly == 10)
    ucKey_Dly = 0;
  if(++uiSeg_Dly == 500)
    uiSeg_Dly = 0;
  if(!(ulms % 1000))
    ucSec++;
  Seg_Disp(pucSeg_Code, ucSeg_Pos);
  if(++ucSeg_Pos == 8) ucSeg_Pos = 0;
}

void Key_Proc(void)
{
  unsigned char ucKey_Val, ucKey_Down;

  if(ucKey_Dly) return;
  ucKey_Dly = 1;

  ucKey_Val = Key_Read();
  ucKey_Down = ucKey_Val & (ucKey_Old ^ ucKey_Val);
  ucKey_Old = ucKey_Val;

  if(ucKey_Down == 4)
    if(++ucState == 2)
      ucState = 0;

  Led_Disp(ucState+1);
}

void Seg_Proc(void)
{
  if(uiSeg_Dly) return;
  uiSeg_Dly = 1;

  switch(ucState)
  {
   	case 0:
      sprintf(pucSeg_Buf, "1 %06u", (unsigned int)ucSec);
      break;
    case 1:
      uiTemp = rd_temperature();
      sprintf(pucSeg_Buf, "2   %04.2f", uiTemp/16.0);
  }
  Seg_Tran(pucSeg_Buf, pucSeg_Code);
}