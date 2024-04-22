/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-07-18
 * @brief       ��ͨ��ADC�ɼ� ʵ��
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
#include "./BSP/ADC/adc.h"


int main(void)
{
    uint16_t adcx;
    float temp;
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* ����ʱ��, 72Mhz */
    delay_init(72);                         /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    adc_init();                             /* ��ʼ��ADC */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "ADC TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "ADC1_CH1_VAL:", BLUE);
    lcd_show_string(30, 130, 200, 16, 16, "ADC1_CH1_VOL:0.000V", BLUE); /* ���ڹ̶�λ����ʾС���� */

    while (1)
    {
        adcx = adc_get_result();
        lcd_show_xnum(134, 110, adcx, 5, 16, 0, BLUE);  /* ��ʾADCC�������ԭʼֵ */
 
        temp = (float)adcx * (3.3 / 4096);              /* ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111 */
        adcx = temp;                                    /* ��ֵ�������ָ�adcx��������ΪadcxΪu16���� */
        lcd_show_xnum(134, 130, adcx, 1, 16, 0, BLUE);  /* ��ʾ��ѹֵ���������֣�3.1111�Ļ������������ʾ3 */

        temp -= adcx;                                   /* ���Ѿ���ʾ����������ȥ��������С�����֣�����3.1111-3=0.1111 */
        temp *= 1000;                                   /* С�����ֳ���1000�����磺0.1111��ת��Ϊ111.1���൱�ڱ�����λС���� */
        lcd_show_xnum(150, 130, temp, 3, 16, 0X80, BLUE);/* ��ʾС�����֣�ǰ��ת��Ϊ��������ʾ����������ʾ�ľ���111. */

        LED0_TOGGLE();
        delay_ms(100);
    }
}

















