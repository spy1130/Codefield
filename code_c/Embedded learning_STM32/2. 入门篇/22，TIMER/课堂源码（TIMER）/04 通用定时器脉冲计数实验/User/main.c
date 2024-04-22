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
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/gtim.h"


extern TIM_HandleTypeDef g_timx_cnt_chy_handle;        /* ��ʱ��x��� */

int main(void)
{
    uint16_t curcnt;
    uint16_t oldcnt;
    uint8_t key;
    uint8_t t = 0;
    
    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* ����ʱ��, 72Mhz */
    delay_init(72);                             /* ��ʱ��ʼ�� */
    usart_init(115200);                         /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                                 /* ��ʼ��LED */
    key_init();                                 /* ��ʼ������ */
    gtim_timx_cnt_chy_init(0);

    while (1)
    {
        key = key_scan(0);
        if(key == KEY0_PRES)
        {
            __HAL_TIM_SET_COUNTER(&g_timx_cnt_chy_handle, 0);
        }
        
        curcnt = __HAL_TIM_GET_COUNTER(&g_timx_cnt_chy_handle);
        if(oldcnt != curcnt)
        {
            oldcnt = curcnt;
            printf("CNT:%d\r\n", oldcnt);
        }
        
        t++;
        
        if(t > 20)
        {
            t = 0;
            LED0_TOGGLE();
        }
        delay_ms(10);
    }
}


