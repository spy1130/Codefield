/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-23
 * @brief       ��ͨ��ADC�ɼ�(DMA��ȡ) ʵ��
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


#define ADC_DMA_BUF_SIZE        50 * 6      /* ADC DMA�ɼ� BUF��С, Ӧ����ADCͨ������������ */
uint16_t g_adc_dma_buf[ADC_DMA_BUF_SIZE];   /* ADC DMA BUF */

extern uint8_t g_adc_dma_sta;               /* DMA����״̬��־, 0,δ���; 1, ����� */

int main(void)
{
    uint16_t i,j;
    uint16_t adcx;
    uint32_t sum;
    float temp;

    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* ����ʱ��, 72Mhz */
    delay_init(72);                             /* ��ʱ��ʼ�� */
    usart_init(115200);                         /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                                 /* ��ʼ��LED */
    lcd_init();                                 /* ��ʼ��LCD */

    adc_nch_dma_init((uint32_t)&g_adc_dma_buf); /* ��ʼ��ADC DMA�ɼ� */

    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "ADC 6CH DMA TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);

    lcd_show_string(30, 110, 200, 12, 12, "ADC1_CH0_VAL:", BLUE);
    lcd_show_string(30, 122, 200, 12, 12, "ADC1_CH0_VOL:0.000V", BLUE); /* ���ڹ̶�λ����ʾС���� */
    
    lcd_show_string(30, 140, 200, 12, 12, "ADC1_CH1_VAL:", BLUE);
    lcd_show_string(30, 152, 200, 12, 12, "ADC1_CH1_VOL:0.000V", BLUE); /* ���ڹ̶�λ����ʾС���� */

    lcd_show_string(30, 170, 200, 12, 12, "ADC1_CH2_VAL:", BLUE);
    lcd_show_string(30, 182, 200, 12, 12, "ADC1_CH2_VOL:0.000V", BLUE); /* ���ڹ̶�λ����ʾС���� */

    lcd_show_string(30, 200, 200, 12, 12, "ADC1_CH3_VAL:", BLUE);
    lcd_show_string(30, 212, 200, 12, 12, "ADC1_CH3_VOL:0.000V", BLUE); /* ���ڹ̶�λ����ʾС���� */

    lcd_show_string(30, 230, 200, 12, 12, "ADC1_CH4_VAL:", BLUE);
    lcd_show_string(30, 242, 200, 12, 12, "ADC1_CH4_VOL:0.000V", BLUE); /* ���ڹ̶�λ����ʾС���� */

    lcd_show_string(30, 260, 200, 12, 12, "ADC1_CH5_VAL:", BLUE);
    lcd_show_string(30, 272, 200, 12, 12, "ADC1_CH5_VOL:0.000V", BLUE); /* ���ڹ̶�λ����ʾС���� */

    adc_dma_enable(ADC_DMA_BUF_SIZE);   /* ����ADC DMA�ɼ� */

    while (1)
    {
        if (g_adc_dma_sta == 1)
        {
            /* ѭ����ʾͨ��0~ͨ��5�Ľ�� */
            for(j = 0; j < 6; j++)  /* ����6��ͨ�� */
            {
                sum = 0; /* ���� */
                for (i = 0; i < ADC_DMA_BUF_SIZE / 6; i++)  /* ÿ��ͨ���ɼ���50������,����50���ۼ� */
                {
                    sum += g_adc_dma_buf[(6 * i) + j];      /* ��ͬͨ����ת�������ۼ� */
                }
                adcx = sum / (ADC_DMA_BUF_SIZE / 6);        /* ȡƽ��ֵ */
                
                /* ��ʾ��� */
                lcd_show_xnum(108, 110 + (j * 30), adcx, 4, 12, 0, BLUE);   /* ��ʾADC�������ԭʼֵ */

                temp = (float)adcx * (3.3 / 4096);  /* ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111 */
                adcx = temp;                        /* ��ֵ�������ָ�adcx��������ΪadcxΪu16���� */
                lcd_show_xnum(108, 122 + (j * 30), adcx, 1, 12, 0, BLUE);   /* ��ʾ��ѹֵ���������֣�3.1111�Ļ������������ʾ3 */

                temp -= adcx;                       /* ���Ѿ���ʾ����������ȥ��������С�����֣�����3.1111-3=0.1111 */
                temp *= 1000;                       /* С�����ֳ���1000�����磺0.1111��ת��Ϊ111.1���൱�ڱ�����λС���� */
                lcd_show_xnum(120, 122 + (j * 30), temp, 3, 12, 0X80, BLUE);/* ��ʾС�����֣�ǰ��ת��Ϊ��������ʾ����������ʾ�ľ���111. */
            }
 
            g_adc_dma_sta = 0;                      /* ���DMA�ɼ����״̬��־ */
            adc_dma_enable(ADC_DMA_BUF_SIZE);       /* ������һ��ADC DMA�ɼ� */
        }

        LED0_TOGGLE();
        delay_ms(100);
    }
}
























