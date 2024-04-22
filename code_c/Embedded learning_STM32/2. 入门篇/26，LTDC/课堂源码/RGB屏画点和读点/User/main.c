/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-4-20
 * @brief       SDRAM 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 F429开发板
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
#include "./BSP/KEY/key.h"
#include "./BSP/SDRAM/sdram.h"
#include "./BSP/LCD/ltdc.h"

int main(void)
{
    uint16_t pix_color = 0;
    
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(360, 25, 2, 8);    /* 设置时钟,180Mhz */
    delay_init(180);                        /* 延时初始化 */
    usart_init(115200);                     /* 初始化USART */
    led_init();                             /* 初始化LED */
    key_init();                             /* 初始化KEY */
    sdram_init();                           /* 初始化SDRAM */
    ltdc_init();

    ltdc_draw_point (100, 100, RED);
    ltdc_draw_point (110, 110, RED);
    ltdc_draw_point (120, 120, RED);
    ltdc_draw_point (130, 130, RED);
    ltdc_draw_point (140, 140, RED);
    ltdc_draw_point (150, 150, RED);
    ltdc_draw_point (101, 101, RED);
    ltdc_draw_point (111, 111, RED);
    ltdc_draw_point (121, 121, RED);
    ltdc_draw_point (131, 131, RED);
    ltdc_draw_point (141, 141, RED);
    ltdc_draw_point (151, 151, RED);
    
    pix_color = ltdc_read_point (151, 151);
    printf("pix_color:%#x \r\n",pix_color);
    
    lcd_show_char(100, 100, 'A', 16, 1, 0xF81F);

    while (1)
    {
        LED0_TOGGLE();
        delay_ms(1000);
    }
}

