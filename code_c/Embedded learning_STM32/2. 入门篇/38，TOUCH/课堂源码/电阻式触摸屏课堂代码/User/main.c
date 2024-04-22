/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-21
 * @brief       TFTLCD(MCU��) ʵ��
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
#include "./BSP/LCD/lcd.h"
#include "./BSP/TOUCH/touch.h"

int main(void)
{   
    uint16_t xval,yval;
    
    HAL_Init();                                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);                 /* ����ʱ��, 72Mhz */
    delay_init(72);                                     /* ��ʱ��ʼ�� */
    usart_init(115200);                                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                                         /* ��ʼ��LED */
    lcd_init();                                         /* ��ʼ��LCD */
    tp_init();
    
    while (1)
    {
        xval = tp_write_and_read_ad(0xD0);
        yval = tp_write_and_read_ad(0x90);
        printf("xval:%d yval:%d\r\n", xval, yval);
        
        delay_ms(200);
    }
}
