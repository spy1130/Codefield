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
    
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    usmart_dev.init(72);                /* 初始化USMART */
    led_init();                         /* 初始化LED */
    lcd_init();                         /* 初始化LCD */
    key_init();                         /* 初始化按键 */
    norflash_init();
    
    while (1)
    {
        key = key_scan(0);

        if (key == KEY1_PRES) /* KEY1按下,写入 */
        {
            norflash_write_page('5', 0x123457); /* 地址范围0~0xFFFFFF */
            printf("write finish \r\n");
        }

        if (key == KEY0_PRES) /* KEY0按下,读取数据 */
        {
            rec_data = norflash_read_data(0x123456);
            printf("read data : %c \r\n", rec_data);
        }

        i++;

        if (i == 20)
        {
            LED0_TOGGLE(); /* LED0闪烁 */
            i = 0;
        }

        delay_ms(10);
    }
}
