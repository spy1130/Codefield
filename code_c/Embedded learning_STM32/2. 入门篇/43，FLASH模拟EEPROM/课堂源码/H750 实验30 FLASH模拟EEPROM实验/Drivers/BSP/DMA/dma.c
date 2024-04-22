/**
 ****************************************************************************************************
 * @file        dma.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-26
 * @brief       DMA 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32H750开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200326
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "./BSP/DMA/dma.h"
#include "./SYSTEM/delay/delay.h"


DMA_HandleTypeDef  g_dma_handle;                  /* DMA句柄 */

extern UART_HandleTypeDef g_uart1_handle;         /* UART句柄 */

/**
 * @brief       串口TX DMA初始化函数
 *   @note      这里的传输形式是固定的, 这点要根据不同的情况来修改
 *              从存储器 -> 外设模式/8位数据宽度/存储器增量模式
 *
 * @param       dma_stream_handle : DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
 * @retval      无
 */
void dma_init(DMA_Stream_TypeDef *dma_stream_handle, uint32_t ch)
{ 
    if ((uint32_t)dma_stream_handle > (uint32_t)DMA2)     /* 得到当前stream是属于DMA2还是DMA1 */
    {
        __HAL_RCC_DMA2_CLK_ENABLE();                      /* DMA2时钟使能 */
    }
    else 
    {
        __HAL_RCC_DMA1_CLK_ENABLE();                      /* DMA1时钟使能 */
    }

    __HAL_LINKDMA(&g_uart1_handle,hdmatx, g_dma_handle);  /* 将DMA与USART1联系起来(发送DMA) */

    /* Tx DMA配置 */
    g_dma_handle.Instance = dma_stream_handle;                    /* 数据流选择 */
    g_dma_handle.Init.Request = ch;                               /* DMA通道选择 */
    g_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;           /* 存储器到外设 */
    g_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;               /* 外设非增量模式 */
    g_dma_handle.Init.MemInc = DMA_MINC_ENABLE;                   /* 存储器增量模式 */
    g_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  /* 外设数据长度:8位 */
    g_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     /* 存储器数据长度:8位 */
    g_dma_handle.Init.Mode = DMA_NORMAL;                          /* 外设流控模式 */
    g_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;             /* 中等优先级 */
    g_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;            /* 关闭FIFO模式 */
    g_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;    /* FIFO阈值配置 */
    g_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;               /* 存储器突发单次传输 */
    g_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;            /* 外设突发单次传输 */

    HAL_DMA_DeInit(&g_dma_handle);   
    HAL_DMA_Init(&g_dma_handle);
}

/**
 * @brief       DMA请求复用器初始化
 *   @note      函数功能包括:
 *              1, 使能DMA时钟
 *              2, 清空对应Stream上面的所有中断多标志
 *              3, 选择DMA请求通道
 *
 * @param       DMA_Streamx : DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
 * @param       ch          : DMA通道选择,范围:1~115(详见<<STM32H7xx参考手册>>16.3.2节,Table 116)
 * @retval      无
 */
void dma_mux_init(DMA_Stream_TypeDef *DMA_Streamx, uint8_t ch)
{
    DMA_TypeDef *DMAx;
    DMAMUX_Channel_TypeDef *DMAMUXx;
    uint8_t streamx;

    if ((uint32_t)DMA_Streamx > (uint32_t)DMA2)   /* 得到当前stream是属于DMA2还是DMA1 */
    {
        DMAx = DMA2;
        RCC->AHB1ENR |= 1 << 1;     /* DMA2时钟使能 */
    }
    else
    {
        DMAx = DMA1;
        RCC->AHB1ENR |= 1 << 0;     /* DMA1时钟使能 */
    }

    while (DMA_Streamx->CR & 0X01); /* 等待DMA可配置 */

    streamx = (((uint32_t)DMA_Streamx - (uint32_t)DMAx) - 0X10) / 0X18; /* 得到stream通道号 */

    if (streamx >= 6)
    {
        DMAx->HIFCR |= 0X3D << (6 * (streamx - 6) + 16);   /* 清空之前该stream上的所有中断标志 */
    }
    else if (streamx >= 4)
    {
        DMAx->HIFCR |= 0X3D << 6 * (streamx - 4);           /* 清空之前该stream上的所有中断标志 */
    }
    else if (streamx >= 2)
    {
        DMAx->LIFCR |= 0X3D << (6 * (streamx - 2) + 16);    /* 清空之前该stream上的所有中断标志 */
    }
    else
    {
        DMAx->LIFCR |= 0X3D << 6 * streamx; /* 清空之前该stream上的所有中断标志 */
    }
    
    if ((uint32_t)DMA_Streamx > (uint32_t)DMA2)
    {
        streamx += 8;  /* 如果是DMA2,通道编号+8 */
    }

    DMAMUXx = (DMAMUX_Channel_TypeDef *)(DMAMUX1_BASE + streamx * 4);   /* 得到对应的DMAMUX通道控制地址 */
    DMAMUXx->CCR = ch & 0XFF;   /* 选择DMA请求通道 */

}

/**
 * @brief       开启一次DMA传输
 * @param       DMA_Streamx : DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
 * @param       ndtr        : 数据传输量
 * @retval      无
 */
void dma_enable(DMA_Stream_TypeDef *DMA_Streamx, uint16_t ndtr)
{
    DMA_Streamx->CR &= ~(1 << 0);   /* 关闭DMA传输 */

    while (DMA_Streamx->CR & 0X1);  /* 确保DMA可以被设置 */

    DMA_Streamx->NDTR = ndtr;       /* DMA 存储器0地址 */
    DMA_Streamx->CR |= 1 << 0;      /* 开启DMA传输 */
}









