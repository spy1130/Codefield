/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-24
 * @brief       RS485 ʵ��
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
#include "./BSP/RS485/rs485.h"


int main(void)
{
    uint8_t key;
    uint8_t i = 0, t = 0;
    uint8_t cnt = 0;
    uint8_t rs485buf[5];

    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* ����ʱ��, 72Mhz */
    delay_init(72);                             /* ��ʱ��ʼ�� */
    usart_init(115200);                         /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(72);                        /* ��ʼ��USMART */
    led_init();                                 /* ��ʼ��LED */
    lcd_init();                                 /* ��ʼ��LCD */
    key_init();                                 /* ��ʼ������ */
    rs485_init(9600);                           /* ��ʼ��RS485 */

    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "RS485 TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:Send", RED);    /* ��ʾ��ʾ��Ϣ */

    lcd_show_string(30, 130, 200, 16, 16, "Count:", RED);       /* ��ʾ��ǰ����ֵ */
    lcd_show_string(30, 150, 200, 16, 16, "Send Data:", RED);   /* ��ʾ���͵����� */
    lcd_show_string(30, 190, 200, 16, 16, "Receive Data:", RED);/* ��ʾ���յ������� */

    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)   /* KEY0����,����һ������ */
        {
            for (i = 0; i < 5; i++)
            {
                rs485buf[i] = cnt + i;      /* ��䷢�ͻ����� */
                lcd_show_xnum(30 + i * 32, 170, rs485buf[i], 3, 16, 0X80, BLUE);    /* ��ʾ���� */
            }

            rs485_send_data(rs485buf, 5);   /* ����5���ֽ� */
        }

        rs485_receive_data(rs485buf, &key);

        if (key)    /* ���յ������� */
        {
            if (key > 5) key = 5;    /* �����5������. */

            for (i = 0; i < key; i++)
            {
                lcd_show_xnum(30 + i * 32, 210, rs485buf[i], 3, 16, 0X80, BLUE);    /* ��ʾ���� */
            }
        }

        t++;
        delay_ms(10);

        if (t == 20)
        {
            LED0_TOGGLE();  /* LED0��˸, ��ʾϵͳ�������� */
            t = 0;
            cnt++;
            lcd_show_xnum(30 + 48, 130, cnt, 3, 16, 0X80, BLUE);    /* ��ʾ���� */
        }
    }
}



















