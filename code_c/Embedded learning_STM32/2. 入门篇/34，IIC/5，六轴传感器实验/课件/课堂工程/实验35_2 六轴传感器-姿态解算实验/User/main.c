/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-4-20
 * @brief       ���ᴫ����-��̬���� ʵ��
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
#include "./BSP/SH3001/sh3001.h"
#include "./BSP/SH3001/imu.h"
#include <math.h>



/**
 * @brief       ��ʾ�Ƕ�
 * @param       x, y : ����
 * @param       title: ����
 * @param       angle: �Ƕ�
 * @retval      ��
 */
void user_show_angle(uint16_t x, uint16_t y, char *title, float angle)
{
    char buf[20];

    sprintf(buf,"%s%3.1fC", title, angle);          /* ��ʽ����� */
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

    sprintf(buf," Temp :%2.1fC", temp);             /* ��ʽ����� */
    lcd_fill(x, y, x + 160, y + 16, WHITE);         /* ����ϴ�����(�����ʾ20���ַ�,20*8=160) */
    lcd_show_string(x, y, 160, 16, 16, buf, BLUE);  /* ��ʾ�ַ��� */
}

/**
 * @brief       ����1����1���ַ�
 * @param       c  : Ҫ���͵��ַ�
 * @retval      �� 
 */
void usart1_send_char(uint8_t c)
{
    while ((USART_UX->SR & 0X40) == 0);            /* �ȴ���һ���ַ�������� */

    USART_UX->DR = (uint8_t)c;                     /* ��Ҫ���͵��ַ� ch д�뵽DR�Ĵ��� */
}

/**
 * @brief       �������ݸ� ANO_TC�����ƴ�����վv4.exe
 * @param       fun  : ������. 0XA0~0XAF
 * @param       data : ���ݻ�����,���28�ֽ�!!
 * @param       len  : data����Ч���ݸ���
 * @retval      �� 
 */
void usart1_niming_report(uint8_t fun, uint8_t *data, uint8_t len)
{
    uint8_t send_buf[32];
    uint8_t i;

    if (len > 28)
    {
        return;    /* ���28�ֽ����� */
    }

    send_buf[len + 4] = 0;  /* У�������� */
    send_buf[0] = 0XAA;     /* ֡ͷ */
    send_buf[1] = 0XAA;     /* ֡ͷ */
    send_buf[2] = fun;      /* ������ */
    send_buf[3] = len;      /* ���ݳ��� */

    for (i = 0; i < len; i++)
    {
        send_buf[4 + i] = data[i];             /* �������� */
    }
    
    for (i = 0; i < len + 4; i++)
    {
        send_buf[len + 4] += send_buf[i];      /* ����У��� */
    }
    
    for (i = 0; i < len + 5; i++)
    {
        usart1_send_char(send_buf[i]);         /* �������ݵ�����1 */
    }
}

/**
 * @brief       ���ͼ��ٶȴ��������ݺ�����������
 * @param       aacx,aacy,aacz    : x,y,z������������ļ��ٶ�ֵ
 * @param       gyrox,gyroy,gyroz : x,y,z�������������������ֵ
 * @retval      �� 
 */
void sh3001_send_data(short aacx, short aacy, short aacz, short gyrox, short gyroy, short gyroz)
{
    uint8_t tbuf[18];
    tbuf[0] = (aacx >> 8) & 0XFF;
    tbuf[1] = aacx & 0XFF;
    tbuf[2] = (aacy >> 8) & 0XFF;
    tbuf[3] = aacy & 0XFF;
    tbuf[4] = (aacz >> 8) & 0XFF;
    tbuf[5] = aacz & 0XFF;
    tbuf[6] = (gyrox >> 8) & 0XFF;
    tbuf[7] = gyrox & 0XFF;
    tbuf[8] = (gyroy >> 8) & 0XFF;
    tbuf[9] = gyroy & 0XFF;
    tbuf[10] = (gyroz >> 8) & 0XFF;
    tbuf[11] = gyroz & 0XFF;
    tbuf[12] = 0;      /* ��Ϊ����MPL��,�޷�ֱ�Ӷ�ȡ����������,��������ֱ�����ε�.��0���. */
    tbuf[13] = 0;
    tbuf[14] = 0;
    tbuf[15] = 0;
    tbuf[16] = 0;
    tbuf[17] = 0;
    usart1_niming_report(0X02, tbuf, 18);             /* �Զ���֡,0X02 */
}

/**
 * @brief       ͨ������1�ϱ���������̬���ݸ�����
 * @param       roll     : �����.��λ0.1�ȡ� -9000 -> 9000 ��Ӧ -90.00  ->  90.00��
 * @param       pitch    : ������.��λ 0.1�ȡ�-18000 -> 18000 ��Ӧ -180.00 -> 180.00 ��
 * @param       yaw      : �����.��λΪ0.1�� -18000 -> 18000  ��Ӧ -180.00 -> 180.00 ��
 * @param       prs      : ��ѹ�Ƹ߶�,��λ:cm
 * @param       fly_mode : ����ģʽ
 * @param       armed    : ����״̬
 * @retval      �� 
 */
void usart1_report_imu(short roll, short pitch, short yaw, int prs, uint8_t fly_mode, uint8_t armed)
{
    uint8_t tbuf[12];
  
    tbuf[0] = (roll >> 8) & 0XFF;
    tbuf[1] = roll & 0XFF;
    tbuf[2] = (pitch >> 8) & 0XFF;
    tbuf[3] = pitch & 0XFF;
    tbuf[4] = (yaw >> 8) & 0XFF;
    tbuf[5] = yaw & 0XFF;
    tbuf[6] = (prs >> 24) & 0XFF;
    tbuf[7] = (prs >> 16) & 0XFF;
    tbuf[8] = (prs >> 8) & 0XFF;
    tbuf[9] = prs & 0XFF;
    tbuf[10] = fly_mode;
    tbuf[11] = armed;
    usart1_niming_report(0X01, tbuf, 12);       /* ״̬֡,0X01 */
}

int main(void)
{
    uint8_t t = 0;
    eulerian_angles_t e_angles;
    float temperature;                          /* �¶�ֵ */
    short acc_data[3];                          /* ���ٶȴ�����ԭʼ���� */
    short gyro_data[3];                         /* ������ԭʼ���� */
    uint8_t key, report = 1;                    /* Ĭ�Ͽ����ϱ� */
    
    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(360, 25, 2, 8);        /* ����ʱ��,180Mhz */
    delay_init(180);                            /* ��ʱ��ʼ�� */
    usart_init(500000);                         /* ���ڳ�ʼ�� */
    usmart_dev.init(90);                        /* USMART��ʼ�� */
    led_init();                                 /* ��ʼ��LED */
    sdram_init();                               /* ��ʼ��SDRAM */
    lcd_init();                                 /* ��ʼ��LCD */
    key_init();                                 /* ��ʼ������ */

    while (sh3001_init())      /* ��ⲻ��SH3001 */
    {
        lcd_show_string(30, 130, 200, 16, 16, "SH3001 Check Failed!", RED);
        delay_ms(500);
        lcd_show_string(30, 130, 200, 16, 16, "Please Check!       ", RED);
        delay_ms(500);
        LED0_TOGGLE();         /* �����˸ */
    }

    imu_init();                /* ��ʼ�� IMU */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "SH3001 TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:UPLOAD ON/OFF", RED);
    
    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)
        {
            report =! report;
            if (report)
            {
                lcd_show_string(30, 210, 200, 16, 16, " UPLOAD ON ", RED);
            }
            else
            {
                lcd_show_string(30, 210, 200, 16, 16, " UPLOAD OFF ", RED);
            }
        }

        t++;

        /* ��ȡ���������� */
        sh3001_get_imu_compdata(acc_data, gyro_data);
        
        /* ����У׼ */
        imu_data_calibration(&gyro_data[0], &gyro_data[1], &gyro_data[2],
                             &acc_data[0],  &acc_data[1],  &acc_data[2]);

        /* ��ȡŷ���� */
        e_angles = imu_get_eulerian_angles(gyro_data[0], gyro_data[1], gyro_data[2],
                                           acc_data[0],  acc_data[1],  acc_data[2]);

        if (report)
        {
            sh3001_send_data(acc_data[0], acc_data[1], acc_data[2], gyro_data[0], gyro_data[1], gyro_data[2]);  /* ���ͼ��ٶ�+������ԭʼ���� */
            usart1_report_imu((int)(e_angles.roll * 100), (int)(e_angles.pitch * 100), (int)(e_angles.yaw * 100), 0, 0, 0);
        }
        
        if ((t % 50) == 0)  /* 0.1�����Ҹ���һ���¶�/ŷ���� */
        {        
            temperature = sh3001_get_temperature();    /* ��ȡ�¶�ֵ */
            user_show_temprate(30, 130, temperature);
            
            user_show_angle(30, 150, "Pitch :", e_angles.pitch);
            user_show_angle(30, 170, " Roll :", e_angles.roll);
            user_show_angle(30, 190, " Yaw  :", e_angles.yaw); 
             
            printf("\r\nPITCH:%f\r\n", e_angles.pitch);
            printf("ROLL:%f\r\n", e_angles.roll);
            printf("YAW:%f\r\n", e_angles.yaw);

            LED0_TOGGLE();
        }
    }
}

