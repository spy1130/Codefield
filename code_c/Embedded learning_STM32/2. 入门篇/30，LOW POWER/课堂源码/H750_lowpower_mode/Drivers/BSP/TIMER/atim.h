/**
 ****************************************************************************************************
 * @file        atim.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.2
 * @date        2020-03-19
 * @brief       �߼���ʱ�� ��������
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
 * V1.0 20200319
 * ��һ�η���
 * V1.1 20200320
 * 1, ����atim_timx_cplm_pwm_init����
 * 2, ����atim_timx_cplm_pwm_gpio_init����
 * V1.2 20200321
 * 1, ����atim_timx_pwmin_chy_process����
 * 2, ����atim_timx_pwmin_chy_init����
 * 3, ����atim_timx_pwmin_chy_restart�Ⱥ���
 *
 ****************************************************************************************************
 */

#ifndef __ATIM_H
#define __ATIM_H

#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* �߼���ʱ�� ���� */


/* TIMX ����Ƚ�ģʽ ���� 
 * ����ͨ��TIM1������Ƚ�ģʽ,����PE9,PE11,PE13,PE14���4·PWM,ռ�ձ�50%,����ÿһ·PWM֮�����λ��Ϊ25%
 * �޸�CCRx�����޸���λ.
 * Ĭ�������TIM1/TIM8
 * ע��: ͨ���޸���Щ�궨��,����֧��TIM1~TIM17����һ����ʱ��,����һ��IO��ʹ������Ƚ�ģʽ,���PWM
 */
#define ATIM_TIMX_COMP_CH1_GPIO_PORT            GPIOE
#define ATIM_TIMX_COMP_CH1_GPIO_PIN             GPIO_PIN_9
#define ATIM_TIMX_COMP_CH1_GPIO_AF              GPIO_AF1_TIM1                                 /* AF����ѡ�� */
#define ATIM_TIMX_COMP_CH1_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE��ʱ��ʹ�� */

#define ATIM_TIMX_COMP_CH2_GPIO_PORT            GPIOE
#define ATIM_TIMX_COMP_CH2_GPIO_PIN             GPIO_PIN_11
#define ATIM_TIMX_COMP_CH2_GPIO_AF              GPIO_AF1_TIM1                                 /* AF����ѡ�� */
#define ATIM_TIMX_COMP_CH2_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE��ʱ��ʹ�� */

#define ATIM_TIMX_COMP_CH3_GPIO_PORT            GPIOE
#define ATIM_TIMX_COMP_CH3_GPIO_PIN             GPIO_PIN_13
#define ATIM_TIMX_COMP_CH3_GPIO_AF              GPIO_AF1_TIM1                                 /* AF����ѡ�� */
#define ATIM_TIMX_COMP_CH3_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE��ʱ��ʹ�� */

#define ATIM_TIMX_COMP_CH4_GPIO_PORT            GPIOE
#define ATIM_TIMX_COMP_CH4_GPIO_PIN             GPIO_PIN_14
#define ATIM_TIMX_COMP_CH4_GPIO_AF              GPIO_AF1_TIM1                                 /* AF����ѡ�� */
#define ATIM_TIMX_COMP_CH4_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE��ʱ��ʹ�� */


#define ATIM_TIMX_COMP                          TIM1            
#define ATIM_TIMX_COMP_CH1_CCRX                 TIM_CHANNEL_1        /* ͨ��1������ȽϼĴ��� */
#define ATIM_TIMX_COMP_CH2_CCRX                 TIM_CHANNEL_2        /* ͨ��2������ȽϼĴ��� */
#define ATIM_TIMX_COMP_CH3_CCRX                 TIM_CHANNEL_3        /* ͨ��3������ȽϼĴ��� */
#define ATIM_TIMX_COMP_CH4_CCRX                 TIM_CHANNEL_4        /* ͨ��4������ȽϼĴ��� */
#define ATIM_TIMX_COMP_CLK_ENABLE()             do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)   /* TIM1 ʱ��ʹ�� */




/* TIMX �������ģʽ ���� 
 * �������û���������Ӳ������, CHY���������, CHYN���������
 * �޸�CCRx�����޸�ռ�ձ�.
 * Ĭ�������TIM1/TIM8
 * ע��: ͨ���޸���Щ�궨��,����֧��TIM1~TIM17����һ����ʱ��,����һ��IO��(ǰ���Ǹö�ʱ�������л����������)
 */

/* ���ͨ������ */
#define ATIM_TIMX_CPLM_CHY_GPIO_PORT            GPIOE
#define ATIM_TIMX_CPLM_CHY_GPIO_PIN             GPIO_PIN_9
#define ATIM_TIMX_CPLM_CHY_GPIO_AF              GPIO_AF1_TIM1                          /* AF����ѡ�� */
#define ATIM_TIMX_CPLM_CHY_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE��ʱ��ʹ�� */

/* �������ͨ������ */
#define ATIM_TIMX_CPLM_CHYN_GPIO_PORT           GPIOE
#define ATIM_TIMX_CPLM_CHYN_GPIO_PIN            GPIO_PIN_8
#define ATIM_TIMX_CPLM_CHYN_GPIO_AF             GPIO_AF1_TIM1                           /* AF����ѡ�� */
#define ATIM_TIMX_CPLM_CHYN_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE��ʱ��ʹ�� */

/* ɲ���������� */
#define ATIM_TIMX_CPLM_BKIN_GPIO_PORT           GPIOE
#define ATIM_TIMX_CPLM_BKIN_GPIO_PIN            GPIO_PIN_15
#define ATIM_TIMX_CPLM_BKIN_GPIO_AF             GPIO_AF1_TIM1                           /* AF����ѡ�� */
#define ATIM_TIMX_CPLM_BKIN_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE��ʱ��ʹ�� */

/* �������ʹ�õĶ�ʱ�� */
#define ATIM_TIMX_CPLM                          TIM1            
#define ATIM_TIMX_CPLM_CHY                      GPIO_AF1_TIM1           
#define ATIM_TIMX_CPLM_CHY_CCRY                 TIM_CHANNEL_1        
#define ATIM_TIMX_CPLM_CLK_ENABLE()             do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)   /* TIM1 ʱ��ʹ�� */




/* TIMX PWM����ģʽ ���� 
 * ��������벶��ʹ�ö�ʱ��TIM1_CH1,����WK_UP����������
 * Ĭ�������TIM1/TIM8�ȸ߼���ʱ�� 
 * ע��: ͨ���޸���9���궨��,����֧��TIM2~TIM5,TIM12/TIM15����һ����ʱ����ͨ��1/ͨ��2
 *       �ر�Ҫע��:���������IO, ͨ��ѡ��, AF���ܵ�Ҳ�ø�!
 */
#define ATIM_TIMX_PWMIN_CHY_GPIO_PORT           GPIOE
#define ATIM_TIMX_PWMIN_CHY_GPIO_PIN            GPIO_PIN_9
#define ATIM_TIMX_PWMIN_CHY_GPIO_AF             GPIO_AF1_TIM1                           /* AF����ѡ�� */
#define ATIM_TIMX_PWMIN_CHY_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE��ʱ��ʹ�� */

#define ATIM_TIMX_PWMIN                         TIM1                       
#define ATIM_TIMX_PWMIN_IRQn                    TIM1_UP_IRQn
#define ATIM_TIMX_PWMIN_IRQHandler              TIM1_UP_IRQHandler
#define ATIM_TIMX_PWMIN_CHY                     TIM_CHANNEL_1                           /* ͨ��Y,  1<= Y <=2*/ 
#define ATIM_TIMX_PWMIN_CHY_CLK_ENABLE()        do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)   /* TIMX ʱ��ʹ�� */

 /* TIM1 / TIM8 �ж����Ĳ����жϷ�����,��Ҫ��������,����TIM2~5/TIM12/TIM15��,����Ҫ���¶��� */
#define ATIM_TIMX_PWMIN_CC_IRQn                 TIM1_CC_IRQn
#define ATIM_TIMX_PWMIN_CC_IRQHandler           TIM1_CC_IRQHandler

/******************************************************************************************/


void atim_timx_comp_pwm_init(uint16_t arr, uint16_t psc);   /* �߼���ʱ�� ����Ƚ�ģʽ���PWM ��ʼ������ */
void atim_timx_comp_pwm_gpio_init(void);                    /* ��ʱ������Ƚ�ģʽͨ�����ų�ʼ������ */

void atim_timx_cplm_pwm_init(uint16_t arr, uint16_t psc);   /* �߼���ʱ�� ������� ��ʼ������ */
void atim_timx_cplm_pwm_gpio_init(void);                    /* ��ʱ���������ģʽͨ�����ų�ʼ������ */

void atim_timx_pwmin_chy_init(void);                        /* �߼���ʱ�� PWM����ģʽ��ʼ�� */
void atim_timx_pwmin_chy_restart(void);                     /* �߼���ʱ�� ����PWM����ģʽ��� */
void atim_timx_pwmin_chy_gpio_init(void);                   /* ��ʱ��PWM����ģʽͨ�����ų�ʼ������ */

#endif

















