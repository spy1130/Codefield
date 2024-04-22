#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./USMART/usmart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/NORFLASH/norflash.h"

int main(void)
{
    uint8_t key;
    uint16_t i = 0;
    uint8_t rec_data = 0;
    
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(72);                /* ��ʼ��USMART */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */
    norflash_init();
    
    while (1)
    {
        key = key_scan(0);

        if (key == KEY1_PRES) /* KEY1����,д�� */
        {
            norflash_write_page('5', 0x123457); /* ��ַ��Χ0~0xFFFFFF */
            printf("write finish \r\n");
        }

        if (key == KEY0_PRES) /* KEY0����,��ȡ���� */
        {
            rec_data = norflash_read_data(0x123456);
            printf("read data : %c \r\n", rec_data);
        }

        i++;

        if (i == 20)
        {
            LED0_TOGGLE(); /* LED0��˸ */
            i = 0;
        }

        delay_ms(10);
    }
}
