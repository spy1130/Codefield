/**
 ****************************************************************************************************
 * @file        ov5640.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-03
 * @brief       OV5640 ��������
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
 * V1.0 20200403
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef _OV5640_H
#define _OV5640_H

#include "./SYSTEM/sys/sys.h"
#include "./BSP/OV5640/sccb.h"

/******************************************************************************************/
/* PWDN��RESET ���� ���� */

#define OV_PWDN_GPIO_PORT               GPIOC
#define OV_PWDN_GPIO_PIN                GPIO_PIN_4
#define OV_PWDN_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC��ʱ��ʹ�� */

#define OV_RESET_GPIO_PORT              GPIOA
#define OV_RESET_GPIO_PIN               GPIO_PIN_7
#define OV_RESET_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

/******************************************************************************************/

/* IO���ƺ��� */
#define OV5640_PWDN(x)    do{ x ? \
                              HAL_GPIO_WritePin(OV_PWDN_GPIO_PORT, OV_PWDN_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(OV_PWDN_GPIO_PORT, OV_PWDN_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)     /* POWER DOWN�����ź� */

#define OV5640_RST(x)     do{ x ? \
                              HAL_GPIO_WritePin(OV_RESET_GPIO_PORT, OV_RESET_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(OV_RESET_GPIO_PORT, OV_RESET_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)     /* ��λ�����ź� */

/* OV5640��ID�ͷ��ʵ�ַ */
#define OV5640_ID           0X5640      /* OV5640��оƬID */
#define OV5640_ADDR         0X78        /* OV5640��IIC��ַ */
 
/* OV5640��ؼĴ������� */
#define OV5640_CHIPIDH      0X300A      /* OV5640оƬID���ֽ� */
#define OV5640_CHIPIDL      0X300B      /* OV5640оƬID���ֽ� */
 

/* ����ӿں��� */
uint8_t ov5640_read_reg(uint16_t reg);
uint8_t ov5640_write_reg(uint16_t reg,uint8_t data);

uint8_t ov5640_init(void);  
void ov5640_jpeg_mode(void);
void ov5640_rgb565_mode(void);

uint8_t ov5640_focus_init(void);
uint8_t ov5640_focus_single(void);
uint8_t ov5640_focus_constant(void);

void ov5640_flash_ctrl(uint8_t sw);
void ov5640_test_pattern(uint8_t mode);

void ov5640_sharpness(uint8_t sharp);
void ov5640_brightness(uint8_t bright);
void ov5640_contrast(uint8_t contrast);
void ov5640_exposure(uint8_t exposure);
void ov5640_light_mode(uint8_t mode);
void ov5640_special_effects(uint8_t eft);
void ov5640_color_saturation(uint8_t sat);

uint8_t ov5640_outsize_set(uint16_t offx,uint16_t offy,uint16_t width,uint16_t height);
uint8_t ov5640_image_window_set(uint16_t offx,uint16_t offy,uint16_t width,uint16_t height); 

#endif





















