#include "./BSP/DAC/dac.h"
#include "./SYSTEM/delay/delay.h"


DAC_HandleTypeDef g_dac_handle;

/* DAC��ʼ������ */
void dac_init(void)
{
    DAC_ChannelConfTypeDef dac_ch_conf;
    
    g_dac_handle.Instance = DAC;
    HAL_DAC_Init(&g_dac_handle);
    
    dac_ch_conf.DAC_Trigger = DAC_TRIGGER_NONE;
    dac_ch_conf.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
    HAL_DAC_ConfigChannel(&g_dac_handle, &dac_ch_conf, DAC_CHANNEL_1);

    HAL_DAC_Start(&g_dac_handle, DAC_CHANNEL_1);
}

/* DAC MSP��ʼ������ */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
    if (hdac->Instance == DAC)
    {
        GPIO_InitTypeDef gpio_init_struct;
        
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_DAC_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_4;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);
    }
}

/**
 * @brief       ����DAC_OUT1������ǲ�
 *   @note      ���Ƶ�� �� 1000 / (dt * samples) Khz, ������dt��С��ʱ��,����С��5usʱ, ����delay_us
 *              ����Ͳ�׼��(���ú���,����ȶ���Ҫʱ��,��ʱ��С��ʱ��,��Щʱ���Ӱ�쵽��ʱ), Ƶ�ʻ�ƫС.
 * 
 * @param       maxval : ���ֵ(0 < maxval < 4096), (maxval + 1)������ڵ���samples/2
 * @param       dt     : ÿ�����������ʱʱ��(��λ: us)
 * @param       samples: ������ĸ���, samples����С�ڵ���(maxval + 1) * 2 , ��maxval���ܵ���0
 * @param       n      : ������θ���,0~65535
 *
 * @retval      ��
 */
void dac_triangular_wave(uint16_t maxval, uint16_t dt, uint16_t samples, uint16_t n)
{
    uint16_t i, j;
    float incval;                               /* ������ */
    float Curval;                               /* ��ǰֵ */
    
    if(samples > ((maxval + 1) * 2))return ;    /* ���ݲ��Ϸ� */
        
    incval = (maxval + 1) / (samples / 2);      /* ��������� */
    
    for(j = 0; j < n; j++)
    { 
        Curval = 0;
        HAL_DAC_SetValue(&g_dac_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Curval);    /* �����0 */
        for(i = 0; i < (samples / 2); i++)      /* ��������� */
        {
            Curval  +=  incval;                 /* �µ����ֵ */
            HAL_DAC_SetValue(&g_dac_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Curval);
            delay_us(dt);
        }
        for(i = 0; i < (samples / 2); i++)      /* ����½��� */
        {
            Curval  -=  incval;                 /* �µ����ֵ */
            HAL_DAC_SetValue(&g_dac_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Curval);
            delay_us(dt);
        }
    }
}



