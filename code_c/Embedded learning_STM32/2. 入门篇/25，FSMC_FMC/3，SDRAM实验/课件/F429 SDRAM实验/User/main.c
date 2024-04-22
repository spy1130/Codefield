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


#if !(__ARMCC_VERSION >= 6010050)   /* ����AC6����������ʹ��AC5������ʱ */

uint16_t testsdram[250000] __attribute__((at(0XC0000000)));   /* ���������� */

#else      /* ʹ��AC6������ʱ */

uint16_t testsdram[250000] __attribute__((section(".bss.ARM.__at_0xC0000000")));

#endif

/**
 * @brief       SDRAM�ڴ����
 * @param       x,y:�������
 * @retval      ��
*/
void fmc_sdram_test(uint16_t x,uint16_t y)
{  
    uint32_t i = 0;
    uint32_t temp = 0;
    uint32_t sval = 0;    /* �ڵ�ַ0���������� */
    lcd_show_string(x, y, 180, y + 16, 16, "Ex Memory Test:    0KB ", RED);

    /* ÿ��16K�ֽ�,д��һ������,�ܹ�д��2048������,�պ���32M�ֽ� */
    for (i = 0; i < 32 * 1024 * 1024; i += 16 * 1024)
    {
        *(__IO uint32_t*)(Bank5_SDRAM_ADDR + i) = temp; 
        temp++;
    }

    /* ���ζ���֮ǰд�������,����У�� */
    for (i = 0; i < 32 * 1024 * 1024; i += 16 * 1024) 
    {
        temp = *(__IO uint32_t*)(Bank5_SDRAM_ADDR + i);

        if (i == 0)
        {
            sval = temp;
        }
        else if (temp <= sval)
        {
            break;     /* �������������һ��Ҫ�ȵ�һ�ζ��������ݴ� */
        }
        
        lcd_show_num(x + 15 * 8, y, (uint16_t)(temp - sval + 1) * 16, 5, 16, 0);    /* ��ʾ�ڴ����� */
        printf("SDRAM Capacity:%dKB\r\n",(uint16_t)(temp-sval + 1) * 16);           /* ��ӡSDRAM���� */
    }
}

int main(void)
{
    uint8_t key;
    uint8_t i = 0;
    uint32_t ts = 0;

    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(360, 25, 2, 8);    /* ����ʱ��,180Mhz */
    delay_init(180);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ��ʼ��USART */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ��KEY */
    sdram_init();                           /* ��ʼ��SDRAM */

    lcd_show_string(30, 50, 200, 16, 16,"STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16,"SDRAM TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16,"ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16,"KEY0:Test Sram", RED);
    lcd_show_string(30, 130, 200, 16, 16,"KEY1:TEST Data", RED);

    for(ts = 0; ts < 250000; ts++)
    {
        testsdram[ts]= ts;       /* Ԥ��������� */
    }
    
    while (1)
    {
        key = key_scan(0);      /* ��֧������ */

        if (key == KEY0_PRES)
        {
            fmc_sdram_test(30, 170);   /* ����SDRAM���� */
        }
        else if(key == KEY1_PRES)       /* ��ӡԤ��������� */
        {
            for(ts = 0; ts < 250000; ts++)
            {
                lcd_show_num(30, 190,testsdram[ts], 6, 16, RED);     /* ��ʾ�������� */
                printf("testsram[%d]:%d\r\n", ts, testsdram[ts]);
            }
        }
        else 
        {
            delay_ms(10);
        }
        
        i++;

        if (i == 20)
            i = 0;

        LED0_TOGGLE();       /* DS0��˸ */
    }
}

