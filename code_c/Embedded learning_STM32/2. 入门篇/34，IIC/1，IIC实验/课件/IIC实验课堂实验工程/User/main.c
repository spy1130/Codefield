#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./USMART/usmart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/24CXX/24cxx.h"

int main(void)
{
    uint8_t key;
    uint8_t i = 0;
    uint8_t data = 0;
    
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟, 72Mhz */
    delay_init(72);                             /* 延时初始化 */
    usart_init(115200);                         /* 串口初始化为115200 */
    led_init();                                 /* 初始化LED */
    key_init();                                 /* 初始化按键 */
    at24c02_init();
    
    while (1)
    {
        key = key_scan(0);

        if (key == KEY1_PRES)
        {
            at24c02_write_one_byte(100, 66);
            printf("write data \r\n");
        }

        if (key == KEY0_PRES)
        {
            data = at24c02_read_one_byte(100);
            printf("read data:%d \r\n", data);
        }

        i++;

        if (i % 20 == 0)
        {
            LED0_TOGGLE();  /* 红灯闪烁 */
            i = 0;
        }

        delay_ms(10);
    }
}
