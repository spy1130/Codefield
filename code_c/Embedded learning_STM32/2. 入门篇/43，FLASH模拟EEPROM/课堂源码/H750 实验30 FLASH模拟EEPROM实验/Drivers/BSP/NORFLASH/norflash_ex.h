/**
 ****************************************************************************************************
 * @file        norflash_ex.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       QSPI ��������
 *              ��ΪSTM32H7��֧��QSPI�ӿڶ�ʱд,��˸ô�������ʵ��QSPI FLASH������д��,ԭ���ǣ�
 *              qspi.c��norflash.c��norflash_ex.c��3���ִ���ȫ���洢��H7���ڲ�FLASH,��֤����
 *              QSPI FLASH��ʱ��,����Ҫ����QSPI FLASH�Ĵ���. �Ӷ�ʵ��QSPI FLASH����д��.
 *
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
 
#ifndef __NORFLASH_EX_H
#define __NORFLASH_EX_H

#include "./SYSTEM/sys/sys.h"


void norflash_ex_erase_chip(void);              /* NOR FLASH ȫƬ���� */
uint16_t norflash_ex_read_id(void);             /* NOR FLASH��ȡID */
void norflash_ex_erase_sector(uint32_t addr);   /* NOR FLASH �������� */
uint8_t norflash_ex_write(uint8_t *pbuf, uint32_t addr, uint16_t datalen);  /* NOR FLASHд������ */
void norflash_ex_read(uint8_t *pbuf, uint32_t addr, uint16_t datalen);      /* NOR FLASH��ȡ���� */

#endif















