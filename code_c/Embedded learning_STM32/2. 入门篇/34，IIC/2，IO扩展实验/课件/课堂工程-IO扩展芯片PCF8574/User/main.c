/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-4-20
 * @brief       IO扩展实验 实验
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
#include "./BSP/KEY/key.h"
#include "./BSP/SDRAM/sdram.h"
#include "./USMART/usmart.h"
#include "./BSP/PCF8574/pcf8574.h"

int main(void)
{
    uint16_t i = 0;
    uint8_t key;
    
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(360, 25, 2, 8);    /* 设置时钟,180Mhz */
    delay_init(180);                        /* 延时初始化 */
    usart_init(115200);                     /* 初始化USART */
    usmart_dev.init(90);                    /* 初始化USMART */
    led_init();                             /* 初始化LED */
    key_init();                             /* 初始化按键 */
    sdram_init();                           /* 初始化SDRAM */
    lcd_init();                             /* 初始化LCD */
    pcf8574_init();
    
    while (1)
    {
        key = key_scan(0);
        
        if (key == KEY0_PRES)
        {
            pcf8574_write_bit(BEEP_IO, 0);
        }
        
        if (PCF8574_INT == 0)   /* 输入IO发生电平变化 */
        {
            key = pcf8574_read_bit(EX_IO);
            
            if (key == 0)
            {
                LED1_TOGGLE();
            }
        }
        
        i++;
        delay_ms(10);

        if (i == 20)
        {
            LED0_TOGGLE();          /* 红灯闪烁 */
            i = 0;
        }
    }
}

