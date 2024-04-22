/**
 ****************************************************************************************************
 * @file        gtim.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.4
 * @date        2020-03-14
 * @brief       ͨ�ö�ʱ�� ��������
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
 * V1.0 20200314
 * ��һ�η���
 * V1.1 20200317
 * ����gtim_timx_pwm_chy_init��gtim_timx_comp_pwm_gpio_init����
 * V1.2 20200317
 * 1,����set_timx_comparey����
 * 2,����get_timx_capture����
 * 3,����gtim_timx_cap_chy_init����
 * 4,����gtim_timx_cap_chy_init����
 * V1.3 20200329
 * 1,֧���ⲿ�����������
 * 2,����gtim_timx_cnt_chy_init����
 * 3,����gtim_timx_cnt_chy_gpio_init����
 * 4,����gtim_timx_cnt_chy_get_count����
 * 5,����gtim_timx_cnt_chy_restart����
 * V1.4 20200329
 * 1,����gtim_timx_npwm_chy_init����
 * 2,����gtim_timx_npwm_chy_gpio_init����
 * 2,����gtim_timx_npwm_chy_set����
 *
 ****************************************************************************************************
 */

#ifndef __GTIM_H
#define __GTIM_H

#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* ͨ�ö�ʱ�� ���� */

/* TIMX �ж϶��� 
 * Ĭ�������TIM2~TIM5, TIM12~TIM17.
 * ע��: ͨ���޸���4���궨��,����֧��TIM1~TIM17����һ����ʱ��.
 */
 
#define GTIM_TIMX_INT                       TIM3
#define GTIM_TIMX_INT_IRQn                  TIM3_IRQn
#define GTIM_TIMX_INT_IRQHandler            TIM3_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 ʱ��ʹ�� */

/******************************************************************************************/

/* TIMX PWM������� 
 * ���������PWM����LED0(RED)������
 * Ĭ�������TIM2~TIM5, TIM12~TIM17.
 * ע��: ͨ���޸���8���궨��,����֧��TIM1~TIM17����һ����ʱ��,����һ��IO�����PWM
 */
/* PB14�����PWM�����߼���ʱ��PWM����ʵ����ź�Դ */
#define GTIM_TIMX_PWM_CHY_GPIO_PORT         GPIOB
#define GTIM_TIMX_PWM_CHY_GPIO_PIN          GPIO_PIN_14
#define GTIM_TIMX_PWM_CHY_GPIO_AF           GPIO_AF2_TIM12                                  /* AF����ѡ�� */
#define GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)     /* PB��ʱ��ʹ�� */

#define GTIM_TIMX_PWM                       TIM12
#define GTIM_TIMX_PWM_CHY                   TIM_CHANNEL_1      /* ͨ��Y,  1<= Y <=4 */
#define GTIM_TIMX_PWM_CHY_CCRX              TIM12->CCR1         /* ͨ��Y������ȽϼĴ��� */
#define GTIM_TIMX_PWM_CHY_CLK_ENABLE()      do{ __HAL_RCC_TIM12_CLK_ENABLE(); }while(0)     /* TIM12 ʱ��ʹ�� */

/* ���������PWM����LED0(REG)������ */
//#define GTIM_TIMX_PWM_CHY_GPIO_PORT         GPIOB
//#define GTIM_TIMX_PWM_CHY_GPIO_PIN          GPIO_PIN_4
//#define GTIM_TIMX_PWM_CHY_GPIO_AF           GPIO_AF2_TIM3                                   /* AF����ѡ�� */
//#define GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)     /* PB��ʱ��ʹ�� */

//#define GTIM_TIMX_PWM                       TIM3 
//#define GTIM_TIMX_PWM_CHY                   TIM_CHANNEL_1      /* ͨ��Y,  1<= Y <=4 */
//#define GTIM_TIMX_PWM_CHY_CCRX              TIM3->CCR1         /* ͨ��Y������ȽϼĴ��� */
//#define GTIM_TIMX_PWM_CHY_CLK_ENABLE()      do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)      /* TIM3 ʱ��ʹ�� */


/* ���������PWM����LED1(GREEN)������ */
//#define GTIM_TIMX_PWM_CHY_GPIO_PORT         GPIOE
//#define GTIM_TIMX_PWM_CHY_GPIO_PIN          GPIO_PIN_6
//#define GTIM_TIMX_PWM_CHY_GPIO_AF           GPIO_AF4_TIM15                                  /* AF����ѡ�� */
//#define GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)     /* PE��ʱ��ʹ�� */

//#define GTIM_TIMX_PWM                       TIM15 
//#define GTIM_TIMX_PWM_CHY                   TIM_CHANNEL_2      /* ͨ��Y,  1<= Y <=4 */
//#define GTIM_TIMX_PWM_CHY_CCRX              TIM15->CCR2        /* ͨ��Y������ȽϼĴ��� */
//#define GTIM_TIMX_PWM_CHY_CLK_ENABLE()      do{ __HAL_RCC_TIM15_CLK_ENABLE(); }while(0)     /* TIM15 ʱ��ʹ�� */


/* ���������PWM����LED2(BLUE)������ */
//#define GTIM_TIMX_PWM_CHY_GPIO_PORT         GPIOE
//#define GTIM_TIMX_PWM_CHY_GPIO_PIN          GPIO_PIN_5
//#define GTIM_TIMX_PWM_CHY_GPIO_AF           GPIO_AF4_TIM15                                  /* AF����ѡ�� */
//#define GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)     /* PE��ʱ��ʹ�� */

//#define GTIM_TIMX_PWM                       TIM15 
//#define GTIM_TIMX_PWM_CHY                   TIM_CHANNEL_1      /* ͨ��Y,  1<= Y <=4 */
//#define GTIM_TIMX_PWM_CHY_CCRX              TIM15->CCR1        /* ͨ��Y������ȽϼĴ��� */
//#define GTIM_TIMX_PWM_CHY_CLK_ENABLE()      do{ __HAL_RCC_TIM15_CLK_ENABLE(); }while(0)     /* TIM15 ʱ��ʹ�� */

/******************************************************************************************/

/* TIMX ���벶���� 
 * ��������벶��ʹ�ö�ʱ��TIM2_CH1,����WK_UP����������
 * Ĭ�������TIM2~TIM5, TIM12~TIM17. 
 * ע��: ͨ���޸���8���궨��,����֧��TIM1~TIM17����һ����ʱ��,����һ��IO�������벶��
 *       �ر�Ҫע��:Ĭ���õ�PA0,���õ�����������!���������IO,��Ӧ����������ʽ/AF���ܵ�Ҳ�ø�!
 */
#define GTIM_TIMX_CAP_CHY_GPIO_PORT            GPIOA
#define GTIM_TIMX_CAP_CHY_GPIO_PIN             GPIO_PIN_0
#define GTIM_TIMX_CAP_CHY_GPIO_AF              GPIO_AF2_TIM5                                 /* AF����ѡ�� */
#define GTIM_TIMX_CAP_CHY_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define GTIM_TIMX_CAP                          TIM5                       
#define GTIM_TIMX_CAP_IRQn                     TIM5_IRQn
#define GTIM_TIMX_CAP_IRQHandler               TIM5_IRQHandler
#define GTIM_TIMX_CAP_CHY                      TIM_CHANNEL_1     /* ͨ��Y,  1<= Y <=4 */
#define GTIM_TIMX_CAP_CHY_CCRX                 TIM5->CCR1        /* ͨ��Y������ȽϼĴ��� */
#define GTIM_TIMX_CAP_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM5_CLK_ENABLE(); }while(0)   /* TIMX ʱ��ʹ�� */

/* TIMX �����������
* ������������ʹ�ö�ʱ��TIM2_CH1,����WK_UP����������
* Ĭ�������TIM2~TIM5, TIM12~TIM17.  ֻ��CH1��CH2ͨ�����������������, CH3/CH4��֧��!
* ע��: ͨ���޸���9���궨��,����֧��TIM1~TIM17����һ����ʱ��,CH1/CH2��ӦIO�����������
*       �ر�Ҫע��:Ĭ���õ�PA0,���õ�����������!���������IO,��Ӧ����������ʽ/AF���ܵ�Ҳ�ø�!
*/
#define GTIM_TIMX_CNT_CHY_GPIO_PORT            GPIOA
#define GTIM_TIMX_CNT_CHY_GPIO_PIN             GPIO_PIN_0
#define GTIM_TIMX_CNT_CHY_GPIO_AF              GPIO_AF1_TIM2                               /* AF����ѡ�� */
#define GTIM_TIMX_CNT_CHY_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0) /* PA��ʱ��ʹ�� */

#define GTIM_TIMX_CNT                          TIM2
#define GTIM_TIMX_CNT_IRQn                     TIM2_IRQn
#define GTIM_TIMX_CNT_IRQHandler               TIM2_IRQHandler
#define GTIM_TIMX_CNT_CHY                      TIM_CHANNEL_1                               /* ͨ��Y,  1<= Y <=2 */
#define GTIM_TIMX_CNT_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM2_CLK_ENABLE(); }while(0)  /* TIMX ʱ��ʹ�� */

/******************************************************************************************/

/* TIMX ���ָ������PWM ���� 
 * ���������PWM����LED2(BLUE)������,����һ�α�ʾһ��PWM��
 * Ĭ�������TIM2~TIM5, TIM12~TIM17. 
 * ע��: ͨ���޸���8���궨��,����֧��TIM1~TIM17����һ����ʱ��,����һ��IO�����PWM
 */
#define GTIM_TIMX_NPWM_CHY_GPIO_PORT            GPIOE
#define GTIM_TIMX_NPWM_CHY_GPIO_PIN             GPIO_PIN_5
#define GTIM_TIMX_NPWM_CHY_GPIO_AF              GPIO_AF4_TIM15                                /* AF����ѡ�� */
#define GTIM_TIMX_NPWM_CHY_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE��ʱ��ʹ�� */

#define GTIM_TIMX_NPWM                          TIM15                       
#define GTIM_TIMX_NPWM_IRQn                     TIM15_IRQn
#define GTIM_TIMX_NPWM_IRQHandler               TIM15_IRQHandler
#define GTIM_TIMX_NPWM_CHY                      TIM_CHANNEL_1     /* ͨ��Y,  1<= Y <=4 */
#define GTIM_TIMX_NPWM_CHY_CCRX                 TIM15->CCR1       /* ͨ��Y������ȽϼĴ��� */
#define GTIM_TIMX_NPWM_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM15_CLK_ENABLE(); }while(0)  /* TIMX ʱ��ʹ�� */

/******************************************************************************************/


void gtim_timx_int_init(uint16_t arr, uint16_t psc);      /* ͨ�ö�ʱ�� ��ʱ�жϳ�ʼ������ */

void gtim_timx_pwm_chy_init(uint16_t arr, uint16_t psc);  /* ͨ�ö�ʱ�� PWM��ʼ������ */
void gtim_timx_comp_pwm_gpio_init(void);                  /* ͨ�ö�ʱ�� PWM-GPIO��ʼ������ */

void set_timx_comparey(uint32_t compare);                 /* ����TIMͨ����ռ�ձȺ��� */
uint32_t get_timx_capture(void);                          /* ��ȡtimx����/�ȽϼĴ���ֵ */
void gtim_timx_cap_chy_init(uint16_t arr, uint16_t psc);  /* ͨ�ö�ʱ�� ���벶���ʼ������ */
void gtim_timx_cap_chy_gpio_init(void);                   /* ͨ�ö�ʱ�� ���벶��GPIO��ʼ������ */

void gtim_timx_cnt_chy_init(uint16_t psc);                /* ͨ�ö�ʱ�� ���������ʼ������ */
void gtim_timx_cnt_chy_gpio_init(void);                   /* ͨ�ö�ʱ�� �������GPIO��ʼ������ */
uint32_t gtim_timx_cnt_chy_get_count(void);               /* ͨ�ö�ʱ�� ��ȡ������� */
void gtim_timx_cnt_chy_restart(void);                     /* ͨ�ö�ʱ�� ���������� */

void gtim_timx_npwm_chy_init(uint16_t arr, uint16_t psc); /* ͨ�ö�ʱ�� ���ָ������PWM��ʼ������ */
void gtim_timx_npwm_chy_gpio_init(void);                  /* ͨ�ö�ʱ�� ���ָ������PWM GPIO��ʼ������ */
void gtim_timx_npwm_chy_set(uint32_t npwm);               /* ͨ�ö�ʱ�� �������PWM�ĸ��� */

#endif

















