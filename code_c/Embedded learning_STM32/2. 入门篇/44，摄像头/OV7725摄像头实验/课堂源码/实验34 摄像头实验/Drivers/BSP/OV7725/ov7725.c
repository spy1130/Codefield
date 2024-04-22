/**
 ****************************************************************************************************
 * @file        ov7725.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       OV7725 ��������
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
 * V1.0 20200426
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/OV7725/sccb.h"
#include "./BSP/OV7725/ov7725.h"
#include "./BSP/OV7725/ov7725cfg.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       OV7725 ���Ĵ���
 * @param       reg : �Ĵ�����ַ
 * @retval      �����ļĴ���ֵ
 */
uint8_t ov7725_read_reg(uint16_t reg)
{
    uint8_t data = 0;
    
    sccb_start();                   /* ��ʼ�ź� */
    sccb_send_byte(OV7725_ADDR);    /* д����дͨѶ��ַ */
    sccb_send_byte(reg);            /* д�Ĵ�����ַ */
    sccb_stop();                    /* ֹͣ�ź� */
    
    /* ���üĴ�����ַ�󣬲��Ƕ� */
    sccb_start();                   /* ��ʼ�ź� */
    sccb_send_byte(OV7725_ADDR | 0X01); /* ���Ͷ�ͨѶ��ַ */
    data = sccb_read_byte();        /* ��ȡ���� */
    sccb_nack();                    /* ��Ӧ�� */
    sccb_stop();                    /* ֹͣ�ź� */

    return data;
}

/**
 * @brief       OV7725 д�Ĵ���
 * @param       reg : �Ĵ�����ַ
 * @param       data: Ҫд��Ĵ�����ֵ
 * @retval      0, �ɹ�; 1, ʧ��;
 */
uint8_t ov7725_write_reg(uint8_t reg, uint8_t data)
{
    uint8_t res = 0;
    
    sccb_start();                           /* ��ʼ�ź� */

    if (sccb_send_byte(OV7725_ADDR))res = 1;/* д����дͨѶ��ַ */

    if (sccb_send_byte(reg))res = 1;        /* д�Ĵ�����ַ */

    if (sccb_send_byte(data))res = 1;       /* д���� */

    sccb_stop();                            /* ֹͣ�ź� */

    return res;
}

/**
 * @brief       ��ʼ�� OV7725
 * @param       ��
 * @retval      0, �ɹ�; 1, ʧ��;
 */
uint8_t ov7725_init(void)
{
    uint16_t i = 0;
    uint16_t reg = 0;

    OV7725_VSYNC_GPIO_CLK_ENABLE(); /* VSYNC����  GPIOʱ��ʹ�� */
    OV7725_WRST_GPIO_CLK_ENABLE();  /* WRST����   GPIOʱ��ʹ�� */
    OV7725_RRST_GPIO_CLK_ENABLE();  /* RRST����   GPIOʱ��ʹ�� */
    OV7725_OE_GPIO_CLK_ENABLE();    /* OE����     GPIOʱ��ʹ�� */
    OV7725_RCLK_GPIO_CLK_ENABLE();  /* RCLK����   GPIOʱ��ʹ�� */
    OV7725_WEN_GPIO_CLK_ENABLE();   /* WEN����    GPIOʱ��ʹ�� */
    __HAL_RCC_GPIOC_CLK_ENABLE();   /* C0~C7����  GPIOʱ��ʹ�� */

    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin=OV7725_VSYNC_GPIO_PIN;
    gpio_initure.Mode=GPIO_MODE_INPUT;
    gpio_initure.Pull=GPIO_PULLUP;
    gpio_initure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OV7725_VSYNC_GPIO_PORT,&gpio_initure);    /* VSYNC ����ģʽ���� */

    gpio_initure.Pin=OV7725_WRST_GPIO_PIN;
    gpio_initure.Mode=GPIO_MODE_OUTPUT_PP;
    gpio_initure.Pull=GPIO_PULLUP;
    gpio_initure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OV7725_WRST_GPIO_PORT,&gpio_initure);     /* WRST  ����ģʽ���� */
    
    gpio_initure.Pin=OV7725_RRST_GPIO_PIN;
    gpio_initure.Mode=GPIO_MODE_OUTPUT_PP;
    gpio_initure.Pull=GPIO_PULLUP;
    gpio_initure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OV7725_RRST_GPIO_PORT,&gpio_initure);     /* RRST  ����ģʽ���� */
    
    gpio_initure.Pin=OV7725_OE_GPIO_PIN;
    gpio_initure.Mode=GPIO_MODE_OUTPUT_PP;
    gpio_initure.Pull=GPIO_PULLUP;
    gpio_initure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OV7725_OE_GPIO_PORT,&gpio_initure);       /* OE    ����ģʽ���� */
    
    gpio_initure.Pin=OV7725_RCLK_GPIO_PIN;
    gpio_initure.Mode=GPIO_MODE_OUTPUT_PP;
    gpio_initure.Pull=GPIO_PULLUP;
    gpio_initure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OV7725_RCLK_GPIO_PORT,&gpio_initure);     /* RCLK  ����ģʽ���� */
    
    gpio_initure.Pin=OV7725_WEN_GPIO_PIN;
    gpio_initure.Mode=GPIO_MODE_OUTPUT_PP;
    gpio_initure.Pull=GPIO_PULLUP;
    gpio_initure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OV7725_WEN_GPIO_PORT,&gpio_initure);      /* WEN   ����ģʽ���� */
    
    gpio_initure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;                     
    gpio_initure.Mode = GPIO_MODE_INPUT;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &gpio_initure);        /* C0~C7 ����ģʽ���� */
    
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_SWJ_NOJTAG();              /* ��ֹJTAG, ʹ��SWD, �ͷ�PB3,PB4������������ͨIO�� */
    
    OV7725_WRST(1);             /* WRST =1 */
    OV7725_RRST(1);             /* RRST =1 */
    OV7725_OE(1);               /* OE =1 */
    OV7725_RCLK(1);             /* RCLK =1 */
    OV7725_WEN(1);              /* WEN =1 */

    sccb_init();                /* ��ʼ��SCCB ��IO�� */

    if (ov7725_write_reg(0x12, 0x80))
    {
        return 1;               /* ��λOV7725 */
    }

    delay_ms(50);
    reg = ov7725_read_reg(0X1c);    /* ��ȡ����ID �߰�λ */
    reg <<= 8;
    reg |= ov7725_read_reg(0X1d);   /* ��ȡ����ID �Ͱ�λ */

    if ((reg != OV7725_MID) && (reg != OV7725_MID1))    /* MID����ȷ ? */
    {
        printf("MID:%d\r\n", reg);
        return 1;
    }

    reg = ov7725_read_reg(0X0a);    /* ��ȡ��ƷID �߰�λ */
    reg <<= 8;
    reg |= ov7725_read_reg(0X0b);   /* ��ȡ��ƷID �Ͱ�λ */

    if (reg != OV7725_PID)          /* PID����ȷ ? */
    {
        printf("HID:%d\r\n", reg);
        return 2;
    }

    /* ��ʼ�� OV7725,����QVGA�ֱ���(320*240) */
    for (i = 0; i < sizeof(ov7725_init_reg_tb1) / sizeof(ov7725_init_reg_tb1[0]); i++)
    {
        ov7725_write_reg(ov7725_init_reg_tb1[i][0], ov7725_init_reg_tb1[i][1]);
    }

    return 0;   /* ok */
}

/**
 * @brief       OV7725 �ƹ�ģʽ����
 * @param       mode : 0~5, �ƹ�ģʽ
 *   @arg       0: �Զ�
 *   @arg       1: �չ�sunny
 *   @arg       2: ����cloudy
 *   @arg       3: �칫��office
 *   @arg       4: ����home
 *   @arg       5: ҹ��night
 * @retval      ��
 */
void ov7725_light_mode(uint8_t mode)
{
    switch (mode)
    {
        case 0: /* auto, �Զ�ģʽ */
            ov7725_write_reg(0x13, 0xff); /* AWB on */
            ov7725_write_reg(0x0e, 0x65);
            ov7725_write_reg(0x2d, 0x00);
            ov7725_write_reg(0x2e, 0x00);
            break;

        case 1: /* sunny, ���� */
            ov7725_write_reg(0x13, 0xfd); /* AWB off */
            ov7725_write_reg(0x01, 0x5a);
            ov7725_write_reg(0x02, 0x5c);
            ov7725_write_reg(0x0e, 0x65);
            ov7725_write_reg(0x2d, 0x00);
            ov7725_write_reg(0x2e, 0x00);
            break;

        case 2: /* cloudy, ���� */
            ov7725_write_reg(0x13, 0xfd); /* AWB off */
            ov7725_write_reg(0x01, 0x58);
            ov7725_write_reg(0x02, 0x60);
            ov7725_write_reg(0x0e, 0x65);
            ov7725_write_reg(0x2d, 0x00);
            ov7725_write_reg(0x2e, 0x00);
            break;

        case 3: /* office, �칫�� */
            ov7725_write_reg(0x13, 0xfd); /* AWB off */
            ov7725_write_reg(0x01, 0x84);
            ov7725_write_reg(0x02, 0x4c);
            ov7725_write_reg(0x0e, 0x65);
            ov7725_write_reg(0x2d, 0x00);
            ov7725_write_reg(0x2e, 0x00);
            break;

        case 4: /* home, ���� */
            ov7725_write_reg(0x13, 0xfd); /* AWB off */
            ov7725_write_reg(0x01, 0x96);
            ov7725_write_reg(0x02, 0x40);
            ov7725_write_reg(0x0e, 0x65);
            ov7725_write_reg(0x2d, 0x00);
            ov7725_write_reg(0x2e, 0x00);
            break;

        case 5: /* night, ҹ�� */
            ov7725_write_reg(0x13, 0xff); /* AWB on */
            ov7725_write_reg(0x0e, 0xe5);
            break;
    }
}

/**
 * @brief       OV7725 ɫ�ʱ��Ͷ�����
 * @param       set : 0~8, ����ɫ�ʱ��Ͷ� -4 ~ 4.
 * @retval      ��
 */
void ov7725_color_saturation(uint8_t sat)
{
    ov7725_write_reg(USAT, sat << 4);
    ov7725_write_reg(VSAT, sat << 4);
}

/**
 * @brief       OV7725 ��������
 * @param       bright : 0~8, �������� -4 ~ 4.
 * @retval      ��
 */
void ov7725_brightness(uint8_t bright)
{
    uint8_t bright_value, sign;

    switch (bright)
    {
        case 8:
            bright_value = 0x48;
            sign = 0x06;
            break;

        case 7:
            bright_value = 0x38;
            sign = 0x06;
            break;

        case 6:
            bright_value = 0x28;
            sign = 0x06;
            break;

        case 5:
            bright_value = 0x18;
            sign = 0x06;
            break;

        case 4:
            bright_value = 0x08;
            sign = 0x06;
            break;

        case 3:
            bright_value = 0x08;
            sign = 0x0e;
            break;

        case 2:
            bright_value = 0x18;
            sign = 0x0e;
            break;

        case 1:
            bright_value = 0x28;
            sign = 0x0e;
            break;

        case 0:
            bright_value = 0x38;
            sign = 0x0e;
            break;
    }

    ov7725_write_reg(BRIGHT, bright_value);
    ov7725_write_reg(SIGN, sign);
}

/**
 * @brief       OV7725 �Աȶ�����
 * @param       contrast : 0~8, ����Աȶ� -4 ~ 4.
 * @retval      ��
 */
void ov7725_contrast(uint8_t contrast)
{
    ov7725_write_reg(CNST, (0x30 - (8 - contrast) * 4));
}

/**
 * @brief       OV7725 ��Ч����
 * @param       eft : 0~6, ��ЧЧ��
 *   @arg       0: ����
 *   @arg       1: ��Ƭ
 *   @arg       2: �ڰ�
 *   @arg       3: ƫ��
 *   @arg       4: ƫ��
 *   @arg       5: ƫ��
 *   @arg       6: ����
 * @retval      ��
 */
void ov7725_special_effects(uint8_t eft)
{
    switch (eft)
    {
        case 0: /* ���� */
            ov7725_write_reg(0xa6, 0x06);   /* TSLB���� */
            ov7725_write_reg(0x60, 0x80);   /* MANV,�ֶ�Vֵ */
            ov7725_write_reg(0x61, 0x80);   /* MANU,�ֶ�Uֵ */
            break;

        case 1: /* ��Ƭ */
            ov7725_write_reg(0xa6, 0x46);
            break;

        case 2: /* �ڰ� */
            ov7725_write_reg(0xa6, 0x26);
            ov7725_write_reg(0x60, 0x80);
            ov7725_write_reg(0x61, 0x80);
            break;

        case 3: /* ƫ�� */
            ov7725_write_reg(0xa6, 0x1e);
            ov7725_write_reg(0x60, 0x80);
            ov7725_write_reg(0x61, 0xc0);
            break;

        case 4: /* ƫ�� */
            ov7725_write_reg(0xa6, 0x1e);
            ov7725_write_reg(0x60, 0x60);
            ov7725_write_reg(0x61, 0x60);
            break;

        case 5: /* ƫ�� */
            ov7725_write_reg(0xa6, 0x1e);
            ov7725_write_reg(0x60, 0xa0);
            ov7725_write_reg(0x61, 0x40);
            break;

        case 6: /* ���� */
            ov7725_write_reg(0xa6, 0x1e);
            ov7725_write_reg(0x60, 0x40);
            ov7725_write_reg(0x61, 0xa0);
            break;

    }
}

/**
 * @brief       ����ͼ���������
 * @param       width   : ���ͼ����, <=320
 * @param       height  : ���ͼ��߶�, <=240
 * @param       mode    : ���ģʽ
 *   @arg       0: QVGA���ģʽ, ���ӷ�Χ��, �������ֱ���: 320*240
 *   @arg       1: VGA���ģʽ , ���ӷ�ΧС, �������ֱ���: 320*240
 * @retval      ��
 */
void ov7725_window_set(uint16_t width, uint16_t height, uint8_t mode)
{
    uint8_t raw, temp;
    uint16_t sx, sy;

    if (mode)
    {
        sx = (640 - width) / 2;
        sy = (480 - height) / 2;

        ov7725_write_reg(COM7, 0x06);       /* ����ΪVGAģʽ */
        ov7725_write_reg(HSTART, 0x23);     /* ˮƽ��ʼλ�� */
        ov7725_write_reg(HSIZE, 0xA0);      /* ˮƽ�ߴ� */
        ov7725_write_reg(VSTRT, 0x07);      /* ��ֱ��ʼλ�� */
        ov7725_write_reg(VSIZE, 0xF0);      /* ��ֱ�ߴ� */
        ov7725_write_reg(HREF, 0x00);
        ov7725_write_reg(HOutSize, 0xA0);   /* ����ߴ� */
        ov7725_write_reg(VOutSize, 0xF0);   /* ����ߴ� */
    }
    else
    {
        sx = (320 - width) / 2;
        sy = (240 - height) / 2;
        ov7725_write_reg(COM7, 0x46);       /* ����ΪQVGAģʽ */
        ov7725_write_reg(HSTART, 0x3f);     /* ˮƽ��ʼλ�� */
        ov7725_write_reg(HSIZE, 0x50);      /* ˮƽ�ߴ� */
        ov7725_write_reg(VSTRT, 0x03);      /* ��ֱ��ʼλ�� */
        ov7725_write_reg(VSIZE, 0x78);      /* ��ֱ�ߴ� */
        ov7725_write_reg(HREF,  0x00);
        ov7725_write_reg(HOutSize, 0x50);   /* ����ߴ� */
        ov7725_write_reg(VOutSize, 0x78);   /* ����ߴ� */
    }

    raw = ov7725_read_reg(HSTART);
    temp = raw + (sx >> 2);                 /* sx��8λ����HSTART,��2λ����HREF[5:4] */
    ov7725_write_reg(HSTART, temp);
    ov7725_write_reg(HSIZE, width >> 2);    /* width��8λ����HSIZE,��2λ����HREF[1:0] */

    raw = ov7725_read_reg(VSTRT);
    temp = raw + (sy >> 1);                 /* sy��8λ����VSTRT,��1λ����HREF[6] */
    ov7725_write_reg(VSTRT, temp);
    ov7725_write_reg(VSIZE, height >> 1);   /* height��8λ����VSIZE,��1λ����HREF[2] */

    raw = ov7725_read_reg(HREF);
    temp = ((sy & 0x01) << 6) | ((sx & 0x03) << 4) | ((height & 0x01) << 2) | (width & 0x03) | raw;
    ov7725_write_reg(HREF, temp);

    ov7725_write_reg(HOutSize, width >> 2);
    ov7725_write_reg(VOutSize, height >> 1);

    ov7725_read_reg(EXHCH);
    temp = (raw | (width & 0x03) | ((height & 0x01) << 2));
    ov7725_write_reg(EXHCH, temp);
}









