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

void UART_Routine() interrupt 4 //UART�ж�
{
	if(RI==1)//�����ж������־λ���������ַ��ͺͽ���
	{
		P2=~SBUF;//������
		UART_SendByte(SBUF);
		RI=0; //��0
	}
}
	