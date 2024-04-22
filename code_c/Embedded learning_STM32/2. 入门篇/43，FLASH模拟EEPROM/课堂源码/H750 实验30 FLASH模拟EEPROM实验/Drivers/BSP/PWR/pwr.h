/**
 ****************************************************************************************************
 * @file        pwr.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.3
 * @date        2020-03-26
 * @brief       �͹���ģʽ ��������
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
 * V1.0 20200325
 * ��һ�η���
 * V1.1 20200325
 * ֧�ֽ���˯��ģʽ������
 * ����pwr_wkup_key_init��pwr_enter_sleep����
 * V1.2 20200325
 * ֧�ֽ���ֹͣģʽ������
 * ����pwr_enter_stop����
 * V1.3 20200326
 * ֧�ֽ������ģʽ������
 * ����pwr_enter_standby����
 *
 ****************************************************************************************************
 */

#ifndef __PWR_H
#define __PWR_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* PWR WKUP ���� ���ź��ж� ���� 
 * ����ͨ��WK_UP�������� MCU,  ��˱��붨��������������Ӧ���жϷ����� 
 */

#define PWR_WKUP_GPIO_PORT              GPIOA
#define PWR_WKUP_GPIO_PIN               GPIO_PIN_0
#define PWR_WKUP_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */
  
#define PWR_WKUP_INT_IRQn               EXTI0_IRQn
#define PWR_WKUP_INT_IRQHandler         EXTI0_IRQHandler

/******************************************************************************************/



void pwr_pvd_init(uint32_t pls);    /* PVD��ѹ����ʼ������ */
void pwr_wkup_key_init(void);       /* ���Ѱ�����ʼ�� */
void pwr_enter_sleep(void);         /* ����˯��ģʽ */
void pwr_enter_stop(void);          /* ����ֹͣģʽ */
void pwr_enter_standby(void);       /* �������ģʽ */

#endif




















