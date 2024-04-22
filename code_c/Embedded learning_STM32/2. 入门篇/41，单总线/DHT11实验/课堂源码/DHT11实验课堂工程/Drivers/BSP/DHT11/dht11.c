/**
 ****************************************************************************************************
 * @file        dht11.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       DHT11������ʪ�ȴ����� ��������
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

#include "./BSP/DHT11/dht11.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"

/**
 * @brief       ��ʼ��DHT11��IO�� DHT11_DQ ͬʱ���DHT11�Ĵ���
 * @param       ��
 * @retval      0, ����
 *              1, ������/������
 */
void dht11_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    DHT11_DQ_GPIO_CLK_ENABLE();     /* ����DHT11_DQ����ʱ�� */

    gpio_init_struct.Pin = DHT11_DQ_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;            /* ��©��� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(DHT11_DQ_GPIO_PORT, &gpio_init_struct);   /* ��ʼ��DHT11_DHT11_DQ���� */
    /* DHT11_DHT11_DQ����ģʽ����,��©���,����, �����Ͳ���������IO������, ��©�����ʱ��(=1), Ҳ���Զ�ȡ�ⲿ�źŵĸߵ͵�ƽ */
}


void dht11_reset(void)
{
    DHT11_DQ_OUT(0);    /* ����DHT11_DQ */
    delay_ms(20);       /* ��������18ms*/
    DHT11_DQ_OUT(1);    /* DHT11_DQ=1 */
    delay_us(13);       /* ��������10~35us */
}

uint8_t dht11_check(void)
{
    uint8_t retry, rval = 0;

    while (DHT11_DQ_IN && retry < 100)
    {
        retry++;
        delay_us(1);
    }       /* DHT11������������Լ83us*/

    if (retry >= 100)   /* ��ʱ */
        rval = 1;
    else
    {
        retry = 0;

        while (! DHT11_DQ_IN && retry < 100)
        {
            retry++;
            delay_us(1);
        }   /* DHT11���ͺ���ٴ�����Լ87us*/

        if (retry >= 100)
            rval = 1;   /* ��ʱ */
    }

    return rval;
}

uint8_t dht11_read_bit(void)
{
    uint8_t retry = 0;  /* ������� */

    while (DHT11_DQ_IN && retry < 100)
    {
        retry++;
        delay_us(1);
    }   /* �ȴ���Ϊ�͵�ƽ */

    retry = 0;

    while (! DHT11_DQ_IN && retry < 100)
    {
        retry++;
        delay_us(1);
    }   /* �ȴ���Ϊ�ߵ�ƽ*/

    delay_us(40);   /* �ȴ�40us */

    if (DHT11_DQ_IN)
        return 1;
    else
        return 0;
}

uint8_t dht11_read_byte(void)
{
    uint8_t i, data = 0;

    for (i = 0; i < 8; i++)
    {
        /* ��λ�����������������һλ */
        data <<= 1;
        /* ��ȡ1bit����*/
        data |= dht11_read_bit();
    }

    return data;
}

uint8_t dht11_read_data(void)   /* return 0:succeed 1:failed */
{
    uint8_t i, buf[5] = {0};
    uint8_t t = 0;
    uint8_t h = 0;
    uint8_t ret = 1;
    
    /* 1������������ʼ�ź� */
    dht11_reset();
    
    /* 2���������ӻ���������Ӧ�ź� */
    dht11_check();
    
    /* 3���ӻ�����40bit���ݣ���������5byte���� */
    for (i = 0; i < 5; i++)
    {
        buf[i] = dht11_read_byte();
    }
    
    /* 4�����ݴ��� */
    if (buf[4] == (buf[0] + buf[1] + buf[2] + buf[3]))
    {
        h = buf[0];
        t = buf[2];
        ret = 0;
    }
    
    printf("T:%d H:%d\r\n",t, h);
    
    return ret;
}



 

















