/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-21
 * @brief       TFTLCD(MCU屏) 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 MiniSTM32 V4开发板
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


/**
 * @brief       清屏函数
 * @param       color: 要清屏的颜色
 * @retval      无
 */
void lcd_clear1(uint16_t color)
{
    uint32_t index = 0;
    uint32_t totalpoint = lcddev.width;
    totalpoint *= lcddev.height;    /* 得到总点数 */
    lcd_set_cursor(0x00, 0x0000);   /* 设置光标位置 */
    lcd_write_ram_prepare();        /* 开始写入GRAM */

    LCD_RS(1);                      /* RS = 1 表示写数据 */
    LCD_CS(0);
    
    for (index = 0; index < totalpoint; index++)
    {
        delay_ms(1);  /* 可以通过调节延时函数来观察LCD屏扫描方式 */
        LCD_DATA_OUT(color);        /* 写入要写的数据 */
        LCD_WR(0);
        LCD_WR(1);
    }
    
    LCD_CS(1);
}

int main(void)
{
    uint8_t x = 0;
    uint8_t lcd_id[12];

    HAL_Init();                                         /* HAL库初始化 */
    sys_stm32_clock_init(RCC_PLL_MUL9);                 /* 设置时钟, 72Mhz */
    delay_init(72);                                     /* 延时初始化 */
    usart_init(115200);                                 /* 串口初始化为115200 */
    led_init();                                         /* 初始化LED */
    lcd_init();                                         /* 初始化LCD */
    g_point_color = RED;
    sprintf((char *)lcd_id, "LCD ID:%04X", lcddev.id);  /* 将LCD ID打印到lcd_id数组 */
    
    lcd_scan_dir(L2R_U2D);  /* 扫描方向定义，默认是从左到右,从上到下，通过形参可以选择8种扫描方向 */
    
    while (1)
    {
        switch (x)
        {
        case 0:
            lcd_clear1(BLUE);
            break;

        case 1:
            lcd_clear1(RED);
            break;

        case 2:
            lcd_clear1(MAGENTA);
            break;

        case 3:
            lcd_clear1(GREEN);
            break;

        case 4:
            lcd_clear1(BRRED);
            break;
        }

        x++;

        if (x == 5)
            x = 0;

        LED0_TOGGLE(); /* 红灯闪烁 */
        delay_ms(1000);
    }
}
