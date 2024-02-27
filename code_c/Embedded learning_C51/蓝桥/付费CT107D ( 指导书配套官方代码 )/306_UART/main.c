#include "tim.h"
#include "seg.h"
#include "stdio.h"
#include "uart.h"

unsigned char ucSec, ucSec1, ucLed;
unsigned char pucSeg_Buf[11], pucSeg_Code[8], ucSeg_Pos;
unsigned char pucUart_Buf[2], ucUart_Num;
unsigned long ulms;

void Uart_Proc(void);

void main(void)
{ 
  Cls_Peripheral();
  Timer1Init();
  UartInit();

  while(1)
  {
    sprintf(pucSeg_Buf, "%08u\r\n", (unsigned int)ucSec);
    Seg_Tran(pucSeg_Buf, pucSeg_Code);
    Uart_Proc();
  }
}

void Time_1(void) interrupt 3
{
  ulms++;
  if(!(ulms % 1000))
  {
    ulms = 0;
    ucSec++;

    ucLed ^= 1;
    Led_Disp(ucLed);
  }	
  Seg_Disp(pucSeg_Code, ucSeg_Pos);
  if(++ucSeg_Pos == 8) ucSeg_Pos = 0;
}

void Uart_Proc(void)
{
  if(ucSec1 != ucSec)
  {
    ucSec1 = ucSec;
    Uart_SendString(pucSeg_Buf);
  }
  if(ucUart_Num == 2)
  {
    ucSec = ((pucUart_Buf[0]-'0')*10) + pucUart_Buf[1]-'0';
    ucUart_Num = 0;
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
