/**
 ****************************************************************************************************
 * @file        dma.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-26
 * @brief       DMA ��������
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
 * V1.0 20200326
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __DMA_H
#define __DMA_H

#include "./SYSTEM/sys/sys.h"


void dma_init(DMA_Stream_TypeDef *dma_stream_handle, uint32_t ch);  /* ����DMAx_CHx */
void dma_mux_init(DMA_Stream_TypeDef *DMA_Streamx, uint8_t ch);     /* DMA����������ʼ�� */
void dma_enable(DMA_Stream_TypeDef *DMA_Streamx, uint16_t ndtr);    /* ʹ��һ��DMA���� */
#endif
