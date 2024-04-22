/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       Flashģ��EEPROM ʵ��
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
#include "./BSP/STMFLASH/stmflash.h"

/* Ҫд�뵽STM32 FLASH���ַ������� */
const uint8_t g_text_buf[] = {"STM32F103 FLASH TEST"};

#define TEXT_LENTH sizeof(g_text_buf) /* ���鳤�� */

/* SIZE��ʾ���ֳ�(2�ֽ�), ��С������2��������, ������ǵĻ�, ǿ�ƶ��뵽2�������� */
#define SIZE TEXT_LENTH / 2 + ((TEXT_LENTH % 2) ? 1 : 0)

#define FLASH_SAVE_ADDR 0X08070000  /* ����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С + 0X08000000) */

int main(void)
{
    uint8_t key = 0;
    uint16_t i = 0;
    uint8_t datatemp[SIZE];
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* ����ʱ��, 72Mhz */
    delay_init(72);                         /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(72);                    /* ��ʼ��USMART */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ������ */

    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "FLASH EEPROM TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY1:Write  KEY0:Read", RED);

    while (1)
    {
        key = key_scan(0);

        if (key == KEY1_PRES) /* KEY1����,д��STM32 FLASH */
        {
            lcd_fill(0, 150, 239, 319, WHITE);  /* ������� */
            lcd_show_string(30, 150, 200, 16, 16, "Start Write FLASH....", RED);
            stmflash_write(FLASH_SAVE_ADDR, (uint16_t *)g_text_buf, SIZE);
            lcd_show_string(30, 150, 200, 16, 16, "FLASH Write Finished!", RED); /* ��ʾ������� */
        
        }

        if (key == KEY0_PRES) /* KEY0����,��ȡ�ַ�������ʾ */
        {
            lcd_show_string(30, 150, 200, 16, 16, "Start Read FLASH.... ", RED);
            stmflash_read(FLASH_SAVE_ADDR, (uint16_t *)datatemp, SIZE);
            lcd_show_string(30, 150, 200, 16, 16, "The Data Readed Is:  ", RED); /* ��ʾ������� */
            lcd_show_string(30, 170, 200, 16, 16, (char *)datatemp, BLUE);       /* ��ʾ�������ַ��� */
        }

        i++;
        delay_ms(10);

        if (i == 20)
        {
            LED0_TOGGLE(); /* ��ʾϵͳ�������� */
            i = 0;
        }
    }
}
