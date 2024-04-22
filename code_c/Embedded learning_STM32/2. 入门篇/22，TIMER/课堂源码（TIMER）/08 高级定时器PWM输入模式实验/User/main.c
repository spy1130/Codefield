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
#include "./BSP/TIMER/atim.h"
#include "./BSP/TIMER/gtim.h"


extern uint16_t g_timxchy_pwmin_sta;    /* PWM����״̬ */
extern uint16_t g_timxchy_pwmin_psc;    /* PWM�����Ƶϵ�� */
extern uint32_t g_timxchy_pwmin_hval;   /* PWM�ĸߵ�ƽ���� */
extern uint32_t g_timxchy_pwmin_cval;   /* PWM�����ڿ�� */

int main(void)
{
    uint8_t t = 0;
    double ht, ct, f, tpsc;

    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* ����ʱ��, 72Mhz */
    delay_init(72);                             /* ��ʱ��ʼ�� */
    usart_init(115200);                         /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                                 /* ��ʼ��LED */
    gtim_timx_pwm_chy_init(10 - 1, 72 - 1);
    
    TIM3->CCR2 = 3;
    
    atim_timx_pwmin_chy_init();
    
    while (1)
    {
        delay_ms(10);
        t++;

        if (t >= 20)    /* ÿ200ms���һ�ν��,����˸LED0,��ʾ�������� */
        {
            if (g_timxchy_pwmin_sta)    /* ������һ������ */
            {
                printf("\r\n");                                     /* �����,����һ�� */
                printf("PWM PSC  :%d\r\n", g_timxchy_pwmin_psc);    /* ��ӡ��Ƶϵ�� */
                printf("PWM Hight:%d\r\n", g_timxchy_pwmin_hval);   /* ��ӡ�ߵ�ƽ���� */
                printf("PWM Cycle:%d\r\n", g_timxchy_pwmin_cval);   /* ��ӡ���� */
                tpsc = ((double)g_timxchy_pwmin_psc + 1) / 72;      /* �õ�PWM����ʱ������ʱ�� */ 
                ht = g_timxchy_pwmin_hval * tpsc;                   /* ����ߵ�ƽʱ�� */
                ct = g_timxchy_pwmin_cval * tpsc;                   /* �������ڳ��� */
                f = (1 / ct) * 1000000;                             /* ����Ƶ�� */
                printf("PWM Hight time:%.3fus\r\n", ht);            /* ��ӡ�ߵ�ƽ������ */
                printf("PWM Cycle time:%.3fus\r\n", ct);            /* ��ӡ����ʱ�䳤�� */
                printf("PWM Frequency :%.3fHz\r\n", f);             /* ��ӡƵ�� */ 
                atim_timx_pwmin_chy_restart(); /* ����PWM������ */
            } 

            LED1_TOGGLE();  /* LED1��GREEN����˸ */
            t = 0;
        }
    }
}





