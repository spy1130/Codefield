// 使用程序前，将跳线J5调整为BTN模式（2、3引脚短接使第一列按键一端接地）
// 将跳线J13调整为IO模式（2、3引脚短接使U25或非门输入信号WR接地）
// 按下P32-INT0引脚对应的按键S5，L1状态翻转
// 按下P33-INT1引脚对应的按键S4，L2状态翻转
#include "reg52.h"

unsigned char ucLed;
// 关闭外设
void Cls_Peripheral(void)
{
  P0 = 0xFF;
  P2 = P2 & 0x1F | 0x80;			// P27~P25清零，再定位Y4C
  P2 &= 0x1F;						// P27~P25清零
  P0 = 0;
  P2 = P2 & 0x1F | 0xA0;			// P27~P25清零，再定位Y5C
  P2 &= 0x1F;						// P27~P25清零
}
// 系统初始化
void Sys_Init(void)
{
  EX0 = 1;
  IT0 = 1;  							// 边沿触发方式(下降沿)
  EX1 = 1;
  IT1 = 1;  							// 边沿触发方式(下降沿)
  EA = 1;  							// 开启总中断
}
// 主函数
void main(void)
{
  Cls_Peripheral();
  Sys_Init();

  while(1);
}
//外部中断0中断服务函数
void isr_intr_0(void) interrupt 0
{
  ucLed ^= 1;
  P0 = ~ucLed;
  P2 = P2 & 0x1F | 0x80; 			// P27~P25清零，再定位Y4C
  P2 &= 0x1F; 						// P27~P25清零
}
//外部中断1中断服务函数
void isr_intr_1(void) interrupt 2
{
  ucLed ^= 2;
  P0 = ~ucLed;
  P2 = P2 & 0x1F | 0x80; 			// P27-P25清零，再定位Y4C
  P2 &= 0x1F; 						// P27-P25清零
}
