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
    
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(360, 25, 2, 8);    /* 设置时钟,180Mhz */
    delay_init(180);                        /* 延时初始化 */
    usart_init(115200);                     /* 初始化USART */
    led_init();                             /* 初始化LED */
    key_init();                             /* 初始化按键 */
    lcd_init();                             /* 初始化LCD */
    sdram_init();                           /* 初始化SDRAM */
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
        
        LED0_TOGGLE(); /* 红灯闪烁 */
        delay_ms(200);
    }
}

