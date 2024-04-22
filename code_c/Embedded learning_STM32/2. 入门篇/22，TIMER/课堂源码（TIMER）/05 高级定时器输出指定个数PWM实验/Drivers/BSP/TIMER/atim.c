#include "./BSP/TIMER/atim.h"


TIM_HandleTypeDef g_timx_npwm_chy_handle;     /* 定时器x句柄 */

static uint8_t g_npwm_remain = 0;

 /* 高级定时器TIMX 通道Y 输出指定个数PWM 初始化函数 */
void atim_timx_npwm_chy_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_npwm_chy = {0};

    g_timx_npwm_chy_handle.Instance = TIM8;                            /* 定时器x */
    g_timx_npwm_chy_handle.Init.Prescaler = psc;                       /* 定时器分频 */
    g_timx_npwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;      /* 递增计数模式 */
    g_timx_npwm_chy_handle.Init.Period = arr;                          /* 自动重装载值 */
    g_timx_npwm_chy_handle.Init.RepetitionCounter = 0;                 /* 重复计数器初始值 */
    HAL_TIM_PWM_Init(&g_timx_npwm_chy_handle);                         /* 初始化PWM */


    timx_oc_npwm_chy.OCMode = TIM_OCMODE_PWM1;                         /* 模式选择PWM 1*/
    timx_oc_npwm_chy.Pulse = arr / 2;                                  /* 设置比较值,此值用来确定占空比 */
                                                                       /* 这里默认设置比较值为自动重装载值的一半,即占空比为50% */
    timx_oc_npwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;                 /* 输出比较极性为高 */
    HAL_TIM_PWM_ConfigChannel(&g_timx_npwm_chy_handle, &timx_oc_npwm_chy, TIM_CHANNEL_1);

    __HAL_TIM_ENABLE_IT(&g_timx_npwm_chy_handle, TIM_IT_UPDATE);
    HAL_TIM_PWM_Start(&g_timx_npwm_chy_handle, TIM_CHANNEL_1);
}

/* 定时器 PWM输出 MSP初始化函数 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM8)
    {
        GPIO_InitTypeDef gpio_init_struct;
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_TIM8_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_6;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;        /* 推挽式复用功能 */
        gpio_init_struct.Pull = GPIO_PULLUP;            /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;  /* 高速 */
        HAL_GPIO_Init(GPIOC, &gpio_init_struct);
        
        HAL_NVIC_SetPriority(TIM8_UP_IRQn, 1, 3);
        HAL_NVIC_EnableIRQ(TIM8_UP_IRQn);
    }
}

/* 高级定时器TIMX NPWM设置PWM个数函数 */
void atim_timx_npwm_chy_set(uint8_t npwm)
{
    if(npwm == 0) return;
    
    g_npwm_remain = npwm;
    HAL_TIM_GenerateEvent(&g_timx_npwm_chy_handle, TIM_EVENTSOURCE_UPDATE);
    __HAL_TIM_ENABLE(&g_timx_npwm_chy_handle);
}

/* 定时器8中断服务函数 */
void TIM8_UP_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_npwm_chy_handle);
}

/* 定时器更新中断回调函数 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM8)
    {
        if(g_npwm_remain)
        {
            TIM8->RCR = g_npwm_remain - 1;
            HAL_TIM_GenerateEvent(&g_timx_npwm_chy_handle, TIM_EVENTSOURCE_UPDATE);
            __HAL_TIM_ENABLE(&g_timx_npwm_chy_handle);
            g_npwm_remain = 0;
        }
        else
        {
            TIM8->CR1 &= ~(1 << 0);
        }
    }
}








