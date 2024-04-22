/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       DS18B20�����¶ȴ����� ʵ��
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
#include "./USMART/usmart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/DS18B20/ds18b20.h"


int main(void)
{
    uint8_t t = 0;
    short temperature;

    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "DS18B20 TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);

    while (ds18b20_init()) /* DS18B20��ʼ�� */
    {
        lcd_show_string(30, 110, 200, 16, 16, "DS18B20 Error", RED);
        delay_ms(200);
        lcd_fill(30, 110, 239, 130 + 16, WHITE);
        delay_ms(200);
    }

    lcd_show_string(30, 110, 200, 16, 16, "DS18B20 OK", RED);
    lcd_show_string(30, 130, 200, 16, 16, "Temp:   . C", BLUE);

    while (1)
    {
        if (t % 10 == 0) /* ÿ100ms��ȡһ�� */
        {
            temperature = ds18b20_get_temperature();

            if (temperature < 0)
            {
                lcd_show_char(30 + 40, 130, '-', 16, 0, BLUE); /* ��ʾ���� */
                temperature = -temperature;                    /* תΪ���� */
            }
            else
            {
                lcd_show_char(30 + 40, 130, ' ', 16, 0, BLUE); /* ȥ������ */
            }

            lcd_show_num(30 + 40 + 8, 130, temperature / 10, 2, 16, BLUE);  /* ��ʾ�������� */
            lcd_show_num(30 + 40 + 32, 130, temperature % 10, 1, 16, BLUE); /* ��ʾС������ */
        }

        delay_ms(10);
        t++;

        if (t == 20)
        {
            t = 0;
            LED0_TOGGLE(); /* LED0��˸ */
        }
    }
}
