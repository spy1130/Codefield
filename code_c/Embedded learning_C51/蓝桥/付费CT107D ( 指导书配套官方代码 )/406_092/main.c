#include "tim.h"
#include "key.h"
#include "seg.h"
#include "stdio.h"
#include "onewire.h"
#include "iic.h"

unsigned int  uiFre_Val, uiTem_Val, uiSeg_Dly;
unsigned int  uiFre_Echo, uiTem_Echo;
unsigned char ucVol_Val, ucVol_Echo, ucVol_Thr, ucVol_Flag;
unsigned char ucState, ucLed=1;
unsigned char pucSeg_Buf[10], pucSeg_Code[8], ucSeg_Pos;
unsigned char ucKey_Dly, ucKey_Old;
unsigned long ulms, ulKey_Time;

void Timer0Init(void)			// 16位计数器模式
{
  AUXR &= 0x7F;						// 定时器时钟12T模式
  TMOD |= 5;						// 设置16位计数器模式
  TL0 = 0;							// 设置计数初值
  TH0 = 0;							// 设置计数初值
  TF0 = 0;							// 清除TF0标志
  TR0 = 1;							// 开始计数
}

void Key_Proc(void);
void Seg_Proc(void);

void main(void)
{
  Cls_Peripheral();
  Timer1Init();
  Timer0Init();

  EEPROM_Read(pucSeg_Buf, 0, 6);
  ucVol_Val = pucSeg_Buf[0];
  uiFre_Val = (pucSeg_Buf[2]<<8)+pucSeg_Buf[1];
  uiTem_Val = (pucSeg_Buf[4]<<8)+pucSeg_Buf[3];
  ucVol_Thr = pucSeg_Buf[5];
  if(ucVol_Thr > 50) ucVol_Thr = 20;

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
  if(ucVol_Flag)
    if(!(ulms % 200))
      ucLed ^= 0x80;
  if(!(ulms % 1000))
  {
    uiFre_Val = (TH0<<8)+TL0;
    TH0 = 0;
    TL0 = 0;
  }
  Led_Disp(ucLed);
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
  if(ucKey_Down) ulKey_Time = ulms;

  switch(ucKey_Down)
  {
    case 4:							// S4
      if(ucState < 3)
      {
        if(++ucState == 3) ucState = 0;
        if(!ucState) ucLed = 1;
        if(ucState == 1) ucLed = 2;
        if(ucState == 2) ucLed = 4;
      }
      else if(ucState < 0x13)
        if(++ucState == 0x13) ucState = 0x10;
      break;
    case 5:							// S5
      pucSeg_Buf[0] = ucVol_Val;
      pucSeg_Buf[1] = uiFre_Val & 0xff;
      pucSeg_Buf[2] = uiFre_Val >> 8;
      pucSeg_Buf[3] = uiTem_Val & 0xff;
      pucSeg_Buf[4] = uiTem_Val >> 8;
      EEPROM_Write(pucSeg_Buf, 0, 5);
      break;
    case 8:							// S8
      if(ucState < 0x10)
      {
        ucState = 0x10;
        ucLed = 0;
        EEPROM_Read(pucSeg_Buf, 0, 5);
        ucVol_Echo = pucSeg_Buf[0];
        uiFre_Echo = (pucSeg_Buf[2]<<8)+pucSeg_Buf[1];
        uiTem_Echo = (pucSeg_Buf[4]<<8)+pucSeg_Buf[3];
      }
      else if(ucState < 0x20)
      {
        ucState = 0;
        ucLed = 1;
      }
      if(ucState == 0x20)
        if(++ucVol_Thr > 50) ucVol_Thr = 1;
      break;
    case 9:							// S9
      if(ucState != 0x20) ucState = 0x20;
      else
      {
        ucState = 0;
        pucSeg_Buf[0] = ucVol_Thr;
        EEPROM_Write(pucSeg_Buf, 5, 1);
      }
  }

  if((ucState==0x20) && (ucKey_Old==8))
    if(ulms-ulKey_Time > 800)
		{
      ulKey_Time = ulms;
      if(++ucVol_Thr > 50) ucVol_Thr = 1;
    }
}

void Seg_Proc(void)
{
  if(uiSeg_Dly) return;
  uiSeg_Dly = 1;

  switch(ucState&0xf0)
  {
    case 0:							// 测量
      switch(ucState)
      {
        case 0:						// 电压
          ucVol_Val = PCF8591_Adc()*50/255;
          if(ucVol_Val > ucVol_Thr)
            ucVol_Flag = 1;
          else
          {
            ucVol_Flag = 0;
            ucLed &= 0x7f;
          }
          sprintf(pucSeg_Buf, "U     %02.1f", (float)ucVol_Val/10.0);
          break;
        case 1:						// 频率
          sprintf(pucSeg_Buf, "F  %5u", (unsigned int)uiFre_Val);
          break;
        case 2:						// 温度
          uiTem_Val = rd_temperature();
          sprintf(pucSeg_Buf, "C   %04.2f", (float)uiTem_Val/16.0);
      }break;
    case 0x10:						// 回显
      switch(ucState)
      {
        case 0x10:					// 电压
          sprintf(pucSeg_Buf, "HU    %02.1f", (float)ucVol_Echo/10.0);
          break;
        case 0x11:					// 频率
          sprintf(pucSeg_Buf, "HF %5u", uiFre_Echo);
          break;
        case 0x12:					// 温度 
          sprintf(pucSeg_Buf, "HC  %04.2f", (float)uiTem_Echo/16.0);
      }break;
    case 0x20:
      sprintf(pucSeg_Buf, "P     %02.1f", (float)ucVol_Thr/10.0);
  }
  Seg_Tran(pucSeg_Buf, pucSeg_Code);
}
