/**
 ****************************************************************************************************
 * @file        tpad.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-21
 * @brief       ���ݴ�������(TPAD) ��������
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
 * V1.0 20200421
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __TPAD_H
#define __TPAD_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* TPAD ���� �� ��ʱ�� ���� */

/* ����ʹ�ö�ʱ�������벶����, ��TPAD���м��
 * ��������벶��ʹ�ö�ʱ��TIM5_CH2, ����TPAD����������
 * ��Ϊ���ǵ�TPAD��������PA1�����ϵ�, PA1ֻ����TIM2_CH2 / TIM5_CH2
 * ���Զ�ʱ��Ҳֻ��������������ѡ, ������Լ���Ƶİ忨, �����ԭ��
 * ͼ������Ӧ���޸ļ���, ����GPIO �� ��Ӧ�Ķ�ʱ����ͨ��
 */
#define TPAD_GPIO_PORT                          GPIOA
#define TPAD_GPIO_PIN                           SYS_GPIO_PIN1
#define TPAD_GPIO_CLK_ENABLE()                  do{ RCC->APB1ENR |= 1 << 2; }while(0)   /* PA��ʱ��ʹ�� */

#define TPAD_TIMX_CAP                           TIM5
#define TPAD_TIMX_CAP_CHY                       2                       /* ͨ��Y,  1<= Y <=4 */
#define TPAD_TIMX_CAP_CHY_CCRX                  TIM5->CCR2              /* ͨ��Y�Ĳ���/�ȽϼĴ��� */
#define TPAD_TIMX_CAP_CHY_CLK_ENABLE()          do{ RCC->APB1ENR |= 1 << 3; }while(0)   /* TIM5 ʱ��ʹ�� */

/******************************************************************************************/

/* ����������ֵ, Ҳ���Ǳ������ g_tpad_default_val + TPAD_GATE_VAL
 * ����Ϊ����Ч����, �Ĵ� TPAD_GATE_VAL, ���Խ���������, ��֮, ��������������
 * ����ʵ������, ѡ����ʵ� TPAD_GATE_VAL ����
 */
#define TPAD_GATE_VAL       100             /* ����������ֵ, Ҳ���Ǳ������ g_tpad_default_val + TPAD_GATE_VAL, ����Ϊ����Ч���� */
#define TPAD_ARR_MAX_VAL    0XFFFF          /* ����ARRֵ, һ������Ϊ��ʱ����ARR���ֵ */


extern volatile uint16_t g_tpad_default_val;/* ���ص�ʱ��(û���ְ���),��������Ҫ��ʱ�� */

/* ��̬����, ���� tapd.c���� */
static void tpad_reset(void);               /* ��λ */
static uint16_t tpad_get_val(void);         /* �õ���ʱ������ֵ */
static uint16_t tpad_get_maxval(uint8_t n); /* ��ȡn��, ��ȡ���ֵ */
static void tpad_timx_cap_init(uint16_t arr, uint16_t psc); /* ��ʱ�����벶���ʼ�� */


/* �ӿں���, ����������.c���� */
uint8_t tpad_init(uint16_t psc);    /* TPAD ��ʼ�� ���� */
uint8_t tpad_scan(uint8_t mode);    /* TPAD ɨ�� ���� */

#endif























