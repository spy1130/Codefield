#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./USMART/usmart.h"
#include "./BSP/MPU/mpu.h"
#include "./BSP/LED/led.h"
#include "./BSP/RTC/rtc.h"

extern RTC_HandleTypeDef g_rtc_handle;  /* RTC句柄 */

int main(void)
{
    RTC_TimeTypeDef rtc_t;
    RTC_DateTypeDef rtc_d;
    
    sys_cache_enable();                 /* 打开L1-Cache */
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(240, 2, 2, 4); /* 设置时钟, 480Mhz */
    delay_init(480);                    /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    usmart_dev.init(240);               /* 初始化USMART */
    led_init();                         /* 初始化LED */
    mpu_memory_protection();            /* 保护相关存储区域 */
    rtc_init();
    
    while (1)
    {
        HAL_RTC_GetTime(&g_rtc_handle, &rtc_t, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&g_rtc_handle, &rtc_d, RTC_FORMAT_BIN);
        
        printf("Date:20%02d-%02d-%02d ",rtc_d.Year, rtc_d.Month, rtc_d.Date);
        printf("Time:%02d:%02d:%02d \r\n",rtc_t.Hours, rtc_t.Minutes, rtc_t.Seconds);
        
        delay_ms(1000);
    }
}


















