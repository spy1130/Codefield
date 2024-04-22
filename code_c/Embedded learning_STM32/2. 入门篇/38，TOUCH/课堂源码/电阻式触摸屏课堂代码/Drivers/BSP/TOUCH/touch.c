/**
 ****************************************************************************************************
 * @file        touch.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-25
 * @brief       触摸屏 驱动代码
 *   @note      支持电阻/电容式触摸屏
 *              触摸屏驱动（支持ADS7843/7846/UH7843/7846/XPT2046/TSC2046/GT9147/GT9271/FT5206等）代码
 *
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
 * V1.0 20200425
 * 第一次发布
 * V1.1 20200606
 * 校准函数, case 5里面, 判定差值不合格增加:
 * 判定差值等于0的判断, 差值不能等于0
 * 
 ****************************************************************************************************
 */

#include "stdio.h"
#include "stdlib.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/TOUCH/touch.h"
#include "./SYSTEM/delay/delay.h"

/* MCU向XTP2046发送命令，XTP2046返回数据 */
uint16_t tp_write_and_read_ad(uint8_t cmd_data)
{
    uint16_t rd_data = 0;
    
    /* 开始状态 */
    T_CLK(0);
    T_MOSI(0);
    T_CS(0);    /* 选中触摸IC */
    
    /* MCU向XTP2046发送数据 */
    for(uint8_t i = 0; i < 8; i++ )
    {
        T_CLK(0);   /* MCU开始准备数据 */
        
        if (cmd_data & 0x80)    /* 数据要取最高位发送，MSB */
        {
            T_MOSI(1);
        }
        else
        {
            T_MOSI(0);
        }
        delay_us(1);    /* MCU准备数据完成 */
        
        T_CLK(1);       /* MCU发送数据，XTP2046开始读取 */
        delay_us(1);    /* XTP2046读取数据完成 */
        
        cmd_data <<= 1; /* 将次高位变为最高位，用于下次取最高位 */
    }
    
    /* 过滤忙信号 */
    T_CLK(0);
    delay_us(1);
    T_CLK(1);
    delay_us(1);
    
    /* MCU读取XTP2046返回数据 */
    for(uint8_t i = 0; i < 16; i++ )
    {
        T_CLK(0);   /* XTP2046开始准备数据 */
        delay_us(1);
        T_CLK(1);   /* MCU开始读取数据 */
        
        rd_data <<= 1;  /* 空出最低位用来保存读取到的数据 */
        rd_data |= T_MISO;  /* MCU读取数据 */
        delay_us(1);
    }

    /* 结束状态 */    
    T_CLK(0);   /* 完整的周期 */
    T_CS(1);    /* 取消选中触摸IC */
    
    return  (rd_data >>= 4);
}


/**
 * @brief       触摸屏初始化
 */
void tp_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    T_PEN_GPIO_CLK_ENABLE();    /* T_PEN脚时钟使能 */
    T_CS_GPIO_CLK_ENABLE();     /* T_CS脚时钟使能 */
    T_MISO_GPIO_CLK_ENABLE();   /* T_MISO脚时钟使能 */
    T_MOSI_GPIO_CLK_ENABLE();   /* T_MOSI脚时钟使能 */
    T_CLK_GPIO_CLK_ENABLE();    /* T_CLK脚时钟使能 */

    gpio_init_struct.Pin = T_PEN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                 /* 输入 */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;           /* 高速 */
    HAL_GPIO_Init(T_PEN_GPIO_PORT, &gpio_init_struct);       /* 初始化T_PEN引脚 */

    gpio_init_struct.Pin = T_MISO_GPIO_PIN;
    HAL_GPIO_Init(T_MISO_GPIO_PORT, &gpio_init_struct);      /* 初始化T_MISO引脚 */

    gpio_init_struct.Pin = T_MOSI_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;             /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;           /* 高速 */
    HAL_GPIO_Init(T_MOSI_GPIO_PORT, &gpio_init_struct);      /* 初始化T_MOSI引脚 */

    gpio_init_struct.Pin = T_CLK_GPIO_PIN;
    HAL_GPIO_Init(T_CLK_GPIO_PORT, &gpio_init_struct);       /* 初始化T_CLK引脚 */

    gpio_init_struct.Pin = T_CS_GPIO_PIN;
    HAL_GPIO_Init(T_CS_GPIO_PORT, &gpio_init_struct);        /* 初始化T_CS引脚 */

}









