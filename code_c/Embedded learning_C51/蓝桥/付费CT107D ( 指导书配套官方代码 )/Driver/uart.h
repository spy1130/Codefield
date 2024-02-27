#ifndef __UART_H
#define __UART_H
#include "STC15F2K60S2.H"

void UartInit(void);
void Uart_SendString(unsigned char *pucStr);
#endif
