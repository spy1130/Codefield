#include "tim.h"
#include "seg.h"
#include "stdio.h"
#include "key.h"
#include "onewire.h"
#include "iic.h"
#include "ultrasonic.h"
#include "uart.h"

unsigned int  uiTem_Val, uiSeg_Dly;
unsigned char ucDis_Val, ucNum;
unsigned char ucDis_Par, ucTem_Par;
unsigned char ucDis_Old, ucTem_Old;
unsigned char ucState, ucLed, ucDac=1;
unsigned char pucSeg_Buf[10], pucSeg_Code[8], ucSeg_Pos;
unsigned char ucKey_Dly, ucKey_Old, ucKey_Long;
unsigned char pucUart_Buf[12], ucUart_Num;
unsigned long ulms, ulKey_Time;

void Key_Proc(void);
void Seg_Proc(void);
void Uart_Proc(void);

void main(void)
{
  Cls_Peripheral();
  Timer1Init();	
  Timer0Init();	
  UartInit();

  EEPROM_Read(pucSeg_Buf, 0, 1);
  ucNum = pucSeg_Buf[0];
  ucTem_Par = 30;
  ucDis_Par = 35;

  while(1)
  {
    Key_Proc();
    Seg_Proc();
//  Uart_Proc();		// 为了便于在线调试，可以在其他功能调通后再加入UART处理
  }
}

void Time_1(void) interrupt 3
{
  ulms++;
  if(++ucKey_Dly == 10) ucKey_Dly = 0;
  if(++uiSeg_Dly == 500) uiSeg_Dly = 0;
  Led_Disp(ucLed);
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
    ucKey_Long = 0;
  }

  if(ulms-ulKey_Time < 1000)
    switch(ucKey_Up)
    {
      case 5:							// S5
        if(ucState < 3)
        {
          ucState = 0x10;
          ucTem_Old = ucTem_Par;
          ucDis_Old = ucDis_Par;
        }
        else
        {
          ucState = 0;
          if((ucTem_Old != ucTem_Par) || (ucDis_Old != ucDis_Par))
          {
            pucSeg_Buf[0] = ++ucNum;
            EEPROM_Write(pucSeg_Buf, 0, 1);
          }
        }
        break;
      case 4:							// S4
        if(ucState < 3)
        {
          if(++ucState == 3) ucState = 0;
        }
        else 
          if(++ucState == 0x12) ucState = 0x10;
        break;
      case 9:							// 9
        if(ucState == 0x10)
        {
          ucTem_Par += 2;
          if(ucTem_Par >= 100) ucTem_Par = 0;
        }
        if(ucState == 0x11)
        {
          ucDis_Par += 5;
          if(ucDis_Par >= 100) ucDis_Par = 0;
        }
        break;
      case 8:							// S8
        if(ucState == 0x10)
        {
          if(ucTem_Par < 2) ucTem_Par = 100;
          ucTem_Par -= 2;
        }
        if(ucState == 0x11)
        {
          if(ucDis_Par < 5) ucDis_Par = 100;
          ucDis_Par -= 5;
        }
    }
  else
  {
    if((ucKey_Old == 9) && !ucKey_Long)
    {
      ucKey_Long = 1;
      ucDac ^= 1;
      if(ucDac) ucLed |= 4;
      else ucLed &= ~4;
    }
    if((ucKey_Old == 8) && ucNum)
    {
      ucNum = 0;
      pucSeg_Buf[0] = ucNum;
      EEPROM_Write(pucSeg_Buf, 0, 1);
    }
  }
}

void Seg_Proc(void)
{
  if(uiSeg_Dly) return;
  uiSeg_Dly = 1;

  switch(ucState)
  {
    case 0:						// 温度数据
      uiTem_Val = rd_temperature();
      if(uiTem_Val > (ucTem_Par<<4)) ucLed |= 1;
      else ucLed &= ~1;
  		sprintf(pucSeg_Buf, "C   %04.2f", (float)uiTem_Val/16.0);
      break;
    case 1:						// 距离数据
      ucDis_Val = Wave_Recv();
      if(ucDis_Val < ucDis_Par) ucLed |= 2;
      else ucLed &= ~2;
  		if(ucDac)
      {
        if(ucDis_Val <= ucDis_Par)
          PCF8591_Dac(102);		// 255*2/5
        else
          PCF8591_Dac(204);		// 255*4/5
      }
      else
          PCF8591_Dac(20);		// 255*0.4/5
      sprintf(pucSeg_Buf, "L    %3u", (unsigned int)ucDis_Val);
      break;
    case 2:						// 变更次数
      sprintf(pucSeg_Buf, "N     %02u", (unsigned int)ucNum);
      break;
    case 0x10:					// 温度参数
      sprintf(pucSeg_Buf, "P  1  %02u", (unsigned int)ucTem_Par);
      break;
    case 0x11:					// 距离参数
      sprintf(pucSeg_Buf, "P  2  %02u", (unsigned int)ucDis_Par);
  }
  Seg_Tran(pucSeg_Buf, pucSeg_Code);
}

void Uart_Proc(void)
{
  if(ucUart_Num > 0)
  {
    if(pucUart_Buf[ucUart_Num-1] == 0xa)
    {
      if((pucUart_Buf[0]=='S') && (pucUart_Buf[1]=='T')\
        && (pucUart_Buf[2]==0xd))
        sprintf(pucUart_Buf, "$%02u,%04.2f\r\n",\
          (unsigned int)ucDis_Val, (float)uiTem_Val/16.0);
      else if((pucUart_Buf[0]=='P') && (pucUart_Buf[1]=='A')\
        && (pucUart_Buf[2]=='R') && (pucUart_Buf[3]=='A')\
        && (pucUart_Buf[4]==0xd))
			sprintf(pucUart_Buf, "#%02u,%02u\r\n",\
          (unsigned int)ucDis_Par, (unsigned int)ucTem_Par);
      else
        sprintf(pucUart_Buf, "ERROR\r\n");
      Uart_SendString(pucUart_Buf);
      ucUart_Num = 0;
    }
    else
      if(ucUart_Num == 6)
      {
        Uart_SendString("ERROR\r\n");
        ucUart_Num = 0;
      }
  }
}

void uart_0(void) interrupt 4
{
  if(RI)
  {
    pucUart_Buf[ucUart_Num++] = SBUF;
    RI = 0;
  }
}
