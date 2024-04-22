/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-4-20
 * @brief       IO��չʵ�� ʵ��
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
#include "./USMART/usmart.h"
#include "./BSP/PCF8574/pcf8574.h"

int main(void)
{
    uint16_t i = 0;
    uint8_t key;
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(360, 25, 2, 8);    /* ����ʱ��,180Mhz */
    delay_init(180);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ��ʼ��USART */
    usmart_dev.init(90);                    /* ��ʼ��USMART */
    led_init();                             /* ��ʼ��LED */
    key_init();                             /* ��ʼ������ */
    sdram_init();                           /* ��ʼ��SDRAM */
    lcd_init();                             /* ��ʼ��LCD */
    pcf8574_init();
    
    while (1)
    {
        key = key_scan(0);
        
        if (key == KEY0_PRES)
        {
            pcf8574_write_bit(BEEP_IO, 0);
        }
        
        if (PCF8574_INT == 0)   /* ����IO������ƽ�仯 */
        {
            key = pcf8574_read_bit(EX_IO);
            
            if (key == 0)
            {
                LED1_TOGGLE();
            }
        }
        
        i++;
        delay_ms(10);

        if (i == 20)
        {
            LED0_TOGGLE();          /* �����˸ */
            i = 0;
        }
    }
}

