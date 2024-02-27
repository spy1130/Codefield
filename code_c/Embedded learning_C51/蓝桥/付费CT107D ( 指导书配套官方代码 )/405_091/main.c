#include "tim.h"
#include "key.h"
#include "seg.h"
#include "stdio.h"
#include "iic.h"

unsigned int  puiInter[4];
unsigned char ucMode=1, ucLedi, ucLedj;
unsigned char ucBright=1, ucBright_Flag, ucRun, ucNum;
unsigned char pucLed_Val[4][8]
  ={0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
    0x81, 0x42, 0x24, 0x18, 0x81, 0x42, 0x24, 0x18,
    0x18, 0x24, 0x42, 0x81, 0x18, 0x24, 0x42, 0x81};
unsigned char ucState, uc800ms, ucLed;
unsigned char pucSeg_Buf[9], pucSeg_Code[8], ucSeg_Pos;
unsigned char ucKey_Dly, ucKey_Old;
unsigned long ulms, ulStart;

void Int_Proc(void);
void Key_Proc(void);
void Seg_Proc(void);
void Led_Proc(void);

void main(void)
{
  Cls_Peripheral();
  Timer1Init();

  Int_Proc();

  while(1)
  {
    Key_Proc();
    Seg_Proc();
    Led_Proc();
  }
}

void Time_1(void) interrupt 3
{
  ulms++;
  if(++ucKey_Dly == 10)
    ucKey_Dly = 0;
  if(!(ulms % 800))
    uc800ms++;
  Seg_Disp(pucSeg_Code, ucSeg_Pos);
  if(++ucSeg_Pos == 8) ucSeg_Pos = 0;
  if(ucRun)
  {
    if(ucBright > ucNum)
      Led_Disp(ucLed);
    else
      Led_Disp(0);
    if(++ucNum == 5)
      ucNum = 0;
  }
  else
    Led_Disp(0);
}

void Int_Proc(void)
{
  unsigned char i;
  EEPROM_Read(pucSeg_Buf, 0, 4);
  for(i=0; i<4; i++)
    if((puiInter[0]>=4) && (puiInter[0]<=12))
      puiInter[i] = pucSeg_Buf[i]*100;
    else
      puiInter[i] = 400;
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
    case 5:							// S5
      ucRun ^= 1;
      ulStart = ulms;
      ucLed = pucLed_Val[0][0];
      ucLedi = 0;
      ucLedj = 1;
      break;
    case 4:							// S4
      if(++ucState == 3)
      {
        ucState = 0;
        pucSeg_Buf[0] = puiInter[0]/100;
        pucSeg_Buf[1] = puiInter[1]/100;
        pucSeg_Buf[2] = puiInter[2]/100;
        pucSeg_Buf[3] = puiInter[3]/100;
        EEPROM_Write(pucSeg_Buf, 0, 4);
      }break;
    case 9:
      if(ucState == 1)
        if(++ucMode == 5)
          ucMode = 1;
      if(ucState == 2)
      {
        puiInter[ucMode-1] += 100;
        if(puiInter[ucMode-1] == 1300)
          puiInter[ucMode-1] = 400;
      }break;
    case 8:
      if(ucState == 1)
        if(--ucMode == 0)
          ucMode = 4;
      if(ucState == 2)
      {
        puiInter[ucMode-1] -= 100;
        if(puiInter[ucMode-1] == 300)
          puiInter[ucMode-1] = 1200;
      }
  }
  if(!ucState && (ucKey_Old == 8))
    ucBright_Flag = 1;
  if(!ucState && !ucKey_Old)
    ucBright_Flag = 0;
}

void Seg_Proc(void)
{
  if(ucState)
  {
    sprintf(pucSeg_Buf, "-%1u- %4u", (unsigned int)ucMode,\
      (unsigned int)puiInter[ucMode-1]);
    if(uc800ms&1)
    {
      if(ucState == 1)
        pucSeg_Buf[1] = 0xff;
      else
      {
        pucSeg_Buf[4] = 0xff;
        pucSeg_Buf[5] = 0xff;
        pucSeg_Buf[6] = 0xff;
        pucSeg_Buf[7] = 0xff;
      }
    }
  }
  else
  {
    if(ucBright_Flag)
      sprintf(pucSeg_Buf, "      -%1u", (unsigned int)ucBright);
    else
      sprintf(pucSeg_Buf, "        ");
  }
  Seg_Tran(pucSeg_Buf, pucSeg_Code);
  ucBright = (PCF8591_Adc()>>6)+1;
}

void Led_Proc(void)
{
  if(ulms-ulStart >= puiInter[ucLedi])
  {
    ulStart = ulms;
    ucLed = pucLed_Val[ucLedi][ucLedj];
    if(++ucLedj == 8)
    {
      ucLedj = 0;
      if(++ucLedi == 4)
        ucLedi = 0;
    }
  }
}
