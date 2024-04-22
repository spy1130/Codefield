/**
 ****************************************************************************************************
 * @file        stmflash.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-2
 * @brief       STM32�ڲ�FLASH��д ��������
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
 * V1.0 20200402
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "./SYSTEM/sys/sys.h"


/* FLASH��ʼ��ַ */
#define STM32_FLASH_BASE        0x08000000      /* STM32 FLASH����ʼ��ַ */
#define STM32_FLASH_SIZE        0x20000         /* STM32 FLASH�ܴ�С */
#define BOOT_FLASH_SIZE         0x4000          /* ǰ16K FLASH���ڱ���BootLoader */
#define FLASH_WAITETIME         50000           /* FLASH�ȴ���ʱʱ�� */


/* FLASH ��������ʼ��ַ,H750xxֻ��BANK1������0��Ч,��128KB */
#define BANK1_FLASH_SECTOR_0    ((uint32_t)0x08000000)  /* Bank1����0��ʼ��ַ, 128 Kbytes  */


uint32_t stmflash_read_word(uint32_t faddr);    /* ��ȡһ����(4�ֽ�) */
void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length);   /* ָ����ַ��ʼд��ָ�����ȵ����� */
void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length);    /* ��ָ����ַ��ʼ����ָ�����ȵ����� */

/* ����д�� */
void test_write(uint32_t waddr, uint32_t wdata);
    
#endif

















