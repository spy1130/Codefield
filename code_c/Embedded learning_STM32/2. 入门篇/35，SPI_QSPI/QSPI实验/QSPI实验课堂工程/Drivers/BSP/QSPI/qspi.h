/**
 ****************************************************************************************************
 * @file        qspi.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       QSPI ��������
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
 * V1.0 20200329
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __QSPI_H
#define __QSPI_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* QSPI ��� ���� ���� */

#define QSPI_BK1_CLK_GPIO_PORT          GPIOB
#define QSPI_BK1_CLK_GPIO_PIN           GPIO_PIN_2
#define QSPI_BK1_CLK_GPIO_AF            GPIO_AF9_QUADSPI
#define QSPI_BK1_CLK_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE; }while(0)   /* PB��ʱ��ʹ�� */

#define QSPI_BK1_NCS_GPIO_PORT          GPIOB
#define QSPI_BK1_NCS_GPIO_PIN           GPIO_PIN_6
#define QSPI_BK1_NCS_GPIO_AF            GPIO_AF10_QUADSPI
#define QSPI_BK1_NCS_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE; }while(0)   /* PB��ʱ��ʹ�� */

#define QSPI_BK1_IO0_GPIO_PORT          GPIOD
#define QSPI_BK1_IO0_GPIO_PIN           GPIO_PIN_11
#define QSPI_BK1_IO0_GPIO_AF            GPIO_AF9_QUADSPI
#define QSPI_BK1_IO0_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOD_CLK_ENABLE; }while(0)   /* PD��ʱ��ʹ�� */

#define QSPI_BK1_IO1_GPIO_PORT          GPIOD
#define QSPI_BK1_IO1_GPIO_PIN           GPIO_PIN_12
#define QSPI_BK1_IO1_GPIO_AF            GPIO_AF9_QUADSPI
#define QSPI_BK1_IO1_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOD_CLK_ENABLE; }while(0)   /* PD��ʱ��ʹ�� */

#define QSPI_BK1_IO2_GPIO_PORT          GPIOD
#define QSPI_BK1_IO2_GPIO_PIN           GPIO_PIN_13
#define QSPI_BK1_IO2_GPIO_AF            GPIO_AF9_QUADSPI
#define QSPI_BK1_IO2_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOD_CLK_ENABLE; }while(0)   /* PD��ʱ��ʹ�� */

#define QSPI_BK1_IO3_GPIO_PORT          GPIOE
#define QSPI_BK1_IO3_GPIO_PIN           GPIO_PIN_2
#define QSPI_BK1_IO3_GPIO_AF            GPIO_AF9_QUADSPI
#define QSPI_BK1_IO3_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOE_CLK_ENABLE; }while(0)   /* PE��ʱ��ʹ�� */

/******************************************************************************************/


uint8_t qspi_wait_flag(uint32_t flag, uint8_t sta, uint32_t wtime); /* QSPI�ȴ�ĳ��״̬ */
uint8_t qspi_init(void);    /* ��ʼ��QSPI */
void qspi_send_cmd(uint8_t cmd, uint32_t addr, uint8_t mode, uint8_t dmcycle);  /* QSPI�������� */
uint8_t qspi_receive(uint8_t *buf, uint32_t datalen);   /* QSPI�������� */
uint8_t qspi_transmit(uint8_t *buf, uint32_t datalen);  /* QSPI�������� */

#endif



















