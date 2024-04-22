/**
 ****************************************************************************************************
 * @file        joypad.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       ��Ϸ�ֱ� ��������
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
 * �޸�˵��
 * V1.0 20200426
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/JOYPAD/joypad.h"

/**
 * @brief       ��ʼ���ֱ��ӿ�
 * @param       ��
 * @retval      ��
 */
void joypad_init(void)
{
    JOYPAD_CLK_GPIO_CLK_ENABLE();  /* CLK   ����IOʱ�ӳ�ʼ�� */
    JOYPAD_LAT_GPIO_CLK_ENABLE();  /* LATCH ����IOʱ�ӳ�ʼ�� */
    JOYPAD_DATA_GPIO_CLK_ENABLE(); /* DATA  ����IOʱ�ӳ�ʼ�� */

    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = JOYPAD_CLK_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(JOYPAD_CLK_GPIO_PORT, &gpio_init_struct); /* JOYPAD_CLK  ����ģʽ���� */

    gpio_init_struct.Pin = JOYPAD_LAT_GPIO_PIN;
    HAL_GPIO_Init(JOYPAD_LAT_GPIO_PORT, &gpio_init_struct); /* JOYPAD_LAT  ����ģʽ���� */

    gpio_init_struct.Pin = JOYPAD_DATA_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(JOYPAD_DATA_GPIO_PORT, &gpio_init_struct); /* JOYPAD_DATA ����ģʽ���� */
}

/**
 * @brief       �ֱ��ӳٺ���
 * @param       t    : Ҫ��ʱ��ʱ��
 * @retval      ��
 */
static void joypad_delay(uint16_t t)
{
    while (t--);
}

/**
 * @brief       ��ȡ�ֱ�����ֵ
 *   @note      FC�ֱ����������ʽ:
 *              ÿ��һ������,���һλ����,���˳��:
 *              A -> B -> SELECT -> START -> UP -> DOWN -> LEFT -> RIGHT.
 *              �ܹ�8λ, ������C��ť���ֱ�, ����C��ʵ�͵��� A + B ͬʱ����.
 *              ������1,�ɿ���0.
 * @param       ��
 * @retval      �������, ��ʽ����:
 *              [7]:��
 *              [6]:��
 *              [5]:��
 *              [4]:��
 *              [3]:Start
 *              [2]:Select
 *              [1]:B
 *              [0]:A
 */
uint8_t joypad_read(void)
{
    volatile uint8_t temp = 0;
    uint8_t t;
    JOYPAD_LAT(1);          /* ���浱ǰ״̬ */
    joypad_delay(80);
    JOYPAD_LAT(0);

    for (t = 0; t < 8; t++) /* ��λ������� */
    {
        temp >>= 1;

        if (JOYPAD_DATA == 0)
        {
            temp |= 0x80;   /* LOAD֮�󣬾͵õ���һ������ */
        }

        JOYPAD_CLK(1);      /* ÿ��һ�����壬�յ�һ������ */
        joypad_delay(80);
        JOYPAD_CLK(0);
        joypad_delay(80);
    }

    return temp;
}
