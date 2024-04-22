#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./USMART/usmart.h"
#include "./BSP/LED/led.h"
#include "./BSP/RTC/rtc.h"

int main(void)
{
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    usart_init(115200);                     /* 串口初始化为115200 */
    usmart_dev.init(72);                    /* 初始化USMART */
    led_init();                             /* 初始化LED */
    rtc_init();
    
    while (1)
    {
        rtc_get_time();
        printf("Date:%04d-%02d-%02d ", calendar.year, calendar.month, calendar.date);
        printf("Time:%02d:%02d:%02d \r\n", calendar.hour, calendar.min, calendar.sec);
        delay_ms(1000);
    }
}
