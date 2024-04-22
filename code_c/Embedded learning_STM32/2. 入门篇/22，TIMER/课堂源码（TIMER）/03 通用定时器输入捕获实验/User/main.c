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
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/TIMER/gtim.h"


extern uint8_t  g_timxchy_cap_sta;  /* ���벶��״̬ */
extern uint16_t g_timxchy_cap_val;  /* ���벶��ֵ */

int main(void)
{
    uint32_t temp = 0;
    uint8_t t = 0;

    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* ����ʱ��, 72Mhz */
    delay_init(72);                         /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                             /* ��ʼ��LED */
    gtim_timx_cap_chy_init(0XFFFF, 72 - 1); /* ��1Mhz��Ƶ�ʼ��� ���� */

    while (1)
    {
        if (g_timxchy_cap_sta & 0X80)       /* �ɹ�������һ�θߵ�ƽ */
        {
            temp = g_timxchy_cap_sta & 0X3F;
            temp *= 65536;                  /* ���ʱ���ܺ� */
            temp += g_timxchy_cap_val;      /* �õ��ܵĸߵ�ƽʱ�� */
            printf("HIGH:%d us\r\n", temp); /* ��ӡ�ܵĸߵ�ƽʱ�� */
            g_timxchy_cap_sta = 0;          /* ������һ�β���*/
        }

        t++;

        if (t > 20)         /* 200ms����һ�� */
        {
            t = 0;
            LED0_TOGGLE();  /* LED0��˸ ,��ʾ�������� */
        }
        delay_ms(10);
    }
}


