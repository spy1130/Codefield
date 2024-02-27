#include "tim.h"
#include "key.h"
#include "seg.h"
#include "stdio.h"
#include "iic.h"

unsigned int  uiFre_Val, uiVol_Val, uiSeg_Dly;
unsigned char ucAdc_Val, ucLed_Ctrl=1, ucSeg_Ctrl=1;
unsigned char ucState, ucLed, ucDac_Flag=1;
unsigned char pucSeg_Buf[10], pucSeg_Code[8], ucSeg_Pos;
unsigned char ucKey_Dly, ucKey_Old;
unsigned long ulms;

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
void Led_Proc(void);

void main(void)
{ 
  Cls_Peripheral();
  Timer1Init();
  Timer0Init();

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
  if(++ucKey_Dly == 10) ucKey_Dly = 0;
  if(++uiSeg_Dly == 500) uiSeg_Dly = 0;
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

  switch(ucKey_Down)
  {
    case 4:							// S4
      ucState ^= 1; break;
    case 5:							// S5
      ucDac_Flag ^= 1; break;
    case 8:							// S8
      ucLed_Ctrl ^= 1; break;
    case 9:							// S9
      ucSeg_Ctrl ^= 1;
  }
}

void Seg_Proc(void)
{
  if(uiSeg_Dly) return;
  uiSeg_Dly = 1;

  ucAdc_Val = PCF8591_Adc();
  if(ucDac_Flag) PCF8591_Dac(102);	// 255*2/5
  else PCF8591_Dac(ucAdc_Val);

  if(ucSeg_Ctrl)
  {
    if(!ucState)
      sprintf(pucSeg_Buf, "U    %03.2f", (float)ucAdc_Val/51.0);	// 5/255
    else
      sprintf(pucSeg_Buf, "F  %5u", (unsigned int)uiFre_Val);
  }
  else
    sprintf(pucSeg_Buf, "        ");
  Seg_Tran(pucSeg_Buf, pucSeg_Code);
}

void Led_Proc(void)
{
  if(ucLed_Ctrl)
  {
    if(!ucState)
    {
      ucLed |= 1; ucLed &= ~2;
    }
    else
    {
      ucLed |= 2; ucLed &= ~1;
    }

    uiVol_Val = ucAdc_Val*100/51;	// 500/255
    if((uiVol_Val < 150) || ((uiVol_Val >= 250) && (uiVol_Val < 350)))
      ucLed &= ~4;
    else
      ucLed |= 4;

    if((uiFre_Val < 1000) || ((uiFre_Val >= 5000) && (uiFre_Val < 10000)))
      ucLed &= ~8;
    else
      ucLed |= 8;

    if(ucDac_Flag) ucLed &= ~0x10;
    else ucLed |= 0x10;
  }
  else
    ucLed = 0;
}
