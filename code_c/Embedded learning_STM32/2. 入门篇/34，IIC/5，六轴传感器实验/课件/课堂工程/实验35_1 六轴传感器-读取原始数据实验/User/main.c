/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-4-20
 * @brief       ���ᴫ����-��ȡԭʼ���� ʵ��
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
#include "./BSP/SH3001/sh3001.h"
#include <math.h>


/**
 * @brief       ��ʾԭʼ����
 * @param       x, y : ����
 * @param       title: ����
 * @param       val  : ֵ
 * @retval      ��
 */
void user_show_mag(uint16_t x, uint16_t y, char *title, int16_t val)
{
    char buf[20];

    sprintf(buf,"%s%d", title, val);                /* ��ʽ����� */
    lcd_fill(x, y, x + 160, y + 16, WHITE);         /* ����ϴ�����(�����ʾ20���ַ�,20*8=160) */
    lcd_show_string(x, y, 160, 16, 16, buf, BLUE);  /* ��ʾ�ַ��� */
}

/**
 * @brief       ��ʾ�¶�
 * @param       x, y : ����
 * @param       temp : �¶�
 * @retval      ��
 */
void user_show_temprate(uint16_t x, uint16_t y, float temp)
{
    char buf[20];

    sprintf(buf,"Temp:%2.1fC", temp);               /* ��ʽ����� */
    lcd_fill(x, y, x + 160, y + 16, WHITE);         /* ����ϴ�����(�����ʾ20���ַ�,20*8=160) */
    lcd_show_string(x, y, 160, 16, 16, buf, BLUE);  /* ��ʾ�ַ��� */
}

int main(void)
{
    uint8_t t = 0;
    float temperature;                          /* �¶�ֵ */
    short acc_data[3];                          /* ���ٶȴ�����ԭʼ���� */
    short gyro_data[3];                         /* ������ԭʼ���� */

    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(360, 25, 2, 8);        /* ����ʱ��,180Mhz */
    delay_init(180);                            /* ��ʱ��ʼ�� */
    usart_init(115200);                         /* ���ڳ�ʼ�� */
    usmart_dev.init(90);                        /* USMART��ʼ�� */
    led_init();                                 /* ��ʼ��LED */
    sdram_init();                               /* ��ʼ��SDRAM */
    lcd_init();                                 /* ��ʼ��LCD */

    while (sh3001_init())                       /* ��ⲻ��SH3001 */
    {
        lcd_show_string(30, 130, 200, 16, 16, "SH3001 Check Failed!", RED);
        delay_ms(500);
        lcd_show_string(30, 130, 200, 16, 16, "Please Check!       ", RED);
        delay_ms(500);
        LED0_TOGGLE();                          /* �����˸ */
    }

    lcd_show_string(30, 50,  200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70,  200, 16, 16, "SH3001 TEST", RED);
    lcd_show_string(30, 90,  200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 130, 200, 16, 16, " Temp :", RED);
    lcd_show_string(30, 150, 200, 16, 16, " ACC_X :", RED);
    lcd_show_string(30, 170, 200, 16, 16, " ACC_Y :", RED);
    lcd_show_string(30, 190, 200, 16, 16, " ACC_Z :", RED);
    lcd_show_string(30, 210, 200, 16, 16, " GYRO_X :", RED);
    lcd_show_string(30, 230, 200, 16, 16, " GYRO_Y :", RED);
    lcd_show_string(30, 250, 200, 16, 16, " GYRO_Z :", RED);

    while (1)
    {
        delay_ms(10);
        t++;

        if (t == 20)    /* 0.2�����Ҹ���һ���¶�/����ԭʼֵ */
        {
            temperature = sh3001_get_temperature();         /* ��ȡ�¶�ֵ */
            user_show_temprate(30, 130, temperature);

            sh3001_get_imu_compdata(acc_data, gyro_data);

            user_show_mag(30, 150, "ACC_X :", acc_data[0]);
            user_show_mag(30, 170, "ACC_Y :", acc_data[1]);
            user_show_mag(30, 190, "ACC_Z :", acc_data[2]);
            
            user_show_mag(30, 210, "GYRO_X:", gyro_data[0]);
            user_show_mag(30, 230, "GYRO_Y:", gyro_data[1]);
            user_show_mag(30, 250, "GYRO_Z:", gyro_data[2]);

            t = 0;
            LED0_TOGGLE();
        }
    }
}


