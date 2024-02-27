#include "tim.h"

void Cls_Peripheral(void)
{
  P0 = 0xFF;
  P2 = P2 & 0x1F | 0x80;			// P27~P25清零，再定位Y4C
  P2 &= 0x1F;						// P27~P25清零
  P0 = 0;
  P2 = P2 & 0x1F | 0xA0;			// P27~P25清零，再定位Y5C
  P2 &= 0x1F;						// P27~P25清零
}

void Led_Disp(unsigned char ucLed)
{ // IO模式（J13-2和J13-3相连）
  P0 = ~ucLed;
  P2 = P2 & 0x1F | 0x80;			// P27~P25清零，再定位Y4C
  P2 &= 0x1F;						// P27~P25清零
//XBYTE[0x8000] = ~ucLed;			// MM模式（J13-2和J13-1相连）
}

void Timer1Init(void)				// 1毫秒@12.000MHz
{
  AUXR &= 0xBF;						// 定时器时钟12T模式
  TMOD &= 0x0F;						// 设置定时器模式
  TL1 = 0x18;						// 设置定时初值
  TH1 = 0xFC;						// 设置定时初值
  TF1 = 0;		 					// 清除TF1标志
  TR1 = 1;			  				// 定时器1开始计时
  ET1 = 1;		  					// 允许定时器1中断
  EA = 1;		  					// 允许系统中断
}
