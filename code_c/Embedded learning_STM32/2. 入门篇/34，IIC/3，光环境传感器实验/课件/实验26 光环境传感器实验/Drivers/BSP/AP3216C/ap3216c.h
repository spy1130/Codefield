/**
 ****************************************************************************************************
 * @file        ap3216c.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-04-20
 * @brief       AP3216C ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ������ F429������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20220420
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __AP3216C_H
#define __AP3216C_H

#include "./SYSTEM/sys/sys.h"


#define AP3216C_ADDR    0X3C    /* AP3216C����IIC��ַ(������һλ) */

uint8_t ap3216c_init(void); 
uint8_t ap3216c_write_one_byte(uint8_t reg, uint8_t data);
uint8_t ap3216c_read_one_byte(uint8_t reg);
void ap3216c_read_data(uint16_t* ir, uint16_t* ps, uint16_t* als);

#endif
