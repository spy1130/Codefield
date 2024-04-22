/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-4-20
 * @brief       NAND FLASH 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 F429开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./USMART/usmart.h"
#include "./BSP/SDRAM/sdram.h"
#include "./MALLOC/malloc.h"
#include "./BSP/NAND/ftl.h"
#include "./BSP/NAND/nand.h"
#include "./BSP/NAND/nandtester.h"

int main(void)
{
    uint8_t key, t = 0;
    uint16_t i;
    uint8_t *buf;
    uint8_t *backbuf;
    uint32_t eccval = 0x3F;

    HAL_Init();                                  /* 初始化HAL库 */
    sys_stm32_clock_init(360, 25, 2, 8);         /* 设置时钟,180Mhz */
    delay_init(180);                             /* 延时初始化 */
    usart_init(115200);                          /* 串口初始化为115200 */
    usmart_dev.init(90);                         /* 初始化USMART */
    led_init();                                  /* 初始化LED */
    key_init();                                  /* 初始化按键 */
    sdram_init();                                /* 初始化SDRAM */
    lcd_init();                                  /* 初始化LCD */

    my_mem_init(SRAMIN);                         /* 初始化内部内存池 */
    my_mem_init(SRAMEX);                         /* 初始化外部内存池 */
    my_mem_init(SRAMCCM);                        /* 初始化CCM内存池 */

    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "NAND TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:Read Sector 2", RED);
    lcd_show_string(30, 130, 200, 16, 16, "KEY1:Write Sector 2", RED);
    lcd_show_string(30, 150, 200, 16, 16, "KEY2:Recover Sector 2", RED);

    while (ftl_init())      /* 检测NAND FLASH,并初始化FTL */
    {
        lcd_show_string(30, 130, 200, 16, 16, "NAND Error!", RED);
        delay_ms(500);
        lcd_show_string(30, 130, 200, 16, 16, "Please Check! ", RED);
        delay_ms(500);
        LED0_TOGGLE();                                                   /* 红灯闪烁 */
    }
    
    backbuf = mymalloc(SRAMIN, NAND_ECC_SECTOR_SIZE);   /* 申请一个扇区的缓存 */
    buf = mymalloc(SRAMIN, NAND_ECC_SECTOR_SIZE);       /* 申请一个扇区的缓存 */
    sprintf((char *)buf, "NAND Size:%dMB", (nand_dev.block_totalnum / 1024) * (nand_dev.page_mainsize / 1024) * nand_dev.block_pagenum);
    lcd_show_string(30, 190, 200, 16, 16, (char *)buf, BLUE);            /* 显示NAND容量 */
    ftl_read_sectors(backbuf, 2, NAND_ECC_SECTOR_SIZE, 1);               /* 预先读取扇区2到备份区域,防止乱写导致文件系统损坏. */

    while (1)
    {
        key = key_scan(0);

        switch (key)
        {
            case KEY0_PRES:                                              /* KEY0按下,读取sector */
                key = ftl_read_sectors(buf, 2, NAND_ECC_SECTOR_SIZE, 1); /* 读取扇区 */

                if (key == 0)   /* 读取成功 */
                {
                    lcd_show_string(30, 210, 200, 16, 16, "USART1 Sending Data...  ", BLUE);
                    printf("Sector 2 data is:\r\n");

                    for (i = 0; i < NAND_ECC_SECTOR_SIZE; i++)
                    {
                        printf("%x ",buf[i]);                            /* 输出数据 */
                    }

                    printf("\r\ndata end.\r\n");
                    lcd_show_string(30, 210, 200, 16, 16, "USART1 Send Data Over!  ", BLUE); 
                }
                break;

            case KEY1_PRES:     /* KEY1按下,写入sector */
                for (i = 0; i < NAND_ECC_SECTOR_SIZE; i++)
                {
                    buf[i] = i + t;     /* 填充数据(随机的,根据t的值来确定) */
                }

                lcd_show_string(30, 210, 210, 16, 16, "Writing data to sector..", BLUE);
                key = ftl_write_sectors(buf, 2, NAND_ECC_SECTOR_SIZE, 1);                    /* 写入扇区 */

                if (key == 0)
                {
                    lcd_show_string(30, 210, 200, 16, 16, "Write data successed    ", BLUE); /* 写入成功 */
                }
                else 
                {
                    lcd_show_string(30, 210, 200, 16, 16, "Write data failed       ", BLUE); /* 写入失败 */
                }
                break;

            case KEY2_PRES:                                                                  /* KEY2按下,恢复sector的数据 */
                lcd_show_string(30, 210, 210, 16, 16, "Recovering data...      ", BLUE);
                key = ftl_write_sectors(backbuf, 2, NAND_ECC_SECTOR_SIZE, 1);                /* 写入扇区 */

                if (key == 0)
                {
                    lcd_show_string(30, 210, 200, 16, 16, "Recovering data OK      ", BLUE); /* 恢复成功 */
                }
                else 
                {
                    lcd_show_string(30, 210, 200, 16, 16, "Recovering data failed  ", BLUE); /* 恢复失败 */
                }
                break;
        }

        t++;
        delay_ms(10);

        if (t == 20)
        {
            LED0_TOGGLE();               /* 红灯闪烁 */
            t = 0;
        }
    }
}
