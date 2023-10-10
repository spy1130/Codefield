#include "reg52.h"
#include "Delay.h"
#include "UART.h"


void main()
{	
	UART_Init();
	while (1)
	{
		
		
	}
}

void UART_Routine() interrupt 4 //UART中断
{
	if(RI==1)//接受中断请求标志位，用于区分发送和接受
	{
		P2=~SBUF;//读数据
		UART_SendByte(SBUF);
		RI=0; //清0
	}
}
	