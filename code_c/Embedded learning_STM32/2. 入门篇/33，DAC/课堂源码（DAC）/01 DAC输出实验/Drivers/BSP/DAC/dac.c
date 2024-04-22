#include "./BSP/DAC/dac.h"


DAC_HandleTypeDef g_dac_handle;

/* DAC初始化函数 */
void dac_init(void)
{
    DAC_ChannelConfTypeDef dac_ch_conf;

    g_dac_handle.Instance = DAC;
    HAL_DAC_Init(&g_dac_handle);                                        /* 初始化DAC */

    dac_ch_conf.DAC_Trigger = DAC_TRIGGER_NONE;                         /* 不使用触发功能 */
    dac_ch_conf.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;            /* DAC输出缓冲关闭 */

    HAL_DAC_ConfigChannel(&g_dac_handle, &dac_ch_conf, DAC_CHANNEL_1);  /* 配置DAC通道1 */
    HAL_DAC_Start(&g_dac_handle, DAC_CHANNEL_1);                        /* 开启DAC通道1 */
}

/* DAC MSP初始化函数 */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
    if (hdac->Instance == DAC)
    {
        GPIO_InitTypeDef gpio_init_struct;

        __HAL_RCC_DAC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_4;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);
    }
}

/* 设置通道输出电压 */
void dac_set_voltage(uint16_t vol)
{
    double temp = vol;
    temp /= 1000;
    temp = temp * 4096 / 3.3;

    if (temp >= 4096)temp = 4095;   /* 如果值大于等于4096, 则取4095 */

    HAL_DAC_SetValue(&g_dac_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, temp); /* 12位右对齐数据格式设置DAC值 */
}





