#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/RNG/rng.h"

int main(void)
{
    uint8_t key;
    uint32_t value = 0;
    uint32_t value_range = 0;
    
    sys_cache_enable();                 /* 打开L1-Cache */
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(240, 2, 2, 4); /* 设置时钟, 480Mhz */
    delay_init(480);                    /* 延时初始化 */
    usart_init(115200);                 /* 初始化串口 */
    led_init();                         /* 初始化LED */
    key_init();                         /* 初始化按键 */
    LED0(0);                            /* 先点亮LED0 */
    rng_init();                         /* RNG初始化 */
    
    while (1)
    {
        key = key_scan(0);              /* 得到键值 */

        if (key)
        {
            switch (key)
            {
                case WKUP_PRES:         /* 控制LED0(RED)翻转 */
                    value = rng_get_random_num();
                    printf("value:%d \r\n", value);
                    LED0_TOGGLE();      /* LED0状态取反 */
                    break;

                case KEY1_PRES:         /* 控制LED1(GREEN)翻转 */
                    value_range = rng_get_random_range(0, 9);
                    printf("value_range:%d \r\n", value_range);
                    LED1_TOGGLE();      /* LED1状态取反 */
                    break;

                case KEY0_PRES:         /* 控制LED2(BLUE)翻转 */
                    LED2_TOGGLE();      /* LED2状态取反 */
                    break;
            } 
        }
        else
        {
            delay_ms(10);
        }
    }
}



