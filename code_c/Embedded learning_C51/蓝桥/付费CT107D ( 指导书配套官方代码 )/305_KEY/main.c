#include "tim.h"
#include "key.h"
#include "seg.h"

unsigned char ucSec, ucLed;
unsigned char ucKey_Dly, ucKey_Old;
unsigned char pucSeg_Buf[8], pucSeg_Code[8], ucSeg_Pos;
unsigned long ulms, ulKey_Time;

void Key_Proc(void);
void SEG_Proc(unsigned char ucSeg_Val);

void main(void)
{
  Cls_Peripheral();
  Timer1Init();

  while(1)
  {
    Key_Proc();
    Seg_Tran(pucSeg_Buf, pucSeg_Code);
  }
}

void Time_1(void) interrupt 3
{
  ulms++;
  if(++ucKey_Dly == 10)
    ucKey_Dly = 0;
  if(!(ulms % 1000))
  {
    ucSec++;
    ucLed ^= 1;
    Led_Disp(ucLed);
  }
  Seg_Disp(pucSeg_Code, ucSeg_Pos);	
  if(++ucSeg_Pos == 8) ucSeg_Pos = 0;
}

void Key_Proc(void)
{
  unsigned char ucKey_Val, ucKey_Down, ucKey_Up;

  if(ucKey_Dly) return;
  ucKey_Dly = 1;

  ucKey_Val = Key_Read();
  ucKey_Down = ucKey_Val & (ucKey_Old ^ ucKey_Val);
  ucKey_Up = ~ucKey_Val & (ucKey_Old ^ ucKey_Val);
  ucKey_Old = ucKey_Val;

  if(ucKey_Down)
  {
    ulKey_Time = ulms;
    SEG_Proc(ucKey_Down);
  }
  if(ucKey_Up)
    SEG_Proc(ucKey_Up);
  if(ucKey_Old && (ulms - ulKey_Time > 1000))
  {
    ulKey_Time = ulms;
    SEG_Proc(ucKey_Old);
  }
}

void SEG_Proc(unsigned char ucSeg_Val)
{
  unsigned char i;

  for(i=0; i<7; i++)
    pucSeg_Buf[i] = pucSeg_Buf[i+1];	
  if(ucSeg_Val < 14)
    pucSeg_Buf[i] = ucSeg_Val-4+'0';
  else
    pucSeg_Buf[i] = ucSeg_Val-14+'A';
}
