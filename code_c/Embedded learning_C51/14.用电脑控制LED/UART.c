#include "reg52.h"

/**
  * @brief 串口初始化，4800bps@12.000MHz
  * @param  无
  * @retval 无
  */
void UART_Init() //4800bps@11.0592MHz
{
	SCON=0x50;          //串口通信接口
	PCON &= 0x7F;		//波特率不倍速
	TMOD &= 0x0F;       //清空定时器1模式位
	TMOD |= 0x20;       //设定定时器1为8位自动重装
	TL1 = 0xFA;			//设置定时初始值
	TH1 = 0xFA;			//设置定时重载值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	EA=1; 				//启动总中断
	ES=1; 				//启动串口中断
}
/**
  * @brief 串口发数据
  * @param  Byte：16进制数据
  * @retval 无
  */
void UART_SendByte(unsigned	char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0; //重新复位
}