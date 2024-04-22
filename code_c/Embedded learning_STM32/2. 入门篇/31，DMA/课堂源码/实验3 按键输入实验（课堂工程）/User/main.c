/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-19
 * @brief       按键输入 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
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

/* 内存到内存 DMA传输 */
void dma_init(void)
{
    /* 使能DMA1时钟 */
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    g_dma_handler.Instance = DMA1_Channel1;
    g_dma_handler.Init.Direction = DMA_MEMORY_TO_MEMORY;
    
    /* 与目标地址相关 */
    g_dma_handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    g_dma_handler.Init.MemInc = DMA_MINC_ENABLE;

    /* 与源地址相关 */
    g_dma_handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_dma_handler.Init.PeriphInc = DMA_PINC_ENABLE;
    
    g_dma_handler.Init.Priority = DMA_PRIORITY_HIGH;
    g_dma_handler.Init.Mode = DMA_NORMAL;   /* 内存到内存是不支持循环模式 */
    
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

    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    usart_init(115200);
    led_init();                             /* 初始化LED */
    key_init();                             /* 初始化按键 */
    beep_init();                            /* 初始化蜂鸣器 */
    LED0(0);                                /* 先点亮LED0 */
    dma_init();
    
    while(1)
    {
        key = key_scan(0);                  /* 得到键值 */
        
        if (key == KEY0_PRES)
        {
            memset(dest_buf, 0, 10);
            dma_enable_transmit(10);
            
            while(1)
            {
                if (__HAL_DMA_GET_FLAG(&g_dma_handler, DMA_FLAG_TC1))
                {
                    __HAL_DMA_CLEAR_FLAG(&g_dma_handler, DMA_FLAG_TC1);
                    printf("传输完成 \r\n");
                    break;
                }
            }
        }

        LED0_TOGGLE();
        delay_ms(200);
    }
}

