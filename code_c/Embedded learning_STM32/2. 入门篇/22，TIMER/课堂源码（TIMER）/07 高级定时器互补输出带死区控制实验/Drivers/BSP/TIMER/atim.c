#include "./BSP/TIMER/atim.h"


TIM_HandleTypeDef g_timx_cplm_pwm_handle;                                  /* 定时器x句柄 */
TIM_BreakDeadTimeConfigTypeDef g_sbreak_dead_time_config;                  /* 死区时间设置 */

/* 高级定时器 互补输出 初始化函数（使用PWM模式1） */
void atim_timx_cplm_pwm_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef tim_oc_cplm_pwm = {0};

    g_timx_cplm_pwm_handle.Instance = TIM1;                                 /* 定时器x */
    g_timx_cplm_pwm_handle.Init.Prescaler = psc;                            /* 定时器预分频系数 */
    g_timx_cplm_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;           /* 递增计数模式 */
    g_timx_cplm_pwm_handle.Init.Period = arr;                               /* 自动重装载值 */
    g_timx_cplm_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;     /* CKD[1:0] = 10, tDTS = 4 * tCK_INT = Ft / 4 = 18Mhz */
    HAL_TIM_PWM_Init(&g_timx_cplm_pwm_handle);

    tim_oc_cplm_pwm.OCMode = TIM_OCMODE_PWM1;                               /* PWM模式1 */
    tim_oc_cplm_pwm.OCPolarity = TIM_OCPOLARITY_HIGH;                       /* OCy 高电平有效 */
    tim_oc_cplm_pwm.OCNPolarity = TIM_OCNPOLARITY_HIGH;                     /* OCyN 高电平有效 */
    tim_oc_cplm_pwm.OCIdleState = TIM_OCIDLESTATE_RESET;                    /* 当MOE=0，OCx=0 */
    tim_oc_cplm_pwm.OCNIdleState = TIM_OCNIDLESTATE_RESET;                  /* 当MOE=0，OCxN=0 */
    HAL_TIM_PWM_ConfigChannel(&g_timx_cplm_pwm_handle, &tim_oc_cplm_pwm, TIM_CHANNEL_1);

    /* 设置死区参数，开启死区中断 */
    g_sbreak_dead_time_config.OffStateRunMode = TIM_OSSR_DISABLE;           /* 运行模式的关闭输出状态 */
    g_sbreak_dead_time_config.OffStateIDLEMode = TIM_OSSI_DISABLE;          /* 空闲模式的关闭输出状态 */
    g_sbreak_dead_time_config.LockLevel = TIM_LOCKLEVEL_OFF;                /* 不用寄存器锁功能 */
    g_sbreak_dead_time_config.BreakState = TIM_BREAK_ENABLE;                /* 使能刹车输入 */
    g_sbreak_dead_time_config.BreakPolarity = TIM_BREAKPOLARITY_HIGH;       /* 刹车输入有效信号极性为高 */
    g_sbreak_dead_time_config.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE; /* 使能AOE位，允许刹车结束后自动恢复输出 */
    HAL_TIMEx_ConfigBreakDeadTime(&g_timx_cplm_pwm_handle, &g_sbreak_dead_time_config);

    HAL_TIM_PWM_Start(&g_timx_cplm_pwm_handle, TIM_CHANNEL_1);              /* OCy 输出使能 */
    HAL_TIMEx_PWMN_Start(&g_timx_cplm_pwm_handle,TIM_CHANNEL_1);            /* OCyN 输出使能 */
}


/* 定时器 PWM输出 MSP初始化函数 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        GPIO_InitTypeDef gpio_init_struct = {0};

        __HAL_RCC_TIM1_CLK_ENABLE();
        __HAL_RCC_GPIOE_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_9;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP; 
        gpio_init_struct.Pull = GPIO_PULLDOWN;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH ;
        HAL_GPIO_Init(GPIOE, &gpio_init_struct);

        gpio_init_struct.Pin = GPIO_PIN_8;
        HAL_GPIO_Init(GPIOE, &gpio_init_struct);

        gpio_init_struct.Pin = GPIO_PIN_15;
        HAL_GPIO_Init(GPIOE, &gpio_init_struct);

        __HAL_RCC_AFIO_CLK_ENABLE();
        __HAL_AFIO_REMAP_TIM1_ENABLE();
    }
}

/**
 * @brief       定时器TIMX 设置输出比较值 & 死区时间
 * @param       ccr: 输出比较值
 * @param       dtg: 死区时间
 *   @arg       dtg[7:5]=0xx时, 死区时间 = dtg[7:0] * tDTS
 *   @arg       dtg[7:5]=10x时, 死区时间 = (64 + dtg[6:0]) * 2  * tDTS
 *   @arg       dtg[7:5]=110时, 死区时间 = (32 + dtg[5:0]) * 8  * tDTS
 *   @arg       dtg[7:5]=111时, 死区时间 = (32 + dtg[5:0]) * 16 * tDTS
 *   @note      tDTS = 1 / (Ft /  CKD[1:0]) = 1 / 18M = 55.56ns
 * @retval      无
 */
void atim_timx_cplm_pwm_set(uint16_t ccr, uint8_t dtg)
{
    __HAL_TIM_SET_COMPARE(&g_timx_cplm_pwm_handle, TIM_CHANNEL_1, ccr);
    g_sbreak_dead_time_config.DeadTime = dtg;
    HAL_TIMEx_ConfigBreakDeadTime(&g_timx_cplm_pwm_handle, &g_sbreak_dead_time_config);
}






