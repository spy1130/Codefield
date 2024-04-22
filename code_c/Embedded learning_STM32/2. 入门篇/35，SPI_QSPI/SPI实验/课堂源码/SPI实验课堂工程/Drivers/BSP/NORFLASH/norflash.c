#include "./BSP/SPI/spi.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/NORFLASH/norflash.h"


void norflash_init(void)
{
    NORFLASH_CS_GPIO_CLK_ENABLE();      /* NORFLASH CS�� ʱ��ʹ�� */

    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = NORFLASH_CS_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(NORFLASH_CS_GPIO_PORT, &gpio_init_struct);    /* SCK����ģʽ����(�������) */

    spi2_init();
    spi2_read_write_byte(0xFF); /* ���DR������ */
    
    NORFLASH_CS(1);
}

uint8_t norflash_rd_sr1(void)
{
    uint8_t rec_data = 0;
    
    NORFLASH_CS(0);
    spi2_read_write_byte(0x05);     /* ��״̬�Ĵ���1 */
    rec_data = spi2_read_write_byte(0xFF);
    NORFLASH_CS(1);
    
    return rec_data;
}

uint8_t norflash_read_data(uint32_t addr)
{
    uint8_t rec_data = 0;
    
    NORFLASH_CS(0);
    
    /* 1 ���Ͷ����� */
    spi2_read_write_byte(0x03);
    
    /* 2 ���͵�ַ */
    spi2_read_write_byte(addr >> 16);
    spi2_read_write_byte(addr >> 8);
    spi2_read_write_byte(addr);
       
    /* 3 ��ȡ���� */
    rec_data = spi2_read_write_byte(0xFF);
    
    NORFLASH_CS(1);
    return rec_data;
}

void norflash_erase_sector(uint32_t addr)
{
    /* 1 дʹ�� */
    NORFLASH_CS(0);
    spi2_read_write_byte(0x06);
    NORFLASH_CS(1);
    
    /* 2 �ȴ����� */
    while(norflash_rd_sr1()&0x01);
    
    /* 3 ������������ָ�� */
    NORFLASH_CS(0);
    spi2_read_write_byte(0x20);
    
    /* 4 ���͵�ַ */
    spi2_read_write_byte(addr >> 16);
    spi2_read_write_byte(addr >> 8);
    spi2_read_write_byte(addr);
    NORFLASH_CS(1);
    
    /* 5 �ȴ����� */
    while(norflash_rd_sr1()&0x01);
}

void norflash_write_page(uint8_t data, uint32_t addr)
{
    /* 1 �������� */
    norflash_erase_sector(addr);
    
    /* 2 дʹ�� */
    NORFLASH_CS(0);
    spi2_read_write_byte(0x06);
    NORFLASH_CS(1);
    
    /* 3 ����ҳдָ�� */
    NORFLASH_CS(0);
    spi2_read_write_byte(0x02);
    
    /* 4 ���͵�ַ */
    spi2_read_write_byte(addr >> 16);
    spi2_read_write_byte(addr >> 8);
    spi2_read_write_byte(addr);
    
    /* 5 Ҫд������� */
    spi2_read_write_byte(data);
    NORFLASH_CS(1);
    
    /* 6 �ȴ�д�����(�ȴ�����) */
    while(norflash_rd_sr1()&0x01);
}


















