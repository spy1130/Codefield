#include "./BSP/OV5640/sccb.h"
#include "./BSP/OV2640/ov2640.h"
#include "./BSP/OV2640/ov2640cfg.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/OV5640/ov5640.h"

uint8_t ov2640_read_reg(uint16_t reg)
{
    uint8_t data = 0;

    sccb_start();                       /* ����SCCB���� */
    sccb_send_byte(OV2640_ADDR);        /* д����ADDR */
    delay_us(100);
    sccb_send_byte(reg);                /* д�Ĵ�����ַ */
    delay_us(100);
    sccb_stop();
    delay_us(100);

    /* ���üĴ�����ַ�󣬲��Ƕ� */
    sccb_start();
    sccb_send_byte(OV2640_ADDR | 0X01); /* ���Ͷ����� */
    delay_us(100);
    data = sccb_read_byte();            /* ��ȡ���� */
    sccb_nack();
    sccb_stop();

    return data;
}

uint8_t ov2640_write_reg(uint16_t reg, uint8_t data)
{
    uint8_t res = 0;

    sccb_start();                       /* ����SCCB���� */
    delay_us(100);

    if (sccb_send_byte(OV2640_ADDR))    /* д����ID */
    {
        res = 1;
    }

    delay_us(100);

    if (sccb_send_byte(reg))            /* д�Ĵ�����ַ */
    {
        res = 1;
    }

    delay_us(100);

    if (sccb_send_byte(data))           /* д���� */
    {
        res = 1;
    }

    delay_us(100);
    sccb_stop();
    
    return res;
}


uint8_t ov2640_init(void)
{
    uint16_t i = 0;
    uint16_t reg;
    
    GPIO_InitTypeDef gpio_init_struct;

    OV_PWDN_GPIO_CLK_ENABLE();      /* ʹ��OV_PWDN��ʱ�� */
    OV_RESET_GPIO_CLK_ENABLE();     /* ʹ��OV_RESET��ʱ�� */

    gpio_init_struct.Pin = OV_PWDN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* ���� */
    HAL_GPIO_Init(OV_PWDN_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��OV_PWDN���� */

    gpio_init_struct.Pin = OV_RESET_GPIO_PIN;
    HAL_GPIO_Init(OV_RESET_GPIO_PORT, &gpio_init_struct);   /* ��ʼ��OV_RESET���� */

    OV2640_PWDN(0);     /* POWER ON */
    delay_ms(10);
    OV2640_RST(0);      /* ����������OV2640��RST��,���ϵ� */
    delay_ms(20);
    OV2640_RST(1);      /* ������λ */
    delay_ms(20);

    sccb_init();        /* ��ʼ��SCCB ��IO�� */
    delay_ms(5);
    
    ov2640_write_reg(OV2640_DSP_RA_DLMT, 0x01);     /* ����sensor�Ĵ��� */
    ov2640_write_reg(OV2640_SENSOR_COM7, 0x80);     /* ��λOV2640 */
    delay_ms(50);
    
    reg = ov2640_read_reg(OV2640_SENSOR_MIDH);      /* ��ȡ����ID �߰�λ */
    reg <<= 8;
    reg |= ov2640_read_reg(OV2640_SENSOR_MIDL);     /* ��ȡ����ID �Ͱ�λ */

    if (reg != OV2640_MID)      /* ID �Ƿ����� */
    {
        printf("MID:%d\r\n", reg);
        return 1;               /* ʧ�� */
    }
    
    reg = ov2640_read_reg(OV2640_SENSOR_PIDH);  /* ��ȡ����ID �߰�λ */
    reg <<= 8;
    reg |= ov2640_read_reg(OV2640_SENSOR_PIDL); /* ��ȡ����ID �Ͱ�λ */

    if (reg != OV2640_PID)      /* ID�Ƿ����� */
    {
        printf("HID:%d\r\n", reg);
        return 1;               /* ʧ�� */
    }
    
    /* ��ʼ�� OV2640 */
    for (i = 0; i < sizeof(ov2640_uxga_init_reg_tbl) / 2; i++)
    {
        ov2640_write_reg(ov2640_uxga_init_reg_tbl[i][0], ov2640_uxga_init_reg_tbl[i][1]);
    }

    return 0;   /* OV2640��ʼ����� */
}

/**
 * @brief       OV2640 �л�ΪJPEGģʽ
 * @param       ��
 * @retval      ��
 */
void ov2640_jpeg_mode(void)
{
    uint16_t i = 0;

    /* ����:YUV422��ʽ */
    for (i = 0; i < (sizeof(ov2640_yuv422_reg_tbl) / 2); i++)
    {
        ov2640_write_reg(ov2640_yuv422_reg_tbl[i][0], ov2640_yuv422_reg_tbl[i][1]); /* ������������ */
    }

    /* ����:���JPEG���� */
    for (i = 0; i < (sizeof(ov2640_jpeg_reg_tbl) / 2); i++)
    {
        ov2640_write_reg(ov2640_jpeg_reg_tbl[i][0], ov2640_jpeg_reg_tbl[i][1]);     /* ������������ */
    }
}

/**
 * @brief       OV2640 �л�ΪRGB565ģʽ
 * @param       ��
 * @retval      ��
 */
void ov2640_rgb565_mode(void)
{
    uint16_t i = 0;

    /* ����:RGB565��� */
    for (i = 0; i < (sizeof(ov2640_rgb565_reg_tbl) / 4); i++)
    {
        ov2640_write_reg(ov2640_rgb565_reg_tbl[i][0], ov2640_rgb565_reg_tbl[i][1]); /* ������������ */
    }
}

/** 
 * @bref        ����ͼ�������С
 * @@param      width : ���(��Ӧ:horizontal)
 * @param       height: �߶�(��Ӧ:vertical)
 * @note        OV2640���ͼ��Ĵ�С(�ֱ���),��ȫ�ɸú���ȷ��
 *              width��height������4�ı���
 * @retval      0     : ���óɹ�
 *              ����  : ����ʧ��
 */
uint8_t ov2640_outsize_set(uint16_t width, uint16_t height)
{
    uint16_t outh;
    uint16_t outw;
    uint8_t temp;

    if (width % 4) return 1;
    if (height % 4) return 2;

    outw = width / 4;
    outh = height/ 4;
    ov2640_write_reg(0xFF, 0x00);    
    ov2640_write_reg(0xE0, 0x04);
    ov2640_write_reg(0x5A, outw & 0xFF);    /* ����OUTW�ĵͰ�λ */
    ov2640_write_reg(0x5B, outh & 0xFF);    /* ����OUTH�ĵͰ�λ */

    temp = (outw >> 8) & 0x03;
    temp |= (outh >> 6) & 0x04;
    ov2640_write_reg(0x5C, temp);           /* ����OUTH/OUTW�ĸ�λ */
    ov2640_write_reg(0xE0, 0x00);

    return 0;
}

