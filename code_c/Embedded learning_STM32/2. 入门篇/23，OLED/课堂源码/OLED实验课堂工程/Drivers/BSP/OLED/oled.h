/**
 ****************************************************************************************************
 * @file        oled.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-21
 * @brief       OLED 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200421
 * 第一次发布
 *
 ****************************************************************************************************
 */
 
#ifndef __OLED_H
#define __OLED_H

#include "stdlib.h" 
#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* OLED 8080 模式引脚 定义 */

/* 片选脚 */
#define OLED_CS_PORT                GPIOD
#define OLED_CS_PIN                 GPIO_PIN_6
#define OLED_CS_CLK_ENABLE()        do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PD口时钟使能 */

/* 数据类型脚 命令/数据*/
#define OLED_RS_PORT                GPIOD
#define OLED_RS_PIN                 GPIO_PIN_3
#define OLED_RS_CLK_ENABLE()        do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PD口时钟使能 */

/* 向OLED读取数据脚 */
#define OLED_RD_PORT                GPIOG
#define OLED_RD_PIN                 GPIO_PIN_13
#define OLED_RD_CLK_ENABLE()        do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PG口时钟使能 */

/* 向OLED写入数据脚 */
#define OLED_WR_PORT                GPIOG
#define OLED_WR_PIN                 GPIO_PIN_14
#define OLED_WR_CLK_ENABLE()        do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PG口时钟使能 */

/* 复位脚 */
#define OLED_RST_PORT               GPIOG
#define OLED_RST_PIN                GPIO_PIN_15
#define OLED_RST_CLK_ENABLE()       do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PG口时钟使能 */

/* 数据脚 */
#define OLED_DATA_PORT               GPIOC
#define OLED_DATA_PIN                GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
#define OLED_DATA_CLK_ENABLE()       do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC口时钟使能 */
/******************************************************************************************/

/* OLED 8080模式相关端口控制函数 定义 */
#define OLED_RST(x)     do{ x ? \
                                  HAL_GPIO_WritePin(OLED_RST_PORT, OLED_RST_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_RST_PORT, OLED_RST_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* 设置RST引脚 */

#define OLED_CS(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_CS_PORT, OLED_CS_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_CS_PORT, OLED_CS_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* 设置CS引脚 */
#define OLED_RS(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_RS_PORT, OLED_RS_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_RS_PORT, OLED_RS_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* 设置RS引脚 */
                              
#define OLED_WR(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_WR_PORT, OLED_WR_PIN, GPIO_PIN_SET) :  \
                                  HAL_GPIO_WritePin(OLED_WR_PORT, OLED_WR_PIN, GPIO_PIN_RESET); \
                        } while (0)     /* 设置WR引脚 */

#define OLED_RD(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_RD_PORT, OLED_RD_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_RD_PORT, OLED_RD_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* 设置RD引脚 */

/* 命令/数据 定义 */
#define OLED_CMD        0       /* 写命令 */
#define OLED_DATA       1       /* 写数据 */

/******************************************************************************************/
    
static void oled_wr_byte(uint8_t data, uint8_t cmd);    /* 写一个字节到OLED */
void oled_init(void);           /* OLED初始化 */


#endif




