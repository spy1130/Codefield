#include "tim.h"
#include "key.h"
#include "seg.h"
#include "stdio.h"
#include "iic.h"
#include "ultrasonic.h"

unsigned int  uiDac;
unsigned char pucDist[10], ucLast, ucTotal;
unsigned char ucNum_Data, ucNum_Echo, ucBlind;
unsigned char ucState, ucLed, ucOper_Flag, ucMeas_Num;
unsigned char pucSeg_Buf[9], pucSeg_Code[8], ucSeg_Pos;
unsigned char ucKey_Dly, ucKey_Old;
unsigned long ulms;

void Key_Proc(void);
void Seg_Proc(void);

void main(void)
{
  Cls_Peripheral();
  Timer1Init();
  Timer0Init();

  EEPROM_Read(pucSeg_Buf, 0, 1);
  if(pucSeg_Buf[0] <= 90) ucBlind = pucSeg_Buf[0];
  else ucBlind = 50;
  EEPROM_Read(pucDist, 1, 10);

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
  if(ucMeas_Num)
  {
    if(!(ulms % 200))
    {
      if(--ucMeas_Num) ucLed ^= 1;
      else ucLed &= 0xfe;
      Led_Disp(ucLed);
    }
  }
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

  switch(ucKey_Down)
  {
    case 4:							// S4
      if(!ucState)
      {
        ucLast = pucDist[ucNum_Data];
        if(++ucNum_Data == 10) ucNum_Data = 0;
        pucDist[ucNum_Data] = Wave_Recv();
        ucTotal = ucLast + pucDist[ucNum_Data];

        if(pucDist[ucNum_Data] <= ucBlind)
          PCF8591_Dac(0);
        else
        {
          uiDac = (pucDist[ucNum_Data]-ucBlind)*1.02;	// 0.02*255/5
          if(uiDac > 255) uiDac = 255;
          PCF8591_Dac(uiDac);
        }

        pucSeg_Buf[0] = pucDist[ucNum_Data];
        EEPROM_Write(pucSeg_Buf, ucNum_Data+1, 1);
        ucMeas_Num = 20;
      }break;
    case 5:							// S5
      if(!ucState)
      {
        ucState = 1;
        ucLed |= 0x40;
      }
      else if(ucState == 1)
      {
        ucState = 0;
        ucLed &= 0xbf;
      }break;
    case 9:							// S9
      if(!ucState)
      {
        ucState = 2;
        ucLed |= 0x80;
      }
      else if(ucState == 2)
      {
        ucState = 0;
        ucLed &= 0x7f;
        pucSeg_Buf[0] = ucBlind;
        EEPROM_Write(pucSeg_Buf, 0, 1);
      }break;
    case 8:							// S8
      switch(ucState)
      {
        case 0:
          ucOper_Flag ^= 1; break;
        case 1:
          if(++ucNum_Echo == 10) ucNum_Echo = 0; break;
        case 2:
          ucBlind += 10;
          if(ucBlind > 90) ucBlind = 0;
      }
  }
  Led_Disp(ucLed);
}

void Seg_Proc(void)
{
  switch(ucState)
  {
    case 0:
      if(!ucOper_Flag)
        sprintf(pucSeg_Buf, "0 %03u%03u", (unsigned int)ucLast,\
          (unsigned int)pucDist[ucNum_Data]);
      else
        sprintf(pucSeg_Buf, "1 %03u%03u", (unsigned int)ucTotal,\
          (unsigned int)pucDist[ucNum_Data]);
      break;
    case 1:
      sprintf(pucSeg_Buf, "%02u   %03u", (unsigned int)ucNum_Echo,\
        (unsigned int)pucDist[ucNum_Echo]);
      break;
    case 2:
      sprintf(pucSeg_Buf, "F     %02u", (unsigned int)ucBlind);
  }
  Seg_Tran(pucSeg_Buf, pucSeg_Code);
}