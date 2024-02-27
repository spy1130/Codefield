# include "init.h"

//函数名：关闭外设的初始化函数
//入口参数：无
//返回值：无
//函数功能：将LED、蜂鸣器和继电器全部关闭

void Cls_Peripheral(void)
{
//	P0 = 0x00;//将外设全都灭掉的数据准备
//	P2 = P2 & 0x1F | 0xA0; // 0x80，选通Y5，也就是外设的锁存器，将数据透传过去
//	P2 &= 0x1F; //将打开的锁存器关闭，使任何一个锁存器都不打开	
//	
//	P0 = 0xFF;//将LED全都灭掉的数据准备
//	P2 = P2 & 0x1F | 0x80; // 0x80，选通Y4，也就是LED的锁存器，将数据透传过去
//	P2 &= 0x1F; //将打开的锁存器关闭，使任何一个锁存器都不打开

	P2 = P2 & 0x1F | 0xA0; // 0x80，选通Y5，也就是外设的锁存器，将数据透传过去	
	P0 = 0x00;//将外设全都灭掉的数据准备
	P2 &= 0x1F; //将打开的锁存器关闭，使任何一个锁存器都不打开	
	
	
	P2 = P2 & 0x1F | 0x80; // 0x80，选通Y4，也就是LED的锁存器，将数据透传过去	
	P0 = 0xFF;//将LED全都灭掉的数据准备
	P2 &= 0x1F; //将打开的锁存器关闭，使任何一个锁存器都不打开	
}

