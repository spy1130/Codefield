#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./USMART/usmart.h"
#include "./BSP/MPU/mpu.h"
#include "./BSP/LED/led.h"
#include "./BSP/RTC/rtc.h"

extern RTC_HandleTypeDef g_rtc_handle;  /* RTC��� */

int main(void)
{
    RTC_TimeTypeDef rtc_t;
    RTC_DateTypeDef rtc_d;
    
    sys_cache_enable();                 /* ��L1-Cache */
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(240, 2, 2, 4); /* ����ʱ��, 480Mhz */
    delay_init(480);                    /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(240);               /* ��ʼ��USMART */
    led_init();                         /* ��ʼ��LED */
    mpu_memory_protection();            /* ������ش洢���� */
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


















