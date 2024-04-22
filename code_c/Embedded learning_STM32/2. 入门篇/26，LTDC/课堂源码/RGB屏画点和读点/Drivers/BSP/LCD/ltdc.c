#include "./BSP/LCD/ltdc.h"
#include "./BSP/LCD/lcd.h"

LTDC_HandleTypeDef  g_ltdc_handle;  /* LTDC��� */
_ltdc_dev lcdltdc;                  /* ����LCD LTDC����Ҫ���� */

uint16_t framebuf[1280][800] __attribute__((at (0xC0000000)));

void ltdc_draw_point (uint16_t x, uint16_t y, uint32_t color) 
{
    if (lcdltdc.dir) 
    {   /* ���� */
        *(uint16_t *)((uint32_t)framebuf + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
    }
    else 
    {   /* ���� */
        *(uint16_t *)((uint32_t)framebuf + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color; 
    }
}

uint32_t ltdc_read_point (uint16_t x, uint16_t y) 
{
    if (lcdltdc.dir) 
    {   /* ���� */
        return *(uint16_t *)((uint32_t)framebuf + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) ;
    }
    else 
    {  /* ���� */
        return *(uint16_t *)((uint32_t)framebuf + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)); 
    }
}


void ltdc_layer1_config(void)
{
    LTDC_LayerCfgTypeDef ltdc_layer_cfg;
    
    /* ����ʾ����λ������ */
    ltdc_layer_cfg.WindowX0 = 0;                /* ���ô��ڵ�����ʼλ�� */ 
    ltdc_layer_cfg.WindowX1 = lcdltdc.pwidth;   /* ���ô��ڵ��н���λ�� */
    ltdc_layer_cfg.WindowY0 = 0;                /* ���ô��ڵĴ�ֱ��ʼλ�� */
    ltdc_layer_cfg.WindowY1 = lcdltdc.pheight;  /* ���ô��ڵĴ�ֱ����λ��*/
    
    /* �����ظ�ʽ���� */
    ltdc_layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565; /* ���õ�ǰ������ظ�ʽ */
    
    /* ������������ */
    ltdc_layer_cfg.Alpha = 255;     /* 255��ȫ��͸�� ���õ�ǰ��ĺ㶨Alphaֵ(͸����) */
    ltdc_layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;  /* ���û������ */
    ltdc_layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;  /* ���û������ */
    
    /* ��֡���������� */
    ltdc_layer_cfg.FBStartAdress = (uint32_t)framebuf;  /* ���õ�ǰ����Դ���ʼλ�� */
    ltdc_layer_cfg.ImageWidth = lcdltdc.pwidth;         /* ���õ�ǰ���ͼ���� */
    ltdc_layer_cfg.ImageHeight = lcdltdc.pheight;       /* ���õ�ǰ���ͼ��߶� */
    
    /* �㱳������ɫ���� */
    ltdc_layer_cfg.Alpha0 = 0;          /* ���õ�ǰ���Ĭ��͸��ֵ */
    ltdc_layer_cfg.Backcolor.Red = 0;
    ltdc_layer_cfg.Backcolor.Blue = 0;
    ltdc_layer_cfg.Backcolor.Green = 0;

    HAL_LTDC_ConfigLayer(&g_ltdc_handle, &ltdc_layer_cfg, LTDC_LAYER_1);
}

/* 4.3 800480 RGB�� */
void ltdc_init(void)
{
    RCC_PeriphCLKInitTypeDef periphclk_init_struct;
    
    /* ��������д�뵽lcdltdc */
    lcdltdc.pwidth = 800;   /* LCD���Ŀ�� */
    lcdltdc.pheight = 480;  /* LCD���ĸ߶� */
    lcdltdc.hsw = 48;       /* ˮƽͬ����� */
    lcdltdc.vsw = 3;        /* ��ֱͬ����� */
    lcdltdc.hbp = 88;       /* ˮƽ���� */
    lcdltdc.vbp = 32;       /* ��ֱ���� */
    lcdltdc.hfp = 40;       /* ˮƽǰ�� */
    lcdltdc.vfp = 13;       /* ��ֱǰ��  */
    
    lcdltdc.dir = 1;
    lcdltdc.pixsize = 2;
    
    lcddev.width = lcdltdc.pwidth;
    lcddev.height = lcdltdc.pheight;
    
    /* ����LCD CLK �����33MHz*/
    periphclk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    periphclk_init_struct.PLLSAI.PLLSAIN = 396;
    periphclk_init_struct.PLLSAI.PLLSAIR = 3;
    periphclk_init_struct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
    HAL_RCCEx_PeriphCLKConfig(&periphclk_init_struct);
    
    /* LTDC���� */
    g_ltdc_handle.Instance = LTDC;
    /* �źż������� */
    g_ltdc_handle.Init.HSPolarity = LTDC_HSPOLARITY_AL;     /* ˮƽͬ������ */
    g_ltdc_handle.Init.VSPolarity = LTDC_VSPOLARITY_AL;     /* ��ֱͬ������ */
    g_ltdc_handle.Init.DEPolarity = LTDC_DEPOLARITY_AL;     /* ����ʹ�ܼ��� */
    g_ltdc_handle.Init.PCPolarity = LTDC_PCPOLARITY_IPC;    /* ����ʱ�Ӽ��� */
    
    /* ʱ��������� */
    g_ltdc_handle.Init.HorizontalSync       = lcdltdc.hsw - 1;  /* ˮƽͬ����� */
    g_ltdc_handle.Init.VerticalSync         = lcdltdc.vsw - 1;    /* ��ֱͬ���߶� */
    g_ltdc_handle.Init.AccumulatedHBP       = lcdltdc.hsw + lcdltdc.hbp - 1;  /* ˮƽͬ�����ؿ�� */
    g_ltdc_handle.Init.AccumulatedVBP       = lcdltdc.vsw + lcdltdc.vbp - 1;  /* ��ֱͬ�����ظ߶� */
    g_ltdc_handle.Init.AccumulatedActiveW   = lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth - 1;  /* �ۼ���Ч��� */
    g_ltdc_handle.Init.AccumulatedActiveH   = lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight  - 1;  /* �ۼ���Ч�߶� */
    g_ltdc_handle.Init.TotalWidth           = lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth + lcdltdc.hfp - 1;      /* �ܿ�� */
    g_ltdc_handle.Init.TotalHeigh           = lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight + lcdltdc.vfp - 1;      /* �ܸ߶� */
    
    /* ��������ɫ���� RGB888*/
    g_ltdc_handle.Init.Backcolor.Red = 0;
    g_ltdc_handle.Init.Backcolor.Green = 0xFF;
    g_ltdc_handle.Init.Backcolor.Blue = 0;

    HAL_LTDC_Init(&g_ltdc_handle);  /* ��ʼ��LTDC */

    ltdc_layer1_config();
    
    LCD_BL(1);  /* �������� */
}

/* �ײ��ʼ������ */
void HAL_LTDC_MspInit(LTDC_HandleTypeDef *hltdc)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_LTDC_CLK_ENABLE();                /* ʹ��LTDCʱ�� */
    __HAL_RCC_GPIOB_CLK_ENABLE();               /* ʹ��GPIOBʱ�� */
    __HAL_RCC_GPIOF_CLK_ENABLE();               /* ʹ��GPIOFʱ�� */
    __HAL_RCC_GPIOG_CLK_ENABLE();               /* ʹ��GPIOGʱ�� */
    __HAL_RCC_GPIOH_CLK_ENABLE();               /* ʹ��GPIOHʱ�� */
    __HAL_RCC_GPIOI_CLK_ENABLE();               /* ʹ��GPIOIʱ�� */
    
    /* ��ʼ��PB5���������� */
    gpio_init_struct.Pin = GPIO_PIN_5;                /* PB5������������Ʊ��� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;      /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;              /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;         /* ���� */
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);
    
    /* ��ʼ��PF10 */
    gpio_init_struct.Pin = GPIO_PIN_10; 
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;          /* ���� */
    gpio_init_struct.Pull = GPIO_NOPULL;              
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;         /* ���� */
    gpio_init_struct.Alternate = GPIO_AF14_LTDC;      /* ����ΪLTDC */
    HAL_GPIO_Init(GPIOF, &gpio_init_struct);
    
    /* ��ʼ��PG6,7,11 */
    gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11;
    HAL_GPIO_Init(GPIOG, &gpio_init_struct);
    
    /* ��ʼ��PH9,10,11,12,13,14,15 */
    gpio_init_struct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                     GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOH, &gpio_init_struct);
    
    /* ��ʼ��PI0,1,2,4,5,6,7,9,10 */
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | \
                     GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOI, &gpio_init_struct); 
}


