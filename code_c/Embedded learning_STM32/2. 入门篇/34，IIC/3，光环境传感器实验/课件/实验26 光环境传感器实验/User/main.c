/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-4-20
 * @brief       APC3216C�⻷�������� ʵ��
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
#include "./BSP/SDRAM/sdram.h"
#include "./USMART/usmart.h"
#include "./BSP/AP3216C/ap3216c.h"

int main(void)
{
    uint16_t ir, als, ps;

    HAL_Init();                                        /* ��ʼ��HAL�� */
    sys_stm32_clock_init(360, 25, 2, 8);               /* ����ʱ��,180Mhz */
    delay_init(180);                                   /* ��ʱ��ʼ�� */
    usart_init(115200);                                /* ��ʼ��USART */
    usmart_dev.init(90);                               /* ��ʼ��USMART */
    led_init();                                        /* ��ʼ��LED */
    sdram_init();                                      /* ��ʼ��SDRAM */
    lcd_init();                                        /* ��ʼ��LCD */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "AP3216C TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);

    while (ap3216c_init())                             /* ��ⲻ��AP3216C */
    {
        lcd_show_string(30, 130, 200, 16, 16, "AP3216C Check Failed!", RED);
        delay_ms(500);
        lcd_show_string(30, 130, 200, 16, 16, "Please Check!      ", RED);
        delay_ms(500);
        LED0_TOGGLE();                                 /* �����˸ */
    }

    lcd_show_string(30, 130, 200, 16, 16, "AP3216C Ready!", RED);
    lcd_show_string(30, 160, 200, 16, 16, " IR:", RED);
    lcd_show_string(30, 180, 200, 16, 16, " PS:", RED);
    lcd_show_string(30, 200, 200, 16, 16, "ALS:", RED);

    while (1)
    {
        ap3216c_read_data(&ir, &ps, &als);             /* ��ȡ����  */
        lcd_show_num(30 + 32, 160, ir, 5, 16, BLUE);   /* ��ʾIR���� */
        lcd_show_num(30 + 32, 180, ps, 5, 16, BLUE);   /* ��ʾPS���� */
        lcd_show_num(30 + 32, 200, als, 5, 16, BLUE);  /* ��ʾALS����  */

        LED0_TOGGLE();                                 /* ��ʾϵͳ�������� */
        delay_ms(120); 
    }
}

