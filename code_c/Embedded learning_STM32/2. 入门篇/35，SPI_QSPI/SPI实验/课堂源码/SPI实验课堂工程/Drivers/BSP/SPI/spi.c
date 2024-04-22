#include "./BSP/SPI/spi.h"

SPI_HandleTypeDef g_spi2_handler; /* SPI2句柄 */

void spi2_init(void)
{
    g_spi2_handler.Instance                 = SPI2_SPI;                     /* SPI2 */
    g_spi2_handler.Init.Mode                = SPI_MODE_MASTER;              /* 设置SPI模式（主机模式） */
    g_spi2_handler.Init.Direction           = SPI_DIRECTION_2LINES;         /* 设置SPI工作方式（全双工） */
    g_spi2_handler.Init.DataSize            = SPI_DATASIZE_8BIT;            /* 设置数据格式（8bit长度） */
    g_spi2_handler.Init.CLKPolarity         = SPI_POLARITY_HIGH;            /* 设置时钟极性（CPOL = 1） */
    g_spi2_handler.Init.CLKPhase            = SPI_PHASE_2EDGE;              /* 设置时钟相位（CPHA = 1） */
    g_spi2_handler.Init.NSS                 = SPI_NSS_SOFT;                 /* 设置片选方式（软件片选，自定义GPIO） */
    g_spi2_handler.Init.BaudRatePrescaler   = SPI_BAUDRATEPRESCALER_256;    /* 设置SPI时钟波特率分频（256分频） */
    g_spi2_handler.Init.FirstBit            = SPI_FIRSTBIT_MSB;             /* 设置大小端模式（MSB高位在前） */
    g_spi2_handler.Init.TIMode              = SPI_TIMODE_DISABLE;           /* 设置帧格式（关闭TI模式） */
    g_spi2_handler.Init.CRCCalculation      = SPI_CRCCALCULATION_DISABLE;   /* 设置CRC校验（关闭CRC校验） */
    g_spi2_handler.Init.CRCPolynomial       = 7;                            /* 设置CRC校验多项式（范围：1~65535） */

    HAL_SPI_Init(&g_spi2_handler);
}

/**
 * @brief       SPI2底层驱动，时钟使能，引脚配置
 *   @note      此函数会被HAL_SPI_Init()调用
 * @param       hspi:SPI句柄
 * @retval      无
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    SPI2_SPI_CLK_ENABLE();
    
    GPIO_InitTypeDef gpio_init_struct;

    if (hspi->Instance == SPI2_SPI)
    {
        SPI2_SCK_GPIO_CLK_ENABLE();     /* SPI2_SCK脚时钟使能 */
        SPI2_MISO_GPIO_CLK_ENABLE();    /* SPI2_MISO脚时钟使能 */
        SPI2_MOSI_GPIO_CLK_ENABLE();    /* SPI2_MOSI脚时钟使能 */

        /* SCK引脚模式设置(复用输出) */
        gpio_init_struct.Pin = SPI2_SCK_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &gpio_init_struct);

        /* MISO引脚模式设置(复用输出) */
        gpio_init_struct.Pin = SPI2_MISO_GPIO_PIN;
        HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &gpio_init_struct);

        /* MOSI引脚模式设置(复用输出) */
        gpio_init_struct.Pin = SPI2_MOSI_GPIO_PIN;
        HAL_GPIO_Init(SPI2_MOSI_GPIO_PORT, &gpio_init_struct);
    }
}

uint8_t spi2_read_write_byte(uint8_t data)
{
    uint8_t rec_data = 0;
    
    HAL_SPI_TransmitReceive(&g_spi2_handler, &data, &rec_data, 1, 1000);
    
    return rec_data;
}
