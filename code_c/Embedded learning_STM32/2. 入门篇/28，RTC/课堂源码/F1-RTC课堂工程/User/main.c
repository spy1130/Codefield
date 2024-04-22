#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./USMART/usmart.h"
#include "./BSP/LED/led.h"
#include "./BSP/RTC/rtc.h"

int main(void)
{
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* ����ʱ��, 72Mhz */
    delay_init(72);                         /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(72);                    /* ��ʼ��USMART */
    led_init();                             /* ��ʼ��LED */
    rtc_init();
    
    while (1)
    {
        rtc_get_time();
        printf("Date:%04d-%02d-%02d ", calendar.year, calendar.month, calendar.date);
        printf("Time:%02d:%02d:%02d \r\n", calendar.hour, calendar.min, calendar.sec);
        delay_ms(1000);
    }
}
