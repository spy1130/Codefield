/**
 ****************************************************************************************************
 * @file        spi.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-2
 * @brief       SPI ��������
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
 * V1.0 20200402
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/SPI/spi.h"


SPI_HandleTypeDef g_spi_handle;  /* SPI��� */

/**
 * @brief       SPI��ʼ������
 *   @note      ����ģʽ,8λ����,��ֹӲ��Ƭѡ
 * @param       ��
 * @retval      ��
 */
void spi2_init(void)
{
    g_spi_handle.Instance = SPI2;                                           /* SP2 */
    g_spi_handle.Init.Mode = SPI_MODE_MASTER;                               /* ����SPI����ģʽ������Ϊ��ģʽ */
    g_spi_handle.Init.Direction = SPI_DIRECTION_2LINES;                     /* ����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ */
    g_spi_handle.Init.DataSize = SPI_DATASIZE_8BIT;                         /* ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ */
    g_spi_handle.Init.CLKPolarity = SPI_POLARITY_HIGH;                      /* ����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ */
    g_spi_handle.Init.CLKPhase = SPI_PHASE_2EDGE;                           /* ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ����� */
    g_spi_handle.Init.NSS = SPI_NSS_SOFT;                                   /* NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ���� */
    g_spi_handle.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;                     /* NSS�ź�����ʧ�� */
    g_spi_handle.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;  /* SPI��ģʽIO״̬����ʹ�� */
    g_spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;        /* ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256 */
    g_spi_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;                          /* ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ */
    g_spi_handle.Init.TIMode = SPI_TIMODE_DISABLE;                          /* �ر�TIģʽ */
    g_spi_handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;          /* �ر�Ӳ��CRCУ�� */
    g_spi_handle.Init.CRCPolynomial = 7;                                    /* CRCֵ����Ķ���ʽ */
    HAL_SPI_Init(&g_spi_handle);
    
    __HAL_SPI_ENABLE(&g_spi_handle);                                        /* ʹ��SPI2 */
    spi2_read_write_byte(0Xff);                                             /* �������� */
}

/**
 * @brief       SPI�ײ�������ʱ��ʹ�ܣ���������
 * @param       hspi:SPI���
 * @note        �˺����ᱻHAL_SPI_Init()����
 * @retval      ��
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    RCC_PeriphCLKInitTypeDef rcc_periph_clk_init;

    SPI2_SPI_CLK_ENABLE();          /* SPI2ʱ��ʹ�� */
    SPI2_SCK_GPIO_CLK_ENABLE();     /* SPI2_SCK��ʱ��ʹ�� */
    SPI2_MISO_GPIO_CLK_ENABLE();    /* SPI2_MISO��ʱ��ʹ�� */
    SPI2_MOSI_GPIO_CLK_ENABLE();    /* SPI2_MOSI��ʱ��ʹ�� */

    /* ����SPI2��ʱ��Դ */
    rcc_periph_clk_init.PeriphClockSelection = RCC_PERIPHCLK_SPI2;      /* ����SPI2ʱ��Դ */
    rcc_periph_clk_init.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL; /* SPI2ʱ��Դʹ��PLL1Q */
    HAL_RCCEx_PeriphCLKConfig(&rcc_periph_clk_init);

    gpio_init_struct.Pin = SPI2_SCK_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* ���� */
    gpio_init_struct.Alternate = SPI2_SCK_GPIO_AF;          /* ���� */
    HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &gpio_init_struct);   /* ��ʼ��SCK���� */

    gpio_init_struct.Pin = SPI2_MISO_GPIO_PIN;
    gpio_init_struct.Alternate = SPI2_MISO_GPIO_AF;         /* ���� */
    HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &gpio_init_struct);  /* ��ʼ��MISO���� */
    
    gpio_init_struct.Pin = SPI2_MOSI_GPIO_PIN;
    gpio_init_struct.Alternate = SPI2_MOSI_GPIO_AF;         /* ���� */
    HAL_GPIO_Init(SPI2_MOSI_GPIO_PORT, &gpio_init_struct);  /* ��ʼ��MOSI���� */
}

/**
 * @brief       SPI2�ٶ����ú���
 *   @note      SPI2ʱ��ѡ������pll1_q_ck, Ϊ240Mhz
 *              SPI�ٶ� = spi_ker_ck / 2^(speed + 1)
 * @param       speed: SPI2ʱ�ӷ�Ƶϵ��,SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_256
 * @retval      ��
 */
void spi2_set_speed(uint32_t speed)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(speed)); /* �ж���Ч�� */
    __HAL_SPI_DISABLE(&g_spi_handle);               /* �ر�SPI */
    g_spi_handle.Instance->CFG1 &= ~(0X7<<28);      /* λ30-28���㣬�������ò����� */
    g_spi_handle.Instance->CFG1 |= speed;           /* ����SPI�ٶ� */
    __HAL_SPI_ENABLE(&g_spi_handle);                /* ʹ��SPI */
}

/**
 * @brief       SPI2��дһ���ֽ�����
 * @param       txdata: Ҫ���͵�����(1�ֽ�)
 * @retval      ���յ�������(1�ֽ�)
 */
uint8_t spi2_read_write_byte(uint8_t txdata)
{
    uint8_t rxdata;
    HAL_SPI_TransmitReceive(&g_spi_handle,&txdata,&rxdata,1, 1000);
    return rxdata;    /* �����յ������� */
}













