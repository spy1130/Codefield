/**
 ****************************************************************************************************
 * @file        led.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-12
 * @brief       LED ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32H750������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200312
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/LED/led.h"

/**
 * @brief       ��ʼ��LED���IO��, ��ʹ��ʱ��
 * @param       ��
 * @retval      ��
 */
void led_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    LED0_GPIO_CLK_ENABLE(); /* LED0ʱ��ʹ�� */
    LED1_GPIO_CLK_ENABLE(); /* LED1ʱ��ʹ�� */
    LED2_GPIO_CLK_ENABLE(); /* LED2ʱ��ʹ�� */

    gpio_init_struct.Pin = LED0_GPIO_PIN;                   /* LED0���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;        /* ���� */
    HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��LED0���� */

    gpio_init_struct.Pin = LED1_GPIO_PIN;                   /* LED1���� */
    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��LED1���� */
    
    gpio_init_struct.Pin = LED2_GPIO_PIN;                   /* LED2���� */
    HAL_GPIO_Init(LED2_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��LED2���� */
    
    LED0(1);    /* �ر� LED0 */
    LED1(1);    /* �ر� LED1 */
    LED2(1);    /* �ر� LED2 */
}





