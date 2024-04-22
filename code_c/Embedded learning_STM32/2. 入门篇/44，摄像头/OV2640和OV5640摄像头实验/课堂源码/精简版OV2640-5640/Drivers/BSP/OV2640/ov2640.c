#include "./BSP/OV5640/sccb.h"
#include "./BSP/OV2640/ov2640.h"
#include "./BSP/OV2640/ov2640cfg.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/OV5640/ov5640.h"

uint8_t ov2640_read_reg(uint16_t reg)
{
    uint8_t data = 0;

    sccb_start();                       /* 启动SCCB传输 */
    sccb_send_byte(OV2640_ADDR);        /* 写器件ADDR */
    delay_us(100);
    sccb_send_byte(reg);                /* 写寄存器地址 */
    delay_us(100);
    sccb_stop();
    delay_us(100);

    /* 设置寄存器地址后，才是读 */
    sccb_start();
    sccb_send_byte(OV2640_ADDR | 0X01); /* 发送读命令 */
    delay_us(100);
    data = sccb_read_byte();            /* 读取数据 */
    sccb_nack();
    sccb_stop();

    return data;
}

uint8_t ov2640_write_reg(uint16_t reg, uint8_t data)
{
    uint8_t res = 0;

    sccb_start();                       /* 启动SCCB传输 */
    delay_us(100);

    if (sccb_send_byte(OV2640_ADDR))    /* 写器件ID */
    {
        res = 1;
    }

    delay_us(100);

    if (sccb_send_byte(reg))            /* 写寄存器地址 */
    {
        res = 1;
    }

    delay_us(100);

    if (sccb_send_byte(data))           /* 写数据 */
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

    OV_PWDN_GPIO_CLK_ENABLE();      /* 使能OV_PWDN脚时钟 */
    OV_RESET_GPIO_CLK_ENABLE();     /* 使能OV_RESET脚时钟 */

    gpio_init_struct.Pin = OV_PWDN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* 高速 */
    HAL_GPIO_Init(OV_PWDN_GPIO_PORT, &gpio_init_struct);    /* 初始化OV_PWDN引脚 */

    gpio_init_struct.Pin = OV_RESET_GPIO_PIN;
    HAL_GPIO_Init(OV_RESET_GPIO_PORT, &gpio_init_struct);   /* 初始化OV_RESET引脚 */

    OV2640_PWDN(0);     /* POWER ON */
    delay_ms(10);
    OV2640_RST(0);      /* 必须先拉低OV2640的RST脚,再上电 */
    delay_ms(20);
    OV2640_RST(1);      /* 结束复位 */
    delay_ms(20);

    sccb_init();        /* 初始化SCCB 的IO口 */
    delay_ms(5);
    
    ov2640_write_reg(OV2640_DSP_RA_DLMT, 0x01);     /* 操作sensor寄存器 */
    ov2640_write_reg(OV2640_SENSOR_COM7, 0x80);     /* 软复位OV2640 */
    delay_ms(50);
    
    reg = ov2640_read_reg(OV2640_SENSOR_MIDH);      /* 读取厂家ID 高八位 */
    reg <<= 8;
    reg |= ov2640_read_reg(OV2640_SENSOR_MIDL);     /* 读取厂家ID 低八位 */

    if (reg != OV2640_MID)      /* ID 是否正常 */
    {
        printf("MID:%d\r\n", reg);
        return 1;               /* 失败 */
    }
    
    reg = ov2640_read_reg(OV2640_SENSOR_PIDH);  /* 读取器件ID 高八位 */
    reg <<= 8;
    reg |= ov2640_read_reg(OV2640_SENSOR_PIDL); /* 读取器件ID 低八位 */

    if (reg != OV2640_PID)      /* ID是否正常 */
    {
        printf("HID:%d\r\n", reg);
        return 1;               /* 失败 */
    }
    
    /* 初始化 OV2640 */
    for (i = 0; i < sizeof(ov2640_uxga_init_reg_tbl) / 2; i++)
    {
        ov2640_write_reg(ov2640_uxga_init_reg_tbl[i][0], ov2640_uxga_init_reg_tbl[i][1]);
    }

    return 0;   /* OV2640初始化完成 */
}

/**
 * @brief       OV2640 切换为JPEG模式
 * @param       无
 * @retval      无
 */
void ov2640_jpeg_mode(void)
{
    uint16_t i = 0;

    /* 设置:YUV422格式 */
    for (i = 0; i < (sizeof(ov2640_yuv422_reg_tbl) / 2); i++)
    {
        ov2640_write_reg(ov2640_yuv422_reg_tbl[i][0], ov2640_yuv422_reg_tbl[i][1]); /* 发送配置数组 */
    }

    /* 设置:输出JPEG数据 */
    for (i = 0; i < (sizeof(ov2640_jpeg_reg_tbl) / 2); i++)
    {
        ov2640_write_reg(ov2640_jpeg_reg_tbl[i][0], ov2640_jpeg_reg_tbl[i][1]);     /* 发送配置数组 */
    }
}

/**
 * @brief       OV2640 切换为RGB565模式
 * @param       无
 * @retval      无
 */
void ov2640_rgb565_mode(void)
{
    uint16_t i = 0;

    /* 设置:RGB565输出 */
    for (i = 0; i < (sizeof(ov2640_rgb565_reg_tbl) / 4); i++)
    {
        ov2640_write_reg(ov2640_rgb565_reg_tbl[i][0], ov2640_rgb565_reg_tbl[i][1]); /* 发送配置数组 */
    }
}

/** 
 * @bref        设置图像输出大小
 * @@param      width : 宽度(对应:horizontal)
 * @param       height: 高度(对应:vertical)
 * @note        OV2640输出图像的大小(分辨率),完全由该函数确定
 *              width和height必须是4的倍数
 * @retval      0     : 设置成功
 *              其他  : 设置失败
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
    ov2640_write_reg(0x5A, outw & 0xFF);    /* 设置OUTW的低八位 */
    ov2640_write_reg(0x5B, outh & 0xFF);    /* 设置OUTH的低八位 */

    temp = (outw >> 8) & 0x03;
    temp |= (outh >> 6) & 0x04;
    ov2640_write_reg(0x5C, temp);           /* 设置OUTH/OUTW的高位 */
    ov2640_write_reg(0xE0, 0x00);

    return 0;
}

