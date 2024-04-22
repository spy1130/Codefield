/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-23
 * @brief       ������ ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
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
#include "./USMART/usmart.h"
#include "./BSP/KEY/key.h"
#include "./BSP/24CXX/24cxx.h"
#include "./BSP/ST480MC/st480mc.h"
#include <math.h>


/* ������ƽ��У׼���� 
 * У׼ԭ����ο�(����2):http://blog.sina.com.cn/s/blog_402c071e0102v8ie.html
 */
int16_t g_magx_offset = 0;      /* x�Ჹ��ֵ */
int16_t g_magy_offset = 0;      /* y�Ჹ��ֵ */

/**
 * @brief       ��ʾ������ԭʼ����
 * @param       x, y : ����
 * @param       title: ����
 * @param       val  : ֵ
 * @retval      ��
 */
void user_show_mag(uint16_t x, uint16_t y, char * title, int16_t val)
{
    char buf[20];

    sprintf(buf,"%s%d", title, val);                /* ��ʽ����� */
    lcd_fill(x, y, x + 160, y + 16, WHITE);         /* ����ϴ�����(�����ʾ20���ַ�,20*8=160) */
    lcd_show_string(x, y, 160, 16, 16, buf, BLUE);  /* ��ʾ�ַ��� */
}

/**
 * @brief       ��ʾ��λ��
 * @param       x, y : ����
 * @param       angle: �Ƕ�
 * @retval      ��
 */
void user_show_angle(uint16_t x, uint16_t y, float angle)
{
    char buf[20];

    sprintf(buf,"Angle:%3.1fC", angle);             /* ��ʽ����� */
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

/**
 * @brief       ����(������)У׼����
 *   @note      ��������ʹ����򵥵�ƽ��У׼����.
 *              ����˺�����,��ˮƽת������������һ��(360��),ת����ɺ�, ��WKUP���˳�!
 * @param       ��
 * @retval      ��
 */
void compass_calibration(void)
{
    int16_t x_min = 0;
    int16_t x_max = 0;
    int16_t y_min = 0;
    int16_t y_max = 0;

    int16_t magx, magy, magz;
    uint8_t res;
    uint8_t key = 0;
    
    lcd_clear(WHITE);
    lcd_show_string(10, 90, 240, 16, 16, "Compass Calibration", RED);
    lcd_show_string(10, 110, 240, 16, 16, "Pls rotate horiz one cycle!", RED);
    lcd_show_string(10, 130, 240, 16, 16, "If done, press WKUP key!", RED);
    
    while (1)
    {
        key = key_scan(0);
        
        if (key == WKUP_PRES)   /* ����У׼ */
        {
            break;
        }
        
        res = st480mc_read_magdata(&magx, &magy, &magz);    /* ��ȡ���� */
        
        if (res == 0)
        {
            x_max = x_max < magx ? magx : x_max;            /* ��¼x���ֵ */
            x_min = x_min > magx ? magx : x_min;            /* ��¼x��Сֵ */
            y_max = y_max < magy ? magy : y_max;            /* ��¼y���ֵ */
            y_min = y_min > magy ? magy : y_min;            /* ��¼y��Сֵ */
        }
        
        LED0_TOGGLE();          /* LED0��˸,��ʾ�������� */
    }
    
    g_magx_offset = (x_max + x_min) / 2;    /* X��ƫ���� */
    g_magy_offset = (y_max + y_min) / 2;    /* Y��ƫ���� */
   
    /* ���ڴ�ӡˮƽУ׼��ز��� */
    printf("x_min:%d\r\n", x_min);
    printf("x_max:%d\r\n", x_max);
    printf("y_min:%d\r\n", y_min);
    printf("y_max:%d\r\n", y_max);
    
    printf("g_magx_offset:%d\r\n", g_magx_offset);
    printf("g_magy_offset:%d\r\n", g_magy_offset);
}

/**
 * @brief       ���̻�ȡ�Ƕ�
 *   @note      ��ȡ��ǰ���̵ĽǶ�(�شŽǶ�)
 * @param       ��
 * @retval      �Ƕ�
 */
float compass_get_angle(void)  
{
    float angle;
    int16_t magx, magy, magz;
    
    st480mc_read_magdata_average(&magx, &magy, &magz, 10);  /* ��ȡԭʼ����, 10��ȡƽ�� */
    
    magx = (magx - g_magx_offset) ;     /* ����У׼����, �����µ���� */
    magy = (magy - g_magy_offset) ;     /* ����У׼����, �����µ���� */

    /* ���ݲ�ͬ���������, ���з�λ�ǻ��� */
    if ((magx > 0) && (magy > 0))
    {
        angle = (atan((double)magy / magx) * 180) / 3.14159f;
    }
    else if ((magx > 0) && (magy < 0))
    {
        angle = 360 + (atan((double)magy / magx) * 180) / 3.14159f;
    }
    else if ((magx == 0) && (magy > 0))
    {
        angle = 90;
    }
    else if ((magx == 0) && (magy < 0))
    {
        angle = 270;
    }
    else if (magx < 0)
    {
        angle = 180 + (atan((double)magy / magx) * 180) / 3.14159f;
    }
    
    if (angle > 360) angle = 360; /* �޶���λ�Ƿ�Χ */
    if (angle < 0) angle = 0;     /* �޶���λ�Ƿ�Χ */

    return angle;
}

int main(void)
{
    uint8_t t = 0;
    uint8_t key;
    float angle;
    float temperature;
    int16_t magx, magy, magz;

    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(84);                    /* ��ʼ��USMART */
    led_init();                             /* ��ʼ��LED */
    key_init();                             /* ��ʼ������ */
    lcd_init();                             /* ��ʼ��LCD */

    while (st480mc_init())    /* ST480MC��ʼ�� */
    {
        lcd_show_string(30, 110, 200, 16, 16, "ST480MC Error", RED);
        delay_ms(200);
        lcd_fill(30, 110, 239, 130 + 16, WHITE);
        delay_ms(200);
    }

RST:
    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "ST480MC TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0 to calibration", RED);
    
    while (1)
    {
        key = key_scan(0);
        
        if (key == KEY0_PRES)       /* KEY0 ���� ,ִ��У׼ */
        {
            compass_calibration();  /* У׼���� */
            lcd_clear(WHITE);       /* ���� */
            goto RST;               /* У׼���,����RST, ������ʾ��ʾ��Ϣ */
        }
        
        delay_ms(10);

        t++;
        
        if (t == 20)     /* 0.2�����Ҹ���һ���¶�/������ԭʼֵ */
        {        
            angle = compass_get_angle();                /* ִ��һ��Լ150ms */
            user_show_angle(30, 130, angle);            /* ��ʾ�Ƕ� */
      
            st480mc_read_temperature(&temperature);     /* ��ȡ��ʪ��ֵ */
            user_show_temprate(30, 150, temperature);   /* ��ʾ�¶� */
            
            st480mc_read_magdata(&magx, &magy, &magz);  /* ��ȡԭʼ���� */
            user_show_mag(30, 170, "MagX:", magx);      /* ��ʾmagx */
            user_show_mag(30, 190, "MagY:", magy);      /* ��ʾmagy */
            user_show_mag(30, 210, "MagZ:", magz);      /* ��ʾmagz */
            
            t = 0;
            LED0_TOGGLE();  /* LED0��˸ */
        }
    }
}








