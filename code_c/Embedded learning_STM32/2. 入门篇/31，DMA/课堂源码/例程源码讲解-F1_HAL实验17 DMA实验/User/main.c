/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-23
 * @brief       DMA ʵ��
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
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./USMART/usmart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/DMA/dma.h"


const uint8_t TEXT_TO_SEND[] = {"����ԭ�� STM32 DMA ����ʵ��"}; /* Ҫѭ�����͵��ַ��� */
#define SEND_BUF_SIZE       (sizeof(TEXT_TO_SEND) + 2) * 200    /* �������ݳ���, ����sizeof(TEXT_TO_SEND) + 2��200��. */

uint8_t g_sendbuf[SEND_BUF_SIZE];           /* �������ݻ����� */
extern DMA_HandleTypeDef  g_dma_handle;     /* DMA��� */
extern UART_HandleTypeDef g_uart1_handle;   /* UART��� */

int main(void)
{
    uint8_t  key = 0;
    uint16_t i, k;
    uint16_t len;
    uint8_t  mask = 0;
    float pro = 0;          /* ���� */

    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */
    dma_init(DMA1_Channel4);            /* ��ʼ������1 TX DMA */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "DMA TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:Start", RED);

    len = sizeof(TEXT_TO_SEND);
    k = 0;
    
    for (i = 0; i < SEND_BUF_SIZE; i++) /* ���ASCII�ַ������� */
    {
        if (k >= len)   /* �뻻�з� */
        {
            if (mask)
            {
                g_sendbuf[i] = 0x0a;
                k = 0;
            }
            else
            {
                g_sendbuf[i] = 0x0d;
                mask++;
            }
        }
        else     /* ����TEXT_TO_SEND��� */
        {
            mask = 0;
            g_sendbuf[i] = TEXT_TO_SEND[k];
            k++;
        }
    }
 
    i = 0;

    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)       /* KEY0���� */
        {
            printf("\r\nDMA DATA:\r\n");
            lcd_show_string(30, 130, 200, 16, 16, "Start Transimit....", BLUE);
            lcd_show_string(30, 150, 200, 16, 16, "   %", BLUE);    /* ��ʾ�ٷֺ� */

            HAL_UART_Transmit_DMA(&g_uart1_handle, g_sendbuf, SEND_BUF_SIZE);
            /* �ȴ�DMA������ɣ���ʱ������������һЩ���飬������  
             * ʵ��Ӧ���У����������ڼ䣬����ִ����������� 
             */
            while (1)
            {
                if ( __HAL_DMA_GET_FLAG(&g_dma_handle, DMA_FLAG_TC4))   /* �ȴ� DMA1_Channel4 ������� */
                {
                    __HAL_DMA_CLEAR_FLAG(&g_dma_handle, DMA_FLAG_TC4);
                    HAL_UART_DMAStop(&g_uart1_handle);                  /* ��������Ժ�رմ���DMA */
                    break;
                }

                pro = DMA1_Channel4->CNDTR; /* �õ���ǰ��ʣ����ٸ����� */
                len = SEND_BUF_SIZE;        /* �ܳ��� */
                pro = 1 - (pro / len);      /* �õ��ٷֱ� */
                pro *= 100;                 /* ����100�� */
                lcd_show_num(30, 150, pro, 3, 16, BLUE);
            } 
            lcd_show_num(30, 150, 100, 3, 16, BLUE);    /* ��ʾ100% */
            lcd_show_string(30, 130, 200, 16, 16, "Transimit Finished!", BLUE); /* ��ʾ������� */
        }

        i++;
        delay_ms(10);

        if (i == 20)
        {
            LED0_TOGGLE();  /* LED0��˸,��ʾϵͳ�������� */
            i = 0;
        }
    }
}




















