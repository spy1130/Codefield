#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/BEEP/beep.h"
#include "./BSP/KEY/key.h"
#include "./BSP/PWR/pwr.h"

int main(void)
{
    uint8_t key;
    uint8_t t = 0;
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* ����ʱ��, 72Mhz */
    delay_init(72);                         /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ�� */
    led_init();                             /* ��ʼ��LED */
    key_init();                             /* ��ʼ������ */
    pwr_wkup_key_init();                    /* WKUP���ų�ʼ�� */
    pwr_pvd_init();                         /* PVD���� */
    
    printf("Enter to LowPower Test \r\n");
    
    while(1)
    {
        key = key_scan(0);                  /* �õ���ֵ */

        if (key)
        {
            switch (key)
            {
                /* �������ģʽ */
                case KEY2_PRES:
                
                    /* ʹ�ܵ�Դʱ�� */
                    __HAL_RCC_PWR_CLK_ENABLE();
                
                    /* ʹ��WKUP�����صĻ��ѹ��� */
                    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
                
                    /* ������ѱ�� */
                    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
                
                    printf("Enter STANDBY Mode \r\n");
                    
                    HAL_PWR_EnterSTANDBYMode();
                
                    printf("Exit STANDBY Mode \r\n");

                    break;

                /* ����ֹͣģʽ */
                case KEY1_PRES:
                    
                    LED1(0);        /* �����̵�,��ʾ����ֹͣģʽ */
                    
                    printf("Enter STOP Mode \r\n");
                    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
                    
                    sys_stm32_clock_init(RCC_PLL_MUL9);     /* ��������ʱ��, 72Mhz */
                    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
                    HAL_SuspendTick();
                
                    printf("Exit STOP Mode \r\n");
                
                    LED1(1);
                    break;
                
                /* ����˯��ģʽ */
                case KEY0_PRES:
                    
                    printf("Enter SLEEP Mode \r\n");
                    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
                    printf("Exit SLEEP Mode \r\n");
                
                    break;
            } 
        }
        
        if ((t % 20) == 0)
        {
            LED0_TOGGLE();              /* ÿ200ms,��תһ��LED0 */
        }

        delay_ms(10);
        t++;
    
    }
}

