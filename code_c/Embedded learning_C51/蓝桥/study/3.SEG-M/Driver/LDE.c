#include "LED.h"
//输入16进制，8位,1亮0灭
void LED(unsigned int num)
{
	P0=~num;
	P2= P2 & 0X1F | 0X80;//开启Y4
	P2 &=0X1F;//关闭Y4~Y6
}