/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-24
 * @brief       DAC��� ʵ��
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
#include "./BSP/ADC/adc.h"
#include "./BSP/DAC/dac.h"
#include "./BSP/KEY/key.h"


int main(void)
{
    uint8_t t = 0; 
    uint8_t key;

    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */
    dac_init();                         /* ��ʼ��DAC1_OUT1ͨ�� */

    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "DAC Triangular WAVE TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:Wave1  KEY1:Wave2", RED);
    lcd_show_string(30, 130, 200, 16, 16, "DAC None", BLUE); /* ��ʾ����� */

    while (1)
    {
        t++;
        key = key_scan(0);                           /* ����ɨ�� */

        if (key == KEY0_PRES)                        /* �߲����� , 100hz���� �� ʵ��ֻ��65.5hz */
        {
            lcd_show_string(30, 130, 200, 16, 16, "DAC Wave1 ", BLUE);
            dac_triangular_wave(4095, 5, 2000, 100); /* ��ֵ4095, ��������5us, 2000��������, 100������ */
            lcd_show_string(30, 130, 200, 16, 16, "DAC None  ", BLUE);
        }
        else if (key == KEY1_PRES)                   /* �Ͳ����� , 100hz���� �� ʵ��99.5hz */
        {
            lcd_show_string(30, 130, 200, 16, 16, "DAC Wave2 ", BLUE);
            dac_triangular_wave(4095, 500, 20, 100); /* ��ֵ4095, ��������500us, 20��������, 100������ */
            lcd_show_string(30, 130, 200, 16, 16, "DAC None  ", BLUE);
        }

        if (t == 10)                                 /* ��ʱʱ�䵽�� */
        {
            LED0_TOGGLE();                           /* LED0��˸ */
            t = 0;
        }

        delay_ms(10);
    }
}











