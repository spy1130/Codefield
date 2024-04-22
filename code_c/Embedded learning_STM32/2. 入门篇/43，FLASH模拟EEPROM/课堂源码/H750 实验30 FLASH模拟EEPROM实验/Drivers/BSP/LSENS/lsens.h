/**
 ****************************************************************************************************
 * @file        lsens.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-28
 * @brief       ���������� ��������
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
 * V1.0 20200328
 * ��һ�η���
 ****************************************************************************************************
 */

#ifndef __LSENS_H
#define __LSENS_H

#include "./SYSTEM/sys/sys.h"
#include "./BSP/ADC/adc3.h"


/******************************************************************************************/
/* ������������ӦADC3���������ź�ͨ�� ���� */

#define LSENS_ADC3_CHX_GPIO_PORT            GPIOC
#define LSENS_ADC3_CHX_GPIO_PIN             GPIO_PIN_1
#define LSENS_ADC3_CHX_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC��ʱ��ʹ�� */


#define LSENS_ADC3_CHX                      ADC_CHANNEL_11      /* ͨ��Y,  0 <= Y <= 19 */ 

/******************************************************************************************/
 

void lsens_init(void);          /* ��ʼ������������ */
uint8_t lsens_get_val(void);    /* ��ȡ������������ֵ */
#endif 





















