/**
 ****************************************************************************************************
 * @file        ov5640.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-03
 * @brief       OV5640 ��������
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
 * V1.0 20200403
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

#include "./BSP/OV5640/sccb.h"
#include "./BSP/OV5640/ov5640.h"
#include "./BSP/OV5640/ov5640af.h"
#include "./BSP/OV5640/ov5640cfg.h"


/**
 * @brief       OV5640 ���Ĵ���
 * @param       reg : �Ĵ�����ַ
 * @retval      �����ļĴ���ֵ
 */
uint8_t ov5640_read_reg(uint16_t reg)
{
    uint8_t data = 0;
    sccb_start();                   /* ����SCCB���� */
    sccb_send_byte(OV5640_ADDR);    /* д����ID */
    sccb_send_byte(reg >> 8);       /* д�Ĵ�����8λ��ַ */
    sccb_send_byte(reg);            /* д�Ĵ�����8λ��ַ */
    sccb_stop();
    
    /* ���üĴ�����ַ�󣬲��Ƕ� */
    sccb_start();
    sccb_send_byte(OV5640_ADDR | 0X01); /* ���Ͷ����� */
    data = sccb_read_byte();        /* ��ȡ���� */
    sccb_nack();
    sccb_stop();
    return data;
}

/**
 * @brief       OV5640 д�Ĵ���
 * @param       reg : �Ĵ�����ַ
 * @param       data: Ҫд��Ĵ�����ֵ
 * @retval      0, �ɹ�; 1, ʧ��;
 */
uint8_t ov5640_write_reg(uint16_t reg, uint8_t data)
{
    uint8_t res = 0;
    sccb_start();   /* ����SCCB���� */

    if (sccb_send_byte(OV5640_ADDR))res = 1;/* д����ID */

    if (sccb_send_byte(reg >> 8))res = 1;   /* д�Ĵ�����8λ��ַ */

    if (sccb_send_byte(reg))res = 1;        /* д�Ĵ�����8λ��ַ */

    if (sccb_send_byte(data))res = 1;       /* д���� */

    sccb_stop();
    return res;
}

/**
 * @brief       OV5640 ��ʼ��
 * @param       ��
 * @retval      0, �ɹ�; 1, ʧ��;
 */
uint8_t ov5640_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    uint16_t i = 0;
    uint16_t reg;
    
    OV_PWDN_GPIO_CLK_ENABLE();      /* ʹ��OV_PWDN��ʱ�� */
    OV_RESET_GPIO_CLK_ENABLE();     /* ʹ��OV_RESET��ʱ�� */

    gpio_init_struct.Pin = OV_PWDN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* ���� */
    HAL_GPIO_Init(OV_PWDN_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��OV_PWDN���� */

    gpio_init_struct.Pin = OV_RESET_GPIO_PIN;
    HAL_GPIO_Init(OV_RESET_GPIO_PORT, &gpio_init_struct);   /* ��ʼ��OV_RESET���� */

    OV5640_RST(0);      /* ����������OV5640��RST��,���ϵ� */
    delay_ms(20);
    OV5640_PWDN(0);     /* POWER ON */
    delay_ms(5);
    OV5640_RST(1);      /* ������λ */
    delay_ms(20);
    
    sccb_init();        /* ��ʼ��SCCB ��IO�� */
    delay_ms(5);
    
    reg = ov5640_read_reg(OV5640_CHIPIDH);  /* ��ȡID �߰�λ */
    reg <<= 8;
    reg |= ov5640_read_reg(OV5640_CHIPIDL); /* ��ȡID �Ͱ�λ */

    if (reg != OV5640_ID)   /* ID�Ƿ�����? */
    {
        printf("ID:%d\r\n", reg);
        return 1;           /* ʧ�� */
    }

    ov5640_write_reg(0x3103, 0X11); /* system clock from pad, bit[1] */
    ov5640_write_reg(0X3008, 0X82); /* ��λ */
    delay_ms(10);

    /* ��ʼ�� OV5640 */
    for (i = 0; i < sizeof(ov5640_init_reg_tbl) / 4; i++)
    {
        ov5640_write_reg(ov5640_init_reg_tbl[i][0], ov5640_init_reg_tbl[i][1]);
    }

    /* ���������Ƿ����� */
    ov5640_flash_ctrl(0);   /* ������� */
    delay_ms(50);
    ov5640_flash_ctrl(0);   /* �ر������ */
    return 0;               /* ok */
}

/**
 * @brief       OV5640 �л�ΪJPEGģʽ
 * @param       ��
 * @retval      ��
 */
void ov5640_jpeg_mode(void)
{
    uint16_t i = 0;

    /* ����:���JPEG���� */
    for (i = 0; i < (sizeof(OV5640_jpeg_reg_tbl) / 4); i++)
    {
        ov5640_write_reg(OV5640_jpeg_reg_tbl[i][0], OV5640_jpeg_reg_tbl[i][1]); /* ������������ */
    }
}

/**
 * @brief       OV5640 �л�ΪRGB565ģʽ
 * @param       ��
 * @retval      ��
 */
void ov5640_rgb565_mode(void)
{
    uint16_t i = 0;

    /* ����:RGB565��� */
    for (i = 0; i < (sizeof(ov5640_rgb565_reg_tbl) / 4); i++)
    {
        ov5640_write_reg(ov5640_rgb565_reg_tbl[i][0], ov5640_rgb565_reg_tbl[i][1]); /* ������������ */
    }
}

/* EV�عⲹ�����ò�����֧��7���ȼ� */
const static uint8_t OV5640_EXPOSURE_TBL[7][6] =
{
    0x10, 0x08, 0x10, 0x08, 0x20, 0x10, /* -3 */
    0x20, 0x18, 0x41, 0x20, 0x18, 0x10, /* - */
    0x30, 0x28, 0x61, 0x30, 0x28, 0x10, /* -1 */
    0x38, 0x30, 0x61, 0x38, 0x30, 0x10, /* 0 */
    0x40, 0x38, 0x71, 0x40, 0x38, 0x10, /* +1 */
    0x50, 0x48, 0x90, 0x50, 0x48, 0x20, /* +2 */
    0x60, 0x58, 0xa0, 0x60, 0x58, 0x20, /* +3 */
};

/**
 * @brief       OV5640 EV�عⲹ��
 * @param       exposure : 0~6, ������ -3 ~ 3.
 * @retval      ��
 */
void ov5640_exposure(uint8_t exposure)
{
    ov5640_write_reg(0x3212, 0x03);     /* start group 3 */
    ov5640_write_reg(0x3a0f, OV5640_EXPOSURE_TBL[exposure][0]);
    ov5640_write_reg(0x3a10, OV5640_EXPOSURE_TBL[exposure][1]);
    ov5640_write_reg(0x3a1b, OV5640_EXPOSURE_TBL[exposure][2]);
    ov5640_write_reg(0x3a1e, OV5640_EXPOSURE_TBL[exposure][3]);
    ov5640_write_reg(0x3a11, OV5640_EXPOSURE_TBL[exposure][4]);
    ov5640_write_reg(0x3a1f, OV5640_EXPOSURE_TBL[exposure][5]);
    ov5640_write_reg(0x3212, 0x13);     /* end group 3 */
    ov5640_write_reg(0x3212, 0xa3);     /* launch group 3 */
}

/* �ƹ�ģʽ������,֧��5��ģʽ */
const static uint8_t OV5640_WHITEBALANCE_TBL[5][7] =
{
    0x04, 0X00, 0X04, 0X00, 0X04, 0X00, 0X00,   /* Auto,�Զ� */
    0x06, 0X1C, 0X04, 0X00, 0X04, 0XF3, 0X01,   /* Sunny,�չ� */
    0x05, 0X48, 0X04, 0X00, 0X07, 0XCF, 0X01,   /* Office,�칫�� */
    0x06, 0X48, 0X04, 0X00, 0X04, 0XD3, 0X01,   /* Cloudy,���� */
    0x04, 0X10, 0X04, 0X00, 0X08, 0X40, 0X01,   /* Home,���� */
};

/**
 * @brief       OV5640 ��ƽ������
 * @param       mode : 0~4, ��ƽ��ģʽ
 *   @arg       0: �Զ�
 *   @arg       1: �չ�sunny
 *   @arg       2: �칫��office
 *   @arg       3: ����cloudy
 *   @arg       4: ����home
 * @retval      ��
 */
void ov5640_light_mode(uint8_t mode)
{
    uint8_t i;
    ov5640_write_reg(0x3212, 0x03);     /* start group 3 */

    for (i = 0; i < 7; i++)
    {
        ov5640_write_reg(0x3400 + i, OV5640_WHITEBALANCE_TBL[mode][i]); /* ���ð�ƽ�� */
    }
    ov5640_write_reg(0x3212, 0x13);     /* end group 3 */
    ov5640_write_reg(0x3212, 0xa3);     /* launch group 3 */
}

/* ɫ�ʱ��Ͷ����ò�����,֧��7���ȼ� */
const static uint8_t OV5640_SATURATION_TBL[7][6] =
{
    0X0C, 0x30, 0X3D, 0X3E, 0X3D, 0X01, /* -3 */
    0X10, 0x3D, 0X4D, 0X4E, 0X4D, 0X01, /* -2 */
    0X15, 0x52, 0X66, 0X68, 0X66, 0X02, /* -1 */
    0X1A, 0x66, 0X80, 0X82, 0X80, 0X02, /* +0 */
    0X1F, 0x7A, 0X9A, 0X9C, 0X9A, 0X02, /* +1 */
    0X24, 0x8F, 0XB3, 0XB6, 0XB3, 0X03, /* +2 */
    0X2B, 0xAB, 0XD6, 0XDA, 0XD6, 0X04, /* +3 */
};

/**
 * @brief       OV5640 ɫ�ʱ��Ͷ�����
 * @param       set : 0~6, ����ɫ�ʱ��Ͷ� -3 ~ 3.
 * @retval      ��
 */
void ov5640_color_saturation(uint8_t sat)
{
    uint8_t i;
    ov5640_write_reg(0x3212, 0x03);     /* start group 3 */
    ov5640_write_reg(0x5381, 0x1c);
    ov5640_write_reg(0x5382, 0x5a);
    ov5640_write_reg(0x5383, 0x06);

    for (i = 0; i < 6; i++)
    {
        ov5640_write_reg(0x5384 + i, OV5640_SATURATION_TBL[sat][i]);    /* ���ñ��Ͷ� */
    }
    ov5640_write_reg(0x538b, 0x98);
    ov5640_write_reg(0x538a, 0x01);
    ov5640_write_reg(0x3212, 0x13);     /* end group 3 */
    ov5640_write_reg(0x3212, 0xa3);     /* launch group 3 */
}

/**
 * @brief       OV5640 ��������
 * @param       bright : 0~8, �������� -4 ~ 4.
 * @retval      ��
 */
void ov5640_brightness(uint8_t bright)
{
    uint8_t brtval;

    if (bright < 4)brtval = 4 - bright;
    else brtval = bright - 4;

    ov5640_write_reg(0x3212, 0x03);	/* start group 3 */
    ov5640_write_reg(0x5587, brtval << 4);

    if (bright < 4)ov5640_write_reg(0x5588, 0x09);
    else ov5640_write_reg(0x5588, 0x01);

    ov5640_write_reg(0x3212, 0x13); /* end group 3 */
    ov5640_write_reg(0x3212, 0xa3); /* launch group 3 */
}

/**
 * @brief       OV5640 �Աȶ�����
 * @param       contrast : 0~6, ����Աȶ� -3 ~ 3.
 * @retval      ��
 */
void ov5640_contrast(uint8_t contrast)
{
    uint8_t reg0val = 0X00;     /* contrast = 3, Ĭ�϶Աȶ� */
    uint8_t reg1val = 0X20;

    switch (contrast)
    {
        case 0: /* -3 */
            reg1val = reg0val = 0X14;
            break;

        case 1: /* -2 */
            reg1val = reg0val = 0X18;
            break;

        case 2: /* -1 */
            reg1val = reg0val = 0X1C;
            break;

        case 4: /* 1 */
            reg0val = 0X10;
            reg1val = 0X24;
            break;

        case 5: /* 2 */
            reg0val = 0X18;
            reg1val = 0X28;
            break;

        case 6: /* 3 */
            reg0val = 0X1C;
            reg1val = 0X2C;
            break;
    }

    ov5640_write_reg(0x3212, 0x03); /* start group 3 */
    ov5640_write_reg(0x5585, reg0val);
    ov5640_write_reg(0x5586, reg1val);
    ov5640_write_reg(0x3212, 0x13); /* end group 3 */
    ov5640_write_reg(0x3212, 0xa3); /* launch group 3 */
}

/**
 * @brief       OV5640 �������
 * @param       sharp : 0~33.
 *   @arg       0   : �ر� 
 *   @arg       1~32: ��ȷ�Χ
 *   @arg       33  : auto
 *
 * @retval      ��
 */
void ov5640_sharpness(uint8_t sharp)
{
    if (sharp < 33) /* �������ֵ */
    {
        ov5640_write_reg(0x5308, 0x65);
        ov5640_write_reg(0x5302, sharp);
    }
    else    /* �Զ���� */
    {
        ov5640_write_reg(0x5308, 0x25);
        ov5640_write_reg(0x5300, 0x08);
        ov5640_write_reg(0x5301, 0x30);
        ov5640_write_reg(0x5302, 0x10);
        ov5640_write_reg(0x5303, 0x00);
        ov5640_write_reg(0x5309, 0x08);
        ov5640_write_reg(0x530a, 0x30);
        ov5640_write_reg(0x530b, 0x04);
        ov5640_write_reg(0x530c, 0x06);
    }
}

/* ��Ч���ò�����,֧��7����Ч */
const static uint8_t OV5640_EFFECTS_TBL[7][3] =
{
    0X06, 0x40, 0X10, /* ���� */
    0X1E, 0xA0, 0X40, /* ��ɫ */
    0X1E, 0x80, 0XC0, /* ůɫ */
    0X1E, 0x80, 0X80, /* �ڰ� */
    0X1E, 0x40, 0XA0, /* ���� */
    0X40, 0x40, 0X10, /* ��ɫ */
    0X1E, 0x60, 0X60, /* ƫ�� */
};

/**
 * @brief       OV5640 ��Ч����
 * @param       eft : 0~6, ��ЧЧ��
 *   @arg       0: ����
 *   @arg       1: ��ɫ
 *   @arg       2: ůɫ
 *   @arg       3: �ڰ�
 *   @arg       4: ƫ��
 *   @arg       5: ��ɫ
 *   @arg       6: ƫ��
 * @retval      ��
 */
void ov5640_special_effects(uint8_t eft)
{
    ov5640_write_reg(0x3212, 0x03); /* start group 3 */
    ov5640_write_reg(0x5580, OV5640_EFFECTS_TBL[eft][0]);
    ov5640_write_reg(0x5583, OV5640_EFFECTS_TBL[eft][1]); /*  sat U */
    ov5640_write_reg(0x5584, OV5640_EFFECTS_TBL[eft][2]); /*  sat V */
    ov5640_write_reg(0x5003, 0x08);
    ov5640_write_reg(0x3212, 0x13); /* end group 3 */
    ov5640_write_reg(0x3212, 0xa3); /* launch group 3 */
}

/**
 * @brief       OV5640 ��������
 * @param       mode : 0~2
 *   @arg       0: �ر� 
 *   @arg       1: ����
 *   @arg       2: ɫ��
 *
 * @retval      ��
 */
void ov5640_test_pattern(uint8_t mode)
{
    if (mode == 0)      /* �رղ������� */
    {
        ov5640_write_reg(0X503D, 0X00);
    }
    else if (mode == 1) /* �������� */
    {
        ov5640_write_reg(0X503D, 0X80);
    }
    else if (mode == 2) /* ɫ����� */
    {
        ov5640_write_reg(0X503D, 0X82);
    }
}

/**
 * @brief       OV5640 ����ƿ���
 * @param       sw : �����״̬
 *   @arg       0: �ر�
 *   @arg       1: ��
 *
 * @retval      ��
 */
void ov5640_flash_ctrl(uint8_t sw)
{
    ov5640_write_reg(0x3016, 0X02);
    ov5640_write_reg(0x301C, 0X02);

    if (sw) /* �� */
    {
        ov5640_write_reg(0X3019, 0X02);
    }
    else    /* �ر� */
    {
        ov5640_write_reg(0X3019, 0X00);
    }
}

/**
 * @brief       ����ͼ�������С
 *   @note      OV5640���ͼ��Ĵ�С(�ֱ���),��ȫ�ɸú���ȷ��
 *              ���ڿ�����scale����,���������ͼ�񴰿�Ϊ: xsize - 2 * offx, ysize - 2 * offy
 *              ʵ�����(width,height), ����: xsize - 2 * offx, ysize - 2 * offy�Ļ����Ͻ������Ŵ���.
 *              һ������offx��offy��ֵΪ16��4,��СҲ�ǿ���,����Ĭ����16��4
 *
 * @param       offx,offy    : ���ͼ����ov5640_image_window_set�趨����(���賤��Ϊxsize��ysize)�ϵ�ƫ��
 * @param       width,height : ʵ�����ͼ��Ŀ�Ⱥ͸߶�
 * @retval      0, �ɹ�; ����, ʧ��;
 */
uint8_t ov5640_outsize_set(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height)
{
    ov5640_write_reg(0X3212, 0X03);         /* ��ʼ��3 */
    
    /* �������þ���ʵ������ߴ�(������) */
    ov5640_write_reg(0x3808, width >> 8);   /* ����ʵ�������ȸ��ֽ� */
    ov5640_write_reg(0x3809, width & 0xff); /* ����ʵ�������ȵ��ֽ� */
    ov5640_write_reg(0x380a, height >> 8);  /* ����ʵ������߶ȸ��ֽ� */
    ov5640_write_reg(0x380b, height & 0xff);/* ����ʵ������߶ȵ��ֽ� */
    
    /* �������þ�������ߴ���ISP�����ȡͼ��Χ
     * ��Χ: xsize-2*offx,ysize-2*offy
     */
    ov5640_write_reg(0x3810, offx >> 8);    /* ����X offset���ֽ� */
    ov5640_write_reg(0x3811, offx & 0xff);  /* ����X offset���ֽ� */

    ov5640_write_reg(0x3812, offy >> 8);    /* ����Y offset���ֽ� */
    ov5640_write_reg(0x3813, offy & 0xff);  /* ����Y offset���ֽ� */

    ov5640_write_reg(0X3212, 0X13);         /* ������3 */
    ov5640_write_reg(0X3212, 0Xa3);         /* ������3���� */
    return 0;
}

/**
 * @brief       ����ͼ�񿪴���С(ISP��С),�Ǳ�Ҫ,һ��������ô˺���
 *   @note      ���������������濪��(���2592*1944),����ov5640_outsize_set�����
 *              ע��:�������Ŀ�Ⱥ͸߶�,������ڵ���ov5640_outsize_set�����Ŀ�Ⱥ͸߶�
 *              ov5640_outsize_set���õĿ�Ⱥ͸߶�,���ݱ��������õĿ�Ⱥ͸߶�,��DSP
 *              �Զ��������ű���,������ⲿ�豸.
 *
 * @param       offx,offy    : ͼ���ڴ��������������ƫ��ֵ
 * @param       width,height : ���(��Ӧ:horizontal)�͸߶�(��Ӧ:vertical)
 * @retval      0, �ɹ�; ����, ʧ��;
 */
uint8_t ov5640_image_window_set(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height)
{
    uint16_t xst, yst, xend, yend;
    xst = offx;
    yst = offy;
    xend = offx + width - 1;
    yend = offy + height - 1;
    ov5640_write_reg(0X3212, 0X03);     /* ��ʼ��3 */
    ov5640_write_reg(0X3800, xst >> 8);
    ov5640_write_reg(0X3801, xst & 0XFF);
    ov5640_write_reg(0X3802, yst >> 8);
    ov5640_write_reg(0X3803, yst & 0XFF);
    ov5640_write_reg(0X3804, xend >> 8);
    ov5640_write_reg(0X3805, xend & 0XFF);
    ov5640_write_reg(0X3806, yend >> 8);
    ov5640_write_reg(0X3807, yend & 0XFF);
    ov5640_write_reg(0X3212, 0X13);     /* ������3 */
    ov5640_write_reg(0X3212, 0Xa3);     /* ������3���� */
    return 0;
}

/**
 * @brief       OV5640 ��ʼ���Զ��Խ�
 * @param       ��
 * @retval      0, �ɹ�; 1, ʧ��;
 */
uint8_t ov5640_focus_init(void)
{
    uint16_t i;
    uint16_t addr = 0x8000;
    uint8_t state = 0x8F;
    
    ov5640_write_reg(0x3000, 0x20); /* reset MCU */

    for (i = 0; i < sizeof(OV5640_AF_Config); i++)
    {
        ov5640_write_reg(addr, OV5640_AF_Config[i]);    /* ������������ */
        addr++;
    }

    ov5640_write_reg(0x3022, 0x00);
    ov5640_write_reg(0x3023, 0x00);
    ov5640_write_reg(0x3024, 0x00);
    ov5640_write_reg(0x3025, 0x00);
    ov5640_write_reg(0x3026, 0x00);
    ov5640_write_reg(0x3027, 0x00);
    ov5640_write_reg(0x3028, 0x00);
    ov5640_write_reg(0x3029, 0x7f);
    ov5640_write_reg(0x3000, 0x00);
    i = 0;

    do
    {
        state = ov5640_read_reg(0x3029);
        delay_ms(5);
        i++;

        if (i > 1000) return 1;
    } while (state != 0x70);

    return 0;
}

/**
 * @brief       OV5640 ִ��һ���Զ��Խ�
 * @param       ��
 * @retval      0, �ɹ�; 1, ʧ��;
 */
uint8_t ov5640_focus_single(void)
{
    uint8_t temp;
    uint16_t retry = 0;
    ov5640_write_reg(0x3022, 0x03);     /* ����һ���Զ��Խ� */

    while (1)
    {
        retry++;
        temp = ov5640_read_reg(0x3029); /* ���Խ����״̬ */

        if (temp == 0x10)break;         /*  focus completed */

        delay_ms(5);

        if (retry > 1000)return 1;
    }

    return 0;
}

/**
 * @brief       OV5640 �����Զ��Խ�,��ʧ����,���Զ������Խ�
 * @param       ��
 * @retval      0, �ɹ�; 1, ʧ��;
 */
uint8_t ov5640_focus_constant(void)
{
    uint8_t temp = 0;
    uint16_t retry = 0;
    ov5640_write_reg(0x3023, 0x01);
    ov5640_write_reg(0x3022, 0x08); /* ����IDLEָ�� */

    do
    {
        temp = ov5640_read_reg(0x3023);
        retry++;

        if (retry > 1000)return 2;

        delay_ms(5);
    } while (temp != 0x00);

    ov5640_write_reg(0x3023, 0x01);
    ov5640_write_reg(0x3022, 0x04); /* ���ͳ����Խ�ָ�� */
    retry = 0;

    do
    {
        temp = ov5640_read_reg(0x3023);
        retry++;

        if (retry > 1000)return 2;

        delay_ms(5);
    } while (temp != 0x00);     /* 0, �Խ����; 1, ���ڶԽ�; */

    return 0;
}






