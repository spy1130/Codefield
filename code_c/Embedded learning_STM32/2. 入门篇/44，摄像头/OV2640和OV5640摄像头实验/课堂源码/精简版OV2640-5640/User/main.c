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

uint8_t g_ov_mode = 0;                  /* bit0: 0,RGB565ģʽ;  1,JPEGģʽ */
uint16_t g_curline = 0;                 /* ����ͷ�������,��ǰ�б�� */
uint16_t g_yoffset = 0;                 /* y�����ƫ���� */

#define jpeg_buf_size   120 * 1024      /* ����JPEG���ݻ���jpeg_buf�Ĵ�С(480KB�ֽ�) */
#define jpeg_line_size  4 * 1024        /* ����DMA��������ʱ,һ�����ݵ����ֵ */


/* JPEG���ݲɼ�,����ͷ����һ��һ�ж�ȡ,�����л��� */
#if !(__ARMCC_VERSION >= 6010050)   /* ����AC6����������ʹ��AC5������ʱ */
uint32_t g_dcmi_line_buf[2][jpeg_line_size] __attribute__((at(0x38000000)));
#else
uint32_t g_dcmi_line_buf[2][jpeg_line_size] __attribute__((section(".bss.ARM.__at_0x38000000")));
#endif

uint32_t g_jpeg_data_buf[jpeg_buf_size];    /* JPEG���ݻ���buf,�������ڲ�SRAM */


/* buf�е�JPEG��Ч���ݳ��� */
volatile uint32_t g_jpeg_data_len = 0;

/* JPEG���ݲɼ���ɱ�־
 * 0,����û�вɼ���;
 * 1,���ݲɼ�����,���ǻ�û����;
 * 2,�����Ѿ����������,���Կ�ʼ��һ֡����
 */
volatile uint8_t g_jpeg_data_ok = 0;


uint16_t g_ov_type = 0;

/**
 * @brief       ����JPEG����
 *   @ntoe      ��DCMI_IRQHandler�жϷ��������汻����
 *              ���ɼ���һ֡JPEG���ݺ�,���ô˺���,�л�JPEG BUF.��ʼ��һ֡�ɼ�.
 *
 * @param       ��
 * @retval      ��
 */
void jpeg_data_process(void)
{
    uint16_t i;
    uint16_t rlen;          /* ʣ�����ݳ��� */
    uint32_t *pbuf;
    g_curline = g_yoffset;  /* ������λ */

    if (g_ov_mode & 0X01)   /* ֻ����JPEG��ʽ��,����Ҫ������. */
    {
        if (g_jpeg_data_ok == 0)    /* jpeg���ݻ�δ�ɼ���? */
        {
            __HAL_DMA_DISABLE(&g_dma_dcmi_handle);      /* �ر�DMA */
            rlen = jpeg_line_size - __HAL_DMA_GET_COUNTER(&g_dma_dcmi_handle); /* �õ�ʣ�����ݳ��� */
            pbuf = g_jpeg_data_buf + g_jpeg_data_len;   /* ƫ�Ƶ���Ч����ĩβ,������� */

            if (DMA1_Stream1->CR & (1 << 19))
            {
                for (i = 0; i < rlen; i++)
                {
                    pbuf[i] = g_dcmi_line_buf[1][i];    /* ��ȡbuf1�����ʣ������ */
                }
            }
            else 
            {
                for (i = 0; i < rlen; i++)
                {
                    pbuf[i] = g_dcmi_line_buf[0][i];    /* ��ȡbuf0�����ʣ������ */
                }
            }
            
            g_jpeg_data_len += rlen;    /* ����ʣ�೤�� */
            g_jpeg_data_ok = 1;         /* ���JPEG���ݲɼ����,�ȴ������������� */
        }

        if (g_jpeg_data_ok == 2)        /* ��һ�ε�jpeg�����Ѿ��������� */
        {
            __HAL_DMA_SET_COUNTER(&g_dma_dcmi_handle, jpeg_line_size); /* ���䳤��Ϊjpeg_buf_size*4�ֽ� */
            __HAL_DMA_ENABLE(&g_dma_dcmi_handle);   /* ���´��� */
            g_jpeg_data_ok = 0;                     /* �������δ�ɼ� */
            g_jpeg_data_len = 0;                    /* �������¿�ʼ */
        }
    }
    else
    {
        lcd_set_cursor(0, 0);
        lcd_write_ram_prepare();        /* ��ʼд��GRAM */
    }
}

/**
 * @brief       JPEG���ݽ��ջص�����
 *   @ntoe      ��DMA1_Stream1_IRQHandler�жϷ��������汻����
 *
 * @param       ��
 * @retval      ��
 */
void jpeg_dcmi_rx_callback(void)
{
    uint16_t i;
    volatile uint32_t *pbuf;
    pbuf = g_jpeg_data_buf + g_jpeg_data_len;   /* ƫ�Ƶ���Ч����ĩβ */

    if (DMA1_Stream1->CR & (1 << 19))           /* buf0����,��������buf1 */
    {
        for (i = 0; i < jpeg_line_size; i++)
        {
            pbuf[i] = g_dcmi_line_buf[0][i];    /* ��ȡbuf0��������� */
        }
        
        g_jpeg_data_len += jpeg_line_size;      /* ƫ�� */
    }
    else    /* buf1����,��������buf0 */
    {
        for (i = 0; i < jpeg_line_size; i++)
        {
            pbuf[i] = g_dcmi_line_buf[1][i];    /* ��ȡbuf1��������� */
        }
        
        g_jpeg_data_len += jpeg_line_size;      /* ƫ�� */
    }
    
    SCB_CleanInvalidateDCache();                /* �����Ч��DCache */
}

/**
 * @brief       JPEG����
 *   @ntoe      JPEG����,ͨ������2���͸�����.
 *
 * @param       ��
 * @retval      ��
 */
void jpeg_test(void)
{
    uint32_t i, jpgstart, jpglen;
    uint8_t headok = 0;
    uint8_t *p;
    
    lcd_clear(WHITE);
    
    if (g_ov_type == 2640)
    {
        ov2640_rgb565_mode();                               /* RGB565ģʽ */
        ov2640_jpeg_mode();                                 /* JPEGģʽ */
        ov2640_outsize_set(lcddev.width, lcddev.height);    /* ����������ʾ */
    }
    else if (g_ov_type == 5640)
    {
        ov5640_rgb565_mode();                   /* RGB565ģʽ */
        ov5640_jpeg_mode();                     /* JPEGģʽ */
        ov5640_outsize_set(4, 0, lcddev.width, lcddev.height);     /* ��������ߴ�QVGA 320*240 */
    }

    dcmi_init();                                /* DCMI���� */
    dcmi_rx_callback = jpeg_dcmi_rx_callback;   /* JPEG�������ݻص����� */
    dcmi_dma_init((uint32_t)&g_dcmi_line_buf[0], (uint32_t)&g_dcmi_line_buf[1], jpeg_line_size, DMA_MDATAALIGN_WORD, DMA_MINC_ENABLE); /* DCMI DMA���� */
    
    dcmi_start();                               /* �������� */

    while (1)
    {
        if (g_jpeg_data_ok == 1)
        {
            p = (uint8_t *)g_jpeg_data_buf;
            printf("g_jpeg_data_len:%d\r\n", g_jpeg_data_len * 4);                  /* ��ӡ���ֽ��� */
            lcd_show_string(30, 210, 210, 16, 16, "Sending JPEG data...", RED);     /* ��ʾ���ڴ������� */
            jpglen = 0;         /* ����jpg�ļ���СΪ0 */
            headok = 0;         /* ���jpgͷ��� */
            
            for (i = 0; i < g_jpeg_data_len * 4; i++)       /* ����0XFF,0XD8��0XFF,0XD9,��ȡjpg�ļ���С */
            {
                if ((p[i] == 0xFF) && (p[i + 1] == 0xD8))   /* �ҵ�FF D8 */
                {
                    jpgstart = i;
                    headok = 1;     /* ����ҵ�jpgͷ(FF D8) */
                }
                
                if ((p[i] == 0xFF) && (p[i+1] == 0xD9) && headok)   /* �ҵ�ͷ�Ժ�,����FF D9 */
                {
                    jpglen = i - jpgstart + 2;
                    break;
                }
            }
            
            if (jpglen)         /* ������jpeg���� */
            {
                p += jpgstart;  /* ƫ�Ƶ�0XFF,0XD8�� */
                
                for (i = 0; i < jpglen; i++)    /* ��������jpg�ļ� */
                {
                    USART2->TDR = p[i];
                    
                    while ((USART2->ISR & 0X40) == 0);  /* ѭ������,ֱ��������� */
                }
                

                lcd_show_string(30, 210, 210, 16, 16, "Send data complete!!", RED); /* ��ʾ����������� */
            }

            g_jpeg_data_ok = 2;     /* ���jpeg���ݴ�������,������DMAȥ�ɼ���һ֡��. */
        }
        
    }
}


/**
 * @brief       RGB565����
 *   @ntoe      RGB����ֱ����ʾ��LCD����
 *
 * @param       ��
 * @retval      ��
 */
void rgb565_test(void)
{
    lcd_clear(WHITE);
    
    if (g_ov_type == 2640)
    {
        ov2640_rgb565_mode();                                   /* RGB565ģʽ */
        ov2640_outsize_set(lcddev.width, lcddev.height);        /* ����������ʾ */
    }
    else if (g_ov_type == 5640)
    {
        ov5640_rgb565_mode();                                   /* RGB565ģʽ */
//        ov5640_focus_init();                                    /* OV5640 ��ʼ���Զ��Խ� */
//        ov5640_focus_constant();                                /* ���������Խ� */
        ov5640_outsize_set(4, 0, lcddev.width, lcddev.height);  /* ����������ʾ */
    }
    
    dcmi_init();                /* DCMI���� */
    dcmi_dma_init((uint32_t)&LCD->LCD_RAM, 0, 1, DMA_MDATAALIGN_HALFWORD, DMA_MINC_DISABLE); /* DCMI DMA����,MCU��,���� */
    dcmi_start();               /* �������� */

    while (1)
    {
        delay_ms(10);
    }
}

int main(void)
{
    uint8_t key = 0;
    uint16_t t = 0;
    
    sys_cache_enable();                     /* ��L1-Cache */
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(240, 2, 2, 4);     /* ����ʱ��, 480Mhz */
    delay_init(480);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(240);                   /* ��ʼ��USMART */
    usart2_init(1500000);                    /* ��ʼ������2������Ϊ1500000 */
    mpu_memory_protection();                /* ������ش洢���� */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ������ */

    /* 10Khz����, 1�����ж�һ��, ����ͳ��֡�� */
    btim_timx_int_init(10000 - 1, 24000 - 1);

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "OV_MOD TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);


    while(1)    /* ģ���ʼ�� */
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
            g_ov_mode = 0;    /* RGB565ģʽ */
            break;
        }
        else if (key == KEY1_PRES)
        {
            g_ov_mode = 1;    /* JPEGģʽ */
            break;
        }

        t++;

        if (t == 100)lcd_show_string(30, 150, 230, 16, 16, "KEY0:RGB565  KEY1:JPEG", RED);  /* ��˸��ʾ��ʾ��Ϣ */

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








