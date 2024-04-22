/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       ��Ϸ�ֱ� ʵ��
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
#include "./BSP/JOYPAD/joypad.h"

/* �ֱ��������Ŷ��� */
const char *JOYPAD_SYMBOL_TBL[8] = {"Right", "Left", "Down", "Up", "Start", "Select", "B", "A"};

int main(void)
{
    uint8_t key;
    uint8_t t = 0, i = 0;

    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    joypad_init();                      /* ��Ϸ�ֱ��ӿڳ�ʼ�� */

    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "JOYPAD TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEYVAL:", RED);
    lcd_show_string(30, 130, 200, 16, 16, "SYMBOL:", RED);

    while (1)
    {
        key = joypad_read();

        if (key) /* �ֱ� �а������� */
        {
            lcd_show_num(86, 110, key, 3, 16, BLUE); /* ��ʾ��ֵ */

            for (i = 0; i < 8; i++)
            {
                if (key & (0X80 >> i))
                {
                    lcd_fill(30 + 56, 130, 30 + 56 + 48, 130 + 16, WHITE);                          /* ���֮ǰ����ʾ */
                    lcd_show_string(30 + 56, 130, 200, 16, 16, (char *)JOYPAD_SYMBOL_TBL[i], BLUE); /* ��ʾ���� */
                }
            }
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
