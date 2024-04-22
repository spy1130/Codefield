/**
 ****************************************************************************************************
 * @file        gt9xxx.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-25
 * @brief       4.3����ݴ�����-GT9xxx ��������
 *   @note      GTϵ�е��ݴ�����ICͨ������,������֧��: GT9147/GT917S/GT968/GT1151/GT9271 �ȶ���
 *              ����IC, ��Щ����IC��ID��һ��, ��������������Ҫ���κ��޸ļ���ͨ��������ֱ������
 *
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
 * V1.0 20200425
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "string.h"
#include "./BSP/TOUCH/ctiic.h"
#include "./BSP/TOUCH/gt9xxx.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

/* return 1fail 0succeed */
uint8_t gt9xxx_wr_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t ret = 0;
    
    ct_iic_start();
    ct_iic_send_byte(GT9XXX_CMD_WR);
    ct_iic_wait_ack();
    ct_iic_send_byte(reg >> 8);
    ct_iic_wait_ack();
    ct_iic_send_byte(reg & 0xFF);
    ct_iic_wait_ack();
    
    for (uint8_t i = 0; i < len; i++)
    {
        ct_iic_send_byte(buf[i]);
        ret = ct_iic_wait_ack();
        
        if (ret) break;
    }
    
    ct_iic_stop();
    return ret;
}

void gt9xxx_rd_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    
    ct_iic_start();
    ct_iic_send_byte(GT9XXX_CMD_WR);
    ct_iic_wait_ack();
    ct_iic_send_byte(reg >> 8);
    ct_iic_wait_ack();
    ct_iic_send_byte(reg & 0xFF);
    ct_iic_wait_ack();
    
    ct_iic_start();
    ct_iic_send_byte(GT9XXX_CMD_RD);    
    ct_iic_wait_ack();
    
    for (i = 0; i < len; i++)
    {
        /* 1:ack 0:nack */
        buf[i] = ct_iic_read_byte(i == (len - 1)?0:1);
    }
    
    ct_iic_stop(); 
}

uint8_t gt9xxx_init(void)
{ 
    uint8_t temp[5];
    
    GPIO_InitTypeDef gpio_init_struct;
    
    GT9XXX_RST_GPIO_CLK_ENABLE();   /* RST����ʱ��ʹ�� */
    GT9XXX_INT_GPIO_CLK_ENABLE();   /* INT����ʱ��ʹ�� */

    gpio_init_struct.Pin = GT9XXX_RST_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(GT9XXX_RST_GPIO_PORT, &gpio_init_struct); /* ��ʼ��RST���� */
    
    gpio_init_struct.Pin = GT9XXX_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(GT9XXX_INT_GPIO_PORT, &gpio_init_struct); /* ��ʼ��INT���� */
    
    ct_iic_init();  /* ��ʼ����������I2C���� */
    
    /* ͨѶ��ַ 0x28/0x29 */
    HAL_GPIO_WritePin(GT9XXX_INT_GPIO_PORT, GT9XXX_INT_GPIO_PIN, GPIO_PIN_SET);     /* INT����ߵ�ƽ */
    GT9XXX_RST(0);  /* ��λ */
    delay_ms(10);
    GT9XXX_RST(1);  /* �ͷŸ�λ */
    delay_ms(10);
    
    /* INT����ģʽ����, ����ģʽ, �������� */
    gpio_init_struct.Pin = GT9XXX_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                /* ���� */
    gpio_init_struct.Pull = GPIO_NOPULL;                    /* ����������������ģʽ */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(GT9XXX_INT_GPIO_PORT, &gpio_init_struct); /* ��ʼ��INT���� */
   
    /* ��ȡID */
    delay_ms(100);
    gt9xxx_rd_reg(GT9XXX_PID_REG, temp, 4);
    temp[4] = 0;
    printf("CTP ID:%s\r\n",temp);
    
    /* ��λ */
    temp[0] = 0x02;
    gt9xxx_wr_reg(GT9XXX_CTRL_REG, temp, 1);
    
    delay_ms(10);
    
    /* ������ȡ���� */
    temp[0] = 0x00;
    gt9xxx_wr_reg(GT9XXX_CTRL_REG, temp, 1);
    
    
    return 0;
}

void gt9xxx_scan(void)
{
    uint8_t mode = 0;
    uint8_t i;
    uint8_t buf[4];
    uint16_t x,y;
    
    /* ��ȡ״̬�Ĵ��� */
    gt9xxx_rd_reg(GT9XXX_GSTID_REG, &mode, 1);
    
    if ((mode & 0x80) && ((mode & 0xF) <= 5))
    {
       i = 0;
       gt9xxx_wr_reg(GT9XXX_GSTID_REG, &i, 1);  /* ������ */
    }
    
    if ((mode & 0xF) && ((mode & 0xF) <= 5))
    {
        gt9xxx_rd_reg(GT9XXX_TP1_REG, buf, 4);
        
        x = ((uint16_t)buf[1] << 8) + buf[0];
        y = ((uint16_t)buf[3] << 8) + buf[2];
        
        printf("x:%d y:%d\r\n",x,y);
    }
    
}






















