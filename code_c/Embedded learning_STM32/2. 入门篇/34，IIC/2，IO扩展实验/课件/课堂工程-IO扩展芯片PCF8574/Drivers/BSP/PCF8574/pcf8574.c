/**
 ****************************************************************************************************
 * @file        pcf8574.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-04-20
 * @brief       PCF8574 驱动代码
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

#include "./BSP/PCF8574/pcf8574.h"
#include "./SYSTEM/delay/delay.h"

/**
 * @brief       初始化PCF8574
 * @param       无
 * @retval      0, 成功;
                1, 失败;
 */
uint8_t pcf8574_init(void)
{
    uint8_t temp = 0;
    GPIO_InitTypeDef gpio_init_struct;
    
    PCF8574_GPIO_CLK_ENABLE();                               /* 使能GPIOB时钟 */

    gpio_init_struct.Pin = PCF8574_GPIO_PIN;                 /* PB12 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                 /* 输入 */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;                /* 高速 */
    HAL_GPIO_Init(PCF8574_GPIO_PORT, &gpio_init_struct);     /* 初始化 */
    
    iic_init();                                              /* IIC初始化 */

    /* 检查PCF8574是否在位 */
    iic_start();
    iic_send_byte(PCF8574_ADDR);                             /* 写地址 */
    temp = iic_wait_ack();                                   /* 等待应答,通过判断是否有ACK应答,来判断PCF8574的状态 */
    iic_stop();                                              /* 产生一个停止条件 */
    
    pcf8574_write_byte (0xFF);
    
    return temp;
}
 



uint8_t pcf8574_read_byte (void)
{ 
    uint8_t temp = 0;
    
    iic_start();                /* 发送起始信号 */
    iic_send_byte(0x41);        /* 发送读操作地址 */
    iic_wait_ack( );            /* 等待pcf8574的应答 */
    temp = iic_read_byte(0);    /* 读取1Byte数据并发nACK信号 */
    iic_stop ();                /* 发送停止信号 */
    return temp;
}



void pcf8574_write_byte (uint8_t data)
{ 
    iic_start();            /* 发送起始信号 */
    iic_send_byte(0x40);    /* 发送写操作地址 */
    iic_wait_ack( );        /* 等待pcf8574的应答 */
    iic_send_byte(data);    /* 发送1Byte数据设置pcf8574的IO状态 */
    iic_wait_ack( );        /* 等待pcf8574的应答 */	
    iic_stop ();            /* 发送停止信号 */
    delay_ms(10);
}


uint8_t pcf8574_read_bit(uint8_t bit)
{
    uint8_t temp = 0;
    
    temp = pcf8574_read_byte ();    /* temp是8个IO口的电平状态 */
    
    if (temp & (1 << bit))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void pcf8574_write_bit(uint8_t bit, uint8_t sta)
{
    uint8_t temp = 0;
    
    temp = pcf8574_read_byte ();    /* temp是8个IO口的电平状态 */
    
    if (sta)
    {
        temp |= (1 << bit);
    }
    else
    {
        temp &= ~(1 << bit);
    }
    
    pcf8574_write_byte (temp);
}







