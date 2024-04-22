#include "./BSP/ADC/adc.h"


DMA_HandleTypeDef g_dma_adc_handle;
ADC_HandleTypeDef g_adc_dma_handle;
uint8_t g_adc_dma_sta;

/* ADC DMA读取 初始化函数 */
void adc_dma_init(uint32_t mar)
{
    ADC_ChannelConfTypeDef adc_ch_conf;
    
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    g_dma_adc_handle.Instance = DMA1_Channel1;
    g_dma_adc_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    g_dma_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    g_dma_adc_handle.Init.MemInc = DMA_MINC_ENABLE;
    g_dma_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    g_dma_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    g_dma_adc_handle.Init.Mode = DMA_NORMAL;
    g_dma_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&g_dma_adc_handle);
    
    __HAL_LINKDMA(&g_adc_dma_handle, DMA_Handle, g_dma_adc_handle);

    g_adc_dma_handle.Instance = ADC1;
    g_adc_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_adc_dma_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;
    g_adc_dma_handle.Init.ContinuousConvMode = ENABLE;
    g_adc_dma_handle.Init.NbrOfConversion = 1;
    g_adc_dma_handle.Init.DiscontinuousConvMode = DISABLE;
    g_adc_dma_handle.Init.NbrOfDiscConversion = 0;
    g_adc_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    HAL_ADC_Init(&g_adc_dma_handle);
    
    HAL_ADCEx_Calibration_Start(&g_adc_dma_handle);
    
    adc_ch_conf.Channel = ADC_CHANNEL_1;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_1;
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf);
    
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    HAL_DMA_Start_IT(&g_dma_adc_handle, (uint32_t)&ADC1->DR, mar, 0);
    HAL_ADC_Start_DMA(&g_adc_dma_handle, &mar ,0);
}

/* ADC MSP初始化函数 */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance == ADC1)
    {
        GPIO_InitTypeDef gpio_init_struct;
        RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
        
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_ADC1_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_1;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct); 
        
        adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;
        adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;
        HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);
    }
}

/* 使能一次ADC DMA传输函数 */
void adc_dma_enable(uint16_t cndtr)
{
    ADC1->CR2 &= ~(1 << 0);
    
    DMA1_Channel1->CCR &= ~(1 << 0);
    while (DMA1_Channel1->CCR & (1 << 0));
    DMA1_Channel1->CNDTR = cndtr;
    DMA1_Channel1->CCR |= 1 << 0;

    ADC1->CR2 |= 1 << 0;
    ADC1->CR2 |= 1 << 22;
    
//    __HAL_ADC_DISABLE(&g_adc_dma_handle);
//    
//    __HAL_DMA_DISABLE(&g_dma_adc_handle);
//    while (__HAL_DMA_GET_FLAG(&g_dma_adc_handle, __HAL_DMA_GET_TC_FLAG_INDEX(&g_dma_adc_handle)));
//    DMA1_Channel1->CNDTR = cndtr;
//    __HAL_DMA_ENABLE(&g_dma_adc_handle);
//    
//    __HAL_ADC_ENABLE(&g_adc_dma_handle);
//    HAL_ADC_Start(&g_adc_dma_handle);
}

/* ADC DMA采集中断服务函数 */
void DMA1_Channel1_IRQHandler(void)
{
    if (DMA1->ISR & (1<<1))
    {
        g_adc_dma_sta = 1;
        DMA1->IFCR |= 1 << 1;
    }
}




















