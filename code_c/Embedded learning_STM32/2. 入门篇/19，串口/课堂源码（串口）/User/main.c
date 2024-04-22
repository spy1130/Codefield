/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       ����� ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"


int main(void)
{
    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* ����ʱ��,72M */
    delay_init(72);                             /* ��ʼ����ʱ���� */
    led_init();                                 /* ��ʼ��LED */
    usart_init(115200);                         /* ��������Ϊ115200 */
    
    printf("������һ��Ӣ���ַ���\r\n\r\n");
    while(1)
    {
        if(g_usart1_rx_flag == 1)
        {
            printf("��������ַ�Ϊ��\r\n");
            HAL_UART_Transmit(&g_uart1_handle, (uint8_t*)g_rx_buffer, 1, 1000);
            while(__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_TC) != 1);
            printf("\r\n");
            g_usart1_rx_flag = 0;
        }
        else
        {
            delay_ms(10);
        }
    }
}

