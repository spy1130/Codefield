/**
 ****************************************************************************************************
 * @file        dac.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.2
 * @date        2020-03-28
 * @brief       DAC ��������
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
 * V1.1 20200328
 * ����dac_triangular_wave����
 * V1.2 20200328
 * ����dac_dma_wave_init����
 *
 ****************************************************************************************************
 */

#ifndef __DAC_H
#define __DAC_H

#include "./SYSTEM/sys/sys.h"


extern DAC_HandleTypeDef g_dac_dma_handle;

void dac_init(uint32_t outx);           /* DACͨ��1��ʼ�� */ 
void dac_set_voltage(uint16_t vol);     /* ����ͨ��1�����ѹ */ 
void dac_triangular_wave(uint16_t maxval, uint16_t dt, uint16_t samples, uint16_t n); /* ������ǲ� */
void dac_dma_wave_init(void);           /* DAC DMA������Ҳ���ʼ������ */


#endif







