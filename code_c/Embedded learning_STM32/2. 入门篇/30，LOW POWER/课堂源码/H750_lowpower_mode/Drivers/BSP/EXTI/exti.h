/**
 ****************************************************************************************************
 * @file        exti.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-13
 * @brief       �ⲿ�ж� ��������
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
 * V1.0 20200313
 * ��һ�η���
 *
 ****************************************************************************************************
 */


#ifndef __EXTI_H
#define __EXTI_H

#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* ���� �� �жϱ�� & �жϷ����� ���� */ 

#define KEY0_INT_GPIO_PORT              GPIOA
#define KEY0_INT_GPIO_PIN               GPIO_PIN_1
#define KEY0_INT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */
#define KEY0_INT_IRQn                   EXTI1_IRQn
#define KEY0_INT_IRQHandler             EXTI1_IRQHandler

#define KEY1_INT_GPIO_PORT              GPIOA
#define KEY1_INT_GPIO_PIN               GPIO_PIN_15
#define KEY1_INT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */
#define KEY1_INT_IRQn                   EXTI15_10_IRQn
#define KEY1_INT_IRQHandler             EXTI15_10_IRQHandler

#define WKUP_INT_GPIO_PORT              GPIOA
#define WKUP_INT_GPIO_PIN               GPIO_PIN_0
#define WKUP_INT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */
#define WKUP_INT_IRQn                   EXTI0_IRQn
#define WKUP_INT_IRQHandler             EXTI0_IRQHandler

/******************************************************************************************/

void extix_init(void); //�ⲿ�жϳ�ʼ��

#endif

















