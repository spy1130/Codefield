/**
 ****************************************************************************************************
 * @file        nrf24l01.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       NRF24L01 ��������
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

#include "./BSP/SPI/spi.h"
#include "./BSP/NRF24L01/nrf24l01.h"
#include "./SYSTEM/delay/delay.h"

extern SPI_HandleTypeDef g_spi2_handler;             /* SPI2��� */
const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01};    /* ���͵�ַ */
const uint8_t RX_ADDRESS[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01};    /* ���͵�ַ */

/**
 * @brief       ���NRF24L01�޸�SPI2����
 * @param       ��
 * @retval      ��
 */
void nrf24l01_spi_init(void)
{
    __HAL_SPI_DISABLE(&g_spi2_handler);                 /* �ȹر�SPI2 */
    g_spi2_handler.Init.CLKPolarity = SPI_POLARITY_LOW; /* ����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ */
    g_spi2_handler.Init.CLKPhase = SPI_PHASE_1EDGE;     /* ����ͬ��ʱ�ӵĵ�1�������أ��������½������ݱ����� */
    HAL_SPI_Init(&g_spi2_handler);
    __HAL_SPI_ENABLE(&g_spi2_handler);                  /* ʹ��SPI2 */
}

/**
 * @brief       ��ʼ��24L01��IO��
 *   @note      ��SPI2ģʽ�ĳ�SCK���е͵�ƽ,��SPI ģʽ0
 * @param       ��
 * @retval      ��
 */
void nrf24l01_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    NRF24L01_CE_GPIO_CLK_ENABLE();  /* CE��ʱ��ʹ�� */
    NRF24L01_CSN_GPIO_CLK_ENABLE(); /* CSN��ʱ��ʹ�� */
    NRF24L01_IRQ_GPIO_CLK_ENABLE(); /* IRQ��ʱ��ʹ�� */

    gpio_init_struct.Pin = NRF24L01_CE_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;             /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;           /* ���� */
    HAL_GPIO_Init(NRF24L01_CE_GPIO_PORT, &gpio_init_struct); /* ��ʼ��CE���� */

    gpio_init_struct.Pin = NRF24L01_CSN_GPIO_PIN;
    HAL_GPIO_Init(NRF24L01_CSN_GPIO_PORT, &gpio_init_struct);/* ��ʼ��CSN���� */

    gpio_init_struct.Pin = NRF24L01_IRQ_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                 /* ���� */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;           /* ���� */
    HAL_GPIO_Init(NRF24L01_IRQ_GPIO_PORT, &gpio_init_struct);/* ��ʼ��CE���� */

    spi2_init();                /* ��ʼ��SPI2 */
    nrf24l01_spi_init();        /* ���NRF���ص��޸�SPI������ */
    NRF24L01_CE(0);             /* ʹ��24L01 */
    NRF24L01_CSN(1);            /* SPIƬѡȡ�� */
}

/**
 * @brief       ���24L01�Ƿ����
 * @param       ��
 * @retval      0, �ɹ�; 1, ʧ��;
 */
uint8_t nrf24l01_check(void)
{
    uint8_t buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
    uint8_t i;
    spi2_set_speed(SPI_SPEED_32);                         /* spi�ٶ�Ϊ7.5Mhz��24L01�����SPIʱ��Ϊ10Mhz�� */
    nrf24l01_write_buf(NRF_WRITE_REG + TX_ADDR, buf, 5);  /* д��5���ֽڵĵ�ַ. */
    nrf24l01_read_buf(TX_ADDR, buf, 5);                   /* ����д��ĵ�ַ */

    for (i = 0; i < 5; i++)
    {
        if (buf[i] != 0XA5) break;
    }
    
    if (i != 5) return 1;   /* ���24L01���� */

    return 0;               /* ��⵽24L01 */
}

/**
 * @brief       NRF24L01д�Ĵ���
 * @param       reg   : �Ĵ�����ַ
 * @param       value : д��Ĵ�����ֵ
 * @retval      ״̬�Ĵ���ֵ
 */
static uint8_t nrf24l01_write_reg(uint8_t reg, uint8_t value)
{
    uint8_t status;
    NRF24L01_CSN(0);                    /* ʹ��SPI���� */
    status = spi2_read_write_byte(reg); /* ���ͼĴ����� */
    spi2_read_write_byte(value);        /* д��Ĵ�����ֵ */
    NRF24L01_CSN(1);                    /* ��ֹSPI���� */
    return status;                      /* ����״ֵ̬ */
}

/**
 * @brief       NRF24L01���Ĵ���
 * @param       reg   : �Ĵ�����ַ
 * @retval      ��ȡ���ļĴ���ֵ;
 */
static uint8_t nrf24l01_read_reg(uint8_t reg)
{
    uint8_t reg_val;
    NRF24L01_CSN(0);            /* ʹ��SPI���� */
    spi2_read_write_byte(reg);  /* ���ͼĴ����� */
    reg_val = spi2_read_write_byte(0XFF);   /* ��ȡ�Ĵ������� */
    NRF24L01_CSN(1);            /* ��ֹSPI���� */
    return reg_val;             /* ����״ֵ̬ */
}

/**
 * @brief       ��ָ��λ�ö���ָ�����ȵ�����
 * @param       reg   : �Ĵ�����ַ
 * @param       pbuf  : ����ָ��
 * @param       len   : ���ݳ���
 * @retval      ״̬�Ĵ���ֵ
 */
static uint8_t nrf24l01_read_buf(uint8_t reg, uint8_t *pbuf, uint8_t len)
{
    uint8_t status, i;
    NRF24L01_CSN(0);    /* ʹ��SPI���� */
    status = spi2_read_write_byte(reg);         /* ���ͼĴ���ֵ(λ��),����ȡ״ֵ̬ */

    for (i = 0; i < len; i++)
    {
        pbuf[i] = spi2_read_write_byte(0XFF);   /* �������� */
    }
    
    NRF24L01_CSN(1);    /* �ر�SPI���� */
    return status;      /* ���ض�����״ֵ̬ */
}

/**
 * @brief       ��ָ��λ��дָ�����ȵ�����
 * @param       reg   : �Ĵ�����ַ
 * @param       pbuf  : ����ָ��
 * @param       len   : ���ݳ���
 * @retval      ״̬�Ĵ���ֵ
 */
static uint8_t nrf24l01_write_buf(uint8_t reg, uint8_t *pbuf, uint8_t len)
{
    uint8_t status, i;
    NRF24L01_CSN(0);    /* ʹ��SPI���� */
    status = spi2_read_write_byte(reg);/* ���ͼĴ���ֵ(λ��),����ȡ״ֵ̬ */

    for (i = 0; i < len; i++)
    {
        spi2_read_write_byte(*pbuf++); /* д������ */
    }
    
    NRF24L01_CSN(1);    /* �ر�SPI���� */
    return status;      /* ���ض�����״ֵ̬ */
}

/**
 * @brief       ����NRF24L01����һ������(���ݳ��� = TX_PLOAD_WIDTH)
 * @param       ptxbuf : �����������׵�ַ
 * @retval      �������״̬
 *   @arg       0    : ���ͳɹ�
 *   @arg       1    : �ﵽ����ʹ���,ʧ��
 *   @arg       0XFF : ��������
 */
uint8_t nrf24l01_tx_packet(uint8_t *ptxbuf)
{
    uint8_t sta;
    uint8_t rval = 0XFF;
    
    NRF24L01_CE(0);
    nrf24l01_write_buf(WR_TX_PLOAD, ptxbuf, TX_PLOAD_WIDTH);    /* д���ݵ�TX BUF  TX_PLOAD_WIDTH���ֽ� */
    NRF24L01_CE(1); /* �������� */

    while (NRF24L01_IRQ != 0);          /* �ȴ�������� */

    sta = nrf24l01_read_reg(STATUS);    /* ��ȡ״̬�Ĵ�����ֵ */
    nrf24l01_write_reg(NRF_WRITE_REG + STATUS, sta);    /* ���TX_DS��MAX_RT�жϱ�־ */

    if (sta & MAX_TX)   /* �ﵽ����ط����� */
    {
        nrf24l01_write_reg(FLUSH_TX, 0xff); /* ���TX FIFO�Ĵ��� */
        rval = 1;
    }

    if (sta & TX_OK)/* ������� */
    {
        rval = 0;   /* ��Ƿ��ͳɹ� */
    }

    return rval;    /* ���ؽ�� */
}

/**
 * @brief       ����NRF24L01����һ������(���ݳ��� = RX_PLOAD_WIDTH)
 * @param       prxbuf : �������ݻ������׵�ַ
 * @retval      �������״̬
 *   @arg       0 : ���ճɹ�
 *   @arg       1 : ʧ��
 */
uint8_t nrf24l01_rx_packet(uint8_t *prxbuf)
{
    uint8_t sta;
    uint8_t rval = 1;
    
    sta = nrf24l01_read_reg(STATUS); /* ��ȡ״̬�Ĵ�����ֵ */
    nrf24l01_write_reg(NRF_WRITE_REG + STATUS, sta); /* ���RX_DS�жϱ�־ */

    if (sta & RX_OK)    /* ���յ����� */
    {
        nrf24l01_read_buf(RD_RX_PLOAD, prxbuf, RX_PLOAD_WIDTH); /* ��ȡ���� */
        nrf24l01_write_reg(FLUSH_RX, 0xff); /* ���RX FIFO�Ĵ��� */
        rval = 0;   /* ��ǽ������ */
    }

    return rval;    /* ���ؽ�� */
}

/**
 * @brief       NRF24L01�������ģʽ
 *   @note      ����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
 *              ��CE��ߺ�,������RXģʽ,�����Խ���������
 * @param       ��
 * @retval      ��
 */
void nrf24l01_rx_mode(void)
{
    NRF24L01_CE(0);
    nrf24l01_write_buf(NRF_WRITE_REG + RX_ADDR_P0, (uint8_t *)RX_ADDRESS, RX_ADR_WIDTH);    /* дRX�ڵ��ַ */

    nrf24l01_write_reg(NRF_WRITE_REG + EN_AA, 0x01);        /* ʹ��ͨ��0���Զ�Ӧ�� */
    nrf24l01_write_reg(NRF_WRITE_REG + EN_RXADDR, 0x01);    /* ʹ��ͨ��0�Ľ��յ�ַ */
    nrf24l01_write_reg(NRF_WRITE_REG + RF_CH, 40);          /* ����RFͨ��Ƶ�� */
    nrf24l01_write_reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);   /* ѡ��ͨ��0����Ч���ݿ�� */
    nrf24l01_write_reg(NRF_WRITE_REG + RF_SETUP, 0x0f);     /* ����TX�������,0db����,2Mbps,���������濪�� */
    nrf24l01_write_reg(NRF_WRITE_REG + CONFIG, 0x0f);       /* ���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ */
    NRF24L01_CE(1); /* CEΪ��,�������ģʽ */
}

/**
 * @brief       NRF24L01���뷢��ģʽ
 *   @note      ����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�
 *              LNA HCURR,PWR_UP,CRCʹ��
 *              ��CE��ߺ�,������TXģʽ,�����Է���������, CEΪ�ߴ���10us,����������.
 * @param       ��
 * @retval      ��
 */
void nrf24l01_tx_mode(void)
{
    NRF24L01_CE(0);
    nrf24l01_write_buf(NRF_WRITE_REG + TX_ADDR, (uint8_t *)TX_ADDRESS, TX_ADR_WIDTH);   /* дTX�ڵ��ַ */
    nrf24l01_write_buf(NRF_WRITE_REG + RX_ADDR_P0, (uint8_t *)RX_ADDRESS, RX_ADR_WIDTH);    /* ����RX�ڵ��ַ,��ҪΪ��ʹ��ACK */

    nrf24l01_write_reg(NRF_WRITE_REG + EN_AA, 0x01);        /* ʹ��ͨ��0���Զ�Ӧ�� */
    nrf24l01_write_reg(NRF_WRITE_REG + EN_RXADDR, 0x01);    /* ʹ��ͨ��0�Ľ��յ�ַ */
    nrf24l01_write_reg(NRF_WRITE_REG + SETUP_RETR, 0x1a);   /* �����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10�� */
    nrf24l01_write_reg(NRF_WRITE_REG + RF_CH, 40);          /* ����RFͨ��Ϊ40 */
    nrf24l01_write_reg(NRF_WRITE_REG + RF_SETUP, 0x0f);     /* ����TX�������,0db����,2Mbps,���������濪�� */
    nrf24l01_write_reg(NRF_WRITE_REG + CONFIG, 0x0e);       /* ���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж� */
    NRF24L01_CE(1); /* CEΪ��,10us���������� */
}










