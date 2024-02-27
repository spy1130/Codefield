#include "tim.h"
#include "key.h"
#include "seg.h"
#include "stdio.h"
#include "ds1302.h"
#include "onewire.h"

unsigned int  uiSeg_Dly;
unsigned char pucRtc[3] = {23,59,50};
unsigned char ucTemp_Val, ucTemp_Flag;
unsigned char pucAlarm[3] = {0,0,0}, ucAlarm_Num;
unsigned char ucState, ucSec, ucLed;
unsigned char pucSeg_Buf[9], pucSeg_Code[8], ucSeg_Pos;
unsigned char ucKey_Dly, ucKey_Old;
unsigned long ulms; 

void Key_Proc(void);
void Seg_Proc(void);

void main(void)
{ 
  Cls_Peripheral();
  Timer1Init();	
  Set_RTC(pucRtc);

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
  if(ucAlarm_Num)
  {
    if(!(ulms % 200))
    {
      if(--ucAlarm_Num)
        ucLed ^= 1;
      else
        ucLed = 0;
      Led_Disp(ucLed);
    }
  }
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

  switch(ucKey_Down)
  {
    case 5:							// S5
      if(!ucState) ucState += 0x10;
      else if(ucState < 0x20)
        if(++ucState == 0x13)
        {
          ucState = 0;
          Set_RTC(pucRtc);
        }break;
    case 4:							// S4
      if(!ucState) ucState += 0x20;
      else if(ucState >= 0x20)
        if(++ucState == 0x23) ucState = 0;
      break; 
    case 9:							// S9
      if(!ucState) break;
      switch(ucState)
      {
        case 0x10:
          if(++pucRtc[0] == 24) pucRtc[0] = 0;
          break;
        case 0x11:
          if(++pucRtc[1] == 60) pucRtc[1] = 0;
          break;
        case 0x12:
          if(++pucRtc[2] == 60) pucRtc[2] = 0;
          break;
        case 0x20:
          if(++pucAlarm[0] == 24) pucAlarm[0] = 0;
          break;
        case 0x21:
          if(++pucAlarm[1] == 60) pucAlarm[1] = 0;
          break;
        case 0x22:
          if(++pucAlarm[2] == 60) pucAlarm[2] = 0;
      }break;
    case 8:							// S8
      if(!ucState) break;
      switch(ucState)
      {
        case 0x10:
          if(!pucRtc[0]) pucRtc[0] = 23;
          else pucRtc[0]--;
          break;
        case 0x11:
          if(!pucRtc[1]) pucRtc[1] = 59;
          else pucRtc[1]--;
          break;
        case 0x12:
          if(!pucRtc[2]) pucRtc[2] = 59;
          else pucRtc[2]--;
          break;
        case 0x20:
          if(!pucAlarm[0]) pucAlarm[0] = 23;
          else pucAlarm[0]--;
          break;
        case 0x21:
          if(!pucAlarm[1]) pucAlarm[1] = 59;
          else pucAlarm[1]--;
          break;
        case 0x22:
          if(!pucAlarm[2]) pucAlarm[2] = 59;
          else pucAlarm[2]--;
      }
  }
  if(!ucState && (ucKey_Old == 8))
    ucTemp_Flag = 1;
  if(!ucState && !ucKey_Old)
    ucTemp_Flag = 0;

  if(ucKey_Down && ucAlarm_Num)
	{
    ucAlarm_Num = 0;
    Led_Disp(0);
	}
}

void Seg_Proc(void)
{
  if(uiSeg_Dly) return;
  uiSeg_Dly = 1;

  if(!ucTemp_Flag)
  {
    if(!ucState)
    {
      Read_RTC(pucRtc);
      if((pucRtc[0]==pucAlarm[0]) && (pucRtc[1]==pucAlarm[1])\
        && (pucRtc[2]==pucAlarm[2]))
        ucAlarm_Num = 25;
    }
    if(ucState < 0x20)
      sprintf(pucSeg_Buf, "%02d-%02d-%02d", (unsigned int)pucRtc[0],\
        (unsigned int)pucRtc[1], (unsigned int)pucRtc[2]);
    else
      sprintf(pucSeg_Buf, "%02d-%02d-%02d", (unsigned int)pucAlarm[0],\
        (unsigned int)pucAlarm[1], (unsigned int)pucAlarm[2]);
    if(ucSec&1)
      switch(ucState)
      {
        case 0x10:
        case 0x20:
          pucSeg_Buf[0] = 0xff;
          pucSeg_Buf[1] = 0xff;
          break;
        case 0x11:
        case 0x21:
          pucSeg_Buf[3] = 0xff;
          pucSeg_Buf[4] = 0xff;
          break;
        case 0x12:
        case 0x22:
          pucSeg_Buf[6] = 0xff;
          pucSeg_Buf[7] = 0xff;
      }
  }
  else
  {
    ucTemp_Val = rd_temperature()>>4;
    sprintf(pucSeg_Buf, "     %02dC", (unsigned int)ucTemp_Val);
  }
  Seg_Tran(pucSeg_Buf, pucSeg_Code);
}
