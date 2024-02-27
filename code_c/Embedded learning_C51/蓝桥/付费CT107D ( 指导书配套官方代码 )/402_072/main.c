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

void Timer0Init(void)				// 16λ������ģʽ
{
  AUXR &= 0x7F;						// ��ʱ��ʱ��12Tģʽ
  TMOD |= 5;						// ����16λ������ģʽ
  TL0 = 0;							// ���ü�����ֵ
  TH0 = 0;							// ���ü�����ֵ
  TF0 = 0;							// ���TF0��־
  TR0 = 1;							// ��ʼ����
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
    case 5:							// S5���л���ʾʱ�ӡ���ѹ��Ƶ�ʻ��¼�
      ucState = (ucState&0xf0)+0x10;
      if(ucState >= 0x40) ucState = 0;
      if(ucState == 0x30)
      {								// ��ȡ�¼�
        EEPROM_Read(pucEvent, 4, 4);
        if(pucEvent[0] > 2) pucEvent[0] = 0;
        if(pucEvent[1] > 24) pucEvent[1] = 0;
        if(pucEvent[2] > 60) pucEvent[2] = 0;
        if(pucEvent[3] > 60) pucEvent[3] = 0;
      }break;
    case 4:							// S4������ѡ��
      switch(ucState&0xf0)
      {
        case 0:						// ѡ����ʾʱ�ӡ�����ʱ�����÷ֻ�������
          if(++ucState == 4)
          {
            ucState = 0;
            Set_RTC(pucRtc);  		// ����ʱ��
          }break;
        case 0x10:					// ѡ����ʾ��ѹ�����õ�ѹ���޻�����
          if(++ucState == 0x13)
          {
            ucState = 0x10;
            // ������ֵ
            pucSeg_Buf[0] = uiVol_Low & 0xff;
            pucSeg_Buf[1] = uiVol_Low >> 8;
            pucSeg_Buf[2] = uiVol_High & 0xff;
            pucSeg_Buf[3] = uiVol_High >> 8;
            EEPROM_Write(pucSeg_Buf, 0, 4);
          }break;
        case 0x20:					// ѡ����ʾƵ�ʻ�����
          if(++ucState == 0x22) ucState = 0x20;
          break;
        case 0x30:					// ѡ����ʾ�¼����ͻ�ʱ��
          if(++ucState == 0x32) ucState = 0x30;
      }break;
    case 9:							// S9��������
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
    case 8:							// S8��������
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
    case 0:							// ��ʾʱ��
      if(!ucState) Read_RTC(pucRtc);
      sprintf(pucSeg_Buf, "%02u-%02u-%02u", (unsigned int)pucRtc[0],\
        (unsigned int)pucRtc[1], (unsigned int)pucRtc[2]);
      if(ucSec&1)
        switch(ucState)
        {
          case 1:						// Ϩ��ʱ
          pucSeg_Buf[0] = 0xff;
          pucSeg_Buf[1] = 0xff;
          break;
        case 2:						// Ϩ���
          pucSeg_Buf[3] = 0xff;
          pucSeg_Buf[4] = 0xff;
          break;
        case 3:						// Ϩ���� 
          pucSeg_Buf[6] = 0xff;
          pucSeg_Buf[7] = 0xff;
       }break;
    case 0x10:
      uiVol_Val = PCF8591_Adc()*19.6;  // 5000/255=19.6
      if((uiVol_Val<uiVol_Low)||(uiVol_Val>uiVol_High))
      {								// �����¼�
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
      {								// ��ʾ��ֵ
        sprintf(pucSeg_Buf, "%04u%04u", uiVol_High, uiVol_Low);
        if(ucSec&1)
          if(ucState == 0x11)
          {							// Ϩ������
            pucSeg_Buf[0] = 0xff;
            pucSeg_Buf[1] = 0xff;
            pucSeg_Buf[2] = 0xff;
            pucSeg_Buf[3] = 0xff;
          }
          else
          {							// Ϩ������
            pucSeg_Buf[4] = 0xff;
            pucSeg_Buf[5] = 0xff;
            pucSeg_Buf[6] = 0xff;
            pucSeg_Buf[7] = 0xff;
          }
      }break;
    case 0x20:
      switch(ucState)
      {
        case 0x20:					// ��ʾƵ��
          sprintf(pucSeg_Buf, "-2-%05u", (unsigned int)uiFre_Val);
          break;
        case 0x21:					// ��ʾ����
          sprintf(pucSeg_Buf, "-3-%05.0f", (float)1000000.0/uiFre_Val);
      }break;
    case 0x30:
      switch(ucState)
      {
        case 0x30:					// ��ʾ�¼�����
          sprintf(pucSeg_Buf, "      %02u", (unsigned int)pucEvent[0]);
          break;
        case 0x31:					// ��ʾ�¼�ʱ��
          sprintf(pucSeg_Buf, "%02u-%02u-%02u", (unsigned int)pucEvent[1],\
            (unsigned int)pucEvent[2], (unsigned int)pucEvent[3]);
      }
  }
  Seg_Tran(pucSeg_Buf, pucSeg_Code);
}
