#include "./BSP/SPI/spi.h"

SPI_HandleTypeDef g_spi2_handler; /* SPI2��� */

void spi2_init(void)
{
    g_spi2_handler.Instance                 = SPI2_SPI;                     /* SPI2 */
    g_spi2_handler.Init.Mode                = SPI_MODE_MASTER;              /* ����SPIģʽ������ģʽ�� */
    g_spi2_handler.Init.Direction           = SPI_DIRECTION_2LINES;         /* ����SPI������ʽ��ȫ˫���� */
    g_spi2_handler.Init.DataSize            = SPI_DATASIZE_8BIT;            /* �������ݸ�ʽ��8bit���ȣ� */
    g_spi2_handler.Init.CLKPolarity         = SPI_POLARITY_HIGH;            /* ����ʱ�Ӽ��ԣ�CPOL = 1�� */
    g_spi2_handler.Init.CLKPhase            = SPI_PHASE_2EDGE;              /* ����ʱ����λ��CPHA = 1�� */
    g_spi2_handler.Init.NSS                 = SPI_NSS_SOFT;                 /* ����Ƭѡ��ʽ�����Ƭѡ���Զ���GPIO�� */
    g_spi2_handler.Init.BaudRatePrescaler   = SPI_BAUDRATEPRESCALER_256;    /* ����SPIʱ�Ӳ����ʷ�Ƶ��256��Ƶ�� */
    g_spi2_handler.Init.FirstBit            = SPI_FIRSTBIT_MSB;             /* ���ô�С��ģʽ��MSB��λ��ǰ�� */
    g_spi2_handler.Init.TIMode              = SPI_TIMODE_DISABLE;           /* ����֡��ʽ���ر�TIģʽ�� */
    g_spi2_handler.Init.CRCCalculation      = SPI_CRCCALCULATION_DISABLE;   /* ����CRCУ�飨�ر�CRCУ�飩 */
    g_spi2_handler.Init.CRCPolynomial       = 7;                            /* ����CRCУ�����ʽ����Χ��1~65535�� */

    HAL_SPI_Init(&g_spi2_handler);
}

/**
 * @brief       SPI2�ײ�������ʱ��ʹ�ܣ���������
 *   @note      �˺����ᱻHAL_SPI_Init()����
 * @param       hspi:SPI���
 * @retval      ��
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    SPI2_SPI_CLK_ENABLE();
    
    GPIO_InitTypeDef gpio_init_struct;

    if (hspi->Instance == SPI2_SPI)
    {
        SPI2_SCK_GPIO_CLK_ENABLE();     /* SPI2_SCK��ʱ��ʹ�� */
        SPI2_MISO_GPIO_CLK_ENABLE();    /* SPI2_MISO��ʱ��ʹ�� */
        SPI2_MOSI_GPIO_CLK_ENABLE();    /* SPI2_MOSI��ʱ��ʹ�� */

        /* SCK����ģʽ����(�������) */
        gpio_init_struct.Pin = SPI2_SCK_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &gpio_init_struct);

        /* MISO����ģʽ����(�������) */
        gpio_init_struct.Pin = SPI2_MISO_GPIO_PIN;
        HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &gpio_init_struct);

        /* MOSI����ģʽ����(�������) */
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
