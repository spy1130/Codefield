/**
 ****************************************************************************************************
 * @file        gt9xxx.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-25
 * @brief       4.3寸电容触摸屏-GT9xxx 驱动代码
 *   @note      GT系列电容触摸屏IC通用驱动,本代码支持: GT9147/GT917S/GT968/GT1151/GT9271 等多种
 *              驱动IC, 这些驱动IC仅ID不一样, 具体代码基本不需要做任何修改即可通过本代码直接驱动
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
 *
 ****************************************************************************************************
 */

#include "string.h"
#include "./BSP/TOUCH/ctiic.h"
#include "./BSP/TOUCH/gt9xxx.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

/* return 1fail 0succeed */
uint8_t gt9xxx_wr_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t ret = 0;
    
    ct_iic_start();
    ct_iic_send_byte(GT9XXX_CMD_WR);
    ct_iic_wait_ack();
    ct_iic_send_byte(reg >> 8);
    ct_iic_wait_ack();
    ct_iic_send_byte(reg & 0xFF);
    ct_iic_wait_ack();
    
    for (uint8_t i = 0; i < len; i++)
    {
        ct_iic_send_byte(buf[i]);
        ret = ct_iic_wait_ack();
        
        if (ret) break;
    }
    
    ct_iic_stop();
    return ret;
}

void gt9xxx_rd_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    
    ct_iic_start();
    ct_iic_send_byte(GT9XXX_CMD_WR);
    ct_iic_wait_ack();
    ct_iic_send_byte(reg >> 8);
    ct_iic_wait_ack();
    ct_iic_send_byte(reg & 0xFF);
    ct_iic_wait_ack();
    
    ct_iic_start();
    ct_iic_send_byte(GT9XXX_CMD_RD);    
    ct_iic_wait_ack();
    
    for (i = 0; i < len; i++)
    {
        /* 1:ack 0:nack */
        buf[i] = ct_iic_read_byte(i == (len - 1)?0:1);
    }
    
    ct_iic_stop(); 
}

uint8_t gt9xxx_init(void)
{ 
    uint8_t temp[5];
    
    GPIO_InitTypeDef gpio_init_struct;
    
    GT9XXX_RST_GPIO_CLK_ENABLE();   /* RST引脚时钟使能 */
    GT9XXX_INT_GPIO_CLK_ENABLE();   /* INT引脚时钟使能 */

    gpio_init_struct.Pin = GT9XXX_RST_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(GT9XXX_RST_GPIO_PORT, &gpio_init_struct); /* 初始化RST引脚 */
    
    gpio_init_struct.Pin = GT9XXX_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(GT9XXX_INT_GPIO_PORT, &gpio_init_struct); /* 初始化INT引脚 */
    
    ct_iic_init();  /* 初始化电容屏的I2C总线 */
    
    /* 通讯地址 0x28/0x29 */
    HAL_GPIO_WritePin(GT9XXX_INT_GPIO_PORT, GT9XXX_INT_GPIO_PIN, GPIO_PIN_SET);     /* INT输出高电平 */
    GT9XXX_RST(0);  /* 复位 */
    delay_ms(10);
    GT9XXX_RST(1);  /* 释放复位 */
    delay_ms(10);
    
    /* INT引脚模式设置, 输入模式, 浮空输入 */
    gpio_init_struct.Pin = GT9XXX_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                /* 输入 */
    gpio_init_struct.Pull = GPIO_NOPULL;                    /* 不带上下拉，浮空模式 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(GT9XXX_INT_GPIO_PORT, &gpio_init_struct); /* 初始化INT引脚 */
   
    /* 读取ID */
    delay_ms(100);
    gt9xxx_rd_reg(GT9XXX_PID_REG, temp, 4);
    temp[4] = 0;
    printf("CTP ID:%s\r\n",temp);
    
    /* 软复位 */
    temp[0] = 0x02;
    gt9xxx_wr_reg(GT9XXX_CTRL_REG, temp, 1);
    
    delay_ms(10);
    
    /* 正常读取坐标 */
    temp[0] = 0x00;
    gt9xxx_wr_reg(GT9XXX_CTRL_REG, temp, 1);
    
    
    return 0;
}

void gt9xxx_scan(void)
{
    uint8_t mode = 0;
    uint8_t i;
    uint8_t buf[4];
    uint16_t x,y;
    
    /* 读取状态寄存器 */
    gt9xxx_rd_reg(GT9XXX_GSTID_REG, &mode, 1);
    
    if ((mode & 0x80) && ((mode & 0xF) <= 5))
    {
       i = 0;
       gt9xxx_wr_reg(GT9XXX_GSTID_REG, &i, 1);  /* 清楚标记 */
    }
    
    if ((mode & 0xF) && ((mode & 0xF) <= 5))
    {
        gt9xxx_rd_reg(GT9XXX_TP1_REG, buf, 4);
        
        x = ((uint16_t)buf[1] << 8) + buf[0];
        y = ((uint16_t)buf[3] << 8) + buf[2];
        
        printf("x:%d y:%d\r\n",x,y);
    }
    
}






















