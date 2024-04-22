/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-24
 * @brief       DAC������Ҳ� ʵ��
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
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/DAC/dac.h"
#include "./BSP/KEY/key.h"
#include "math.h"


uint16_t g_dac_sin_buf[4096];            /* �������ݻ����� */

/**
 * @brief       �������Ҳ����к���
 *   @note      �豣֤: maxval > samples/2
 * @param       maxval : ���ֵ(0 < maxval < 2048)
 * @param       samples: ������ĸ���
 * @retval      ��
 */
void dac_creat_sin_buf(uint16_t maxval, uint16_t samples)
{
    uint8_t i;
    float outdata = 0;                     /* ��ż����������� */
    float inc = (2 * 3.1415962) / samples; /* ���������������x���� */

    if(maxval <= (samples / 2))return ;	   /* ���ݲ��Ϸ� */

    for (i = 0; i < samples; i++)
    {
        /* 
         * ���Ҳ���������ʽ��y = Asin(��x + �գ�+ b
         * ����ÿ�����yֵ������ֵ�Ŵ�maxval����������������ƫ��maxval����������
         * ע�⣺DAC�޷��������ѹ��������Ҫ����������ƫ��һ����ֵ���������������߶�������������
         */
        outdata = maxval * sin(inc * i) + maxval;
        if (outdata > 4095)
            outdata = 4095; /* �����޶� */
        //printf("%f\r\n",outdata);
        g_dac_sin_buf[i] = outdata;
    }
}

int main(void)
{
    uint8_t t = 0;
    uint8_t key;
    
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */

    dac_dma_wave_init();
    
    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "DAC DMA Sine WAVE TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:3Khz  KEY1:30Khz", RED);
    
    dac_creat_sin_buf(2048, 100);
    dac_dma_wave_enable(100, 10 - 1, 72 - 1);  /* 100Khz����Ƶ��, 100����, �õ�1Khz�����Ҳ� */
    
    while (1)
    {
        t++;
        key = key_scan(0);                                  /* ����ɨ�� */

        if (key == KEY0_PRES)                               /* �߲����� */
        {
            dac_creat_sin_buf(2048, 100);
            dac_dma_wave_enable(100, 10 - 1, 24 - 1);       /* 300Khz����Ƶ��, 100����, �õ����3KHz�����Ҳ�. */
        }
        else if (key == KEY1_PRES)                          /* �Ͳ����� */
        {
            dac_creat_sin_buf(2048, 10);
            dac_dma_wave_enable(10, 10 - 1, 24 - 1);        /* 300Khz����Ƶ��, 10����, ���Եõ����30KHz�����Ҳ�. */
        }

        if (t == 40)        /* ��ʱʱ�䵽�� */
        {
            LED0_TOGGLE();  /* LED0��˸ */
            t = 0;
        }

        delay_ms(5);
    }
}













