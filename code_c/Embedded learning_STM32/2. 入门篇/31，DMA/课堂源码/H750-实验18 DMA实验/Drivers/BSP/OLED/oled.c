/**
 ****************************************************************************************************
 * @file        oled.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-22
 * @brief       OLED ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32H750������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200322
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "stdlib.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/OLED/oledfont.h"
#include "./SYSTEM/delay/delay.h"

/* 
 * OLED���Դ�
 * ÿ���ֽڱ�ʾ8������, 128,��ʾ��128��, 8��ʾ��64��, ��λ��ʾ������. 
 * ����:g_oled_gram[0][0],�����˵�һ��,��1~8�е�����. g_oled_gram[0][0].0,����ʾ����(0,0)
 * ���Ƶ�: g_oled_gram[1][0].1,��ʾ����(1,1), g_oled_gram[10][1].2,��ʾ����(10,10), 
 * 
 * ��Ÿ�ʽ����(��λ��ʾ������).
 * [0]0 1 2 3 ... 127
 * [1]0 1 2 3 ... 127
 * [2]0 1 2 3 ... 127
 * [3]0 1 2 3 ... 127
 * [4]0 1 2 3 ... 127
 * [5]0 1 2 3 ... 127
 * [6]0 1 2 3 ... 127
 * [7]0 1 2 3 ... 127
 */
static uint8_t g_oled_gram[128][8];

/**
 * @brief       �����Դ浽OLED
 * @param       ��
 * @retval      ��
 */
void oled_refresh_gram(void)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)
    {
        oled_wr_byte (0xb0 + i, OLED_CMD); /* ����ҳ��ַ��0~7�� */
        oled_wr_byte (0x00, OLED_CMD);     /* ������ʾλ�á��е͵�ַ */
        oled_wr_byte (0x10, OLED_CMD);     /* ������ʾλ�á��иߵ�ַ */

        for (n = 0; n < 128; n++)
        {
            oled_wr_byte(g_oled_gram[n][i], OLED_DATA);
        }
    }
}

#if OLED_MODE == 1    /* ʹ��8080��������OLED */

/**
 * @brief       ͨ��ƴ�յķ�����OLED���һ��8λ����
 * @param       data: Ҫ���������
 * @retval      ��
 */
static void oled_data_out(uint8_t data)
{
    uint16_t dat = data & 0X0F;
    GPIOC->ODR &= ~(0XF << 6);      /* ���6~9 */
    GPIOC->ODR |= dat << 6;         /* D[3:0]-->PC[9:6] */
    
    GPIOC->ODR &= ~(0X1 << 11);              /* ���11 */
    GPIOC->ODR |= ((data >> 4) & 0x01) << 11;/* D4 */
    
    GPIOD->ODR &= ~(0X1 << 3);               /* ���3 */
    GPIOD->ODR |= ((data >> 5) & 0x01) << 3; /* D5 */
    
    GPIOB->ODR &= ~(0X3<<8);                 /* ���8,9 */
    GPIOB->ODR |= ((data >> 6) & 0x01) << 8; /* D6 */
    GPIOB->ODR |= ((data >> 7) & 0x01) << 9; /* D7 */
}

/**
 * @brief       ��OLEDд��һ���ֽ�
 * @param       data: Ҫ���������
 * @param       cmd: ����/�����־ 0,��ʾ����;1,��ʾ����;
 * @retval      ��
 */
static void oled_wr_byte(uint8_t data, uint8_t cmd)
{
    oled_data_out(data);
    OLED_RS(cmd);
    OLED_CS(0);
    OLED_WR(0);
    OLED_WR(1);
    OLED_CS(1);
    OLED_RS(1);
}

#else   /* ʹ��SPI����OLED */

/**
 * @brief       ��OLEDд��һ���ֽ�
 * @param       data: Ҫ���������
 * @param       cmd: ����/�����־ 0,��ʾ����;1,��ʾ����;
 * @retval      ��
 */
static void oled_wr_byte(uint8_t data, uint8_t cmd)
{
    uint8_t i;
    OLED_RS(cmd);   /* д���� */
    OLED_CS(0);

    for (i = 0; i < 8; i++)
    {
        OLED_SCLK(0);

        if (data & 0x80)
        {
            OLED_SDIN(1);
        }
        else 
        {
            OLED_SDIN(0);
        }
        OLED_SCLK(1);
        data <<= 1;
    }

    OLED_CS(1);
    OLED_RS(1);
}

#endif

/**
 * @brief       ����OLED��ʾ
 * @param       ��
 * @retval      ��
 */
void oled_display_on(void)
{
    oled_wr_byte(0X8D, OLED_CMD);   /* SET DCDC���� */
    oled_wr_byte(0X14, OLED_CMD);   /* DCDC ON */
    oled_wr_byte(0XAF, OLED_CMD);   /* DISPLAY ON */
}
 
/**
 * @brief       �ر�OLED��ʾ
 * @param       ��
 * @retval      ��
 */
void oled_display_off(void)
{
    oled_wr_byte(0X8D, OLED_CMD);   /* SET DCDC���� */
    oled_wr_byte(0X10, OLED_CMD);   /* DCDC OFF */
    oled_wr_byte(0XAE, OLED_CMD);   /* DISPLAY OFF */
}

/**
 * @brief       ��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
 * @param       ��
 * @retval      ��
 */
void oled_clear(void)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)for (n = 0; n < 128; n++)g_oled_gram[n][i] = 0X00;

    oled_refresh_gram();    /* ������ʾ */
}

/**
 * @brief       OLED���� 
 * @param       x  : 0~127
 * @param       y  : 0~63
 * @param       dot: 1 ��� 0,���
 * @retval      ��
 */ 
void oled_draw_point(uint8_t x, uint8_t y, uint8_t dot)
{
    uint8_t pos, bx, temp = 0;

    if (x > 127 || y > 63) return;  /* ������Χ�� */ 
    
    pos = y / 8;        /* ����GRAM�����y�������ڵ��ֽ�, ÿ���ֽڿ��Դ洢8�������� */
    
    bx = y % 8;         /* ȡ����,�������y�ڶ�Ӧ�ֽ������λ��,����(y)λ�� */
    temp = 1 << bx;     /* ��λ��ʾ���к�, �õ�y��Ӧ��bitλ��,����bit����1 */

    if (dot)    /* ��ʵ�ĵ� */
    {
        g_oled_gram[x][pos] |= temp;
    }
    else        /* ���յ�,������ʾ */
    {
        g_oled_gram[x][pos] &= ~temp;
    }
}

/**
 * @brief       OLED���������� 
 *   @note:     ע��:��Ҫȷ��: x1<=x2; y1<=y2  0<=x1<=127  0<=y1<=63
 * @param       x1,y1: �������
 * @param       x2,y2: �յ����� 
 * @param       dot: 1 ��� 0,���
 * @retval      ��
 */ 
void oled_fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot)
{
    uint8_t x, y;

    for (x = x1; x <= x2; x++)
    {
        for (y = y1; y <= y2; y++)oled_draw_point(x, y, dot);
    }

    oled_refresh_gram();    /* ������ʾ */
}

/**
 * @brief       ��ָ��λ����ʾһ���ַ�,���������ַ� 
 * @param       x   : 0~127
 * @param       y   : 0~63
 * @param       size: ѡ������ 12/16/24
 * @param       mode: 0,������ʾ;1,������ʾ
 * @retval      ��
 */ 
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    uint8_t *pfont = 0;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); /* �õ�����һ���ַ���Ӧ������ռ���ֽ��� */
    chr = chr - ' ';        /* �õ�ƫ�ƺ��ֵ,��Ϊ�ֿ��Ǵӿո�ʼ�洢��,��һ���ַ��ǿո� */

    if (size == 12)         /* ����1206���� */
    {
        pfont = (uint8_t *)oled_asc2_1206[chr];        
    }
    else if (size == 16)    /* ����1608���� */ 
    {
        pfont = (uint8_t *)oled_asc2_1608[chr];
    }
    else if (size == 24)    /* ����2412���� */
    {
        pfont = (uint8_t *)oled_asc2_2412[chr];
    }
    else                    /* û�е��ֿ� */
    {
        return;   
    }
    
    for (t = 0; t < csize; t++)
    { 
        temp = pfont[t];
        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)oled_draw_point(x, y, mode);
            else oled_draw_point(x, y, !mode);

            temp <<= 1;
            y++;

            if ((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

/**
 * @brief       ƽ������, m^n
 * @param       m: ����
 * @param       n: ָ�� 
 * @retval      ��
 */
static uint32_t oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while (n--)
    {
        result *= m;
    }

    return result;
}

/**
 * @brief       ��ʾlen������ 
 * @param       x,y : ��ʼ����
 * @param       num : ��ֵ(0 ~ 2^32)
 * @param       len : ��ʾ���ֵ�λ��
 * @param       size: ѡ������ 12/16/24
 * @retval      ��
 */  
void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)  /* ������ʾλ��ѭ�� */ 
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;  /* ��ȡ��Ӧλ������ */

        if (enshow == 0 && t < (len - 1))   /* û��ʹ����ʾ,�һ���λҪ��ʾ */
        {
            if (temp == 0)
            {
                oled_show_char(x + (size / 2)*t, y, ' ', size, 1); /* ��ʾ�ո�,վλ */
                continue;   /* �����¸�һλ */
            }
            else
            {
                enshow = 1; /* ʹ����ʾ */
            }
        }

        oled_show_char(x + (size / 2)*t, y, temp + '0', size, 1);    /* ��ʾ�ַ� */
    }
} 

/**
 * @brief       ��ʾ�ַ��� 
 * @param       x,y : ��ʼ����
 * @param       size: ѡ������ 12/16/24
 * @param       *p  : �ַ���ָ��,ָ���ַ����׵�ַ 
 * @retval      ��
 */ 
void oled_show_string(uint8_t x, uint8_t y, const char *p, uint8_t size)
{
    while ((*p <= '~') && (*p >= ' '))  /* �ж��ǲ��ǷǷ��ַ�! */
    {
        if (x > (128 - (size / 2)))     /* ���Խ�� */
        {
            x = 0;
            y += size;      /* ���� */
        }

        if (y > (64 - size))/* �߶�Խ�� */
        {
            y = x = 0;
            oled_clear();  
        }

        oled_show_char(x, y, *p, size, 1);   /* ��ʾһ���ַ� */
        x += size / 2;      /* ASCII�ַ����Ϊ���ֿ�ȵ�һ�� */
        p++;
    }
}

/**
 * @brief       ��ʼ��OLED(SSD1306)
 * @param       ��
 * @retval      ��
 */
void oled_init(void)
{
    GPIO_InitTypeDef  gpio_init_struct;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

#if OLED_MODE==1         /* ʹ��8080����ģʽ */

    gpio_init_struct.Pin=GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8; /* PA4,6,7,8���� */
    gpio_init_struct.Mode=GPIO_MODE_OUTPUT_PP;          /* ������� */
    gpio_init_struct.Pull=GPIO_PULLUP;                  /* ���� */
    gpio_init_struct.Speed=GPIO_SPEED_FREQ_VERY_HIGH;   /* ���� */
    HAL_GPIO_Init(GPIOA,&gpio_init_struct);

    /* PB7,8,9,10,11���� */
    gpio_init_struct.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
    HAL_GPIO_Init(GPIOB,&gpio_init_struct);

    /* PC4,6~9,PC11���� */
    gpio_init_struct.Pin=GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11;
    HAL_GPIO_Init(GPIOC,&gpio_init_struct);

    /* PD3 ���� */
    gpio_init_struct.Pin=GPIO_PIN_3;
    HAL_GPIO_Init(GPIOD,&gpio_init_struct);

    OLED_WR(1);
    OLED_RD(1);

#else               /* ʹ��4��SPI ����ģʽ */

    gpio_init_struct.Pin=OLED_SPI_RST_PIN;
    gpio_init_struct.Mode=GPIO_MODE_OUTPUT_PP;              /* ������� */
    gpio_init_struct.Pull=GPIO_PULLUP;                      /* ���� */
    gpio_init_struct.Speed=GPIO_SPEED_FREQ_VERY_HIGH;       /* ���� */
    HAL_GPIO_Init(OLED_SPI_RST_PORT,&gpio_init_struct);     /* RST����ģʽ���� */

    gpio_init_struct.Pin=OLED_SPI_CS_PIN;
    HAL_GPIO_Init(OLED_SPI_CS_PORT,&gpio_init_struct);      /* CS����ģʽ���� */

    gpio_init_struct.Pin=OLED_SPI_RS_PIN;
    HAL_GPIO_Init(OLED_SPI_RS_PORT,&gpio_init_struct);      /* RS����ģʽ���� */

    gpio_init_struct.Pin=OLED_SPI_SCLK_PIN;
    HAL_GPIO_Init(OLED_SPI_SCLK_PORT,&gpio_init_struct);    /* SCLK����ģʽ���� */
    
    gpio_init_struct.Pin=OLED_SPI_SDIN_PIN;
    HAL_GPIO_Init(OLED_SPI_SDIN_PORT,&gpio_init_struct);    /* SDIN����ģʽ���� */
    
    OLED_SDIN(1);
    OLED_SCLK(1);
#endif
    OLED_CS(1);
    OLED_RS(1);

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
    oled_clear();
}  
