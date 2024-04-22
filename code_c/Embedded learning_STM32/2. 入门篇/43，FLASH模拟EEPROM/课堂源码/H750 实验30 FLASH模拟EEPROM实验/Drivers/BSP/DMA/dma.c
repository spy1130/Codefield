/**
 ****************************************************************************************************
 * @file        dma.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-26
 * @brief       DMA ��������
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
 * V1.0 20200326
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/DMA/dma.h"
#include "./SYSTEM/delay/delay.h"


DMA_HandleTypeDef  g_dma_handle;                  /* DMA��� */

extern UART_HandleTypeDef g_uart1_handle;         /* UART��� */

/**
 * @brief       ����TX DMA��ʼ������
 *   @note      ����Ĵ�����ʽ�ǹ̶���, ���Ҫ���ݲ�ͬ��������޸�
 *              �Ӵ洢�� -> ����ģʽ/8λ���ݿ��/�洢������ģʽ
 *
 * @param       dma_stream_handle : DMA������,DMA1_Stream0~7/DMA2_Stream0~7
 * @retval      ��
 */
void dma_init(DMA_Stream_TypeDef *dma_stream_handle, uint32_t ch)
{ 
    if ((uint32_t)dma_stream_handle > (uint32_t)DMA2)     /* �õ���ǰstream������DMA2����DMA1 */
    {
        __HAL_RCC_DMA2_CLK_ENABLE();                      /* DMA2ʱ��ʹ�� */
    }
    else 
    {
        __HAL_RCC_DMA1_CLK_ENABLE();                      /* DMA1ʱ��ʹ�� */
    }

    __HAL_LINKDMA(&g_uart1_handle,hdmatx, g_dma_handle);  /* ��DMA��USART1��ϵ����(����DMA) */

    /* Tx DMA���� */
    g_dma_handle.Instance = dma_stream_handle;                    /* ������ѡ�� */
    g_dma_handle.Init.Request = ch;                               /* DMAͨ��ѡ�� */
    g_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;           /* �洢�������� */
    g_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;               /* ���������ģʽ */
    g_dma_handle.Init.MemInc = DMA_MINC_ENABLE;                   /* �洢������ģʽ */
    g_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  /* �������ݳ���:8λ */
    g_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     /* �洢�����ݳ���:8λ */
    g_dma_handle.Init.Mode = DMA_NORMAL;                          /* ��������ģʽ */
    g_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;             /* �е����ȼ� */
    g_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;            /* �ر�FIFOģʽ */
    g_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;    /* FIFO��ֵ���� */
    g_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;               /* �洢��ͻ�����δ��� */
    g_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;            /* ����ͻ�����δ��� */

    HAL_DMA_DeInit(&g_dma_handle);   
    HAL_DMA_Init(&g_dma_handle);
}

/**
 * @brief       DMA����������ʼ��
 *   @note      �������ܰ���:
 *              1, ʹ��DMAʱ��
 *              2, ��ն�ӦStream����������ж϶��־
 *              3, ѡ��DMA����ͨ��
 *
 * @param       DMA_Streamx : DMA������,DMA1_Stream0~7/DMA2_Stream0~7
 * @param       ch          : DMAͨ��ѡ��,��Χ:1~115(���<<STM32H7xx�ο��ֲ�>>16.3.2��,Table 116)
 * @retval      ��
 */
void dma_mux_init(DMA_Stream_TypeDef *DMA_Streamx, uint8_t ch)
{
    DMA_TypeDef *DMAx;
    DMAMUX_Channel_TypeDef *DMAMUXx;
    uint8_t streamx;

    if ((uint32_t)DMA_Streamx > (uint32_t)DMA2)   /* �õ���ǰstream������DMA2����DMA1 */
    {
        DMAx = DMA2;
        RCC->AHB1ENR |= 1 << 1;     /* DMA2ʱ��ʹ�� */
    }
    else
    {
        DMAx = DMA1;
        RCC->AHB1ENR |= 1 << 0;     /* DMA1ʱ��ʹ�� */
    }

    while (DMA_Streamx->CR & 0X01); /* �ȴ�DMA������ */

    streamx = (((uint32_t)DMA_Streamx - (uint32_t)DMAx) - 0X10) / 0X18; /* �õ�streamͨ���� */

    if (streamx >= 6)
    {
        DMAx->HIFCR |= 0X3D << (6 * (streamx - 6) + 16);   /* ���֮ǰ��stream�ϵ������жϱ�־ */
    }
    else if (streamx >= 4)
    {
        DMAx->HIFCR |= 0X3D << 6 * (streamx - 4);           /* ���֮ǰ��stream�ϵ������жϱ�־ */
    }
    else if (streamx >= 2)
    {
        DMAx->LIFCR |= 0X3D << (6 * (streamx - 2) + 16);    /* ���֮ǰ��stream�ϵ������жϱ�־ */
    }
    else
    {
        DMAx->LIFCR |= 0X3D << 6 * streamx; /* ���֮ǰ��stream�ϵ������жϱ�־ */
    }
    
    if ((uint32_t)DMA_Streamx > (uint32_t)DMA2)
    {
        streamx += 8;  /* �����DMA2,ͨ�����+8 */
    }

    DMAMUXx = (DMAMUX_Channel_TypeDef *)(DMAMUX1_BASE + streamx * 4);   /* �õ���Ӧ��DMAMUXͨ�����Ƶ�ַ */
    DMAMUXx->CCR = ch & 0XFF;   /* ѡ��DMA����ͨ�� */

}

/**
 * @brief       ����һ��DMA����
 * @param       DMA_Streamx : DMA������,DMA1_Stream0~7/DMA2_Stream0~7
 * @param       ndtr        : ���ݴ�����
 * @retval      ��
 */
void dma_enable(DMA_Stream_TypeDef *DMA_Streamx, uint16_t ndtr)
{
    DMA_Streamx->CR &= ~(1 << 0);   /* �ر�DMA���� */

    while (DMA_Streamx->CR & 0X1);  /* ȷ��DMA���Ա����� */

    DMA_Streamx->NDTR = ndtr;       /* DMA �洢��0��ַ */
    DMA_Streamx->CR |= 1 << 0;      /* ����DMA���� */
}









