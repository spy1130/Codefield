/**
 ****************************************************************************************************
 * @file        ov7725.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       OV7725 ��������
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
 * V1.0 20200426
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef _OV7725_H
#define _OV7725_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* OV7725ģ��ĵ� VSYNC/WRST/RRST/OE/RCLK/WEN ���� ���� 
 * D0~D7, �������Ŷ�, ������, ����Ͳ������ﶨ����,ֱ����ov7725_init�����޸�.��������ֲ
 * ��ʱ��, ���˸���6��IO��, ���ø�ov7725_init�����D0~D7���ڵ�IO��.
 */

#define OV7725_VSYNC_GPIO_PORT          GPIOA
#define OV7725_VSYNC_GPIO_PIN           GPIO_PIN_8
#define OV7725_VSYNC_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* OV7725֡ͬ����, ����IO��ʱ��ʹ�� */

#define OV7725_WRST_GPIO_PORT           GPIOD
#define OV7725_WRST_GPIO_PIN            GPIO_PIN_6
#define OV7725_WRST_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* FIFOдָ�븴λ��, ����IO��ʱ��ʹ�� */

#define OV7725_RRST_GPIO_PORT           GPIOG
#define OV7725_RRST_GPIO_PIN            GPIO_PIN_14
#define OV7725_RRST_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* FIFO��ָ�븴λ��, ����IO��ʱ��ʹ�� */

#define OV7725_OE_GPIO_PORT             GPIOG
#define OV7725_OE_GPIO_PIN              GPIO_PIN_15
#define OV7725_OE_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* FIFO�������ʹ�ܽ�, ����IO��ʱ��ʹ�� */

#define OV7725_RCLK_GPIO_PORT           GPIOB
#define OV7725_RCLK_GPIO_PIN            GPIO_PIN_4
#define OV7725_RCLK_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* FIFO������ʱ�ӽ�, ����IO��ʱ��ʹ�� */

#define OV7725_WEN_GPIO_PORT            GPIOB
#define OV7725_WEN_GPIO_PIN             GPIO_PIN_3
#define OV7725_WEN_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* FIFOд����ʹ�ܽ�, ����IO��ʱ��ʹ�� */

/******************************************************************************************/

/* OV7725��ض˿ڶ��� */
#define OV7725_VSYNC(x)   do{ x ? \
                              HAL_GPIO_WritePin(OV7725_VSYNC_GPIO_PORT, OV7725_VSYNC_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(OV7725_VSYNC_GPIO_PORT, OV7725_VSYNC_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)  /* VSYNC */

#define OV7725_WRST(x)    do{ x ? \
                              HAL_GPIO_WritePin(OV7725_WRST_GPIO_PORT, OV7725_WRST_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(OV7725_WRST_GPIO_PORT, OV7725_WRST_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)    /* WRST */

//#define OV7725_RRST(x)    do{ x ? \
//                              HAL_GPIO_WritePin(OV7725_RRST_GPIO_PORT, OV7725_RRST_GPIO_PIN, GPIO_PIN_SET) : \
//                              HAL_GPIO_WritePin(OV7725_RRST_GPIO_PORT, OV7725_RRST_GPIO_PIN, GPIO_PIN_RESET); \
//                          }while(0)    /* RRST */
#define OV7725_RRST(x)    x ? (OV7725_RRST_GPIO_PORT->BSRR = OV7725_RRST_GPIO_PIN) : (OV7725_RRST_GPIO_PORT->BRR = OV7725_RRST_GPIO_PIN)
                          
#define OV7725_OE(x)      do{ x ? \
                              HAL_GPIO_WritePin(OV7725_OE_GPIO_PORT, OV7725_OE_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(OV7725_OE_GPIO_PORT, OV7725_OE_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)        /* OE */

#define OV7725_RCLK(x)      x ? (OV7725_RCLK_GPIO_PORT->BSRR = OV7725_RCLK_GPIO_PIN) : (OV7725_RCLK_GPIO_PORT->BRR = OV7725_RCLK_GPIO_PIN)    /* RCLK */
//#define OV7725_RCLK(x)      HAL_GPIO_TogglePin(OV7725_RCLK_GPIO_PORT,OV7725_RCLK_GPIO_PIN);

#define OV7725_WEN(x)     do{ x ? \
                              HAL_GPIO_WritePin(OV7725_WEN_GPIO_PORT, OV7725_WEN_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(OV7725_WEN_GPIO_PORT, OV7725_WEN_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)      /* WEN */

#define OV7725_DATA         GPIOC->IDR & 0X00FF                                                 /* D0~D7 */


/* OV7725 ID */
#define OV7725_MID              0X7FA2      /* MID , ��һ��MID*/
#define OV7725_MID1             0X7FFF      /* MID1, �ڶ���MID */
#define OV7725_PID              0X7721

/* OV7670��SCCB��ַ */
#define OV7725_ADDR             0X42


uint8_t ov7725_init(void);                          /* OV7725��ʼ�� */
uint8_t ov7725_read_reg(uint16_t reg);              /* OV7725 ���Ĵ��� */
uint8_t ov7725_write_reg(uint8_t reg, uint8_t data);/* OV7725д�Ĵ��� */

void ov7725_light_mode(uint8_t mode);       /* OV7725 �ƹ�ģʽ���� */
void ov7725_color_saturation(uint8_t sat);  /* OV7725 ɫ�ʱ��Ͷ����� */
void ov7725_brightness(uint8_t bright);     /* OV7725 �������� */
void ov7725_contrast(uint8_t contrast);     /* OV7725 �Աȶ����� */
void ov7725_special_effects(uint8_t eft);   /* OV7725 ��Ч���� */
void ov7725_window_set(uint16_t width, uint16_t height, uint8_t mode);  /* OV7725 ����������� */

#endif





















