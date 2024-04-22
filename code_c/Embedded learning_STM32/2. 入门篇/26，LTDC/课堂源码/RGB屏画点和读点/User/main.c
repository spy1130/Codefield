/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-4-20
 * @brief       SDRAM ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ������ F429������
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
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SDRAM/sdram.h"
#include "./BSP/LCD/ltdc.h"

int main(void)
{
    uint16_t pix_color = 0;
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(360, 25, 2, 8);    /* ����ʱ��,180Mhz */
    delay_init(180);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ��ʼ��USART */
    led_init();                             /* ��ʼ��LED */
    key_init();                             /* ��ʼ��KEY */
    sdram_init();                           /* ��ʼ��SDRAM */
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

