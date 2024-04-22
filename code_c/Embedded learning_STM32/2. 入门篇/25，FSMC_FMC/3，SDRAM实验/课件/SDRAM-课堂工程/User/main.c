#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SDRAM/sdram.h"

int main(void)
{
    uint8_t key_value = 0;
    uint16_t tmp_value = 0;
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(360, 25, 2, 8);    /* ����ʱ��,180Mhz */
    delay_init(180);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ��ʼ��USART */
    led_init();                             /* ��ʼ��LED */
    key_init();                             /* ��ʼ������ */
    lcd_init();                             /* ��ʼ��LCD */
    sdram_init();                           /* ��ʼ��SDRAM */
    printf("sdram project \r\n");
    
    while(1)
    {
        key_value = key_scan(0);
        
        if (key_value == KEY1_PRES)
        {
            *(uint16_t *)(SDRAM_BASE_ADDR + 0x1000) = 0x8888;
            printf("Write Data succeed \r\n");
        }
        
        if (key_value == KEY0_PRES)
        {
            tmp_value = *(uint16_t *)(SDRAM_BASE_ADDR + 0x1000);
            printf("tmp_value %#x \r\n", tmp_value);
        }
        
        LED0_TOGGLE(); /* �����˸ */
        delay_ms(200);
    }
}

