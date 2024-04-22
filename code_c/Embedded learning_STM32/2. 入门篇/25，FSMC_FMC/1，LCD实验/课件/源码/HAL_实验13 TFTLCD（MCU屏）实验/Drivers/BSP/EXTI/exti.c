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
 * V1.0 20200419
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

/**
 * @brief       KEY0 �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void KEY0_INT_IRQHandler(void)
{
    delay_ms(20);                   /* ���� */
    EXTI->PR = KEY0_INT_GPIO_PIN;   /* ���KEY0�����ж��� ���жϱ�־λ */

    if (KEY0 == 0)
    {
        LED0_TOGGLE();              /* LED0 ״̬ȡ�� */ 
        LED1_TOGGLE();              /* LED1 ״̬ȡ�� */ 
    }
}

/**
 * @brief       KEY1 �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void KEY1_INT_IRQHandler(void)
{ 
    delay_ms(20);                   /* ���� */
    EXTI->PR = KEY1_INT_GPIO_PIN;   /* ���KEY1�����ж��� ���жϱ�־λ */

    if (KEY1 == 0)
    {
        LED0_TOGGLE();              /* LED0 ״̬ȡ�� */ 
    }
}

/**
 * @brief       KEY1 �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void KEY2_INT_IRQHandler(void)
{ 
    delay_ms(20);                   /* ���� */
    EXTI->PR = KEY2_INT_GPIO_PIN;   /* ���KEY2�����ж��� ���жϱ�־λ */

    if (KEY2 == 0)
    {
        LED1_TOGGLE();              /* LED1 ״̬ȡ�� */ 
    }
}

/**
 * @brief       WK_UP �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void WKUP_INT_IRQHandler(void)
{ 
    delay_ms(20);                   /* ���� */
    EXTI->PR = WKUP_INT_GPIO_PIN;   /* ���WKUP�����ж��� ���жϱ�־λ */

    if (WK_UP == 1)
    {
        BEEP_TOGGLE();              /* LED2 ״̬ȡ�� */ 
    }
}

/**
 * @brief       �ⲿ�жϳ�ʼ������
 * @param       ��
 * @retval      ��
 */
void extix_init(void)
{
    key_init();
    sys_nvic_ex_config(KEY0_INT_GPIO_PORT, KEY0_INT_GPIO_PIN, SYS_GPIO_FTIR);   /* KEY0����Ϊ�½��ش����ж� */
    sys_nvic_ex_config(KEY1_INT_GPIO_PORT, KEY1_INT_GPIO_PIN, SYS_GPIO_FTIR);   /* KEY1����Ϊ�½��ش����ж� */
    sys_nvic_ex_config(KEY2_INT_GPIO_PORT, KEY2_INT_GPIO_PIN, SYS_GPIO_FTIR);   /* KEY2����Ϊ�½��ش����ж� */
    sys_nvic_ex_config(WKUP_INT_GPIO_PORT, WKUP_INT_GPIO_PIN, SYS_GPIO_RTIR);   /* WKUP����Ϊ�����ش����ж� */

    sys_nvic_init( 0, 2, KEY0_INT_IRQn, 2); /* ��ռ0�������ȼ�2����2 */
    sys_nvic_init( 1, 2, KEY1_INT_IRQn, 2); /* ��ռ1�������ȼ�2����2 */
    sys_nvic_init( 2, 2, KEY2_INT_IRQn, 2); /* ��ռ2�������ȼ�2����2 */
    sys_nvic_init( 3, 2, WKUP_INT_IRQn, 2); /* ��ռ3�������ȼ�2����2 */
}












