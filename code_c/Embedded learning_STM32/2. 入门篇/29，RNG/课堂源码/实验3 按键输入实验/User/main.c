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
    
    sys_cache_enable();                 /* ��L1-Cache */
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(240, 2, 2, 4); /* ����ʱ��, 480Mhz */
    delay_init(480);                    /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ��ʼ������ */
    led_init();                         /* ��ʼ��LED */
    key_init();                         /* ��ʼ������ */
    LED0(0);                            /* �ȵ���LED0 */
    rng_init();                         /* RNG��ʼ�� */
    
    while (1)
    {
        key = key_scan(0);              /* �õ���ֵ */

        if (key)
        {
            switch (key)
            {
                case WKUP_PRES:         /* ����LED0(RED)��ת */
                    value = rng_get_random_num();
                    printf("value:%d \r\n", value);
                    LED0_TOGGLE();      /* LED0״̬ȡ�� */
                    break;

                case KEY1_PRES:         /* ����LED1(GREEN)��ת */
                    value_range = rng_get_random_range(0, 9);
                    printf("value_range:%d \r\n", value_range);
                    LED1_TOGGLE();      /* LED1״̬ȡ�� */
                    break;

                case KEY0_PRES:         /* ����LED2(BLUE)��ת */
                    LED2_TOGGLE();      /* LED2״̬ȡ�� */
                    break;
            } 
        }
        else
        {
            delay_ms(10);
        }
    }
}



