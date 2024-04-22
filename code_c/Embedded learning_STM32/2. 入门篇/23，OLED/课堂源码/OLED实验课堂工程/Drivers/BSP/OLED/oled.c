/**
 ****************************************************************************************************
 * @file        oled.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-22
 * @brief       OLED ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200421
 * ��һ�η���
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
        oled_wr_byte(0xb0 + i, OLED_CMD) ;  /* ����ҳ��ַ��0~7��*/
        oled_wr_byte(0x00, OLED_CMD) ;      /* ������ʾλ��-�е͵�ַ */ 
        oled_wr_byte(0x10, OLED_CMD) ;      /* ������ʾλ��-�иߵ�ַ */
        
        for (n = 0; n < 128; n++)
        {
            oled_wr_byte( g_oled_gram[ n ][ i ], OLED_DATA) ;
        }
    }
}

void oled_draw_point(uint8_t  x, uint8_t  y, uint8_t  dot) 
{
    uint8_t pos, bx, temp = 0;
    
    if (x > 127 || y > 63)  return;    /* ������Χ�� */
    
    pos = y / 8;    /*  ҳ��ַ */
    bx = y % 8;     /*  ����y�ڶ�Ӧ�ֽ������λ�� */
    temp = 1 << bx; /*  ת����y��Ӧ��bitλ�� */

    if ( dot )  /*  ��ʵ�ĵ� */
        g_oled_gram[ x ][ pos ] |= temp;
    else
        g_oled_gram[ x ][ pos ] &= ~temp;
}

/* 16*16��С���ַ�A�ĵ����������飺*/
uint8_t oled_ascii_1608[]=
{
  0x00,0x04,0x00,0x3C,0x03,0xC4,0x1C,0x40,
  0x07,0x40,0x00,0xE4,0x00,0x1C,0x00,0x04
} ;

void oled_show_char_test(uint8_t  x, uint8_t  y, uint8_t mode)
{
    uint8_t temp, t1, t;
    uint8_t y0 = y;                 /* ����y�ĳ�ֵ */

    for(t = 0; t < 16; t++)         /* �ܹ�16���ֽڣ�Ҫ����һ�� */
    {
        temp = oled_ascii_1608[t];  /* ���λ�ȡ�������� */

        for(t1 = 0; t1 < 8; t1++)
        {
            if(temp & 0X80)     /* �������Ч����Ҫ������ */
                oled_draw_point(x, y, mode);
            else                /* �������Ч������Ҫ������ */
                oled_draw_point(x, y, !mode);

            temp <<= 1;         /* ��λ��������λ��λ�����λ����ֱ�Ӷ��� */
            y++;                /* y�������� */

            if((y - y0) == 16)  /* ��ʾ��һ���� */
            {
                y = y0;         /* y���긴λ */
                x++;            /* x������� */
                break;          /* ���� forѭ�� */
            }
        }
    }
}


/**
 * @brief       ��ʼ��OLED(SSD1306)
 * @param       ��
 * @retval      ��
 */
void oled_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_GPIOC_CLK_ENABLE();     /* ʹ��PORTCʱ�� */
    __HAL_RCC_GPIOD_CLK_ENABLE();     /* ʹ��PORTDʱ�� */
    __HAL_RCC_GPIOG_CLK_ENABLE();     /* ʹ��PORTGʱ�� */
    
    /* PC0 ~ 7 ���� */
    gpio_init_struct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;                
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;        /* ���� */
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);                /* PC0 ~ 7 ���� */

    gpio_init_struct.Pin = GPIO_PIN_3|GPIO_PIN_6;           /* PD3, PD6 ���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;        /* ���� */
    HAL_GPIO_Init(GPIOD, &gpio_init_struct);                /* PD3, PD6 ���� */
    
    gpio_init_struct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;        /* ���� */
    HAL_GPIO_Init(GPIOG, &gpio_init_struct);                /* WR/RD/RST����ģʽ���� */

    OLED_WR(1);
    OLED_RD(1);

    OLED_CS(1);
    OLED_RS(1);
    
    /* ��λʱ�� */
    OLED_RST(0);
    delay_ms(100);
    OLED_RST(1);

    oled_wr_byte(0xAE, OLED_CMD);   /* �ر���ʾ */
    oled_wr_byte(0xD5, OLED_CMD);   /* ����ʱ�ӷ�Ƶ����,��Ƶ�� */
    oled_wr_byte(80, OLED_CMD);     /* [3:0],��Ƶ����;[7:4],��Ƶ�� */
    oled_wr_byte(0xA8, OLED_CMD);   /* ��������·�� */
    oled_wr_byte(0X3F, OLED_CMD);   /* Ĭ��0X3F(1/64) */
    oled_wr_byte(0xD3, OLED_CMD);   /* ������ʾƫ�� */
    oled_wr_byte(0X00, OLED_CMD);   /* Ĭ��Ϊ0 */

    oled_wr_byte(0x40, OLED_CMD);   /* ������ʾ��ʼ�� [5:0],����. */

    oled_wr_byte(0x8D, OLED_CMD);   /* ��ɱ����� */
    oled_wr_byte(0x14, OLED_CMD);   /* bit2������/�ر� */
    oled_wr_byte(0x20, OLED_CMD);   /* �����ڴ��ַģʽ */
    oled_wr_byte(0x02, OLED_CMD);   /* [1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10; */
    oled_wr_byte(0xA1, OLED_CMD);   /* ���ض�������,bit0:0,0->0;1,0->127; */
    oled_wr_byte(0xC8, OLED_CMD);   /* ����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·�� */
    oled_wr_byte(0xDA, OLED_CMD);   /* ����COMӲ���������� */
    oled_wr_byte(0x12, OLED_CMD);   /* [5:4]���� */

    oled_wr_byte(0x81, OLED_CMD);   /* �Աȶ����� */
    oled_wr_byte(0xEF, OLED_CMD);   /* 1~255;Ĭ��0X7F (��������,Խ��Խ��) */
    oled_wr_byte(0xD9, OLED_CMD);   /* ����Ԥ������� */
    oled_wr_byte(0xf1, OLED_CMD);   /* [3:0],PHASE 1;[7:4],PHASE 2; */
    oled_wr_byte(0xDB, OLED_CMD);   /* ����VCOMH ��ѹ���� */
    oled_wr_byte(0x30, OLED_CMD);   /* [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc; */

    oled_wr_byte(0xA4, OLED_CMD);   /* ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����) */
    oled_wr_byte(0xA6, OLED_CMD);   /* ������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ */
    oled_wr_byte(0xAF, OLED_CMD);   /* ������ʾ */
    
}

void oled_data_out(uint8_t data)
{
    GPIOC->ODR = (GPIOC->ODR & 0XFF00) | (data & 0X00FF);
}


static void oled_wr_byte(uint8_t data, uint8_t cmd)
{
    OLED_RS (cmd);  /* �������ͣ��ɴ��ξ��� */
    OLED_CS ( 0 );  /* ����Ƭѡ�ߣ�ѡ��SSD1306 */
    OLED_WR ( 0 );  /* ����WR�ߣ�׼������ */
    oled_data_out(data); /* WR�͵�ƽ�ڼ䣬׼������ */
    OLED_WR ( 1 );  /* ��WR�����أ����ݷ��� */
    OLED_CS ( 1 );  /* ȡ��Ƭѡ */
    OLED_RS ( 1 );  /* �ͷ�RS�ߣ��ָ�Ĭ�� */
}

void oled_draw_point_test(uint8_t x, uint8_t y)
{
    /* ҳ��ַģʽ */
    uint8_t page_num = y / 8;
    
    /* 1������ҳ��ַ */
    oled_wr_byte(0xB0 | page_num, OLED_CMD);
    
    /* 2�������е�ַ */
    oled_wr_byte((x & 0x0F) | 0x00, OLED_CMD);      /* �е�ַ����λ */
    oled_wr_byte((x & 0xF0) >> 4 | 0x10, OLED_CMD); /* �е�ַ����λ */
    
    /* 3������1�ֽ����� */
    oled_wr_byte(1 << (y % 8), OLED_DATA);
}

void oled_clear(void)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)
    {
        oled_wr_byte (0xb0 + i, OLED_CMD); /* ����ҳ��ַ��0~7�� */
        oled_wr_byte (0x00, OLED_CMD);     /* ������ʾλ�á��е͵�ַ */
        oled_wr_byte (0x10, OLED_CMD);     /* ������ʾλ�á��иߵ�ַ */

        for (n = 0; n < 128; n++)
        {
            oled_wr_byte(0x00, OLED_DATA);
        }
    }
}




















