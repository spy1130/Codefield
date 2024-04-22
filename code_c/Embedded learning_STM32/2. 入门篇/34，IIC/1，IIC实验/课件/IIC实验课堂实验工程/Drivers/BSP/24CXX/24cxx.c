#include "./BSP/IIC/myiic.h"
#include "./BSP/24CXX/24cxx.h"
#include "./SYSTEM/delay/delay.h"

void at24c02_init(void)
{
    iic_init();
}

void at24c02_write_one_byte(uint8_t addr, uint8_t data)
{
    /* 1��������ʼ�ź� */
    iic_start();
    
    /* 2������ͨѶ��ַ(д������ַ) */
    iic_send_byte(0xA0);
    
    /* 3���ȴ�Ӧ���ź� */
    iic_wait_ack();
    
    /* 4�������ڴ��ַ */
    iic_send_byte(addr);
    
    /* 5���ȴ�Ӧ���ź� */
    iic_wait_ack();
    
    /* 6������д������ */
    iic_send_byte(data);
    
    /* 7���ȴ�Ӧ���ź� */
    iic_wait_ack();
    
    /* 8������ֹͣ�ź� */
    iic_stop();
    
    /* �ȴ�EEPROMд����� */
    delay_ms(10);
}

uint8_t at24c02_read_one_byte(uint8_t addr)
{
    uint8_t rec = 0;
    
    /* 1��������ʼ�ź� */
    iic_start();
    
    /* 2������ͨѶ��ַ(д������ַ) */
    iic_send_byte(0xA0);
    
    /* 3���ȴ�Ӧ���ź� */
    iic_wait_ack();
    
    /* 4�������ڴ��ַ */
    iic_send_byte(addr);
    
    /* 5���ȴ�Ӧ���ź� */
    iic_wait_ack();
    
    /* 6��������ʼ�ź� */
    iic_start();
    
    /* 7������ͨѶ��ַ(��������ַ) */
    iic_send_byte(0xA1);
    
    /* 8���ȴ�Ӧ���ź� */
    iic_wait_ack();
    
    /* 9���ȴ��������� */
    rec = iic_read_byte(0);
    /* 10�����ͷ�Ӧ�𣨻�ȡ�õ�ַ���ɣ� */
    
    /* 11������ֹͣ�ź� */
    iic_stop();
    
    return rec;
}

