/**
 ****************************************************************************************************
 * @file        exti.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-19
 * @brief       �ⲿ�ж� ��������
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
 * V1.0 20200420
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/BEEP/beep.h"
#include "./BSP/KEY/key.h"
#include "./BSP/EXTI/exti.h"
#include "./BSP/OV7725/ov7725.h"

/**
 * @brief       KEY0 �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void KEY0_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY0_INT_GPIO_PIN);         /* �����жϴ����ú��� ���KEY0�����ж��� ���жϱ�־λ */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

/**
 * @brief       KEY1 �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void KEY1_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY1_INT_GPIO_PIN);         /* �����жϴ����ú��� ���KEY1�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_INT_GPIO_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

/**
 * @brief       KEY2 �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void KEY2_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY2_INT_GPIO_PIN);        /* �����жϴ����ú��� ���KEY2�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY2_INT_GPIO_PIN);        /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

/**
 * @brief       WK_UP �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void WKUP_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(WKUP_INT_GPIO_PIN);        /* �����жϴ����ú��� ���KEY_UP�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
    __HAL_GPIO_EXTI_CLEAR_IT(WKUP_INT_GPIO_PIN);        /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}

/**
 * @brief       �жϷ����������Ҫ��������
                ��HAL�������е��ⲿ�жϷ�����������ô˺���
 * @param       GPIO_Pin:�ж����ź�
 * @retval      ��
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(20);      /* ���� */

    switch (GPIO_Pin)
    {
        case KEY0_INT_GPIO_PIN:
            if (KEY0 == 0)
            {
                LED0_TOGGLE();  /* LED0 ״̬ȡ�� */
                LED1_TOGGLE();  /* LED1 ״̬ȡ�� */
            }

            break;

        case KEY1_INT_GPIO_PIN:
            if (KEY1 == 0)
            {
                LED0_TOGGLE();  /* LED0 ״̬ȡ�� */
            }

            break;

        case KEY2_INT_GPIO_PIN:
            if (KEY2 == 0)
            {
                LED1_TOGGLE();  /* LED1 ״̬ȡ�� */
            }

            break;

        case WKUP_INT_GPIO_PIN:
            if (WK_UP == 1)
            {
                BEEP_TOGGLE();  /* ������״̬ȡ�� */
            }

            break;
    }
}

/**
 * @brief       �ⲿ�жϳ�ʼ������
 * @param       ��
 * @retval      ��
 */
void extix_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    KEY0_GPIO_CLK_ENABLE();                                  /* KEY0ʱ��ʹ�� */
    KEY1_GPIO_CLK_ENABLE();                                  /* KEY1ʱ��ʹ�� */
    KEY2_GPIO_CLK_ENABLE();                                  /* KEY2ʱ��ʹ�� */
    WKUP_GPIO_CLK_ENABLE();                                  /* WKUPʱ��ʹ�� */

    gpio_init_struct.Pin = KEY0_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �����ش��� */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &gpio_init_struct);    /* KEY0����Ϊ�½��ش����ж� */

    gpio_init_struct.Pin = KEY1_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �����ش��� */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &gpio_init_struct);    /* KEY1����Ϊ�½��ش����ж� */

    gpio_init_struct.Pin = KEY2_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �����ش��� */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY2_INT_GPIO_PORT, &gpio_init_struct);    /* KEY2����Ϊ�½��ش����ж� */

    gpio_init_struct.Pin = WKUP_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;             /* �����ش��� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(WKUP_GPIO_PORT, &gpio_init_struct);        /* WKUP����Ϊ�½��ش����ж� */

    HAL_NVIC_SetPriority(KEY0_INT_IRQn, 0, 2);               /* ��ռ0�������ȼ�2 */
    HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);                       /* ʹ���ж���1 */

    HAL_NVIC_SetPriority(KEY1_INT_IRQn, 1, 2);               /* ��ռ1�������ȼ�2 */
    HAL_NVIC_EnableIRQ(KEY1_INT_IRQn);                       /* ʹ���ж���15 */

    HAL_NVIC_SetPriority(KEY2_INT_IRQn, 2, 2);               /* ��ռ2�������ȼ�2 */
    HAL_NVIC_EnableIRQ(KEY2_INT_IRQn);                       /* ʹ���ж���15 */

    HAL_NVIC_SetPriority(WKUP_INT_IRQn, 3, 2);               /* ��ռ3�������ȼ�2 */
    HAL_NVIC_EnableIRQ(WKUP_INT_IRQn);                       /* ʹ���ж���0 */
}

/* OV7725 ֡�жϱ�־
 * 0, ��ʾ��һ֡�����Ѿ���������, �µ�֡�жϵ���, ������FIFOд����
 * 1, ��ʾ��һ֡���ݻ�û�д�����, �µ�֡�жϵ���, ������FIFOд����
 */
uint8_t g_ov7725_vsta = 0;

/**
 * @brief       OV7725 VSYNC �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void OV7725_VSYNC_INT_IRQHandler(void)
{
    if (__HAL_GPIO_EXTI_GET_IT(OV7725_VSYNC_GPIO_PIN))   /* ��OV7725_VSYNC_GPIO_PIN�ߵ��ж� */
    {
        if (g_ov7725_vsta == 0) /* ��һ֡�����Ѿ�������? */
        {
            OV7725_WRST(0);     /* ��λдָ�� */
            OV7725_WRST(1);     /* ������λ */
            OV7725_WEN(1);      /* ����д��FIFO */
            g_ov7725_vsta = 1;  /* ���֡�ж� */
        }
        else
        {
            OV7725_WEN(0);      /* ��ֹд��FIFO */
        }
    }

    __HAL_GPIO_EXTI_CLEAR_IT(OV7725_VSYNC_GPIO_PIN);   /* ���OV7725_VSYNC_GPIO_PIN�ϵ��жϱ�־λ */
}

/**
 * @brief       OV7725 VSYNC�ⲿ�жϳ�ʼ������
 * @param       ��
 * @retval      ��
 */
void exti_ov7725_vsync_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = OV7725_VSYNC_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;              /* �����ش��� */
    HAL_GPIO_Init(OV7725_VSYNC_GPIO_PORT, &gpio_init_struct); /* ����OV7725��֡�жϴ��� */

    HAL_NVIC_SetPriority(OV7725_VSYNC_INT_IRQn, 0, 0);        /* ��ռ0�������ȼ�0��Hal��Ĭ����2 */
    HAL_NVIC_EnableIRQ(OV7725_VSYNC_INT_IRQn);                /* ʹ���ж���1 */
}









