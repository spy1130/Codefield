/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       跑马灯 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/TIMER/atim.h"


extern TIM_HandleTypeDef g_timx_comp_pwm_handle;       /* 定时器x句柄 */

int main(void)
{
    uint8_t t = 0;

    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    led_init();                         /* 初始化LED */
    atim_timx_comp_pwm_init(1000 - 1, 72 - 1);
    
    __HAL_TIM_SET_COMPARE(&g_timx_comp_pwm_handle, TIM_CHANNEL_1, 250 - 1);
    __HAL_TIM_SET_COMPARE(&g_timx_comp_pwm_handle, TIM_CHANNEL_2, 500 - 1);
    __HAL_TIM_SET_COMPARE(&g_timx_comp_pwm_handle, TIM_CHANNEL_3, 750 - 1);
    __HAL_TIM_SET_COMPARE(&g_timx_comp_pwm_handle, TIM_CHANNEL_4, 1000 - 1);

    while (1)
    {
        delay_ms(10);
        t++;

        if (t >= 20)
        {
            LED0_TOGGLE(); /* LED0(RED)闪烁 */
            t = 0;
        }
    }
}





