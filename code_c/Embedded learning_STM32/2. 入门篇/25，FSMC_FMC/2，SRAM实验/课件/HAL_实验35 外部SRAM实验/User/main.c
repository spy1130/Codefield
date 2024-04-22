/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-27
 * @brief       �ⲿSRAM ʵ��
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
#include "./BSP/SRAM/sram.h"


/* ����������, ��ʼ��ַΪ: SRAM_BASE_ADDR */
#if (__ARMCC_VERSION >= 6010050)
uint32_t g_test_buffer[250000] __attribute__((section(".bss.ARM.__at_0x68000000")));
#else
uint32_t g_test_buffer[250000] __attribute__((at(SRAM_BASE_ADDR)));
#endif

/**
 * @brief       �ⲿ�ڴ����(���֧��1M�ֽ��ڴ����)
 * @param       ��
 * @retval      ��
 */
void fsmc_sram_test(uint16_t x, uint16_t y)
{
    uint32_t i = 0;
    uint8_t temp = 0;
    uint8_t sval = 0; /* �ڵ�ַ0���������� */

    lcd_show_string(x, y, 239, y + 16, 16, "Ex Memory Test:   0KB", BLUE);

    /* ÿ��4K�ֽ�,д��һ������,�ܹ�д��256������,�պ���1M�ֽ� */
    for (i = 0; i < 1024 * 1024; i += 4096)
    {
        sram_write(&temp, i, 1);
        temp++;
    }

    /* ���ζ���֮ǰд�������,����У�� */
    for (i = 0; i < 1024 * 1024; i += 4096)
    {
        sram_read(&temp, i, 1);

        if (i == 0)
        {
            sval = temp;
        }
        else if (temp <= sval)
        {
            break; /* �������������һ��Ҫ�ȵ�һ�ζ��������ݴ� */
        }
        lcd_show_xnum(x + 15 * 8, y, (uint16_t)(temp - sval + 1) * 4, 4, 16, 0, BLUE); /* ��ʾ�ڴ����� */
    }
}

int main(void)
{
    uint8_t key;
    uint8_t i = 0;
    uint32_t ts = 0;

    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(72);                /* ��ʼ��USMART */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */
    sram_init();                        /* SRAM��ʼ�� */

    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "SRAM TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:Test Sram", RED);
    lcd_show_string(30, 130, 200, 16, 16, "KEY1:TEST Data", RED);

    for (ts = 0; ts < 250000; ts++)
    {
        g_test_buffer[ts] = ts; /* Ԥ��������� */
    }

    while (1)
    {
        key = key_scan(0); /* ��֧������ */

        if (key == KEY0_PRES)
        {
            fsmc_sram_test(30, 150);    /* ����SRAM���� */
        }
        else if (key == KEY1_PRES)      /* ��ӡԤ��������� */
        {
            for (ts = 0; ts < 250000; ts++)
            {
                lcd_show_xnum(30, 170, g_test_buffer[ts], 6, 16, 0, BLUE); /* ��ʾ�������� */
            }
        }
        else
        {
            delay_ms(10);
        }

        i++;

        if (i == 20)
        {
            i = 0;
            LED0_TOGGLE(); /* LED0��˸ */
        }
    }
}






















