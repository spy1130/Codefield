#include "stdio.h"
#include "stdlib.h"
#include "./BSP/TOUCH/touch.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       SPI写数据
 *   @note      向触摸屏IC写入1 byte数据
 * @param       data: 要写入的数据
 * @retval      无
 */
static void tp_write_byte(uint8_t data)
{
    uint8_t count = 0;

    for (count = 0; count < 8; count++)
    {
        if (data & 0x80)    /* 发送1 */
        {
            T_MOSI(1);
        }
        else                /* 发送0 */
        {
            T_MOSI(0);
        }

        data <<= 1;
        T_CLK(0);
        delay_us(1);
        T_CLK(1);           /* 上升沿有效 */
    }
}

/**
 * @brief       SPI读数据
 *   @note      从触摸屏IC读取adc值
 * @param       cmd: 指令
 * @retval      读取到的数据,ADC值(12bit)
 */
static uint16_t tp_read_ad(uint8_t cmd)
{
    uint8_t count = 0;
    uint16_t num = 0;
    
    T_CLK(0);           /* 先拉低时钟 */
    T_MOSI(0);          /* 拉低数据线 */
    
    T_CS(0);            /* 选中触摸屏IC */
    tp_write_byte(cmd); /* 发送命令字 */
    delay_us(6);        /* ADS7846的转换时间最长为6us */
    
    T_CLK(0);
    delay_us(1);
    T_CLK(1);           /* 给1个时钟，清除BUSY */
    delay_us(1);
    T_CLK(0);

    for (count = 0; count < 16; count++)    /* 读出16位数据,只有高12位有效 */
    {
        num <<= 1;
        T_CLK(0);       /* 下降沿有效 */
        delay_us(1);
        T_CLK(1);

        if (T_MISO)num++;
    }

    num >>= 4;          /* 只有高12位有效. */
    T_CS(1);            /* 释放片选 */
    return num;
}



/**
 * @brief       触摸屏初始化
 * @param       无
 * @retval      0,没有进行校准
 *              1,进行过校准
 */
uint8_t tp_init(void)
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

    return 1;
}









