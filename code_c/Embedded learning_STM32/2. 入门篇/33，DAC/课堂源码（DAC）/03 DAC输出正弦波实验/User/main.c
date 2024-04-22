/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-24
 * @brief       DAC输出正弦波 实验
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
#include "./BSP/LCD/lcd.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/DAC/dac.h"
#include "./BSP/KEY/key.h"
#include "math.h"


uint16_t g_dac_sin_buf[4096];            /* 发送数据缓冲区 */

/**
 * @brief       产生正弦波序列函数
 *   @note      需保证: maxval > samples/2
 * @param       maxval : 最大值(0 < maxval < 2048)
 * @param       samples: 采样点的个数
 * @retval      无
 */
void dac_creat_sin_buf(uint16_t maxval, uint16_t samples)
{
    uint8_t i;
    float outdata = 0;                     /* 存放计算后的数字量 */
    float inc = (2 * 3.1415962) / samples; /* 计算相邻两个点的x轴间隔 */

    if(maxval <= (samples / 2))return ;	   /* 数据不合法 */

    for (i = 0; i < samples; i++)
    {
        /* 
         * 正弦波函数解析式：y = Asin(ωx + φ）+ b
         * 计算每个点的y值，将峰值放大maxval倍，并将曲线向上偏移maxval到正数区域
         * 注意：DAC无法输出负电压，所以需要将曲线向上偏移一个峰值的量，让整个曲线都落在正数区域
         */
        outdata = maxval * sin(inc * i) + maxval;
        if (outdata > 4095)
            outdata = 4095; /* 上限限定 */
        //printf("%f\r\n",outdata);
        g_dac_sin_buf[i] = outdata;
    }
}

int main(void)
{
    uint8_t t = 0;
    uint8_t key;
    
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    led_init();                         /* 初始化LED */
    lcd_init();                         /* 初始化LCD */
    key_init();                         /* 初始化按键 */

    dac_dma_wave_init();
    
    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "DAC DMA Sine WAVE TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:3Khz  KEY1:30Khz", RED);
    
    dac_creat_sin_buf(2048, 100);
    dac_dma_wave_enable(100, 10 - 1, 72 - 1);  /* 100Khz触发频率, 100个点, 得到1Khz的正弦波 */
    
    while (1)
    {
        t++;
        key = key_scan(0);                                  /* 按键扫描 */

        if (key == KEY0_PRES)                               /* 高采样率 */
        {
            dac_creat_sin_buf(2048, 100);
            dac_dma_wave_enable(100, 10 - 1, 24 - 1);       /* 300Khz触发频率, 100个点, 得到最高3KHz的正弦波. */
        }
        else if (key == KEY1_PRES)                          /* 低采样率 */
        {
            dac_creat_sin_buf(2048, 10);
            dac_dma_wave_enable(10, 10 - 1, 24 - 1);        /* 300Khz触发频率, 10个点, 可以得到最高30KHz的正弦波. */
        }

        if (t == 40)        /* 定时时间到了 */
        {
            LED0_TOGGLE();  /* LED0闪烁 */
            t = 0;
        }

        delay_ms(5);
    }
}













