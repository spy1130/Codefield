#include "tim.h"
#include "key.h"
#include "seg.h"
#include "stdio.h"
#include "onewire.h"

sbit Pwm_Out = P3^4;
unsigned int  uiSeg_Dly;
unsigned char ucTemp_Val, ucTemp_Flag;
unsigned char ucState, ucSec, ucLed=1;
unsigned char ucDelay, ucPwm, ucPlus=2;
unsigned char pucSeg_Buf[9], pucSeg_Code[8], ucSeg_Pos;
unsigned char ucKey_Dly, ucKey_Old;
unsigned long ulms;

void Key_Proc(void);
void Seg_Proc(void);

void Timer0Init(void)				// 100微秒@12.000MHz
{
  AUXR |= 0x80;						// 定时器时钟1T模式
  TMOD &= 0xF0;						// 设置定时器模式
  TL0 = 0x50;						// 设置定时初值
  TH0 = 0xFB;						// 设置定时初值
  TF0 = 0;							// 清除TF0标志
  TR0 = 1;							// 定时器0开始计时
  ET0 = 1;							// 允许定时器0中断
}

void main(void)
{
  Cls_Peripheral();
  Timer1Init();
  Timer0Init();

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
    if(ucDelay) ucDelay--;
    else if(ucSec) ucSec--;
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
      if(++ucState == 3)				// 切换工作模式
        ucState = 0;
      switch(ucState)
      {
        case 0:
          ucLed |= 1; ucLed &= ~6;	// L1亮
          ucPlus = 2; break;
        case 1:
          ucLed |= 2; ucLed &= ~5;	// L2亮
          ucPlus = 3; break;
        case 2:
          ucLed |= 4; ucLed &= ~3;	// L3亮
          ucPlus = 7;
      }break;
    case 5:							// S5
      ucSec += 60;					// 切换定时
      if(ucSec >= 180) ucSec = 0;
      else if(ucSec >= 120) ucSec = 120;
      else if(ucSec >= 60) ucSec = 60;
      ucDelay = 2;
      break;
    case 9:							// S9
      ucSec = 0;						// 清除定时
      break;
    case 8:							// S8
      ucTemp_Flag ^= 1;				// 切换室温显示
  }
}

void Seg_Proc(void)
{
  if(uiSeg_Dly) return;
  uiSeg_Dly = 1;

  if(!ucTemp_Flag)
    sprintf(pucSeg_Buf, "-%1u- %04u", \
      (unsigned int)ucState+1, (unsigned int)ucSec);
  else
  {
    ucTemp_Val = rd_temperature()>>4;
    sprintf(pucSeg_Buf, "-4-  %2uC", (unsigned int)ucTemp_Val);
  }
  Seg_Tran(pucSeg_Buf, pucSeg_Code);
}

void Time_0(void) interrupt 1
{
  if(ucSec)
  {
    if(++ucPwm == 10) ucPwm = 0;
    if(!ucPwm)
    {
      Pwm_Out = 1;
      ucLed |= 0x80;
    }
    else if(ucPwm == ucPlus)
    {
      Pwm_Out = 0;
      ucLed &= 0x7F;
    }
  }
  else
    ucLed &= 0x7f;
  Led_Disp(ucLed);
}
