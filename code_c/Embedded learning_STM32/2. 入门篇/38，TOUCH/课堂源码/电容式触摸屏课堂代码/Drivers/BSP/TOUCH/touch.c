#include "stdio.h"
#include "stdlib.h"
#include "./BSP/TOUCH/touch.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       SPIд����
 *   @note      ������ICд��1 byte����
 * @param       data: Ҫд�������
 * @retval      ��
 */
static void tp_write_byte(uint8_t data)
{
    uint8_t count = 0;

    for (count = 0; count < 8; count++)
    {
        if (data & 0x80)    /* ����1 */
        {
            T_MOSI(1);
        }
        else                /* ����0 */
        {
            T_MOSI(0);
        }

        data <<= 1;
        T_CLK(0);
        delay_us(1);
        T_CLK(1);           /* ��������Ч */
    }
}

/**
 * @brief       SPI������
 *   @note      �Ӵ�����IC��ȡadcֵ
 * @param       cmd: ָ��
 * @retval      ��ȡ��������,ADCֵ(12bit)
 */
static uint16_t tp_read_ad(uint8_t cmd)
{
    uint8_t count = 0;
    uint16_t num = 0;
    
    T_CLK(0);           /* ������ʱ�� */
    T_MOSI(0);          /* ���������� */
    
    T_CS(0);            /* ѡ�д�����IC */
    tp_write_byte(cmd); /* ���������� */
    delay_us(6);        /* ADS7846��ת��ʱ���Ϊ6us */
    
    T_CLK(0);
    delay_us(1);
    T_CLK(1);           /* ��1��ʱ�ӣ����BUSY */
    delay_us(1);
    T_CLK(0);

    for (count = 0; count < 16; count++)    /* ����16λ����,ֻ�и�12λ��Ч */
    {
        num <<= 1;
        T_CLK(0);       /* �½�����Ч */
        delay_us(1);
        T_CLK(1);

        if (T_MISO)num++;
    }

    num >>= 4;          /* ֻ�и�12λ��Ч. */
    T_CS(1);            /* �ͷ�Ƭѡ */
    return num;
}



/**
 * @brief       ��������ʼ��
 * @param       ��
 * @retval      0,û�н���У׼
 *              1,���й�У׼
 */
uint8_t tp_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    T_PEN_GPIO_CLK_ENABLE();    /* T_PEN��ʱ��ʹ�� */
    T_CS_GPIO_CLK_ENABLE();     /* T_CS��ʱ��ʹ�� */
    T_MISO_GPIO_CLK_ENABLE();   /* T_MISO��ʱ��ʹ�� */
    T_MOSI_GPIO_CLK_ENABLE();   /* T_MOSI��ʱ��ʹ�� */
    T_CLK_GPIO_CLK_ENABLE();    /* T_CLK��ʱ��ʹ�� */

    gpio_init_struct.Pin = T_PEN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                 /* ���� */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;           /* ���� */
    HAL_GPIO_Init(T_PEN_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��T_PEN���� */

    gpio_init_struct.Pin = T_MISO_GPIO_PIN;
    HAL_GPIO_Init(T_MISO_GPIO_PORT, &gpio_init_struct);      /* ��ʼ��T_MISO���� */

    gpio_init_struct.Pin = T_MOSI_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;             /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;           /* ���� */
    HAL_GPIO_Init(T_MOSI_GPIO_PORT, &gpio_init_struct);      /* ��ʼ��T_MOSI���� */

    gpio_init_struct.Pin = T_CLK_GPIO_PIN;
    HAL_GPIO_Init(T_CLK_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��T_CLK���� */

    gpio_init_struct.Pin = T_CS_GPIO_PIN;
    HAL_GPIO_Init(T_CS_GPIO_PORT, &gpio_init_struct);        /* ��ʼ��T_CS���� */

    return 1;
}









