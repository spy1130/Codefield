/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-19
 * @brief       �������� ʵ��
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
#include "./BSP/LED/led.h"
#include "./BSP/BEEP/beep.h"
#include "./BSP/KEY/key.h"
#include "string.h"

DMA_HandleTypeDef g_dma_handler;

uint8_t src_buf[10] = {0x0a, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
uint8_t dest_buf[10] = {0};

/* �ڴ浽�ڴ� DMA���� */
void dma_init(void)
{
    /* ʹ��DMA1ʱ�� */
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    g_dma_handler.Instance = DMA1_Channel1;
    g_dma_handler.Init.Direction = DMA_MEMORY_TO_MEMORY;
    
    /* ��Ŀ���ַ��� */
    g_dma_handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    g_dma_handler.Init.MemInc = DMA_MINC_ENABLE;

    /* ��Դ��ַ��� */
    g_dma_handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_dma_handler.Init.PeriphInc = DMA_PINC_ENABLE;
    
    g_dma_handler.Init.Priority = DMA_PRIORITY_HIGH;
    g_dma_handler.Init.Mode = DMA_NORMAL;   /* �ڴ浽�ڴ��ǲ�֧��ѭ��ģʽ */
    
    HAL_DMA_Init(&g_dma_handler);
    HAL_DMA_Start(&g_dma_handler, (uint32_t)src_buf, (uint32_t)dest_buf, 0);
}

void dma_enable_transmit(uint16_t cndtr)
{
    __HAL_DMA_DISABLE(&g_dma_handler);
    
//    DMA1_Channel1->CNDTR = cndtr;
    g_dma_handler.Instance->CNDTR = cndtr;
    
    __HAL_DMA_ENABLE(&g_dma_handler);
}

int main(void)
{
    uint8_t key;

    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* ����ʱ��, 72Mhz */
    delay_init(72);                         /* ��ʱ��ʼ�� */
    usart_init(115200);
    led_init();                             /* ��ʼ��LED */
    key_init();                             /* ��ʼ������ */
    beep_init();                            /* ��ʼ�������� */
    LED0(0);                                /* �ȵ���LED0 */
    dma_init();
    
    while(1)
    {
        key = key_scan(0);                  /* �õ���ֵ */
        
        if (key == KEY0_PRES)
        {
            memset(dest_buf, 0, 10);
            dma_enable_transmit(10);
            
            while(1)
            {
                if (__HAL_DMA_GET_FLAG(&g_dma_handler, DMA_FLAG_TC1))
                {
                    __HAL_DMA_CLEAR_FLAG(&g_dma_handler, DMA_FLAG_TC1);
                    printf("������� \r\n");
                    break;
                }
            }
        }

        LED0_TOGGLE();
        delay_ms(200);
    }
}

