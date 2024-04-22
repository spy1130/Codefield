/**
 ****************************************************************************************************
 * @file        qspi.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       QSPI ��������
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
 * V1.0 20200329
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/QSPI/qspi.h"


QSPI_HandleTypeDef qspi_handle;    /* QSPI��� */

/**
 * @brief       �ȴ�״̬��־
 * @param       flag : ��Ҫ�ȴ��ı�־λ
 * @param       sta  : ��Ҫ�ȴ���״̬
 * @param       wtime: �ȴ�ʱ��
 * @retval      0, �ȴ��ɹ�; 1, �ȴ�ʧ��.
 */
uint8_t qspi_wait_flag(uint32_t flag, uint8_t sta, uint32_t wtime)
{
    uint8_t flagsta = 0;

    while (wtime)
    {
        flagsta = (QUADSPI->SR & flag) ? 1 : 0; /* ��ȡ״̬��־ */

        if (flagsta == sta)break;

        wtime--;
    }

    if (wtime)return 0;
    else return 1;
}

/**
 * @brief       ��ʼ��QSPI�ӿ�
 * @param       ��
 * @retval      0, �ɹ�; 1, ʧ��.
 */
uint8_t qspi_init(void)
{
    qspi_handle.Instance = QUADSPI;                                  /* QSPI */
    qspi_handle.Init.ClockPrescaler = 1;                             /* QPSI��Ƶ�ȣ�BY25Q128���Ƶ��Ϊ108M��
                                                                        ���Դ˴�Ӧ��Ϊ2��QSPIƵ�ʾ�Ϊ220/(1+1)=110MHZ
                                                                        ��΢�е㳬Ƶ�����������ͺã����о�ֻ�ܽ���Ƶ�� */
    qspi_handle.Init.FifoThreshold = 4;                              /* FIFO��ֵΪ4���ֽ� */
    qspi_handle.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;/* ������λ�������(DDRģʽ��,��������Ϊ0) */
    qspi_handle.Init.FlashSize = POSITION_VAL(0X2000000)-1;          /* SPI FLASH��С��BY25Q128��СΪ32M�ֽ� */
    qspi_handle.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_3_CYCLE; /* Ƭѡ�ߵ�ƽʱ��Ϊ3��ʱ��(9.1*3=27.3ns),���ֲ������tSHSL���� */
    qspi_handle.Init.ClockMode = QSPI_CLOCK_MODE_3;                  /* ģʽ3 */
    qspi_handle.Init.FlashID = QSPI_FLASH_ID_1;                      /* ��һƬflash */
    qspi_handle.Init.DualFlash = QSPI_DUALFLASH_DISABLE;             /* ��ֹ˫����ģʽ */
    
    if(HAL_QSPI_Init(&qspi_handle) == HAL_OK) 
    {
        return 0;      /* QSPI��ʼ���ɹ� */
    }
    else
    {
        return 1;
    }
}

/**
 * @brief       QSPI�ײ�����,�������ã�ʱ��ʹ��
 * @param       hqspi:QSPI���
 * @note        �˺����ᱻHAL_QSPI_Init()����
 * @retval      0, �ɹ�; 1, ʧ��.
 */
void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_QSPI_CLK_ENABLE();      /* ʹ��QSPIʱ�� */
    __HAL_RCC_GPIOB_CLK_ENABLE();     /* GPIOBʱ��ʹ�� */
    __HAL_RCC_GPIOD_CLK_ENABLE();     /* GPIODʱ��ʹ�� */
    __HAL_RCC_GPIOE_CLK_ENABLE();     /* GPIOEʱ��ʹ�� */

    gpio_init_struct.Pin = QSPI_BK1_NCS_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                     /* ���� */
    gpio_init_struct.Pull = GPIO_PULLUP;                         /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;          /* ���� */
    gpio_init_struct.Alternate = GPIO_AF10_QUADSPI;              /* ����ΪQSPI */
    HAL_GPIO_Init(QSPI_BK1_NCS_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��QSPI_BK1_NCS���� */

    gpio_init_struct.Pin = QSPI_BK1_CLK_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                     /* ���� */
    gpio_init_struct.Pull = GPIO_PULLUP;                         /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;          /* ���� */
    gpio_init_struct.Alternate = GPIO_AF9_QUADSPI;               /* ����ΪQSPI */
    HAL_GPIO_Init(QSPI_BK1_CLK_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��QSPI_BK1_CLK���� */
    
    gpio_init_struct.Pin = QSPI_BK1_IO0_GPIO_PIN;
    HAL_GPIO_Init(QSPI_BK1_IO0_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��QSPI_BK1_IO0���� */
    
    gpio_init_struct.Pin = QSPI_BK1_IO1_GPIO_PIN;
    HAL_GPIO_Init(QSPI_BK1_IO1_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��QSPI_BK1_IO1���� */
    
    gpio_init_struct.Pin = QSPI_BK1_IO2_GPIO_PIN;
    HAL_GPIO_Init(QSPI_BK1_IO2_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��QSPI_BK1_IO2���� */
    
    gpio_init_struct.Pin = QSPI_BK1_IO3_GPIO_PIN;
    HAL_GPIO_Init(QSPI_BK1_IO3_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��QSPI_BK1_IO3���� */
}

/**
 * @brief       QSPI��������
 * @param       cmd : Ҫ���͵�ָ��
 * @param       addr: ���͵���Ŀ�ĵ�ַ
 * @param       mode: ģʽ,��ϸλ��������:
 *   @arg       mode[1:0]: ָ��ģʽ; 00,��ָ��;  01,���ߴ���ָ��; 10,˫�ߴ���ָ��; 11,���ߴ���ָ��.
 *   @arg       mode[3:2]: ��ַģʽ; 00,�޵�ַ;  01,���ߴ����ַ; 10,˫�ߴ����ַ; 11,���ߴ����ַ.
 *   @arg       mode[5:4]: ��ַ����; 00,8λ��ַ; 01,16λ��ַ;     10,24λ��ַ;     11,32λ��ַ.
 *   @arg       mode[7:6]: ����ģʽ; 00,������;  01,���ߴ�������; 10,˫�ߴ�������; 11,���ߴ�������.
 * @param       dmcycle: ��ָ��������
 * @retval      ��
 */
void qspi_send_cmd(uint8_t cmd, uint32_t addr, uint8_t mode, uint8_t dmcycle)
{
    QSPI_CommandTypeDef qspi_command_handle;

    qspi_command_handle.Instruction = cmd;                              /* ָ�� */
    qspi_command_handle.Address = addr;                                 /* ��ַ */
    qspi_command_handle.DummyCycles = dmcycle;                          /* ���ÿ�ָ�������� */

    if(((mode >> 0) & 0x03) == 0)
    qspi_command_handle.InstructionMode = QSPI_INSTRUCTION_NONE;        /* ָ��ģʽ */
    else if(((mode >> 0) & 0x03) == 1)
    qspi_command_handle.InstructionMode = QSPI_INSTRUCTION_1_LINE;      /* ָ��ģʽ */
    else if(((mode >> 0) & 0x03) == 2)
    qspi_command_handle.InstructionMode = QSPI_INSTRUCTION_2_LINES;     /* ָ��ģʽ */
    else if(((mode >> 0) & 0x03) == 3)
    qspi_command_handle.InstructionMode = QSPI_INSTRUCTION_4_LINES;     /* ָ��ģʽ */

    if(((mode >> 2) & 0x03) == 0)
    qspi_command_handle.AddressMode = QSPI_ADDRESS_NONE;                /* ��ַģʽ */
    else if(((mode >> 2) & 0x03) == 1)
    qspi_command_handle.AddressMode = QSPI_ADDRESS_1_LINE;              /* ��ַģʽ */
    else if(((mode >> 2) & 0x03) == 2)
    qspi_command_handle.AddressMode = QSPI_ADDRESS_2_LINES;             /* ��ַģʽ */
    else if(((mode >> 2) & 0x03) == 3)
    qspi_command_handle.AddressMode = QSPI_ADDRESS_4_LINES;             /* ��ַģʽ */

    if(((mode >> 4)&0x03) == 0)
    qspi_command_handle.AddressSize = QSPI_ADDRESS_8_BITS;              /* ��ַ���� */
    else if(((mode >> 4) & 0x03) == 1)
    qspi_command_handle.AddressSize = QSPI_ADDRESS_16_BITS;             /* ��ַ���� */
    else if(((mode >> 4) & 0x03) == 2)
    qspi_command_handle.AddressSize = QSPI_ADDRESS_24_BITS;             /* ��ַ���� */
    else if(((mode >> 4) & 0x03) == 3)
    qspi_command_handle.AddressSize = QSPI_ADDRESS_32_BITS;             /* ��ַ���� */

    if(((mode >> 6) & 0x03) == 0)
    qspi_command_handle.DataMode=QSPI_DATA_NONE;                        /* ����ģʽ */
    else if(((mode >> 6) & 0x03) == 1)
    qspi_command_handle.DataMode = QSPI_DATA_1_LINE;                    /* ����ģʽ */
    else if(((mode >> 6) & 0x03) == 2)
    qspi_command_handle.DataMode = QSPI_DATA_2_LINES;                   /* ����ģʽ */
    else if(((mode >> 6) & 0x03) == 3)
    qspi_command_handle.DataMode = QSPI_DATA_4_LINES;                   /* ����ģʽ */

    qspi_command_handle.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;            /* ÿ�ζ�����ָ�� */
    qspi_command_handle.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  /* �޽����ֽ� */
    qspi_command_handle.DdrMode = QSPI_DDR_MODE_DISABLE;                /* �ر�DDRģʽ */
    qspi_command_handle.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;

    HAL_QSPI_Command(&qspi_handle, &qspi_command_handle, 5000);
}

/**
 * @brief       QSPI����ָ�����ȵ�����
 * @param       buf     : �������ݻ������׵�ַ
 * @param       datalen : Ҫ��������ݳ���
 * @retval      0, �ɹ�; ����, �������.
 */
uint8_t qspi_receive(uint8_t *buf, uint32_t datalen)
{
    qspi_handle.Instance->DLR = datalen - 1;   /* �������ݳ��� */
    if (HAL_QSPI_Receive(&qspi_handle, buf, 5000) == HAL_OK) 
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 * @brief       QSPI����ָ�����ȵ�����
 * @param       buf     : �������ݻ������׵�ַ
 * @param       datalen : Ҫ��������ݳ���
 * @retval      0, �ɹ�; ����, �������.
 */
uint8_t qspi_transmit(uint8_t *buf, uint32_t datalen)
{
    qspi_handle.Instance->DLR = datalen - 1; /* �������ݳ��� */
    if (HAL_QSPI_Transmit(&qspi_handle, buf, 5000) == HAL_OK)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


















