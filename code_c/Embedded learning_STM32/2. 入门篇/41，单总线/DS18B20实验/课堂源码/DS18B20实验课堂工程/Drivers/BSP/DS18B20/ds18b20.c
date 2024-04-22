/**
 ****************************************************************************************************
 * @file        ds18b20.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       DS18B20�����¶ȴ����� ��������
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

#include "./SYSTEM/delay/delay.h"
#include "./BSP/DS18B20/ds18b20.h"

/**
 * @brief       ��ʼ��DS18B20��IO�� DS18B20_DQ ͬʱ���DS18B20�Ĵ���
 * @param       ��
 */
void ds18b20_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    DS18B20_DQ_GPIO_CLK_ENABLE();   /* ����DS18B20_DQ����ʱ�� */

    gpio_init_struct.Pin = DS18B20_DQ_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;            /* ��©��� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(DS18B20_DQ_GPIO_PORT, &gpio_init_struct); /* ��ʼ��DS18B20_DQ���� */
    /* DS18B20_DQ����ģʽ����,��©���,����, �����Ͳ���������IO������, ��©�����ʱ��(=1), Ҳ���Զ�ȡ�ⲿ�źŵĸߵ͵�ƽ */
}

void ds18b20_reset(void)
{
    DS18B20_DQ_OUT(0);  /* ����DS18B20_DQ, ��λ */
    delay_us(750);      /* ��ʱ750us */
    DS18B20_DQ_OUT(1);  /* DS18B20_DQ=1, �ͷ����� */
    delay_us(15);       /* ��ʱ15us */
}

uint8_t  ds18b20_check(void)    /* return 0:succeed   1:fail */
{
    uint8_t retry, rval = 0;    /* ������� */

    while (DS18B20_DQ_IN && retry < 200)
    {
        retry++;
        delay_us(1);
    }   /* �ȴ�DS18B20_DQ��ͣ��ȴ�200us*/

    if (retry >= 200)   /* ��ʱ */
    {
        rval = 1;
    }
    else
    {
        retry = 0;

        while (! DS18B20_DQ_IN && retry < 240)
        {
            retry++;
            delay_us(1);
        }   /* �ȴ�DS18B20_DQ��ߣ��ȴ�240us */

        if (retry >= 240)   /* ��ʱ */
        {
            rval = 1;
        }
    }

    return rval;
}

void ds18b20_write_0(void)
{
    DS18B20_DQ_OUT(0);  /* ����DS18B20_DQ */
    delay_us(60);       /* ��ʱ60us */
    DS18B20_DQ_OUT(1);  /* �ͷ�DS18B20_DQ */
    delay_us(2);        /* ��ʱ2us */
}

void ds18b20_write_1(void)
{
    DS18B20_DQ_OUT(0);  /* ����DS18B20_DQ */
    delay_us(2);        /* ��ʱ2us */
    DS18B20_DQ_OUT(1);  /* �ͷ�DS18B20_DQ */
    delay_us(60);       /* ��ʱ60us */
}

void ds18b20_write_byte(uint8_t data)
{
    uint8_t j;

    for (j = 0; j < 8; j++)
    {
        if (data & 0x01)
        {
            ds18b20_write_1();  /* Write 1*/
        }
        else
        {
            ds18b20_write_0();  /* Write 0*/
        }

        data >>= 1;     /* ���ƣ���ȡ��һλ���� */
    }
}

uint8_t  ds18b20_read_bit(void)
{
    uint8_t data = 0;

    DS18B20_DQ_OUT(0);  /* ����DS18B20_DQ */
    delay_us(2);        /* ��ʱ2us */
    DS18B20_DQ_OUT(1);  /* �ͷ�DS18B20_DQ */
    delay_us(12);       /* ��ʱ12us */

    if (DS18B20_DQ_IN)
    {
        data = 1;
    }
    
    delay_us(50);
    return data;
}

uint8_t  ds18b20_read_byte(void)
{
    uint8_t i, b, data = 0;

    for (i = 0; i < 8; i++)
    {
        /* �������λ���� ,��λ���ݺ���� */
        b = ds18b20_read_bit();
        /* ���data��ÿһλ */
        data |= b << i;
    }

    return data;
}

float ds18b20_get_temperature(void)
{
    uint8_t TL, TH;
    uint16_t temp = 0;
    float temperature = 0;
    
    /* 1����ʼ�� */
    ds18b20_reset();
    ds18b20_check();
    
    /* 2������ROM���� */
    ds18b20_write_byte(0xCC);
    
    /* 3������DS18B20�������� */
    ds18b20_write_byte(0x44);
    
    /* 4����ʼ�� */
    ds18b20_reset();
    ds18b20_check();
    
    /* 5������ROM���� */
    ds18b20_write_byte(0xCC);
    
    /* 6������DS18B20�������� */
    ds18b20_write_byte(0xBE);   /* ��ȡRAM���� */
    
    /* 7����ȡ�����ֽ����� */
    TL = ds18b20_read_byte();
    TH = ds18b20_read_byte();
    
    /* 8���¶��������� (���¶�)*/
    temp = TL + (TH << 8);
}



