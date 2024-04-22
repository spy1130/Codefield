#include "./BSP/SPI/spi.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/NORFLASH/norflash.h"


void norflash_init(void)
{
    NORFLASH_CS_GPIO_CLK_ENABLE();      /* NORFLASH CS脚 时钟使能 */

    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = NORFLASH_CS_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(NORFLASH_CS_GPIO_PORT, &gpio_init_struct);    /* SCK引脚模式设置(复用输出) */

    spi2_init();
    spi2_read_write_byte(0xFF); /* 清除DR的作用 */
    
    NORFLASH_CS(1);
}

uint8_t norflash_rd_sr1(void)
{
    uint8_t rec_data = 0;
    
    NORFLASH_CS(0);
    spi2_read_write_byte(0x05);     /* 读状态寄存器1 */
    rec_data = spi2_read_write_byte(0xFF);
    NORFLASH_CS(1);
    
    return rec_data;
}

uint8_t norflash_read_data(uint32_t addr)
{
    uint8_t rec_data = 0;
    
    NORFLASH_CS(0);
    
    /* 1 发送读命令 */
    spi2_read_write_byte(0x03);
    
    /* 2 发送地址 */
    spi2_read_write_byte(addr >> 16);
    spi2_read_write_byte(addr >> 8);
    spi2_read_write_byte(addr);
       
    /* 3 读取数据 */
    rec_data = spi2_read_write_byte(0xFF);
    
    NORFLASH_CS(1);
    return rec_data;
}

void norflash_erase_sector(uint32_t addr)
{
    /* 1 写使能 */
    NORFLASH_CS(0);
    spi2_read_write_byte(0x06);
    NORFLASH_CS(1);
    
    /* 2 等待空闲 */
    while(norflash_rd_sr1()&0x01);
    
    /* 3 发送扇区擦除指令 */
    NORFLASH_CS(0);
    spi2_read_write_byte(0x20);
    
    /* 4 发送地址 */
    spi2_read_write_byte(addr >> 16);
    spi2_read_write_byte(addr >> 8);
    spi2_read_write_byte(addr);
    NORFLASH_CS(1);
    
    /* 5 等待空闲 */
    while(norflash_rd_sr1()&0x01);
}

void norflash_write_page(uint8_t data, uint32_t addr)
{
    /* 1 擦除扇区 */
    norflash_erase_sector(addr);
    
    /* 2 写使能 */
    NORFLASH_CS(0);
    spi2_read_write_byte(0x06);
    NORFLASH_CS(1);
    
    /* 3 发送页写指令 */
    NORFLASH_CS(0);
    spi2_read_write_byte(0x02);
    
    /* 4 发送地址 */
    spi2_read_write_byte(addr >> 16);
    spi2_read_write_byte(addr >> 8);
    spi2_read_write_byte(addr);
    
    /* 5 要写入的数据 */
    spi2_read_write_byte(data);
    NORFLASH_CS(1);
    
    /* 6 等待写入完成(等待空闲) */
    while(norflash_rd_sr1()&0x01);
}


















