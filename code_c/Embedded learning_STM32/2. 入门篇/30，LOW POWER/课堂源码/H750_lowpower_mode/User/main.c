/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-23
 * @brief       ����ģʽ ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� Mini Pro H750������
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
#include "./BSP/MPU/mpu.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/PWR/pwr.h"
#include "./BSP/KEY/key.h"


int main(void)
{
    uint8_t t = 0;
    uint8_t key = 0;

    sys_cache_enable();                 /* ��L1-Cache */
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(240, 2, 2, 4); /* ����ʱ��, 480Mhz */
    delay_init(480);                    /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    mpu_memory_protection();            /* ������ش洢���� */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */
    pwr_wkup_key_init();                /* ���Ѱ�����ʼ�� */
    
    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "STANDBY TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:Enter STOP MODE", RED);
    lcd_show_string(30, 130, 200, 16, 16, "KEY1:Enter STANDBY MODE", RED);
    lcd_show_string(30, 150, 200, 16, 16, "KEY_UP:Exit LOW MODE", RED);
    
    printf("Enter to LowPower Test \r\n");
    
    while (1)
    {
        key = key_scan(0);      /* �õ���ֵ */
        
        if (key)
        {
            switch (key)
            {
                case KEY1_PRES:     /* �������ģʽ */
                    printf("Enter STANDBY Mode \r\n");
                    pwr_enter_standby();
                    /* �Ӵ���ģʽ�����൱��ϵͳ����(��λ), ��˲���ִ�е����� */
                    printf("Exit STANDBY Mode \r\n");
                    break;
                
                case KEY0_PRES:     /* ����ֹͣģʽ */
                    LED1(0);        /* �����̵�,��ʾ����ֹͣģʽ */ 
                    printf("Enter STOP Mode\r\n");
                    pwr_enter_stop();
                    /* ��ֹͣģʽ����, ��Ҫ��������ϵͳʱ��, 480Mhz */
                    sys_stm32_clock_init(240, 2, 2, 4);
                    printf("Exit STOP Mode \r\n");
                    LED1(1);        /* �ر��̵�,��ʾ�˳�ֹͣģʽ */
                    break;
            }
        }

        if ((t % 20) == 0)
        {
            LED0_TOGGLE();         /* ÿ200ms,��תһ��LED0 */
        }

        delay_ms(10);
        t++;
    }
}

















