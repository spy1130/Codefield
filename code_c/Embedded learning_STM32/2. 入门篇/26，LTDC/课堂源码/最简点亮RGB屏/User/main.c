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
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(360, 25, 2, 8);    /* ����ʱ��,180Mhz */
    delay_init(180);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ��ʼ��USART */
    led_init();                             /* ��ʼ��LED */
    key_init();                             /* ��ʼ��KEY */
    sdram_init();                           /* ��ʼ��SDRAM */
    ltdc_init();
    
    while (1)
    {
        LED0_TOGGLE();
        delay_ms(1000);
    }
}

