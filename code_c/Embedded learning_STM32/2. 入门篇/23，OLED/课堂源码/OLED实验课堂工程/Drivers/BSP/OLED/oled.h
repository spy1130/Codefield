/**
 ****************************************************************************************************
 * @file        oled.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-21
 * @brief       OLED ��������
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
 
#ifndef __OLED_H
#define __OLED_H

#include "stdlib.h" 
#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* OLED 8080 ģʽ���� ���� */

/* Ƭѡ�� */
#define OLED_CS_PORT                GPIOD
#define OLED_CS_PIN                 GPIO_PIN_6
#define OLED_CS_CLK_ENABLE()        do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PD��ʱ��ʹ�� */

/* �������ͽ� ����/����*/
#define OLED_RS_PORT                GPIOD
#define OLED_RS_PIN                 GPIO_PIN_3
#define OLED_RS_CLK_ENABLE()        do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PD��ʱ��ʹ�� */

/* ��OLED��ȡ���ݽ� */
#define OLED_RD_PORT                GPIOG
#define OLED_RD_PIN                 GPIO_PIN_13
#define OLED_RD_CLK_ENABLE()        do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PG��ʱ��ʹ�� */

/* ��OLEDд�����ݽ� */
#define OLED_WR_PORT                GPIOG
#define OLED_WR_PIN                 GPIO_PIN_14
#define OLED_WR_CLK_ENABLE()        do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PG��ʱ��ʹ�� */

/* ��λ�� */
#define OLED_RST_PORT               GPIOG
#define OLED_RST_PIN                GPIO_PIN_15
#define OLED_RST_CLK_ENABLE()       do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PG��ʱ��ʹ�� */

/* ���ݽ� */
#define OLED_DATA_PORT               GPIOC
#define OLED_DATA_PIN                GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
#define OLED_DATA_CLK_ENABLE()       do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC��ʱ��ʹ�� */
/******************************************************************************************/

/* OLED 8080ģʽ��ض˿ڿ��ƺ��� ���� */
#define OLED_RST(x)     do{ x ? \
                                  HAL_GPIO_WritePin(OLED_RST_PORT, OLED_RST_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_RST_PORT, OLED_RST_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* ����RST���� */

#define OLED_CS(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_CS_PORT, OLED_CS_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_CS_PORT, OLED_CS_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* ����CS���� */
#define OLED_RS(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_RS_PORT, OLED_RS_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_RS_PORT, OLED_RS_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* ����RS���� */
                              
#define OLED_WR(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_WR_PORT, OLED_WR_PIN, GPIO_PIN_SET) :  \
                                  HAL_GPIO_WritePin(OLED_WR_PORT, OLED_WR_PIN, GPIO_PIN_RESET); \
                        } while (0)     /* ����WR���� */

#define OLED_RD(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_RD_PORT, OLED_RD_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_RD_PORT, OLED_RD_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* ����RD���� */

/* ����/���� ���� */
#define OLED_CMD        0       /* д���� */
#define OLED_DATA       1       /* д���� */

/******************************************************************************************/
    
static void oled_wr_byte(uint8_t data, uint8_t cmd);    /* дһ���ֽڵ�OLED */
void oled_init(void);           /* OLED��ʼ�� */


#endif




