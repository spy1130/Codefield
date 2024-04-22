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
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/TIMER/gtim.h"


extern TIM_HandleTypeDef g_timx_pwm_chy_handle;     /* 定时器x句柄 */

int main(void)
{
    uint16_t ledrpwmval = 0;
    uint8_t dir = 1;
    
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟,72M */
    delay_init(72);                             /* 初始化延时函数 */
    led_init();                                 /* 初始化LED */
    gtim_timx_pwm_chy_init(500 - 1, 72 - 1);
    
    while(1)
    {
        delay_ms(10);

        if (dir)ledrpwmval++;               /* dir==1 ledrpwmval递增 */
        else ledrpwmval--;                  /* dir==0 ledrpwmval递减 */

        if (ledrpwmval > 300)dir = 0;       /* ledrpwmval到达300后，方向为递减 */
        if (ledrpwmval == 0)dir = 1;        /* ledrpwmval递减到0后，方向改为递增 */

        /* 修改比较值控制占空比 */
        __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, TIM_CHANNEL_2, ledrpwmval);
    }
}

