/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       ����� ʵ��
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
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/atim.h"


int main(void)
{
    uint8_t t = 0;

    HAL_Init();                                /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);        /* ����ʱ��, 72Mhz */
    delay_init(72);                            /* ��ʱ��ʼ�� */
    usart_init(115200);                        /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                                /* ��ʼ��LED */
    atim_timx_cplm_pwm_init(1000 - 1, 72 - 1);
    atim_timx_cplm_pwm_set(700 - 1, 100);
    
    while (1)
    {
        delay_ms(10);
        t++;

        if (t >= 20)
        {
            LED0_TOGGLE(); /* LED0(RED)��˸ */
            t = 0;
        }
    }
}





