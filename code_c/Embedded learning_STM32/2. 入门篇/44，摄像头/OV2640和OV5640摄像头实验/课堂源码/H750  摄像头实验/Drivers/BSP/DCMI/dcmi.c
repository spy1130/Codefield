/**
 ****************************************************************************************************
 * @file        dcmi.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-03
 * @brief       DCMI ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32H750������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200403
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/DCMI/dcmi.h"
#include "./BSP/OV5640/ov5640.h"


DCMI_HandleTypeDef g_dcmi_handle;           /* DCMI��� */
DMA_HandleTypeDef g_dma_dcmi_handle;        /* DMA��� */

uint8_t g_ov_frame = 0;                     /* ֡�� */
extern void jpeg_data_process(void);        /* JPEG���ݴ����� */

/**
 * @brief       DCMI ��ʼ��
 *   @note      IO��Ӧ��ϵ����:
 *              ����ͷģ�� ------------ STM32������
 *               OV_D0~D7  ------------  PC6/PC7/PC8/PC9/PC11/PD3/PB8/PB9
 *               OV_SCL    ------------  PB10
 *               OV_SDA    ------------  PB11
 *               OV_VSYNC  ------------  PB7
 *               OV_HREF   ------------  PA4
 *               OV_RESET  ------------  PA7
 *               OV_PCLK   ------------  PA6
 *               OV_PWDN   ------------  PC4
 *              ����������ʼ��OV_D0~D7/OV_VSYNC/OV_HREF/OV_PCLK���ź�(11��).
 *
 * @param       ��
 * @retval      ��
 */
void dcmi_init(void)
{
    g_dcmi_handle.Instance = DCMI;
    g_dcmi_handle.Init.SynchroMode = DCMI_SYNCHRO_HARDWARE;    /* Ӳ��ͬ��HSYNC,VSYNC */
    g_dcmi_handle.Init.PCKPolarity = DCMI_PCKPOLARITY_RISING;  /* PCLK ��������Ч */
    g_dcmi_handle.Init.VSPolarity = DCMI_VSPOLARITY_LOW;       /* VSYNC �͵�ƽ��Ч */
    g_dcmi_handle.Init.HSPolarity = DCMI_HSPOLARITY_LOW;       /* HSYNC �͵�ƽ��Ч */
    g_dcmi_handle.Init.CaptureRate = DCMI_CR_ALL_FRAME;        /* ȫ֡���� */
    g_dcmi_handle.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B; /* 8λ���ݸ�ʽ */
    HAL_DCMI_Init(&g_dcmi_handle);                             /* ��ʼ��DCMI���˺����Ὺ��֡�ж� */

    /* �ر����жϡ�VSYNC�жϡ�ͬ�������жϺ�����ж� */
    //__HAL_DCMI_DISABLE_IT(&g_dcmi_handle,DCMI_IT_LINE|DCMI_IT_VSYNC|DCMI_IT_ERR|DCMI_IT_OVR);

    /* �ر������жϣ�����HAL_DCMI_Init()��Ĭ�ϴ򿪺ܶ��жϣ�������Щ�ж�
      �Ժ����Ǿ���Ҫ����Щ�ж�����Ӧ�Ĵ�������Ļ��ͻᵼ�¸��ָ��������⣬
      ������Щ�жϺܶ඼����Ҫ���������ｫ��ȫ���رյ���Ҳ���ǽ�IER�Ĵ������㡣
      �ر��������ж��Ժ��ٸ����Լ���ʵ��������ʹ����Ӧ���ж� */
    DCMI->IER=0x0;

    __HAL_DCMI_ENABLE_IT(&g_dcmi_handle,DCMI_IT_FRAME);      /* ʹ��֡�ж� */
    __HAL_DCMI_ENABLE(&g_dcmi_handle);                       /* ʹ��DCMI */
}

/**
 * @brief       DCMI�ײ��������������ã�ʱ��ʹ�ܣ��ж�����
 * @param       hdcmi:DCMI���
 * @note        �˺����ᱻHAL_DCMI_Init()����
 * @retval      ��
 */
void HAL_DCMI_MspInit(DCMI_HandleTypeDef* hdcmi)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_DCMI_CLK_ENABLE();                /* ʹ��DCMIʱ�� */
    __HAL_RCC_GPIOA_CLK_ENABLE();               /* ʹ��GPIOAʱ�� */
    __HAL_RCC_GPIOB_CLK_ENABLE();               /* ʹ��GPIOBʱ�� */
    __HAL_RCC_GPIOC_CLK_ENABLE();               /* ʹ��GPIOCʱ�� */
    __HAL_RCC_GPIOD_CLK_ENABLE();               /* ʹ��GPIODʱ�� */
    
    gpio_init_struct.Pin = GPIO_PIN_4 | GPIO_PIN_6;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* ���츴�� */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; /* ���� */
    gpio_init_struct.Alternate = GPIO_AF13_DCMI;        /* ����ΪDCMI */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);            /* ��ʼ��PA4��6���� */

    gpio_init_struct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);            /* ��ʼ��PB7,8,9���� */

    gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_11;
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);            /* ��ʼ��PC6,7,8,9,11���� */

    gpio_init_struct.Pin = GPIO_PIN_3; 
    HAL_GPIO_Init(GPIOD, &gpio_init_struct);            /* ��ʼ��PD3���� */

    HAL_NVIC_SetPriority(DCMI_IRQn, 2, 2);              /* ��ռ���ȼ�2�������ȼ�2 */
    HAL_NVIC_EnableIRQ(DCMI_IRQn);                      /* ʹ��DCMI�ж� */
}

/**
 * @brief       DCMI DMA����
 * @param       mem0addr: �洢����ַ0     ��Ҫ�洢����ͷ���ݵ��ڴ��ַ(Ҳ�����������ַ)
 * @param       mem1addr: �洢����ַ1     ��ֻʹ��mem0addr��ʱ��,��ֵ����Ϊ0
 * @param       memsize : �洢������      0~65535
 * @param       memblen : �洢��λ��      DMA_MDATAALIGN_BYTE,8λ;DMA_MDATAALIGN_HALFWORD,16λ;DMA_MDATAALIGN_WORD,32λ
 * @param       meminc  : �洢��������ʽ  DMA_MINC_DISABLE,������; DMA_MINC_ENABLE,����
 * @retval      ��
 */
void dcmi_dma_init(uint32_t mem0addr,uint32_t mem1addr,uint16_t memsize,uint32_t memblen,uint32_t meminc)
{ 
    __HAL_RCC_DMA1_CLK_ENABLE();                                        /* ʹ��DMA1ʱ�� */
    __HAL_LINKDMA(&g_dcmi_handle, DMA_Handle, g_dma_dcmi_handle);       /* ��DMA��DCMI��ϵ���� */
    __HAL_DMA_DISABLE_IT(&g_dma_dcmi_handle, DMA_IT_TC);                /* �ȹر�DMA��������ж�(������ʹ��MCU����ʱ�����ֻ��������) */

    g_dma_dcmi_handle.Instance = DMA1_Stream1;                          /* DMA1������1 */
    g_dma_dcmi_handle.Init.Request = DMA_REQUEST_DCMI;                  /* DCMI��DMA���� */
    g_dma_dcmi_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;            /* ���赽�洢�� */
    g_dma_dcmi_handle.Init.PeriphInc = DMA_PINC_DISABLE;                /* ���������ģʽ */
    g_dma_dcmi_handle.Init.MemInc = meminc;                             /* �洢������ģʽ */
    g_dma_dcmi_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;   /* �������ݳ���:32λ */
    g_dma_dcmi_handle.Init.MemDataAlignment = memblen;                  /* �洢�����ݳ���:8/16/32λ */
    g_dma_dcmi_handle.Init.Mode = DMA_CIRCULAR;                         /* ʹ��ѭ��ģʽ */
    g_dma_dcmi_handle.Init.Priority = DMA_PRIORITY_HIGH;                /* �����ȼ� */
    g_dma_dcmi_handle.Init.FIFOMode = DMA_FIFOMODE_ENABLE;              /* ʹ��FIFO */
    g_dma_dcmi_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL; /* ʹ��1/2��FIFO */
    g_dma_dcmi_handle.Init.MemBurst = DMA_MBURST_SINGLE;                /* �洢��ͻ������ */
    g_dma_dcmi_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;             /* ����ͻ�����δ��� */
    HAL_DMA_DeInit(&g_dma_dcmi_handle);                                 /* �������ǰ������ */
    HAL_DMA_Init(&g_dma_dcmi_handle);                                   /* ��ʼ��DMA */
    
    /* �ڿ���DMA֮ǰ��ʹ��__HAL_UNLOCK()����һ��DMA,��ΪHAL_DMA_Statrt()HAL_DMAEx_MultiBufferStart()
      ����������һ��ʼҪ��ʹ��__HAL_LOCK()����DMA,������__HAL_LOCK()���жϵ�ǰ��DMA״̬�Ƿ�Ϊ����״̬�������
      ����״̬�Ļ���ֱ�ӷ���HAL_BUSY�������ᵼ�º���HAL_DMA_Statrt()��HAL_DMAEx_MultiBufferStart()������DMA����
      ����ֱ�ӱ�������DMAҲ�Ͳ�������������Ϊ�˱���������������������DMA֮ǰ�ȵ���__HAL_UNLOC()�Ƚ���һ��DMA�� */
    
    __HAL_UNLOCK(&g_dma_dcmi_handle);
    if (mem1addr == 0)  /* ����DMA����ʹ��˫���� */
    {
        HAL_DMA_Start(&g_dma_dcmi_handle, (uint32_t)&DCMI->DR, mem0addr, memsize);
    }
    else                /* ʹ��˫���� */
    {
        HAL_DMAEx_MultiBufferStart(&g_dma_dcmi_handle, (uint32_t)&DCMI->DR, mem0addr, mem1addr, memsize); /* ����˫���� */
        __HAL_DMA_ENABLE_IT(&g_dma_dcmi_handle, DMA_IT_TC);     /* ������������ж� */
        HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 2, 3);          /* DMA�ж����ȼ� */
        HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
    }
}

/**
 * @brief       DCMI,��������
 * @param       ��
 * @retval      ��
 */
void dcmi_start(void)
{  
    lcd_set_cursor(0,0);                  /* �������굽ԭ�� */
    lcd_write_ram_prepare();              /* ��ʼд��GRAM */
    __HAL_DMA_ENABLE(&g_dma_dcmi_handle); /* ʹ��DMA */
    DCMI->CR |= DCMI_CR_CAPTURE;          /* DCMI����ʹ�� */
}

/**
 * @brief       DCMI,�رմ���
 * @param       ��
 * @retval      ��
 */
void dcmi_stop(void)
{ 
    DCMI->CR &= ~(DCMI_CR_CAPTURE);         /* DCMI����ر� */

    while (DCMI->CR & 0X01);                /* �ȴ�������� */

    __HAL_DMA_DISABLE(&g_dma_dcmi_handle);  /* �ر�DMA */
} 

/**
 * @brief       DCMI�жϷ�����
 * @param       ��
 * @retval      ��
 */
void DCMI_IRQHandler(void)
{
    HAL_DCMI_IRQHandler(&g_dcmi_handle);
}

/**
 * @brief       DCMI�жϻص�������
 * @param       hdcmi:DCMI���
 * @note        ����һ֡ͼ����
 * @retval      ��
 */
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
    jpeg_data_process();    /* jpeg���ݴ��� */
    LED1_TOGGLE();          /* LED1��˸ */
    g_ov_frame++;

    /* ����ʹ��֡�ж�,��ΪHAL_DCMI_IRQHandler()������ر�֡�ж� */
    __HAL_DCMI_ENABLE_IT(&g_dcmi_handle, DCMI_IT_FRAME);
}

void (*dcmi_rx_callback)(void); /* DCMI DMA���ջص����� */

/**
 * @brief       DMA1������1�жϷ�����(��˫����ģʽ���õ�)
 * @param       ��
 * @retval      ��
 */
void DMA1_Stream1_IRQHandler(void)
{
    if (__HAL_DMA_GET_FLAG(&g_dma_dcmi_handle, DMA_FLAG_TCIF1_5) != RESET)  /* DMA������� */
    {
        __HAL_DMA_CLEAR_FLAG(&g_dma_dcmi_handle, DMA_FLAG_TCIF1_5); /* ���DMA��������жϱ�־λ */
        dcmi_rx_callback();     /* ִ������ͷ���ջص�����,��ȡ���ݵȲ����������洦�� */
    } 
}


/******************************************************************************************/
/* ������������,��usmart����,���ڵ��Դ��� */

/**
 * @brief       DCMI������ʾ����
 * @param       sx,sy       : LCD����ʼ����
 * @param       width,height: LCD��ʾ��Χ.
 * @retval      ��
 */
void dcmi_set_window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    dcmi_stop();
    lcd_clear(WHITE);
    lcd_set_window(sx, sy, width, height);
    ov5640_outsize_set(0, 0, width, height);
    lcd_set_cursor(0, 0);
    lcd_write_ram_prepare();               /* ��ʼд��GRAM */
    __HAL_DMA_ENABLE(&g_dma_dcmi_handle);  /* ����DMA1,Stream1 */
    DCMI->CR |= DCMI_CR_CAPTURE;           /* DCMI����ʹ�� */
}

/**
 * @brief       ͨ��usmart����,����������.
 * @param       pclk/hsync/vsync : �����źŵ���Ч��ƽ����
 * @retval      ��
 */
void dcmi_cr_set(uint8_t pclk, uint8_t hsync, uint8_t vsync)
{
    HAL_DCMI_DeInit(&g_dcmi_handle);                            /* ���ԭ�������� */
    g_dcmi_handle.Instance = DCMI;
    g_dcmi_handle.Init.SynchroMode = DCMI_SYNCHRO_HARDWARE;     /* Ӳ��ͬ��HSYNC,VSYNC */
    g_dcmi_handle.Init.PCKPolarity = pclk<<5;                   /* PCLK ��������Ч */
    g_dcmi_handle.Init.VSPolarity = vsync<<7;                   /* VSYNC �͵�ƽ��Ч */
    g_dcmi_handle.Init.HSPolarity = hsync<<6;                   /* HSYNC �͵�ƽ��Ч */
    g_dcmi_handle.Init.CaptureRate = DCMI_CR_ALL_FRAME;         /* ȫ֡���� */
    g_dcmi_handle.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;  /* 8λ���ݸ�ʽ */
    HAL_DCMI_Init(&g_dcmi_handle);                              /* ��ʼ��DCMI */
    g_dcmi_handle.Instance->CR |= DCMI_MODE_CONTINUOUS;         /* ����ģʽ */
}

/******************************************************************************************/







