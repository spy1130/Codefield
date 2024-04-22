/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       ����ͨ�� ʵ��
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
#include "./BSP/NRF24L01/nrf24l01.h"


int main(void)
{
    uint8_t key, mode;
    uint16_t t = 0;
    uint8_t tmp_buf[33];

    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */
    nrf24l01_init();                    /* ��ʼ��NRF24L01 */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "NRF24L01 TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);

    while (nrf24l01_check()) /* ���NRF24L01�Ƿ����� */
    {
        lcd_show_string(30, 110, 200, 16, 16, "NRF24L01 Error", RED);
        delay_ms(200);
        lcd_fill(30, 110, 239, 130 + 16, WHITE);
        delay_ms(200);
    }

    lcd_show_string(30, 110, 200, 16, 16, "NRF24L01 OK", RED);

    while (1) /* �����û�ѡ��ģʽ */
    {
        key = key_scan(0);

        if (key == KEY0_PRES)
        {
            mode = 0; /* ����ģʽ */
            break;
        }
        else if (key == KEY1_PRES)
        {
            mode = 1; /* ����ģʽ */
            break;
        }

        t++;

        if (t == 100) /* ��ʾ��ʾ��Ϣ */
        {
            lcd_show_string(10, 130, 230, 16, 16, "KEY0:RX_Mode  KEY1:TX_Mode", RED);
        }

        if (t == 200) /* �ر���ʾ��Ϣ */
        {
            lcd_fill(10, 130, 230, 150 + 16, WHITE);
            t = 0;
        }

        delay_ms(5);
    }

    lcd_fill(10, 130, 240, 166, WHITE); /* ����������ʾ */

    if (mode == 0) /* RXģʽ */
    {
        lcd_show_string(30, 130, 200, 16, 16, "NRF24L01 RX_Mode", BLUE);
        lcd_show_string(30, 150, 200, 16, 16, "Received DATA:", BLUE);
        nrf24l01_rx_mode(); /* ����RXģʽ */

        while (1)
        {
            if (nrf24l01_rx_packet(tmp_buf) == 0) /* һ�����յ���Ϣ,����ʾ����. */
            {
                tmp_buf[32] = 0; /* �����ַ��������� */
                lcd_show_string(0, 170, lcddev.width - 1, 32, 16, (char *)tmp_buf, BLUE);
            }
            else
                delay_us(100);

            t++;

            if (t == 10000) /* ��Լ1s�Ӹı�һ��״̬ */
            {
                t = 0;
                LED0_TOGGLE();
            }
        }
    }
    else /* TXģʽ */
    {
        lcd_show_string(30, 130, 200, 16, 16, "NRF24L01 TX_Mode", BLUE);
        nrf24l01_tx_mode(); /* ����TXģʽ */
        mode = ' ';         /* �ӿո����ʼ���� */

        while (1)
        {
            /*  !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ */
            if (nrf24l01_tx_packet(tmp_buf) == 0) /* ���ͳɹ� */
            {
                lcd_show_string(30, 150, 239, 32, 16, "Sended DATA:", BLUE);
                lcd_show_string(0, 170, lcddev.width - 1, 32, 16, (char *)tmp_buf, BLUE);
                key = mode;

                for (t = 0; t < 32; t++)
                {
                    key++;

                    if (key > ('~'))
                        key = ' ';

                    tmp_buf[t] = key;
                }

                mode++;

                if (mode > '~')
                    mode = ' ';

                tmp_buf[32] = 0; /* ��������� */
            }
            else
            {
                lcd_fill(0, 150, lcddev.width, 170 + 16 * 3, WHITE); /* �����ʾ */
                lcd_show_string(30, 150, lcddev.width - 1, 32, 16, "Send Failed ", BLUE);
            }

            LED0_TOGGLE();
            delay_ms(200);
        }
    }
}
