/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       OV7725����ͷ ʵ��
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
#include "./BSP/TPAD/tpad.h"
#include "./BSP/EXTI/exti.h"
#include "./BSP/TIMER/btim.h"
#include "./BSP/OV7725/ov7725.h"

extern uint8_t g_ov7725_vsta;  /* ��exit.c�� �涨�� */
extern uint8_t g_ov7725_frame; /* ��timer.c���涨�� */

/* 
 * ������OV7725ģ���FIFO���������޷�����һ֡640*480�Ļ�����������QVGA��VGAģʽ������
 * 320*240�ķֱ���,VGAģʽ��֧�����ţ����Կ���ʹ�ø�С�ķֱ�������ʾ�����߿��Ը�����Ҫ�����޸�
 */
uint16_t g_ov7725_wwidth = 320;  /* Ĭ�ϴ��ڿ��Ϊ320 */
uint16_t g_ov7725_wheight = 240; /* Ĭ�ϴ��ڸ߶�Ϊ240 */

const char *LMODE_TBL[6] = {"Auto", "Sunny", "Cloudy", "Office", "Home", "Night"};                     /* 6�ֹ���ģʽ */
const char *EFFECTS_TBL[7] = {"Normal", "Negative", "B&W", "Redish", "Greenish", "Bluish", "Antique"}; /* 7����Ч */

/**
 * @brief       ����LCD��ʾ
 *   @note      �ú�����OV7725ģ��FIFO��������ݿ�����LCD��Ļ��
 * @param       ��
 * @retval      ��
 */
void ov7725_camera_refresh(void)
{
    uint32_t i, j;
    uint16_t color;

    if (g_ov7725_vsta) /* ��֡�жϸ��� */
    {
        lcd_scan_dir(U2D_L2R);  /* ���ϵ���, ������ */

        lcd_set_window((lcddev.width - g_ov7725_wwidth) / 2, (lcddev.height - g_ov7725_wheight) / 2,
                       g_ov7725_wwidth, g_ov7725_wheight); /* ����ʾ�������õ���Ļ���� */

        lcd_write_ram_prepare(); /* ��ʼд��GRAM */
        
        OV7725_RRST(0); /* ��ʼ��λ��ָ�� */
        OV7725_RCLK(0);
        OV7725_RCLK(1);
        OV7725_RCLK(0);
        OV7725_RRST(1); /* ��λ��ָ����� */
        OV7725_RCLK(1);

        for (i = 0; i < g_ov7725_wheight; i++)
        {
            for (j = 0; j < g_ov7725_wwidth; j++)
            {
                OV7725_RCLK(0);
                color = OV7725_DATA;    /* ������ */
                OV7725_RCLK(1);
                color <<= 8;
                OV7725_RCLK(0);
                color |= OV7725_DATA;   /* ������ */
                OV7725_RCLK(1);
                LCD->LCD_RAM = color;
            }
        }

        g_ov7725_vsta = 0;          /* ����֡�жϱ�� */
        g_ov7725_frame++;
        lcd_scan_dir(DFT_SCAN_DIR); /* �ָ�Ĭ��ɨ�跽�� */
    }
}

int main(void)
{
    uint8_t key;
    uint8_t i = 0;
    char msgbuf[15]; /* ��Ϣ������ */
    uint8_t tm = 0;
    uint8_t lightmode = 0, effect = 0;
    uint8_t saturation = 4, brightness = 4, contrast = 4;

    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(72);                /* ��ʼ��USMART */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */
    tpad_init(6);                       /* TPAD ��ʼ�� */

    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "OV7725 TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:Light Mode", RED);
    lcd_show_string(30, 130, 200, 16, 16, "KEY1:Saturation", RED);
    lcd_show_string(30, 150, 200, 16, 16, "KEY2:Brightness", RED);
    lcd_show_string(30, 170, 200, 16, 16, "KEY_UP:Contrast", RED);
    lcd_show_string(30, 190, 200, 16, 16, "TPAD:Effects", RED);
    lcd_show_string(30, 210, 200, 16, 16, "OV7725 Init...", RED);

    while (1) /* ��ʼ��OV7725 */
    {
        if (ov7725_init() == 0)
        {
            lcd_show_string(30, 210, 200, 16, 16, "OV7725 Init OK       ", RED);

            while (1)
            {
                key = key_scan(0);

                if (key == KEY0_PRES)
                {
                    g_ov7725_wwidth = 320;                                  /* Ĭ�ϴ��ڿ��Ϊ320 */
                    g_ov7725_wheight = 240;                                 /* Ĭ�ϴ��ڸ߶�Ϊ240 */
                    ov7725_window_set(g_ov7725_wwidth, g_ov7725_wheight, 0);/* QVGAģʽ��� */
                    break;
                }
                else if (key == KEY1_PRES)
                {
                    g_ov7725_wwidth = 320;                                  /* Ĭ�ϴ��ڿ��Ϊ320 */
                    g_ov7725_wheight = 240;                                 /* Ĭ�ϴ��ڸ߶�Ϊ240 */
                    ov7725_window_set(g_ov7725_wwidth, g_ov7725_wheight, 1);/* VGAģʽ��� */
                    break;
                }

                i++;

                if (i == 100)
                    lcd_show_string(30, 230, 210, 16, 16, "KEY0:QVGA  KEY1:VGA", RED); /* ��˸��ʾ��ʾ��Ϣ */

                if (i == 200)
                {
                    lcd_fill(30, 230, 210, 250 + 16, WHITE);
                    i = 0;
                }

                delay_ms(5);
            }

            ov7725_light_mode(lightmode);
            ov7725_color_saturation(saturation);
            ov7725_brightness(brightness);
            ov7725_contrast(contrast);
            ov7725_special_effects(effect);

            OV7725_OE(0); /* ʹ��OV7725 FIFO������� */

            break;
        }
        else
        {
            lcd_show_string(30, 190, 200, 16, 16, "OV7725 Error!!", RED);
            delay_ms(200);
            lcd_fill(30, 190, 239, 246, WHITE);
            delay_ms(200);
        }
    }

    btim_timx_int_init(10000, 7200 - 1); /* 10Khz����Ƶ��,1�����ж� */
    exti_ov7725_vsync_init();            /* ʹ��OV7725 VSYNC�ⲿ�ж�, ����֡�ж� */
    lcd_clear(BLACK);

    while (1)
    {
        key = key_scan(0); /* ��֧������ */

        if (key)
        {
            tm = 20;

            switch (key)
            {
                case KEY0_PRES: /* �ƹ�ģʽLight Mode */
                    lightmode++;

                    if (lightmode > 5)
                        lightmode = 0;

                    ov7725_light_mode(lightmode);
                    sprintf((char *)msgbuf, "%s", LMODE_TBL[lightmode]);
                    break;

                case KEY1_PRES: /* ���Ͷ�Saturation */
                    saturation++;

                    if (saturation > 8)
                        saturation = 0;

                    ov7725_color_saturation(saturation);
                    sprintf((char *)msgbuf, "Saturation:%d", saturation);
                    break;

                case KEY2_PRES: /* ����brightness */
                    brightness++;

                    if (brightness > 8)
                        brightness = 0;

                    ov7725_brightness(brightness);
                    sprintf((char *)msgbuf, "Brightness:%d", brightness);
                    break;

                case WKUP_PRES: /* �Աȶ�Contrast */
                    contrast++;

                    if (contrast > 8)
                        contrast = 0;

                    ov7725_contrast(contrast);
                    sprintf((char *)msgbuf, "Contrast:%d", contrast);
                    break;
                }
        }

        if (tpad_scan(0)) /* ��⵽�������� */
        {
            effect++;

            if (effect > 6)
                effect = 0;

            ov7725_special_effects(effect); /* ������Ч */
            sprintf((char *)msgbuf, "%s", EFFECTS_TBL[effect]);
            tm = 20;
        }

        ov7725_camera_refresh(); /* ������ʾ */

        if (tm)
        {
            lcd_show_string((lcddev.width - 240) / 2 + 30, (lcddev.height - 320) / 2 + 60, 200, 16, 16, msgbuf, BLUE);
            tm--;
        }

        i++;

        if (i >= 15) /* DS0��˸ */
        {
            i = 0;
            LED0_TOGGLE(); /* LED0��˸ */
        }
    }
}
