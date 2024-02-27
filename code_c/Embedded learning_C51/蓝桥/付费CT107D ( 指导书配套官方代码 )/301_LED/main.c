// 使用程序前，将J13调整为IO模式（2-3脚短接）
#include "reg52.h"
// #include "absacc.h"
// 关闭外设
void Cls_Peripheral(void)
{ // IO模式（J13-2和J13-3相连）
  P0 = 0xFF;
  P2 = P2 & 0x1F | 0x80;			// P27~P25清零，再定位Y4C
  P2 &= 0x1F;						// P27~P25清零
  P0 = 0;
  P2 = P2 & 0x1F | 0xA0;			// P27~P25清零，再定位Y5C
  P2 &= 0x1F;						// P27~P25清零
//XBYTE[0x8000] = 0xFF;				// MM模式（J13-2和J13-1相连）
//XBYTE[0xA000] = 0;
}
// LED显示
void Led_Disp(unsigned char ucLed)
{ // IO模式（J13-2和J13-3相连）
  P0 = ~ucLed;
  P2 = P2 & 0x1F | 0x80;			// P27~P25清零，再定位Y4C
  P2 &= 0x1F;						// P27~P25清零
//XBYTE[0x8000] = ~ucLed;			// MM模式（J13-2和J13-1相连）
}
// 延时函数（最小约1ms@12MHz）
void Delay(unsigned int num)
{
  unsigned int i;
  while(num--)
    for(i=0; i<628; i++);
}
// 主函数
void main(void)
{
  unsigned char i, j;

  Cls_Peripheral();

  while(1)
  { // 4个亮度等级
    for(i=0; i<4; i++)
      for(j=0; j<100; j++)
      {
        Led_Disp(0xff);
        Delay(i+1);
        Led_Disp(0);
        Delay(4-i);
      }
  }
}
