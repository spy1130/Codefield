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

    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(360, 25, 2, 8);    /* ����ʱ��,180Mhz */
    delay_init(180);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(90);                    /* ��ʼ��USMART */
    led_init();                             /* ��ʼ��LED */
    key_init();                             /* ��ʼ������ */
    sdram_init();                           /* ��ʼ��SDRAM */

    my_mem_init(SRAMIN);                    /* ��ʼ���ڲ��ڴ�� */
    my_mem_init(SRAMEX);                    /* ��ʼ���ⲿ�ڴ�� */
    my_mem_init(SRAMCCM);                   /* ��ʼ��CCM�ڴ�� */
    
    nand_init();                            /* NAND FLASH��ʼ�� */
    
    while (1)
    {
        key = key_scan(0);

        switch (key)
        {
            case KEY0_PRES:     /* KEY0����,��ȡ���� */
                nand_readpage(100 * (128/2), 0, rd_buf, 512);
                printf("Read data: \r\n");
                for (i = 0; i < 512; i++)
                {
                    printf("%#x ", rd_buf[i]);     /* ���ڴ�ӡ��ȡ�������� */
                }
                printf("end      \r\n");

                break;

            case KEY1_PRES:     /* KEY1����,д������ */
                
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

            case KEY2_PRES:     /* KEY2����,д������ */
                key = nand_writepage(100 * (128/2), 300, "123456789ABCD", 13);      /* д������ */
                
                if (key == 0)
                {
                    printf("Write data successed    \r\n");
                }
                else 
                {
                    printf("Write data failed       \r\n");
                }
                break;
                
            case WKUP_PRES:     /* WKUP���£����� */

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
            LED0_TOGGLE();               /* �����˸ */
            t = 0;
        }
    }
}
