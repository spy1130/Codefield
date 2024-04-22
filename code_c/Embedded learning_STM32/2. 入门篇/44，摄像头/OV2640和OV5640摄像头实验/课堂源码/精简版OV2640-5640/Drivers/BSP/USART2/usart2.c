/**
 ****************************************************************************************************
 * @file        usart2.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-03
 * @brief       ����2 ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32H750������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200403
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/USART2/usart2.h"


UART_HandleTypeDef g_uart2_handle;  /* USART2��� */

/**
 * @brief       ����2��ʼ������
 * @note        ע��:����2��ʱ��ԴƵ����sys�Ѿ����ù���
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 */
void usart2_init(uint32_t baudrate)
{
    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_USART2_CLK_ENABLE();  /* ʹ��USART2ʱ�� */
    USART2_TX_GPIO_CLK_ENABLE();    /* ʹ�ܴ���TX��ʱ�� */
    USART2_RX_GPIO_CLK_ENABLE();    /* ʹ�ܴ���RX��ʱ�� */

    gpio_init_struct.Pin = USART2_TX_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* ���� */
    gpio_init_struct.Alternate = USART2_TX_GPIO_AF;         /* ����ΪUSART2 */
    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &gpio_init_struct);  /* ��ʼ������TX���� */

    gpio_init_struct.Pin = USART2_RX_GPIO_PIN;
    gpio_init_struct.Alternate = USART2_RX_GPIO_AF;         /* ����ΪUSART2 */
    HAL_GPIO_Init(USART2_RX_GPIO_PORT, &gpio_init_struct);  /* ��ʼ������RX���� */

    g_uart2_handle.Instance = USART2;                       /* USART2 */
    g_uart2_handle.Init.BaudRate = baudrate;                /* ������ */
    g_uart2_handle.Init.WordLength = UART_WORDLENGTH_8B;    /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_uart2_handle.Init.StopBits = UART_STOPBITS_1;         /* һ��ֹͣλ */
    g_uart2_handle.Init.Parity = UART_PARITY_NONE;          /* ����żУ��λ */
    g_uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;    /* ��Ӳ������ */
    g_uart2_handle.Init.Mode = UART_MODE_TX;                /* ��ģʽ */
    HAL_UART_Init(&g_uart2_handle);                         /* ʹ��USART2 */
}






