#include "reg52.h"
#include "Delay.h"
#include "UART.h"

unsigned char sec=0;

void main()
{	
	UART_Init();
	while (1)
	{
		UART_SendByte(sec);
		sec++;
		Delay(1000); //��ֹ����̫�죬����
		
	}
}
