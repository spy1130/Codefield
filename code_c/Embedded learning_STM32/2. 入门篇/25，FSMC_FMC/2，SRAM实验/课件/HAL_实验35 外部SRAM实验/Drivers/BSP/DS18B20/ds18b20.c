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
 * @brief       ��λDS18B20
 * @param       data: Ҫд�������
 * @retval      ��
 */
static void ds18b20_reset(void)
{
    DS18B20_DQ_OUT(0);  /* ����DQ,��λ */
    delay_us(750);      /* ����750us */
    DS18B20_DQ_OUT(1);  /* DQ=1, �ͷŸ�λ */
    delay_us(15);       /* �ӳ�15US */
}

/**
 * @brief       �ȴ�DS18B20�Ļ�Ӧ
 * @param       ��
 * @retval      0, DS18B20����
 *              1, DS18B20�쳣/������
 */
uint8_t ds18b20_check(void)
{
    uint8_t retry = 0;
    uint8_t rval = 0;

    while (DS18B20_DQ_IN && retry < 200)    /* �ȴ�DQ���, �ȴ�200us */
    {
        retry++;
        delay_us(1);
    }

    if (retry >= 200)
    {
        rval = 1;
    }
    else
    {
        retry = 0;

        while (!DS18B20_DQ_IN && retry < 240)   /* �ȴ�DQ���, �ȴ�240us */
        {
            retry++;
            delay_us(1);
        }

        if (retry >= 240) rval = 1;
    }

    return rval;
}

/**
 * @brief       ��DS18B20��ȡһ��λ
 * @param       ��
 * @retval      ��ȡ����λֵ: 0 / 1
 */
static uint8_t ds18b20_read_bit(void)
{
    uint8_t data = 0;
    DS18B20_DQ_OUT(0);
    delay_us(2);
    DS18B20_DQ_OUT(1);
    delay_us(12);

    if (DS18B20_DQ_IN)
    {
        data = 1;
    }

    delay_us(50);
    return data;
}

/**
 * @brief       ��DS18B20��ȡһ���ֽ�
 * @param       ��
 * @retval      ����������
 */
static uint8_t ds18b20_read_byte(void)
{
    uint8_t i, b, data = 0;

    for (i = 0; i < 8; i++)
    {
        b = ds18b20_read_bit(); /* DS18B20�������λ���� ,��λ���ݺ���� */
        
        data |= b << i;         /* ���data��ÿһλ */ 
    }

    return data;
}

/**
 * @brief       дһ���ֽڵ�DS18B20
 * @param       data: Ҫд����ֽ�
 * @retval      ��
 */
static void ds18b20_write_byte(uint8_t data)
{
    uint8_t j;

    for (j = 1; j <= 8; j++)
    {
        if (data & 0x01)
        {
            DS18B20_DQ_OUT(0);  /*  Write 1 */
            delay_us(2);
            DS18B20_DQ_OUT(1);
            delay_us(60);
        }
        else
        {
            DS18B20_DQ_OUT(0);  /*  Write 0 */
            delay_us(60);
            DS18B20_DQ_OUT(1);
            delay_us(2);
        }

        data >>= 1;             /* ����,��ȡ��һλ���� */
    }
}

/**
 * @brief       ��ʼ�¶�ת��
 * @param       ��
 * @retval      ��
 */
static void ds18b20_start(void)
{
    ds18b20_reset();
    ds18b20_check();
    ds18b20_write_byte(0xcc);   /*  skip rom */
    ds18b20_write_byte(0x44);   /*  convert */
}

/**
 * @brief       ��ʼ��DS18B20��IO�� DQ ͬʱ���DS18B20�Ĵ���
 * @param       ��
 * @retval      0, ����
 *              1, ������/������
 */
uint8_t ds18b20_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    DS18B20_DQ_GPIO_CLK_ENABLE();   /* ����DQ����ʱ�� */

    gpio_init_struct.Pin = DS18B20_DQ_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;            /* ��©��� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(DS18B20_DQ_GPIO_PORT, &gpio_init_struct); /* ��ʼ��DS18B20_DQ���� */
    /* DS18B20_DQ����ģʽ����,��©���,����, �����Ͳ���������IO������, ��©�����ʱ��(=1), Ҳ���Զ�ȡ�ⲿ�źŵĸߵ͵�ƽ */

    ds18b20_reset();
    return ds18b20_check();
}

/**
 * @brief       ��ds18b20�õ��¶�ֵ(���ȣ�0.1C)
 * @param       ��
 * @retval      �¶�ֵ ��-550~1250��
 *   @note      ���ص��¶�ֵ�Ŵ���10��.
 *              ʵ��ʹ�õ�ʱ��,Ҫ����10����ʵ���¶�.
 */
short ds18b20_get_temperature(void)
{
    uint8_t flag = 1;           /* Ĭ���¶�Ϊ���� */
    uint8_t TL, TH;
    short temp;
    
    ds18b20_start();            /*  ds1820 start convert */
    ds18b20_reset();
    ds18b20_check();
    ds18b20_write_byte(0xcc);   /*  skip rom */
    ds18b20_write_byte(0xbe);   /*  convert */
    TL = ds18b20_read_byte();   /*  LSB */
    TH = ds18b20_read_byte();   /*  MSB */

    if (TH > 7)
    {/* �¶�Ϊ�����鿴DS18B20���¶ȱ�ʾ���������洢�������ݵ�ԭ��һ�£�
				��������Ϊ�Ĵ����洢������������������Ϊ�Ĵ����洢ֵ��λȡ����+1
				��������ֱ��ȡ��ʵ�ʵĸ������֣��������Ĳ���Ϊȡ�����һ�������ǵ���λ����+1���н�λ�ʹ������࣬
				������������ʱû����+1�Ĵ���������Ҫ����	*/
        TH = ~TH;
        TL = ~TL;
        flag = 0;   
    }

    temp = TH;      /* ��ø߰�λ */
    temp <<= 8;
    temp += TL;     /* ��õװ�λ */
		
		/* ת����ʵ���¶� */
    if (flag == 0)
    {/* ���¶�ת���ɸ��¶ȣ������+1�ο�ǰ���˵�� */
			temp = (double)(temp+1) * 0.625;
			temp = -temp;   
    }
		else
		{
			temp = (double)temp * 0.625;				
		}
    
    return temp;
}
















