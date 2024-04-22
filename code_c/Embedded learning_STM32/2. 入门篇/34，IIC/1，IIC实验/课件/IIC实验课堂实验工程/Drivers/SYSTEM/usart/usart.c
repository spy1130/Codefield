/**
 ****************************************************************************************************
 * @file        usart.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       ���ڳ�ʼ������(һ���Ǵ���1)��֧��printf
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
 * �޸�˵��
 * V1.0 20200420
 * ��һ�η���
 * V1.1 20210310
 * �޸�HAL��汾Ϊ1.8.3�����򲿷ִ���
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"


/* ���ʹ��os,����������ͷ�ļ�����. */
#if SYS_SUPPORT_OS
#include "includes.h" /* os ʹ�� */
#endif

/******************************************************************************************/
/* �������´���, ֧��printf����, ������Ҫѡ��use MicroLIB */

#if 1

#if (__ARMCC_VERSION >= 6010050)            /* ʹ��AC6������ʱ */
__asm(".global __use_no_semihosting\n\t");  /* ������ʹ�ð�����ģʽ */
__asm(".global __ARM_use_no_argv \n\t");    /* AC6����Ҫ����main����Ϊ�޲�����ʽ�����򲿷����̿��ܳ��ְ�����ģʽ */

#else
/* ʹ��AC5������ʱ, Ҫ�����ﶨ��__FILE �� ��ʹ�ð�����ģʽ */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* ��ʹ�ð�����ģʽ��������Ҫ�ض���_ttywrch\_sys_exit\_sys_command_string����,��ͬʱ����AC6��AC5ģʽ */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* ����_sys_exit()�Ա���ʹ�ð�����ģʽ */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}


/* FILE �� stdio.h���涨��. */
FILE __stdout;

/* MDK����Ҫ�ض���fputc����, printf�������ջ�ͨ������fputc����ַ��������� */
int fputc(int ch, FILE *f)
{
    while ((USART_UX->SR & 0X40) == 0);     /* �ȴ���һ���ַ�������� */

    USART_UX->DR = (uint8_t)ch;             /* ��Ҫ���͵��ַ� ch д�뵽DR�Ĵ��� */
    return ch;
}
#endif
/******************************************************************************************/

#if USART_EN_RX /*���ʹ���˽���*/

/* ���ջ���, ���USART_REC_LEN���ֽ�. */
uint8_t g_usart_rx_buf[USART_REC_LEN];

/*  ����״̬
 *  bit15��      ������ɱ�־
 *  bit14��      ���յ�0x0d
 *  bit13~0��    ���յ�����Ч�ֽ���Ŀ
*/
uint16_t g_usart_rx_sta = 0;

uint8_t aRxBuffer[RXBUFFERSIZE];  /* HAL��ʹ�õĴ��ڽ��ջ��� */
UART_HandleTypeDef uartx_handler; /* UART��� */

/**
 * @brief       ����X�жϷ�����
 * @param       ��
 * @retval      ��
 */
void usart_init(uint32_t bound)
{
    /*UART ��ʼ������*/
    uartx_handler.Instance = USART_UX;                                       /* USART_UX */
    uartx_handler.Init.BaudRate = bound;                                     /* ������ */
    uartx_handler.Init.WordLength = UART_WORDLENGTH_8B;                      /* �ֳ�Ϊ8λ���ݸ�ʽ */
    uartx_handler.Init.StopBits = UART_STOPBITS_1;                           /* һ��ֹͣλ */
    uartx_handler.Init.Parity = UART_PARITY_NONE;                            /* ����żУ��λ */
    uartx_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* ��Ӳ������ */
    uartx_handler.Init.Mode = UART_MODE_TX_RX;                               /* �շ�ģʽ */
    HAL_UART_Init(&uartx_handler);                                           /* HAL_UART_Init()��ʹ��UART1 */
#if USART_EN_RX
    HAL_UART_Receive_IT(&uartx_handler, (uint8_t *)aRxBuffer, RXBUFFERSIZE); /* �ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ��������������� */
    HAL_NVIC_EnableIRQ(USART_UX_IRQn);                                       /* ʹ��USART1�ж�ͨ�� */
    HAL_NVIC_SetPriority(USART_UX_IRQn, 3, 3);                               /* ��2��������ȼ�:��ռ���ȼ�3�������ȼ�3 */
#endif
}

/**
 * @brief       UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
                                �˺����ᱻHAL_UART_Init()����
 * @param       huart:���ھ��
 * @retval      ��
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    /*GPIO�˿����ò������λ��*/
    GPIO_InitTypeDef gpio_initure;

    if (huart->Instance == USART_UX)                        /*HAL���ô˽ӿڽ��д��� MSP��ʼ��*/
    {
        /* IO �� ʱ������ */
        USART_TX_GPIO_CLK_ENABLE();                         /* ʹ�ܴ���TX��ʱ�� */
        USART_RX_GPIO_CLK_ENABLE();                         /* ʹ�ܴ���RX��ʱ�� */
        USART_UX_CLK_ENABLE();                              /* ʹ�ܴ���ʱ�� */
        gpio_initure.Pin = USART_TX_GPIO_PIN;               /* ���ڷ������ź� */
        gpio_initure.Mode = GPIO_MODE_AF_PP;                /* ����������� */
        gpio_initure.Pull = GPIO_PULLUP;                    /* ���� */
        gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;          /* IO�ٶ�����Ϊ���� */
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpio_initure);
                
        gpio_initure.Pin = USART_RX_GPIO_PIN;               /* ����RX�� ģʽ���� */
        gpio_initure.Mode = GPIO_MODE_AF_INPUT;    
        HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpio_initure);   /* ����RX�� �������ó�����ģʽ */
    }
}

/**
 * @brief       UART���ݽ��ջص��ӿ�
                ���ݴ������������
 * @param       huart:���ھ��
 * @retval      ��
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART_UX)                    /* ����Ǵ���1 */
    {
        if ((g_usart_rx_sta & 0x8000) == 0)             /* ����δ��� */
        {
            if (g_usart_rx_sta & 0x4000)                /* ���յ���0x0d */
            {
                if (aRxBuffer[0] != 0x0a)
                {
                    g_usart_rx_sta = 0;                 /* ���մ���,���¿�ʼ */
                }
                else
                {
                    g_usart_rx_sta |= 0x8000;           /* ��������� */
                }
            }
            else                                        /* ��û�յ�0X0D */
            {
                if (aRxBuffer[0] == 0x0d)
                    g_usart_rx_sta |= 0x4000;
                else
                {
                    g_usart_rx_buf[g_usart_rx_sta & 0X3FFF] = aRxBuffer[0];
                    g_usart_rx_sta++;

                    if (g_usart_rx_sta > (USART_REC_LEN - 1))
                    {
                        g_usart_rx_sta = 0;             /* �������ݴ���,���¿�ʼ���� */
                    }
                }
            }
        }
    }
}

/**
 * @brief       ����X�жϷ�����
                ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
 * @param       ��
 * @retval      ��
 */
void USART_UX_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS                                                   /* ʹ��OS */
    OSIntEnter();
#endif
    HAL_UART_IRQHandler(&uartx_handler);                                /* ����HAL���жϴ����ú��� */

    while (HAL_UART_Receive_IT(&uartx_handler, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK) /* һ�δ������֮�����¿����жϲ�����RxXferCountΪ1 */
    {
        /* �������Ῠ�������� */
    }

#if SYSTEM_SUPPORT_OS                                                   /* ʹ��OS */
    OSIntExit();
#endif
}
#endif
