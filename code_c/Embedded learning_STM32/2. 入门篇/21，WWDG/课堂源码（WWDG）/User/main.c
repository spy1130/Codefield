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
#include "./BSP/WDG/wdg.h"


int main(void)
{
    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* ����ʱ��,72M */
    delay_init(72);                             /* ��ʼ����ʱ���� */
    usart_init(115200);                         /* ����������Ϊ115200 */
    led_init();                                 /* ��ʼ��LED */
    
    if(__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET)
    {
        printf("���ڿ��Ź���λ\r\n");
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }
    else
    {
        printf("�ⲿ��λ\r\n");
    }
    
    delay_ms(500);
    printf("���ڴ�������ι��\r\n\r\n");
    wwdg_init(0x7f, 0x5f, WWDG_PRESCALER_8);
    
    while(1)
    {
        delay_ms(90);
        HAL_WWDG_Refresh(&g_wwdg_handle);
        LED0_TOGGLE();
    }
}

