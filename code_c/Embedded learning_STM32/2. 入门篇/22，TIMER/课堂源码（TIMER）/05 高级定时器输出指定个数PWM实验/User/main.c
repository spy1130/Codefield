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
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/atim.h"


int main(void)
{
    uint8_t key;
    uint8_t t = 0;
    GPIO_InitTypeDef gpio_init_struct;
    
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    led_init();                         /* 初始化LED */
    key_init();                         /* 初始化按键 */
    
    /* 把PE5设置为输入，避免与PC6冲突 */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    gpio_init_struct.Pin = GPIO_PIN_5;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                /* 输入 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);
    
    atim_timx_npwm_chy_init(5000 - 1, 7200 - 1);
    
    atim_timx_npwm_chy_set(5);
    
    while (1)
    {
        key = key_scan(0);
        if(key == KEY0_PRES)
        {
            atim_timx_npwm_chy_set(6);
        }
 
        t++;
        delay_ms(10);

        if (t > 50)                    /* 控制LED0闪烁, 提示程序运行状态 */
        {
            t = 0;
            LED0_TOGGLE();
        }
    }
}






