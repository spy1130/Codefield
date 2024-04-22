#include "./BSP/PWMDAC/pwmdac.h"


TIM_HandleTypeDef g_timx_pwm_chy_handle;

/* PWM DAC初始化 */
void pwmdac_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_pwm_chy = {0};
    
    g_timx_pwm_chy_handle.Instance = TIM1;
    g_timx_pwm_chy_handle.Init.Prescaler = psc;
    g_timx_pwm_chy_handle.Init.Period = arr;
    g_timx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_timx_pwm_chy_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_PWM_Init(&g_timx_pwm_chy_handle);
    
    timx_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;
    timx_oc_pwm_chy.Pulse = 0;
    timx_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;
    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle, &timx_oc_pwm_chy, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle, TIM_CHANNEL_1);
}

/* TIM MSP初始化函数 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM1)
    {
        GPIO_InitTypeDef gpio_init_struct;
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_TIM1_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_8;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* 推挽复用 */
        gpio_init_struct.Pull = GPIO_PULLUP;                /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* 高速 */
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);
    }
}

/* 设置PWM DAC输出电压 */
void pwmdac_set_voltage(uint16_t vol)
{
    float temp = vol;
    temp /= 1000;
    temp = temp * 256 / 3.3;
    __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, TIM_CHANNEL_1, temp);
}



