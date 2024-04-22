/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-4-20
 * @brief       APC3216C光环境传感器 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 F429开发板
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
#include "./BSP/LCD/lcd.h"
#include "./BSP/SDRAM/sdram.h"
#include "./USMART/usmart.h"
#include "./BSP/AP3216C/ap3216c.h"

int main(void)
{
    uint16_t ir, als, ps;

    HAL_Init();                                        /* 初始化HAL库 */
    sys_stm32_clock_init(360, 25, 2, 8);               /* 设置时钟,180Mhz */
    delay_init(180);                                   /* 延时初始化 */
    usart_init(115200);                                /* 初始化USART */
    usmart_dev.init(90);                               /* 初始化USMART */
    led_init();                                        /* 初始化LED */
    sdram_init();                                      /* 初始化SDRAM */
    lcd_init();                                        /* 初始化LCD */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "AP3216C TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);

    while (ap3216c_init())                             /* 检测不到AP3216C */
    {
        lcd_show_string(30, 130, 200, 16, 16, "AP3216C Check Failed!", RED);
        delay_ms(500);
        lcd_show_string(30, 130, 200, 16, 16, "Please Check!      ", RED);
        delay_ms(500);
        LED0_TOGGLE();                                 /* 红灯闪烁 */
    }

    lcd_show_string(30, 130, 200, 16, 16, "AP3216C Ready!", RED);
    lcd_show_string(30, 160, 200, 16, 16, " IR:", RED);
    lcd_show_string(30, 180, 200, 16, 16, " PS:", RED);
    lcd_show_string(30, 200, 200, 16, 16, "ALS:", RED);

    while (1)
    {
        ap3216c_read_data(&ir, &ps, &als);             /* 读取数据  */
        lcd_show_num(30 + 32, 160, ir, 5, 16, BLUE);   /* 显示IR数据 */
        lcd_show_num(30 + 32, 180, ps, 5, 16, BLUE);   /* 显示PS数据 */
        lcd_show_num(30 + 32, 200, als, 5, 16, BLUE);  /* 显示ALS数据  */

        LED0_TOGGLE();                                 /* 提示系统正在运行 */
        delay_ms(120); 
    }
}

