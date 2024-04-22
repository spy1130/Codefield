#include "./BSP/TIMER/atim.h"


TIM_HandleTypeDef g_timx_pwmin_chy_handle;   /* 定时器x句柄 */

/* PWM输入状态(g_timxchy_cap_sta)
 * 0,没有成功捕获.
 * 1,已经成功捕获了
 */
uint8_t g_timxchy_pwmin_sta  = 0;   /* PWM输入状态 */
uint16_t g_timxchy_pwmin_psc  = 0;  /* PWM输入分频系数 */
uint32_t g_timxchy_pwmin_hval = 0;  /* PWM的高电平脉宽 */
uint32_t g_timxchy_pwmin_cval = 0;  /* PWM的周期宽度 */

/* PWM输入模式 初始化函数，采样时钟频率为72Mhz,精度约13.8ns */
void atim_timx_pwmin_chy_init(void)
{
    TIM_SlaveConfigTypeDef slave_config = {0};
    TIM_IC_InitTypeDef tim_ic_pwmin_chy = {0};

    g_timx_pwmin_chy_handle.Instance = TIM8;                        /* 定时器8 */
    g_timx_pwmin_chy_handle.Init.Prescaler = 0;                     /* 定时器预分频系数 */
    g_timx_pwmin_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;  /* 递增计数模式 */
    g_timx_pwmin_chy_handle.Init.Period = 65535;                    /* 自动重装载值 */
    HAL_TIM_IC_Init(&g_timx_pwmin_chy_handle);
    
    /* 从模式配置，IT1触发更新 */
    slave_config.SlaveMode = TIM_SLAVEMODE_RESET;                   /* 从模式：复位模式 */
    slave_config.InputTrigger = TIM_TS_TI1FP1;                      /* 定时器输入触发源：TI1FP1 */
    slave_config.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;      /* 上升沿检测 */
    slave_config.TriggerFilter = 0;                                 /* 不滤波 */
    HAL_TIM_SlaveConfigSynchro(&g_timx_pwmin_chy_handle, &slave_config);

    /* IC1捕获：上升沿触发TI1FP1 */
    tim_ic_pwmin_chy.ICPolarity = TIM_ICPOLARITY_RISING;            /* 上升沿检测 */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_DIRECTTI;        /* 选择输入端IC1映射到TI1 */
    tim_ic_pwmin_chy.ICPrescaler = TIM_ICPSC_DIV1;                  /* 不分频 */
    tim_ic_pwmin_chy.ICFilter = 0;                                  /* 不滤波 */
    HAL_TIM_IC_ConfigChannel(&g_timx_pwmin_chy_handle, &tim_ic_pwmin_chy, TIM_CHANNEL_1);

    /* IC2捕获：下降沿触发TI1FP2 */
    tim_ic_pwmin_chy.ICPolarity = TIM_ICPOLARITY_FALLING;           /* 下降沿检测 */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_INDIRECTTI;      /* 选择输入端IC2映射到TI1 */
    HAL_TIM_IC_ConfigChannel(&g_timx_pwmin_chy_handle, &tim_ic_pwmin_chy, TIM_CHANNEL_2);

    HAL_TIM_IC_Start_IT(&g_timx_pwmin_chy_handle, TIM_CHANNEL_1);
    HAL_TIM_IC_Start(&g_timx_pwmin_chy_handle, TIM_CHANNEL_2);
}

/* 定时器 输入捕获 MSP初始化函数 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM8)
    {
        GPIO_InitTypeDef gpio_init_struct = {0};

        __HAL_RCC_TIM8_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_6;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLDOWN;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOC, &gpio_init_struct);
        
        /* TIM1/TIM8有独立的输入捕获中断服务函数 */
        HAL_NVIC_SetPriority(TIM8_CC_IRQn, 1, 3);
        HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);
    }
}

/* 定时器8 输入捕获 中断服务函数，仅TIM1/TIM8有这个函数,其他普通定时器没有这个中断服务函数! */
void TIM8_CC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_pwmin_chy_handle); /* 定时器共用处理函数 */
}

/* PWM输入模式 重新启动捕获 */
void atim_timx_pwmin_chy_restart(void)
{
    sys_intx_disable();                     /* 关闭中断 */

    g_timxchy_pwmin_sta = 0;                /* 清零状态,重新开始检测 */
    g_timxchy_pwmin_hval=0;
    g_timxchy_pwmin_cval=0;

    sys_intx_enable();                      /* 打开中断 */
}

/* 定时器输入捕获中断处理回调函数 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM8)
    {
        if(g_timxchy_pwmin_sta == 0)
        {
            if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
            {
                g_timxchy_pwmin_hval = HAL_TIM_ReadCapturedValue(&g_timx_pwmin_chy_handle, TIM_CHANNEL_2) + 1 + 1;
                g_timxchy_pwmin_cval = HAL_TIM_ReadCapturedValue(&g_timx_pwmin_chy_handle, TIM_CHANNEL_1) + 1 + 1;
                g_timxchy_pwmin_sta = 1;
            }
        }
    }
}






