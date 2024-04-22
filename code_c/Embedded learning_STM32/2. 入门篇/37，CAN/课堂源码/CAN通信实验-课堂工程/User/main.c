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
    
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    usart_init(115200);                     /* 串口初始化为115200 */
    usmart_dev.init(72);                    /* 初始化USMART */
    led_init();                             /* 初始化LED */
    key_init();                             /* 初始化按键 */
    can_init();                             /* 初始化CAN */
    
    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES) /* KEY0按下,发送一次数据 */
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
            LED0_TOGGLE(); /* 提示系统正在运行 */
            t = 0;
            cnt++;
        }
    }
}
