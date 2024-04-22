#include "STC15F2K60S2.H"
#include "INT.h"

void INT0_init()
{
	IT0=1;//下降沿触发
	EX0=1;//配置外中断0使能
	
	EA=1;//打开总中断
}

void INT1_init()
{
	IT1=1;//下降沿触发
	EX1=1;//配置外中断1使能
	
	EA=1;//打开总中断
}