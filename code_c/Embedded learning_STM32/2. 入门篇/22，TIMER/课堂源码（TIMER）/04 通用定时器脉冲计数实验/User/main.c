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
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/gtim.h"


extern TIM_HandleTypeDef g_timx_cnt_chy_handle;        /* 定时器x句柄 */

int main(void)
{
    uint16_t curcnt;
    uint16_t oldcnt;
    uint8_t key;
    uint8_t t = 0;
    
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟, 72Mhz */
    delay_init(72);                             /* 延时初始化 */
    usart_init(115200);                         /* 串口初始化为115200 */
    led_init();                                 /* 初始化LED */
    key_init();                                 /* 初始化按键 */
    gtim_timx_cnt_chy_init(0);

    while (1)
    {
        key = key_scan(0);
        if(key == KEY0_PRES)
        {
            __HAL_TIM_SET_COUNTER(&g_timx_cnt_chy_handle, 0);
        }
        
        curcnt = __HAL_TIM_GET_COUNTER(&g_timx_cnt_chy_handle);
        if(oldcnt != curcnt)
        {
            oldcnt = curcnt;
            printf("CNT:%d\r\n", oldcnt);
        }
        
        t++;
        
        if(t > 20)
        {
            t = 0;
            LED0_TOGGLE();
        }
        delay_ms(10);
    }
}


