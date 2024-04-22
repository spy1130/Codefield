/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-24
 * @brief       SPI ʵ��
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
#include "./BSP/NORFLASH/norflash.h"

/* Ҫд�뵽FLASH���ַ������� */
const uint8_t g_text_buf[] = {"STM32 SPI TEST"};

#define TEXT_SIZE sizeof(g_text_buf) /* TEXT�ַ������� */

int main(void)
{
    uint8_t key;
    uint16_t i = 0;
    uint8_t datatemp[TEXT_SIZE];
    uint32_t flashsize;
    uint16_t id = 0;

    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(72);                /* ��ʼ��USMART */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */
    norflash_init();                    /* ��ʼ��NORFLASH */

    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "SPI TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY1:Write  KEY0:Read", RED); /* ��ʾ��ʾ��Ϣ */

    id = norflash_read_id(); /* ��ȡFLASH ID */

    while ((id == 0) || (id == 0XFFFF)) /* ��ⲻ��FLASHоƬ */
    {
        lcd_show_string(30, 130, 200, 16, 16, "FLASH Check Failed!", RED);
        delay_ms(500);
        lcd_show_string(30, 130, 200, 16, 16, "Please Check!      ", RED);
        delay_ms(500);
        LED0_TOGGLE(); /* LED0��˸ */
    }

    lcd_show_string(30, 130, 200, 16, 16, "SPI FLASH Ready!", BLUE);
    flashsize = 16 * 1024 * 1024; /* FLASH ��СΪ16M�ֽ� */

    while (1)
    {
        key = key_scan(0);

        if (key == KEY1_PRES) /* KEY1����,д�� */
        {
            lcd_fill(0, 150, 239, 319, WHITE); /* ������� */
            lcd_show_string(30, 150, 200, 16, 16, "Start Write FLASH....", BLUE);
            sprintf((char *)datatemp, "%s%d", (char *)g_text_buf, i);
            norflash_write((uint8_t *)datatemp, flashsize - 100, TEXT_SIZE);      /* �ӵ�����100����ַ����ʼ,д��SIZE���ȵ����� */
            lcd_show_string(30, 150, 200, 16, 16, "FLASH Write Finished!", BLUE); /* ��ʾ������� */
        }

        if (key == KEY0_PRES) /* KEY0����,��ȡ�ַ�������ʾ */
        {
            lcd_show_string(30, 150, 200, 16, 16, "Start Read FLASH... . ", BLUE);
            norflash_read(datatemp, flashsize - 100, TEXT_SIZE);                   /* �ӵ�����100����ַ����ʼ,����SIZE���ֽ� */
            lcd_show_string(30, 150, 200, 16, 16, "The Data Readed Is:   ", BLUE); /* ��ʾ������� */
            lcd_show_string(30, 170, 200, 16, 16, (char *)datatemp, BLUE);         /* ��ʾ�������ַ��� */
        }

        i++;

        if (i == 20)
        {
            LED0_TOGGLE(); /* LED0��˸ */
            i = 0;
        }

        delay_ms(10);
    }
}
