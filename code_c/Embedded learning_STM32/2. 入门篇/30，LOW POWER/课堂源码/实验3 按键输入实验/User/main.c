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
    
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    usart_init(115200);                     /* 串口初始化 */
    led_init();                             /* 初始化LED */
    key_init();                             /* 初始化按键 */
    pwr_wkup_key_init();                    /* WKUP引脚初始化 */
    pwr_pvd_init();                         /* PVD配置 */
    
    printf("Enter to LowPower Test \r\n");
    
    while(1)
    {
        key = key_scan(0);                  /* 得到键值 */

        if (key)
        {
            switch (key)
            {
                /* 进入待机模式 */
                case KEY2_PRES:
                
                    /* 使能电源时钟 */
                    __HAL_RCC_PWR_CLK_ENABLE();
                
                    /* 使能WKUP上升沿的唤醒功能 */
                    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
                
                    /* 清除唤醒标记 */
                    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
                
                    printf("Enter STANDBY Mode \r\n");
                    
                    HAL_PWR_EnterSTANDBYMode();
                
                    printf("Exit STANDBY Mode \r\n");

                    break;

                /* 进入停止模式 */
                case KEY1_PRES:
                    
                    LED1(0);        /* 点亮绿灯,提示进入停止模式 */
                    
                    printf("Enter STOP Mode \r\n");
                    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
                    
                    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 重新设置时钟, 72Mhz */
                    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
                    HAL_SuspendTick();
                
                    printf("Exit STOP Mode \r\n");
                
                    LED1(1);
                    break;
                
                /* 进入睡眠模式 */
                case KEY0_PRES:
                    
                    printf("Enter SLEEP Mode \r\n");
                    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
                    printf("Exit SLEEP Mode \r\n");
                
                    break;
            } 
        }
        
        if ((t % 20) == 0)
        {
            LED0_TOGGLE();              /* 每200ms,翻转一次LED0 */
        }

        delay_ms(10);
        t++;
    
    }
}

