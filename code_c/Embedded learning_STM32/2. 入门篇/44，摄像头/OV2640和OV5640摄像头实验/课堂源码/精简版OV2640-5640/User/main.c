#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./USMART/usmart.h"
#include "./BSP/MPU/mpu.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/btim.h"
#include "./BSP/DCMI/dcmi.h"
#include "./BSP/OV5640/ov5640.h"
#include "./BSP/USART2/usart2.h"
#include "./BSP/OV2640/ov2640.h"

uint8_t g_ov_mode = 0;                  /* bit0: 0,RGB565模式;  1,JPEG模式 */
uint16_t g_curline = 0;                 /* 摄像头输出数据,当前行编号 */
uint16_t g_yoffset = 0;                 /* y方向的偏移量 */

#define jpeg_buf_size   120 * 1024      /* 定义JPEG数据缓存jpeg_buf的大小(480KB字节) */
#define jpeg_line_size  4 * 1024        /* 定义DMA接收数据时,一行数据的最大值 */


/* JPEG数据采集,摄像头采用一行一行读取,定义行缓存 */
#if !(__ARMCC_VERSION >= 6010050)   /* 不是AC6编译器，即使用AC5编译器时 */
uint32_t g_dcmi_line_buf[2][jpeg_line_size] __attribute__((at(0x38000000)));
#else
uint32_t g_dcmi_line_buf[2][jpeg_line_size] __attribute__((section(".bss.ARM.__at_0x38000000")));
#endif

uint32_t g_jpeg_data_buf[jpeg_buf_size];    /* JPEG数据缓存buf,定义在内部SRAM */


/* buf中的JPEG有效数据长度 */
volatile uint32_t g_jpeg_data_len = 0;

/* JPEG数据采集完成标志
 * 0,数据没有采集完;
 * 1,数据采集完了,但是还没处理;
 * 2,数据已经处理完成了,可以开始下一帧接收
 */
volatile uint8_t g_jpeg_data_ok = 0;


uint16_t g_ov_type = 0;

/**
 * @brief       处理JPEG数据
 *   @ntoe      在DCMI_IRQHandler中断服务函数里面被调用
 *              当采集完一帧JPEG数据后,调用此函数,切换JPEG BUF.开始下一帧采集.
 *
 * @param       无
 * @retval      无
 */
void jpeg_data_process(void)
{
    uint16_t i;
    uint16_t rlen;          /* 剩余数据长度 */
    uint32_t *pbuf;
    g_curline = g_yoffset;  /* 行数复位 */

    if (g_ov_mode & 0X01)   /* 只有在JPEG格式下,才需要做处理. */
    {
        if (g_jpeg_data_ok == 0)    /* jpeg数据还未采集完? */
        {
            __HAL_DMA_DISABLE(&g_dma_dcmi_handle);      /* 关闭DMA */
            rlen = jpeg_line_size - __HAL_DMA_GET_COUNTER(&g_dma_dcmi_handle); /* 得到剩余数据长度 */
            pbuf = g_jpeg_data_buf + g_jpeg_data_len;   /* 偏移到有效数据末尾,继续添加 */

            if (DMA1_Stream1->CR & (1 << 19))
            {
                for (i = 0; i < rlen; i++)
                {
                    pbuf[i] = g_dcmi_line_buf[1][i];    /* 读取buf1里面的剩余数据 */
                }
            }
            else 
            {
                for (i = 0; i < rlen; i++)
                {
                    pbuf[i] = g_dcmi_line_buf[0][i];    /* 读取buf0里面的剩余数据 */
                }
            }
            
            g_jpeg_data_len += rlen;    /* 加上剩余长度 */
            g_jpeg_data_ok = 1;         /* 标记JPEG数据采集完成,等待其他函数处理 */
        }

        if (g_jpeg_data_ok == 2)        /* 上一次的jpeg数据已经被处理了 */
        {
            __HAL_DMA_SET_COUNTER(&g_dma_dcmi_handle, jpeg_line_size); /* 传输长度为jpeg_buf_size*4字节 */
            __HAL_DMA_ENABLE(&g_dma_dcmi_handle);   /* 重新传输 */
            g_jpeg_data_ok = 0;                     /* 标记数据未采集 */
            g_jpeg_data_len = 0;                    /* 数据重新开始 */
        }
    }
    else
    {
        lcd_set_cursor(0, 0);
        lcd_write_ram_prepare();        /* 开始写入GRAM */
    }
}

/**
 * @brief       JPEG数据接收回调函数
 *   @ntoe      在DMA1_Stream1_IRQHandler中断服务函数里面被调用
 *
 * @param       无
 * @retval      无
 */
void jpeg_dcmi_rx_callback(void)
{
    uint16_t i;
    volatile uint32_t *pbuf;
    pbuf = g_jpeg_data_buf + g_jpeg_data_len;   /* 偏移到有效数据末尾 */

    if (DMA1_Stream1->CR & (1 << 19))           /* buf0已满,正常处理buf1 */
    {
        for (i = 0; i < jpeg_line_size; i++)
        {
            pbuf[i] = g_dcmi_line_buf[0][i];    /* 读取buf0里面的数据 */
        }
        
        g_jpeg_data_len += jpeg_line_size;      /* 偏移 */
    }
    else    /* buf1已满,正常处理buf0 */
    {
        for (i = 0; i < jpeg_line_size; i++)
        {
            pbuf[i] = g_dcmi_line_buf[1][i];    /* 读取buf1里面的数据 */
        }
        
        g_jpeg_data_len += jpeg_line_size;      /* 偏移 */
    }
    
    SCB_CleanInvalidateDCache();                /* 清除无效化DCache */
}

/**
 * @brief       JPEG测试
 *   @ntoe      JPEG数据,通过串口2发送给电脑.
 *
 * @param       无
 * @retval      无
 */
void jpeg_test(void)
{
    uint32_t i, jpgstart, jpglen;
    uint8_t headok = 0;
    uint8_t *p;
    
    lcd_clear(WHITE);
    
    if (g_ov_type == 2640)
    {
        ov2640_rgb565_mode();                               /* RGB565模式 */
        ov2640_jpeg_mode();                                 /* JPEG模式 */
        ov2640_outsize_set(lcddev.width, lcddev.height);    /* 满屏缩放显示 */
    }
    else if (g_ov_type == 5640)
    {
        ov5640_rgb565_mode();                   /* RGB565模式 */
        ov5640_jpeg_mode();                     /* JPEG模式 */
        ov5640_outsize_set(4, 0, lcddev.width, lcddev.height);     /* 设置输出尺寸QVGA 320*240 */
    }

    dcmi_init();                                /* DCMI配置 */
    dcmi_rx_callback = jpeg_dcmi_rx_callback;   /* JPEG接收数据回调函数 */
    dcmi_dma_init((uint32_t)&g_dcmi_line_buf[0], (uint32_t)&g_dcmi_line_buf[1], jpeg_line_size, DMA_MDATAALIGN_WORD, DMA_MINC_ENABLE); /* DCMI DMA配置 */
    
    dcmi_start();                               /* 启动传输 */

    while (1)
    {
        if (g_jpeg_data_ok == 1)
        {
            p = (uint8_t *)g_jpeg_data_buf;
            printf("g_jpeg_data_len:%d\r\n", g_jpeg_data_len * 4);                  /* 打印总字节数 */
            lcd_show_string(30, 210, 210, 16, 16, "Sending JPEG data...", RED);     /* 提示正在传输数据 */
            jpglen = 0;         /* 设置jpg文件大小为0 */
            headok = 0;         /* 清除jpg头标记 */
            
            for (i = 0; i < g_jpeg_data_len * 4; i++)       /* 查找0XFF,0XD8和0XFF,0XD9,获取jpg文件大小 */
            {
                if ((p[i] == 0xFF) && (p[i + 1] == 0xD8))   /* 找到FF D8 */
                {
                    jpgstart = i;
                    headok = 1;     /* 标记找到jpg头(FF D8) */
                }
                
                if ((p[i] == 0xFF) && (p[i+1] == 0xD9) && headok)   /* 找到头以后,再找FF D9 */
                {
                    jpglen = i - jpgstart + 2;
                    break;
                }
            }
            
            if (jpglen)         /* 正常的jpeg数据 */
            {
                p += jpgstart;  /* 偏移到0XFF,0XD8处 */
                
                for (i = 0; i < jpglen; i++)    /* 发送整个jpg文件 */
                {
                    USART2->TDR = p[i];
                    
                    while ((USART2->ISR & 0X40) == 0);  /* 循环发送,直到发送完毕 */
                }
                

                lcd_show_string(30, 210, 210, 16, 16, "Send data complete!!", RED); /* 提示传输结束设置 */
            }

            g_jpeg_data_ok = 2;     /* 标记jpeg数据处理完了,可以让DMA去采集下一帧了. */
        }
        
    }
}


/**
 * @brief       RGB565测试
 *   @ntoe      RGB数据直接显示在LCD上面
 *
 * @param       无
 * @retval      无
 */
void rgb565_test(void)
{
    lcd_clear(WHITE);
    
    if (g_ov_type == 2640)
    {
        ov2640_rgb565_mode();                                   /* RGB565模式 */
        ov2640_outsize_set(lcddev.width, lcddev.height);        /* 满屏缩放显示 */
    }
    else if (g_ov_type == 5640)
    {
        ov5640_rgb565_mode();                                   /* RGB565模式 */
//        ov5640_focus_init();                                    /* OV5640 初始化自动对焦 */
//        ov5640_focus_constant();                                /* 启动持续对焦 */
        ov5640_outsize_set(4, 0, lcddev.width, lcddev.height);  /* 满屏缩放显示 */
    }
    
    dcmi_init();                /* DCMI配置 */
    dcmi_dma_init((uint32_t)&LCD->LCD_RAM, 0, 1, DMA_MDATAALIGN_HALFWORD, DMA_MINC_DISABLE); /* DCMI DMA配置,MCU屏,竖屏 */
    dcmi_start();               /* 启动传输 */

    while (1)
    {
        delay_ms(10);
    }
}

int main(void)
{
    uint8_t key = 0;
    uint16_t t = 0;
    
    sys_cache_enable();                     /* 打开L1-Cache */
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(240, 2, 2, 4);     /* 设置时钟, 480Mhz */
    delay_init(480);                        /* 延时初始化 */
    usart_init(115200);                     /* 串口初始化为115200 */
    usmart_dev.init(240);                   /* 初始化USMART */
    usart2_init(1500000);                    /* 初始化串口2波特率为1500000 */
    mpu_memory_protection();                /* 保护相关存储区域 */
    led_init();                             /* 初始化LED */
    lcd_init();                             /* 初始化LCD */
    key_init();                             /* 初始化按键 */

    /* 10Khz计数, 1秒钟中断一次, 用于统计帧率 */
    btim_timx_int_init(10000 - 1, 24000 - 1);

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "OV_MOD TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);


    while(1)    /* 模块初始化 */
    {
        if (!ov2640_init())
        {
//            while (ov2640_init())
//            {
//                lcd_show_string(30, 130, 240, 16, 16, "OV2640 ERROR", RED);
//                delay_ms(200);
//                lcd_fill(30, 130, 239, 170, WHITE);
//                delay_ms(200);
//                LED0_TOGGLE();
//            }
            g_ov_type = 2640;
            lcd_show_string(30, 130, 200, 16, 16, "OV2640 OK", RED);
            break;
        }
        else if (!ov5640_init())
        {
//            while (ov5640_init())
//            {
//                lcd_show_string(30, 130, 240, 16, 16, "OV5640 ERROR", RED);
//                delay_ms(200);
//                lcd_fill(30, 130, 239, 170, WHITE);
//                delay_ms(200);
//                LED0_TOGGLE();
//            }
            g_ov_type = 5640;
            lcd_show_string(30, 130, 200, 16, 16, "OV5640 OK", RED);
            break;
        }

        lcd_show_string(30, 130, 200, 16, 16, "NO MODULE", RED);
        delay_ms(200);
        lcd_fill(30, 130, 239, 170, WHITE);
        delay_ms(200);
        LED0_TOGGLE();
    }

    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)
        {
            g_ov_mode = 0;    /* RGB565模式 */
            break;
        }
        else if (key == KEY1_PRES)
        {
            g_ov_mode = 1;    /* JPEG模式 */
            break;
        }

        t++;

        if (t == 100)lcd_show_string(30, 150, 230, 16, 16, "KEY0:RGB565  KEY1:JPEG", RED);  /* 闪烁显示提示信息 */

        if (t == 200)
        {
            lcd_fill(30, 150, 210, 150 + 16, WHITE);
            t = 0;
            LED0_TOGGLE();
        }

        delay_ms(5);
    }

    if (g_ov_mode == 1)
    {
        jpeg_test();
    }
    else 
    {
        rgb565_test();
    }
}








