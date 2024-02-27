#include "tim.h"
#include "key.h"
#include "seg.h"
#include "stdio.h"
#include "ds1302.h"
#include "iic.h"

unsigned int  uiSeg_Dly;
unsigned int  uiVol_Val, uiFre_Val;
unsigned int  uiVol_Low, uiVol_High;
unsigned char pucRtc[3] = {23, 59, 50};
unsigned char ucState, ucSec, pucEvent[4];
unsigned char pucSeg_Buf[9], pucSeg_Code[8], ucSeg_Pos;
unsigned char ucKey_Dly, ucKey_Old;
unsigned long ulms;

void Timer0Init(void)				// 16位计数器模式
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

  Set_RTC(pucRtc);
  EEPROM_Read(pucSeg_Buf, 0, 4);
  uiVol_Low = (pucSeg_Buf[1]<<8) + pucSeg_Buf[0];
  uiVol_High = (pucSeg_Buf[3]<<8) + pucSeg_Buf[2];
  if(uiVol_Low > 5000) uiVol_Low = 1000;
  if(uiVol_High > 5000) uiVol_High = 2000;

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
  {
    ucSec++;
    uiFre_Val = (TH0<<8)+TL0;
    TH0 = 0;
    TL0 = 0;
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
    case 5:							// S5：切换显示时钟、电压、频率或事件
      ucState = (ucState&0xf0)+0x10;
      if(ucState >= 0x40) ucState = 0;
      if(ucState == 0x30)
      {								// 读取事件
        EEPROM_Read(pucEvent, 4, 4);
        if(pucEvent[0] > 2) pucEvent[0] = 0;
        if(pucEvent[1] > 24) pucEvent[1] = 0;
        if(pucEvent[2] > 60) pucEvent[2] = 0;
        if(pucEvent[3] > 60) pucEvent[3] = 0;
      }break;
    case 4:							// S4：功能选择
      switch(ucState&0xf0)
      {
        case 0:						// 选择显示时钟、设置时、设置分或设置秒
          if(++ucState == 4)
          {
            ucState = 0;
            Set_RTC(pucRtc);  		// 设置时钟
          }break;
        case 0x10:					// 选择显示电压、设置电压上限或下限
          if(++ucState == 0x13)
          {
            ucState = 0x10;
            // 保存阈值
            pucSeg_Buf[0] = uiVol_Low & 0xff;
            pucSeg_Buf[1] = uiVol_Low >> 8;
            pucSeg_Buf[2] = uiVol_High & 0xff;
            pucSeg_Buf[3] = uiVol_High >> 8;
            EEPROM_Write(pucSeg_Buf, 0, 4);
          }break;
        case 0x20:					// 选择显示频率或周期
          if(++ucState == 0x22) ucState = 0x20;
          break;
        case 0x30:					// 选择显示事件类型或时间
          if(++ucState == 0x32) ucState = 0x30;
      }break;
    case 9:							// S9：参数加
      switch(ucState)
      {
        case 1:
          if(++pucRtc[0] >= 24) pucRtc[0] = 0;
          break;
        case 2:
          if(++pucRtc[1] >= 60) pucRtc[1] = 0;
          break;
        case 3:
          if(++pucRtc[2] >= 60) pucRtc[2] = 0;
          break;
        case 0x11:
          if(uiVol_High <= 4500) uiVol_High += 500;
          break;
        case 0x12:
          if(uiVol_High - uiVol_Low > 500) uiVol_Low += 500;
      }break;
    case 8:							// S8：参数减
      switch(ucState)
      {
        case 1:
          if(pucRtc[0]) pucRtc[0]--;
          else pucRtc[0] = 23;
          break;
        case 2:
          if(pucRtc[1]) pucRtc[1]--;
          else pucRtc[1] = 59;
          break;
        case 3:
          if(pucRtc[2]) pucRtc[2]--;
          else pucRtc[2] = 59;
          break;
        case 0x11:
          if(uiVol_High - uiVol_Low > 500) uiVol_High -= 500;
          break;
        case 0x12:
          if(uiVol_Low >= 500) uiVol_Low -= 500;
      }
  }
  Led_Disp(ucState+0x11);
}

void Seg_Proc(void)
{
  if(uiSeg_Dly) return;
  uiSeg_Dly = 1;

  switch(ucState&0xf0)
  {
    case 0:							// 显示时钟
      if(!ucState) Read_RTC(pucRtc);
      sprintf(pucSeg_Buf, "%02u-%02u-%02u", (unsigned int)pucRtc[0],\
        (unsigned int)pucRtc[1], (unsigned int)pucRtc[2]);
      if(ucSec&1)
        switch(ucState)
        {
          case 1:						// 熄灭时
          pucSeg_Buf[0] = 0xff;
          pucSeg_Buf[1] = 0xff;
          break;
        case 2:						// 熄灭分
          pucSeg_Buf[3] = 0xff;
          pucSeg_Buf[4] = 0xff;
          break;
        case 3:						// 熄灭秒 
          pucSeg_Buf[6] = 0xff;
          pucSeg_Buf[7] = 0xff;
       }break;
    case 0x10:
      uiVol_Val = PCF8591_Adc()*19.6;  // 5000/255=19.6
      if((uiVol_Val<uiVol_Low)||(uiVol_Val>uiVol_High))
      {								// 保存事件
        if(uiVol_Val<uiVol_Low) pucSeg_Buf[0] = 0;
        else pucSeg_Buf[0] = 1;
        pucSeg_Buf[1] = pucRtc[0];
        pucSeg_Buf[2] = pucRtc[1];
        pucSeg_Buf[3] = pucRtc[2];
        EEPROM_Write(pucSeg_Buf, 4, 4);
      }
      if(ucState == 0x10)
        sprintf(pucSeg_Buf, "-1- %04u", uiVol_Val);
      else
      {								// 显示阈值
        sprintf(pucSeg_Buf, "%04u%04u", uiVol_High, uiVol_Low);
        if(ucSec&1)
          if(ucState == 0x11)
          {							// 熄灭上限
            pucSeg_Buf[0] = 0xff;
            pucSeg_Buf[1] = 0xff;
            pucSeg_Buf[2] = 0xff;
            pucSeg_Buf[3] = 0xff;
          }
          else
          {							// 熄灭下限
            pucSeg_Buf[4] = 0xff;
            pucSeg_Buf[5] = 0xff;
            pucSeg_Buf[6] = 0xff;
            pucSeg_Buf[7] = 0xff;
          }
      }break;
    case 0x20:
      switch(ucState)
      {
        case 0x20:					// 显示频率
          sprintf(pucSeg_Buf, "-2-%05u", (unsigned int)uiFre_Val);
          break;
        case 0x21:					// 显示周期
          sprintf(pucSeg_Buf, "-3-%05.0f", (float)1000000.0/uiFre_Val);
      }break;
    case 0x30:
      switch(ucState)
      {
        case 0x30:					// 显示事件类型
          sprintf(pucSeg_Buf, "      %02u", (unsigned int)pucEvent[0]);
          break;
        case 0x31:					// 显示事件时间
          sprintf(pucSeg_Buf, "%02u-%02u-%02u", (unsigned int)pucEvent[1],\
            (unsigned int)pucEvent[2], (unsigned int)pucEvent[3]);
      }
  }
  Seg_Tran(pucSeg_Buf, pucSeg_Code);
}
