#include "./BSP/LCD/ltdc.h"
#include "./BSP/LCD/lcd.h"

LTDC_HandleTypeDef  g_ltdc_handle;  /* LTDC句柄 */
_ltdc_dev lcdltdc;                  /* 管理LCD LTDC的重要参数 */

/* 4.3 800480 RGB屏 */
void ltdc_init(void)
{
    RCC_PeriphCLKInitTypeDef periphclk_init_struct;
    
    /* 裸屏数据写入到lcdltdc */
    lcdltdc.pwidth = 800;   /* LCD面板的宽度 */
    lcdltdc.pheight = 480;  /* LCD面板的高度 */
    lcdltdc.hsw = 48;       /* 水平同步宽度 */
    lcdltdc.vsw = 3;        /* 垂直同步宽度 */
    lcdltdc.hbp = 88;       /* 水平后廊 */
    lcdltdc.vbp = 32;       /* 垂直后廊 */
    lcdltdc.hfp = 40;       /* 水平前廊 */
    lcdltdc.vfp = 13;       /* 垂直前廊  */
    
    /* 设置LCD CLK ，获得33MHz*/
    periphclk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    periphclk_init_struct.PLLSAI.PLLSAIN = 396;
    periphclk_init_struct.PLLSAI.PLLSAIR = 3;
    periphclk_init_struct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
    HAL_RCCEx_PeriphCLKConfig(&periphclk_init_struct);
    
    /* LTDC配置 */
    g_ltdc_handle.Instance = LTDC;
    /* 信号极性设置 */
    g_ltdc_handle.Init.HSPolarity = LTDC_HSPOLARITY_AL;     /* 水平同步极性 */
    g_ltdc_handle.Init.VSPolarity = LTDC_VSPOLARITY_AL;     /* 垂直同步极性 */
    g_ltdc_handle.Init.DEPolarity = LTDC_DEPOLARITY_AL;     /* 数据使能极性 */
    g_ltdc_handle.Init.PCPolarity = LTDC_PCPOLARITY_IPC;    /* 像素时钟极性 */
    
    /* 时序参数设置 */
    g_ltdc_handle.Init.HorizontalSync       = lcdltdc.hsw - 1;  /* 水平同步宽度 */
    g_ltdc_handle.Init.VerticalSync         = lcdltdc.vsw - 1;    /* 垂直同步高度 */
    g_ltdc_handle.Init.AccumulatedHBP       = lcdltdc.hsw + lcdltdc.hbp - 1;  /* 水平同步后沿宽度 */
    g_ltdc_handle.Init.AccumulatedVBP       = lcdltdc.vsw + lcdltdc.vbp - 1;  /* 垂直同步后沿高度 */
    g_ltdc_handle.Init.AccumulatedActiveW   = lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth - 1;  /* 累加有效宽度 */
    g_ltdc_handle.Init.AccumulatedActiveH   = lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight  - 1;  /* 累加有效高度 */
    g_ltdc_handle.Init.TotalWidth           = lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth + lcdltdc.hfp - 1;      /* 总宽度 */
    g_ltdc_handle.Init.TotalHeigh           = lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight + lcdltdc.vfp - 1;      /* 总高度 */
    
    /* 背景层颜色设置 RGB888*/
    g_ltdc_handle.Init.Backcolor.Red = 0;
    g_ltdc_handle.Init.Backcolor.Green = 0xFF;
    g_ltdc_handle.Init.Backcolor.Blue = 0;

    HAL_LTDC_Init(&g_ltdc_handle);  /* 初始化LTDC */

    LCD_BL(1);  /* 点亮背光 */
}

/* 底层初始化工作 */
void HAL_LTDC_MspInit(LTDC_HandleTypeDef *hltdc)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_LTDC_CLK_ENABLE();                /* 使能LTDC时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();               /* 使能GPIOB时钟 */
    __HAL_RCC_GPIOF_CLK_ENABLE();               /* 使能GPIOF时钟 */
    __HAL_RCC_GPIOG_CLK_ENABLE();               /* 使能GPIOG时钟 */
    __HAL_RCC_GPIOH_CLK_ENABLE();               /* 使能GPIOH时钟 */
    __HAL_RCC_GPIOI_CLK_ENABLE();               /* 使能GPIOI时钟 */
    
    /* 初始化PB5，背光引脚 */
    gpio_init_struct.Pin = GPIO_PIN_5;                /* PB5推挽输出，控制背光 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;      /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;              /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;         /* 高速 */
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);
    
    /* 初始化PF10 */
    gpio_init_struct.Pin = GPIO_PIN_10; 
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;          /* 复用 */
    gpio_init_struct.Pull = GPIO_NOPULL;              
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;         /* 高速 */
    gpio_init_struct.Alternate = GPIO_AF14_LTDC;      /* 复用为LTDC */
    HAL_GPIO_Init(GPIOF, &gpio_init_struct);
    
    /* 初始化PG6,7,11 */
    gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11;
    HAL_GPIO_Init(GPIOG, &gpio_init_struct);
    
    /* 初始化PH9,10,11,12,13,14,15 */
    gpio_init_struct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                     GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOH, &gpio_init_struct);
    
    /* 初始化PI0,1,2,4,5,6,7,9,10 */
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | \
                     GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOI, &gpio_init_struct); 
}


