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
    uint8_t wd_buf[512];
    uint8_t rd_buf[512];

    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(360, 25, 2, 8);    /* 设置时钟,180Mhz */
    delay_init(180);                        /* 延时初始化 */
    usart_init(115200);                     /* 串口初始化为115200 */
    usmart_dev.init(90);                    /* 初始化USMART */
    led_init();                             /* 初始化LED */
    key_init();                             /* 初始化按键 */
    sdram_init();                           /* 初始化SDRAM */

    my_mem_init(SRAMIN);                    /* 初始化内部内存池 */
    my_mem_init(SRAMEX);                    /* 初始化外部内存池 */
    my_mem_init(SRAMCCM);                   /* 初始化CCM内存池 */
    
    nand_init();                            /* NAND FLASH初始化 */
    
    while (1)
    {
        key = key_scan(0);

        switch (key)
        {
            case KEY0_PRES:     /* KEY0按下,读取数据 */
                nand_readpage(100 * (128/2), 0, rd_buf, 512);
                printf("Read data: \r\n");
                for (i = 0; i < 512; i++)
                {
                    printf("%#x ", rd_buf[i]);     /* 串口打印读取到的数据 */
                }
                printf("end      \r\n");

                break;

            case KEY1_PRES:     /* KEY1按下,写入数据 */
                
                printf("\r\n");

                for (i = 0; i < 256; i++)
                {
                    wd_buf[i] = i;
                    printf("%#x ",wd_buf[i]);
                }

                key = nand_writepage(100 * (128/2), 0, wd_buf, NAND_ECC_SECTOR_SIZE / 2);

                if (key == 0)
                {
                    printf("Write data successed    \r\n");
                }
                else 
                {
                    printf("Write data failed       \r\n");
                }
                break;

            case KEY2_PRES:     /* KEY2按下,写入数据 */
                key = nand_writepage(100 * (128/2), 300, "123456789ABCD", 13);      /* 写入扇区 */
                
                if (key == 0)
                {
                    printf("Write data successed    \r\n");
                }
                else 
                {
                    printf("Write data failed       \r\n");
                }
                break;
                
            case WKUP_PRES:     /* WKUP按下，测试 */

                key = nand_eraseblock(100);
                if (key == 0)
                {
                    printf("ERASE data successed    \r\n");
                }
                else 
                {
                    printf("ERASE data failed       \r\n");
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
