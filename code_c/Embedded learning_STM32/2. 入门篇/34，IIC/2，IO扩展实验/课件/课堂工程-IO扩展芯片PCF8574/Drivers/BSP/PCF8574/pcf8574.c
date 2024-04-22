/**
 ****************************************************************************************************
 * @file        pcf8574.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-04-20
 * @brief       PCF8574 ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ������ F429������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20220420
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/PCF8574/pcf8574.h"
#include "./SYSTEM/delay/delay.h"

/**
 * @brief       ��ʼ��PCF8574
 * @param       ��
 * @retval      0, �ɹ�;
                1, ʧ��;
 */
uint8_t pcf8574_init(void)
{
    uint8_t temp = 0;
    GPIO_InitTypeDef gpio_init_struct;
    
    PCF8574_GPIO_CLK_ENABLE();                               /* ʹ��GPIOBʱ�� */

    gpio_init_struct.Pin = PCF8574_GPIO_PIN;                 /* PB12 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                 /* ���� */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;                /* ���� */
    HAL_GPIO_Init(PCF8574_GPIO_PORT, &gpio_init_struct);     /* ��ʼ�� */
    
    iic_init();                                              /* IIC��ʼ�� */

    /* ���PCF8574�Ƿ���λ */
    iic_start();
    iic_send_byte(PCF8574_ADDR);                             /* д��ַ */
    temp = iic_wait_ack();                                   /* �ȴ�Ӧ��,ͨ���ж��Ƿ���ACKӦ��,���ж�PCF8574��״̬ */
    iic_stop();                                              /* ����һ��ֹͣ���� */
    
    pcf8574_write_byte (0xFF);
    
    return temp;
}
 



uint8_t pcf8574_read_byte (void)
{ 
    uint8_t temp = 0;
    
    iic_start();                /* ������ʼ�ź� */
    iic_send_byte(0x41);        /* ���Ͷ�������ַ */
    iic_wait_ack( );            /* �ȴ�pcf8574��Ӧ�� */
    temp = iic_read_byte(0);    /* ��ȡ1Byte���ݲ���nACK�ź� */
    iic_stop ();                /* ����ֹͣ�ź� */
    return temp;
}



void pcf8574_write_byte (uint8_t data)
{ 
    iic_start();            /* ������ʼ�ź� */
    iic_send_byte(0x40);    /* ����д������ַ */
    iic_wait_ack( );        /* �ȴ�pcf8574��Ӧ�� */
    iic_send_byte(data);    /* ����1Byte��������pcf8574��IO״̬ */
    iic_wait_ack( );        /* �ȴ�pcf8574��Ӧ�� */	
    iic_stop ();            /* ����ֹͣ�ź� */
    delay_ms(10);
}


uint8_t pcf8574_read_bit(uint8_t bit)
{
    uint8_t temp = 0;
    
    temp = pcf8574_read_byte ();    /* temp��8��IO�ڵĵ�ƽ״̬ */
    
    if (temp & (1 << bit))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void pcf8574_write_bit(uint8_t bit, uint8_t sta)
{
    uint8_t temp = 0;
    
    temp = pcf8574_read_byte ();    /* temp��8��IO�ڵĵ�ƽ״̬ */
    
    if (sta)
    {
        temp |= (1 << bit);
    }
    else
    {
        temp &= ~(1 << bit);
    }
    
    pcf8574_write_byte (temp);
}







