/**
 ****************************************************************************************************
 * @file        oled.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-22
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

#include "stdlib.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/OLED/oledfont.h"
#include "./SYSTEM/delay/delay.h"

static uint8_t g_oled_gram[128][8];

void oled_refresh_gram(void)
{
    uint8_t i,n;

    for (i = 0; i < 8; i++)
    {
        oled_wr_byte(0xb0 + i, OLED_CMD) ;  /* 设置页地址（0~7）*/
        oled_wr_byte(0x00, OLED_CMD) ;      /* 设置显示位置-列低地址 */ 
        oled_wr_byte(0x10, OLED_CMD) ;      /* 设置显示位置-列高地址 */
        
        for (n = 0; n < 128; n++)
        {
            oled_wr_byte( g_oled_gram[ n ][ i ], OLED_DATA) ;
        }
    }
}

void oled_draw_point(uint8_t  x, uint8_t  y, uint8_t  dot) 
{
    uint8_t pos, bx, temp = 0;
    
    if (x > 127 || y > 63)  return;    /* 超出范围了 */
    
    pos = y / 8;    /*  页地址 */
    bx = y % 8;     /*  计算y在对应字节里面的位置 */
    temp = 1 << bx; /*  转换后y对应的bit位置 */

    if ( dot )  /*  画实心点 */
        g_oled_gram[ x ][ pos ] |= temp;
    else
        g_oled_gram[ x ][ pos ] &= ~temp;
}

/* 16*16大小，字符A的点阵数据数组：*/
uint8_t oled_ascii_1608[]=
{
  0x00,0x04,0x00,0x3C,0x03,0xC4,0x1C,0x40,
  0x07,0x40,0x00,0xE4,0x00,0x1C,0x00,0x04
} ;

void oled_show_char_test(uint8_t  x, uint8_t  y, uint8_t mode)
{
    uint8_t temp, t1, t;
    uint8_t y0 = y;                 /* 保存y的初值 */

    for(t = 0; t < 16; t++)         /* 总共16个字节，要遍历一遍 */
    {
        temp = oled_ascii_1608[t];  /* 依次获取点阵数据 */

        for(t1 = 0; t1 < 8; t1++)
        {
            if(temp & 0X80)     /* 这个点有效，需要画出来 */
                oled_draw_point(x, y, mode);
            else                /* 这个点无效，不需要画出来 */
                oled_draw_point(x, y, !mode);

            temp <<= 1;         /* 低位数据往高位移位，最高位数据直接丢弃 */
            y++;                /* y坐标自增 */

            if((y - y0) == 16)  /* 显示完一列了 */
            {
                y = y0;         /* y坐标复位 */
                x++;            /* x坐标递增 */
                break;          /* 跳出 for循环 */
            }
        }
    }
}


/**
 * @brief       初始化OLED(SSD1306)
 * @param       无
 * @retval      无
 */
void oled_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_GPIOC_CLK_ENABLE();     /* 使能PORTC时钟 */
    __HAL_RCC_GPIOD_CLK_ENABLE();     /* 使能PORTD时钟 */
    __HAL_RCC_GPIOG_CLK_ENABLE();     /* 使能PORTG时钟 */
    
    /* PC0 ~ 7 设置 */
    gpio_init_struct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;                
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;        /* 中速 */
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);                /* PC0 ~ 7 设置 */

    gpio_init_struct.Pin = GPIO_PIN_3|GPIO_PIN_6;           /* PD3, PD6 设置 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;        /* 中速 */
    HAL_GPIO_Init(GPIOD, &gpio_init_struct);                /* PD3, PD6 设置 */
    
    gpio_init_struct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;        /* 中速 */
    HAL_GPIO_Init(GPIOG, &gpio_init_struct);                /* WR/RD/RST引脚模式设置 */

    OLED_WR(1);
    OLED_RD(1);

    OLED_CS(1);
    OLED_RS(1);
    
    /* 复位时序 */
    OLED_RST(0);
    delay_ms(100);
    OLED_RST(1);

    oled_wr_byte(0xAE, OLED_CMD);   /* 关闭显示 */
    oled_wr_byte(0xD5, OLED_CMD);   /* 设置时钟分频因子,震荡频率 */
    oled_wr_byte(80, OLED_CMD);     /* [3:0],分频因子;[7:4],震荡频率 */
    oled_wr_byte(0xA8, OLED_CMD);   /* 设置驱动路数 */
    oled_wr_byte(0X3F, OLED_CMD);   /* 默认0X3F(1/64) */
    oled_wr_byte(0xD3, OLED_CMD);   /* 设置显示偏移 */
    oled_wr_byte(0X00, OLED_CMD);   /* 默认为0 */

    oled_wr_byte(0x40, OLED_CMD);   /* 设置显示开始行 [5:0],行数. */

    oled_wr_byte(0x8D, OLED_CMD);   /* 电荷泵设置 */
    oled_wr_byte(0x14, OLED_CMD);   /* bit2，开启/关闭 */
    oled_wr_byte(0x20, OLED_CMD);   /* 设置内存地址模式 */
    oled_wr_byte(0x02, OLED_CMD);   /* [1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10; */
    oled_wr_byte(0xA1, OLED_CMD);   /* 段重定义设置,bit0:0,0->0;1,0->127; */
    oled_wr_byte(0xC8, OLED_CMD);   /* 设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数 */
    oled_wr_byte(0xDA, OLED_CMD);   /* 设置COM硬件引脚配置 */
    oled_wr_byte(0x12, OLED_CMD);   /* [5:4]配置 */

    oled_wr_byte(0x81, OLED_CMD);   /* 对比度设置 */
    oled_wr_byte(0xEF, OLED_CMD);   /* 1~255;默认0X7F (亮度设置,越大越亮) */
    oled_wr_byte(0xD9, OLED_CMD);   /* 设置预充电周期 */
    oled_wr_byte(0xf1, OLED_CMD);   /* [3:0],PHASE 1;[7:4],PHASE 2; */
    oled_wr_byte(0xDB, OLED_CMD);   /* 设置VCOMH 电压倍率 */
    oled_wr_byte(0x30, OLED_CMD);   /* [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc; */

    oled_wr_byte(0xA4, OLED_CMD);   /* 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏) */
    oled_wr_byte(0xA6, OLED_CMD);   /* 设置显示方式;bit0:1,反相显示;0,正常显示 */
    oled_wr_byte(0xAF, OLED_CMD);   /* 开启显示 */
    
}

void oled_data_out(uint8_t data)
{
    GPIOC->ODR = (GPIOC->ODR & 0XFF00) | (data & 0X00FF);
}


static void oled_wr_byte(uint8_t data, uint8_t cmd)
{
    OLED_RS (cmd);  /* 数据类型，由传参决定 */
    OLED_CS ( 0 );  /* 拉低片选线，选中SSD1306 */
    OLED_WR ( 0 );  /* 拉低WR线，准备数据 */
    oled_data_out(data); /* WR低电平期间，准备数据 */
    OLED_WR ( 1 );  /* 在WR上升沿，数据发出 */
    OLED_CS ( 1 );  /* 取消片选 */
    OLED_RS ( 1 );  /* 释放RS线，恢复默认 */
}

void oled_draw_point_test(uint8_t x, uint8_t y)
{
    /* 页地址模式 */
    uint8_t page_num = y / 8;
    
    /* 1、发送页地址 */
    oled_wr_byte(0xB0 | page_num, OLED_CMD);
    
    /* 2、发送列地址 */
    oled_wr_byte((x & 0x0F) | 0x00, OLED_CMD);      /* 列地址低四位 */
    oled_wr_byte((x & 0xF0) >> 4 | 0x10, OLED_CMD); /* 列地址高四位 */
    
    /* 3、发送1字节数据 */
    oled_wr_byte(1 << (y % 8), OLED_DATA);
}

void oled_clear(void)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)
    {
        oled_wr_byte (0xb0 + i, OLED_CMD); /* 设置页地址（0~7） */
        oled_wr_byte (0x00, OLED_CMD);     /* 设置显示位置—列低地址 */
        oled_wr_byte (0x10, OLED_CMD);     /* 设置显示位置—列高地址 */

        for (n = 0; n < 128; n++)
        {
            oled_wr_byte(0x00, OLED_DATA);
        }
    }
}




















