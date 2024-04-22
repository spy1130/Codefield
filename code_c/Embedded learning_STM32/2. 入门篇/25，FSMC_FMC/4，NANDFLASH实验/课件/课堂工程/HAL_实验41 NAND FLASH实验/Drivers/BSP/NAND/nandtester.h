/**
 ****************************************************************************************************
 * @file        nandtester.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-4-20
 * @brief       NAND FLASH USMART测试代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 F429开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20220420
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __NANDTESTER_H
#define __NANDTESTER_H

#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/

uint8_t test_writepage(uint32_t pagenum, uint16_t colnum, uint16_t writebytes);
uint8_t test_readpage(uint32_t pagenum, uint16_t colnum, uint16_t readbytes);
uint8_t test_copypageandwrite(uint32_t spnum, uint32_t dpnum, uint16_t colnum, uint16_t writebytes);
uint8_t test_readspare(uint32_t pagenum, uint16_t colnum, uint16_t readbytes);
void test_readallblockinfo(uint32_t sblock);
uint8_t test_ftlwritesectors(uint32_t secx, uint16_t secsize, uint16_t seccnt);
uint8_t test_ftlreadsectors(uint32_t secx, uint16_t secsize, uint16_t seccnt);

#endif
