#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./USMART/usmart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/CAN/can.h"


int main(void)
{
    uint8_t key;
    uint8_t i = 0, t = 0;
    uint8_t cnt = 0;
    uint8_t canbuf[8];
    uint8_t rec_len = 0;
    uint8_t rec_buf[8];
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* ����ʱ��, 72Mhz */
    delay_init(72);                         /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(72);                    /* ��ʼ��USMART */
    led_init();                             /* ��ʼ��LED */
    key_init();                             /* ��ʼ������ */
    can_init();                             /* ��ʼ��CAN */
    
    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES) /* KEY0����,����һ������ */
        {
            for (i = 0; i < 8; i++)
            {
                canbuf[i] = i + cnt;
            }
            can_send_message(0x12345678, canbuf, 8);
        }
        
        rec_len = can_receive_message(rec_buf);
        
        if (rec_len)
        {
            for (uint8_t x = 0; x < rec_len; x++)
            {
                printf("%x ",rec_buf[x]);
            }
            printf("\r\n");
        }
        
        t++;
        delay_ms(10);

        if (t == 20)
        {
            LED0_TOGGLE(); /* ��ʾϵͳ�������� */
            t = 0;
            cnt++;
        }
    }
}
