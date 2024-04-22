#include "./BSP/TIMER/atim.h"


TIM_HandleTypeDef g_timx_pwmin_chy_handle;   /* ��ʱ��x��� */

/* PWM����״̬(g_timxchy_cap_sta)
 * 0,û�гɹ�����.
 * 1,�Ѿ��ɹ�������
 */
uint8_t g_timxchy_pwmin_sta  = 0;   /* PWM����״̬ */
uint16_t g_timxchy_pwmin_psc  = 0;  /* PWM�����Ƶϵ�� */
uint32_t g_timxchy_pwmin_hval = 0;  /* PWM�ĸߵ�ƽ���� */
uint32_t g_timxchy_pwmin_cval = 0;  /* PWM�����ڿ�� */

/* PWM����ģʽ ��ʼ������������ʱ��Ƶ��Ϊ72Mhz,����Լ13.8ns */
void atim_timx_pwmin_chy_init(void)
{
    TIM_SlaveConfigTypeDef slave_config = {0};
    TIM_IC_InitTypeDef tim_ic_pwmin_chy = {0};

    g_timx_pwmin_chy_handle.Instance = TIM8;                        /* ��ʱ��8 */
    g_timx_pwmin_chy_handle.Init.Prescaler = 0;                     /* ��ʱ��Ԥ��Ƶϵ�� */
    g_timx_pwmin_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;  /* ��������ģʽ */
    g_timx_pwmin_chy_handle.Init.Period = 65535;                    /* �Զ���װ��ֵ */
    HAL_TIM_IC_Init(&g_timx_pwmin_chy_handle);
    
    /* ��ģʽ���ã�IT1�������� */
    slave_config.SlaveMode = TIM_SLAVEMODE_RESET;                   /* ��ģʽ����λģʽ */
    slave_config.InputTrigger = TIM_TS_TI1FP1;                      /* ��ʱ�����봥��Դ��TI1FP1 */
    slave_config.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;      /* �����ؼ�� */
    slave_config.TriggerFilter = 0;                                 /* ���˲� */
    HAL_TIM_SlaveConfigSynchro(&g_timx_pwmin_chy_handle, &slave_config);

    /* IC1���������ش���TI1FP1 */
    tim_ic_pwmin_chy.ICPolarity = TIM_ICPOLARITY_RISING;            /* �����ؼ�� */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_DIRECTTI;        /* ѡ�������IC1ӳ�䵽TI1 */
    tim_ic_pwmin_chy.ICPrescaler = TIM_ICPSC_DIV1;                  /* ����Ƶ */
    tim_ic_pwmin_chy.ICFilter = 0;                                  /* ���˲� */
    HAL_TIM_IC_ConfigChannel(&g_timx_pwmin_chy_handle, &tim_ic_pwmin_chy, TIM_CHANNEL_1);

    /* IC2�����½��ش���TI1FP2 */
    tim_ic_pwmin_chy.ICPolarity = TIM_ICPOLARITY_FALLING;           /* �½��ؼ�� */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_INDIRECTTI;      /* ѡ�������IC2ӳ�䵽TI1 */
    HAL_TIM_IC_ConfigChannel(&g_timx_pwmin_chy_handle, &tim_ic_pwmin_chy, TIM_CHANNEL_2);

    HAL_TIM_IC_Start_IT(&g_timx_pwmin_chy_handle, TIM_CHANNEL_1);
    HAL_TIM_IC_Start(&g_timx_pwmin_chy_handle, TIM_CHANNEL_2);
}

/* ��ʱ�� ���벶�� MSP��ʼ������ */
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
        
        /* TIM1/TIM8�ж��������벶���жϷ����� */
        HAL_NVIC_SetPriority(TIM8_CC_IRQn, 1, 3);
        HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);
    }
}

/* ��ʱ��8 ���벶�� �жϷ���������TIM1/TIM8���������,������ͨ��ʱ��û������жϷ�����! */
void TIM8_CC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_pwmin_chy_handle); /* ��ʱ�����ô����� */
}

/* PWM����ģʽ ������������ */
void atim_timx_pwmin_chy_restart(void)
{
    sys_intx_disable();                     /* �ر��ж� */

    g_timxchy_pwmin_sta = 0;                /* ����״̬,���¿�ʼ��� */
    g_timxchy_pwmin_hval=0;
    g_timxchy_pwmin_cval=0;

    sys_intx_enable();                      /* ���ж� */
}

/* ��ʱ�����벶���жϴ���ص����� */
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






