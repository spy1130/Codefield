/**
 ****************************************************************************************************
 * @file        dht11.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       DHT11数字温湿度传感器 驱动代码
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

#include "./BSP/DHT11/dht11.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"

/**
 * @brief       初始化DHT11的IO口 DHT11_DQ 同时检测DHT11的存在
 * @param       无
 * @retval      0, 正常
 *              1, 不存在/不正常
 */
void dht11_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    DHT11_DQ_GPIO_CLK_ENABLE();     /* 开启DHT11_DQ引脚时钟 */

    gpio_init_struct.Pin = DHT11_DQ_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;            /* 开漏输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(DHT11_DQ_GPIO_PORT, &gpio_init_struct);   /* 初始化DHT11_DHT11_DQ引脚 */
    /* DHT11_DHT11_DQ引脚模式设置,开漏输出,上拉, 这样就不用再设置IO方向了, 开漏输出的时候(=1), 也可以读取外部信号的高低电平 */
}


void dht11_reset(void)
{
    DHT11_DQ_OUT(0);    /* 拉低DHT11_DQ */
    delay_ms(20);       /* 拉低至少18ms*/
    DHT11_DQ_OUT(1);    /* DHT11_DQ=1 */
    delay_us(13);       /* 主机拉高10~35us */
}

uint8_t dht11_check(void)
{
    uint8_t retry, rval = 0;

    while (DHT11_DQ_IN && retry < 100)
    {
        retry++;
        delay_us(1);
    }       /* DHT11会拉低数据线约83us*/

    if (retry >= 100)   /* 超时 */
        rval = 1;
    else
    {
        retry = 0;

        while (! DHT11_DQ_IN && retry < 100)
        {
            retry++;
            delay_us(1);
        }   /* DHT11拉低后会再次拉高约87us*/

        if (retry >= 100)
            rval = 1;   /* 超时 */
    }

    return rval;
}

uint8_t dht11_read_bit(void)
{
    uint8_t retry = 0;  /* 定义变量 */

    while (DHT11_DQ_IN && retry < 100)
    {
        retry++;
        delay_us(1);
    }   /* 等待变为低电平 */

    retry = 0;

    while (! DHT11_DQ_IN && retry < 100)
    {
        retry++;
        delay_us(1);
    }   /* 等待变为高电平*/

    delay_us(40);   /* 等待40us */

    if (DHT11_DQ_IN)
        return 1;
    else
        return 0;
}

uint8_t dht11_read_byte(void)
{
    uint8_t i, data = 0;

    for (i = 0; i < 8; i++)
    {
        /* 高位数据先输出，先左移一位 */
        data <<= 1;
        /* 读取1bit数据*/
        data |= dht11_read_bit();
    }

    return data;
}

uint8_t dht11_read_data(void)   /* return 0:succeed 1:failed */
{
    uint8_t i, buf[5] = {0};
    uint8_t t = 0;
    uint8_t h = 0;
    uint8_t ret = 1;
    
    /* 1、主机发出起始信号 */
    dht11_reset();
    
    /* 2、主机检测从机发出的响应信号 */
    dht11_check();
    
    /* 3、从机返回40bit数据，主机接收5byte数据 */
    for (i = 0; i < 5; i++)
    {
        buf[i] = dht11_read_byte();
    }
    
    /* 4、数据处理 */
    if (buf[4] == (buf[0] + buf[1] + buf[2] + buf[3]))
    {
        h = buf[0];
        t = buf[2];
        ret = 0;
    }
    
    printf("T:%d H:%d\r\n",t, h);
    
    return ret;
}



 

















