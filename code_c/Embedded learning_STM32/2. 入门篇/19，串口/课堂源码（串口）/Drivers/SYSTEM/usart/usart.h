#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"


extern UART_HandleTypeDef g_uart1_handle;       /* HAL UART句柄 */
extern uint8_t g_rx_buffer[1];                  /* HAL库使用的串口接收数据缓冲区 */
extern uint8_t g_usart1_rx_flag;                /* 串口接收到数据标志 */

void usart_init(uint32_t bound);                /* 串口初始化函数 */

#endif


