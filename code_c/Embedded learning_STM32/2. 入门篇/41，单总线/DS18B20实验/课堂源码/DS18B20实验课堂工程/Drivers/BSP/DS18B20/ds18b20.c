/**
 ****************************************************************************************************
 * @file        ds18b20.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       DS18B20数字温度传感器 驱动代码
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
 * V1.0 20200426
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/delay/delay.h"
#include "./BSP/DS18B20/ds18b20.h"

/**
 * @brief       初始化DS18B20的IO口 DS18B20_DQ 同时检测DS18B20的存在
 * @param       无
 */
void ds18b20_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    DS18B20_DQ_GPIO_CLK_ENABLE();   /* 开启DS18B20_DQ引脚时钟 */

    gpio_init_struct.Pin = DS18B20_DQ_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;            /* 开漏输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(DS18B20_DQ_GPIO_PORT, &gpio_init_struct); /* 初始化DS18B20_DQ引脚 */
    /* DS18B20_DQ引脚模式设置,开漏输出,上拉, 这样就不用再设置IO方向了, 开漏输出的时候(=1), 也可以读取外部信号的高低电平 */
}

void ds18b20_reset(void)
{
    DS18B20_DQ_OUT(0);  /* 拉低DS18B20_DQ, 复位 */
    delay_us(750);      /* 延时750us */
    DS18B20_DQ_OUT(1);  /* DS18B20_DQ=1, 释放总线 */
    delay_us(15);       /* 延时15us */
}

uint8_t  ds18b20_check(void)    /* return 0:succeed   1:fail */
{
    uint8_t retry, rval = 0;    /* 定义变量 */

    while (DS18B20_DQ_IN && retry < 200)
    {
        retry++;
        delay_us(1);
    }   /* 等待DS18B20_DQ变低，等待200us*/

    if (retry >= 200)   /* 超时 */
    {
        rval = 1;
    }
    else
    {
        retry = 0;

        while (! DS18B20_DQ_IN && retry < 240)
        {
            retry++;
            delay_us(1);
        }   /* 等待DS18B20_DQ变高，等待240us */

        if (retry >= 240)   /* 超时 */
        {
            rval = 1;
        }
    }

    return rval;
}

void ds18b20_write_0(void)
{
    DS18B20_DQ_OUT(0);  /* 拉低DS18B20_DQ */
    delay_us(60);       /* 延时60us */
    DS18B20_DQ_OUT(1);  /* 释放DS18B20_DQ */
    delay_us(2);        /* 延时2us */
}

void ds18b20_write_1(void)
{
    DS18B20_DQ_OUT(0);  /* 拉低DS18B20_DQ */
    delay_us(2);        /* 延时2us */
    DS18B20_DQ_OUT(1);  /* 释放DS18B20_DQ */
    delay_us(60);       /* 延时60us */
}

void ds18b20_write_byte(uint8_t data)
{
    uint8_t j;

    for (j = 0; j < 8; j++)
    {
        if (data & 0x01)
        {
            ds18b20_write_1();  /* Write 1*/
        }
        else
        {
            ds18b20_write_0();  /* Write 0*/
        }

        data >>= 1;     /* 右移，获取高一位数据 */
    }
}

uint8_t  ds18b20_read_bit(void)
{
    uint8_t data = 0;

    DS18B20_DQ_OUT(0);  /* 拉低DS18B20_DQ */
    delay_us(2);        /* 延时2us */
    DS18B20_DQ_OUT(1);  /* 释放DS18B20_DQ */
    delay_us(12);       /* 延时12us */

    if (DS18B20_DQ_IN)
    {
        data = 1;
    }
    
    delay_us(50);
    return data;
}

uint8_t  ds18b20_read_byte(void)
{
    uint8_t i, b, data = 0;

    for (i = 0; i < 8; i++)
    {
        /* 先输出低位数据 ,高位数据后输出 */
        b = ds18b20_read_bit();
        /* 填充data的每一位 */
        data |= b << i;
    }

    return data;
}

float ds18b20_get_temperature(void)
{
    uint8_t TL, TH;
    uint16_t temp = 0;
    float temperature = 0;
    
    /* 1、初始化 */
    ds18b20_reset();
    ds18b20_check();
    
    /* 2、发送ROM命令 */
    ds18b20_write_byte(0xCC);
    
    /* 3、发送DS18B20操作命令 */
    ds18b20_write_byte(0x44);
    
    /* 4、初始化 */
    ds18b20_reset();
    ds18b20_check();
    
    /* 5、发送ROM命令 */
    ds18b20_write_byte(0xCC);
    
    /* 6、发送DS18B20操作命令 */
    ds18b20_write_byte(0xBE);   /* 读取RAM命令 */
    
    /* 7、读取两个字节数据 */
    TL = ds18b20_read_byte();
    TH = ds18b20_read_byte();
    
    /* 8、温度数据运算 (正温度)*/
    temp = TL + (TH << 8);
}



