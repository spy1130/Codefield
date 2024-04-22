#include "./BSP/TIMER/gtim.h"


TIM_HandleTypeDef g_timx_cap_chy_handle;     /* ��ʱ��x��� */

/* ͨ�ö�ʱ��ͨ��y ���벶�� ��ʼ������ */
void gtim_timx_cap_chy_init(uint16_t arr, uint16_t psc)
{
    TIM_IC_InitTypeDef timx_ic_cap_chy = {0};

    g_timx_cap_chy_handle.Instance = TIM5;                              /* ��ʱ��5 */
    g_timx_cap_chy_handle.Init.Prescaler = psc;                         /* ��ʱ����Ƶ */
    g_timx_cap_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* ��������ģʽ */
    g_timx_cap_chy_handle.Init.Period = arr;                            /* �Զ���װ��ֵ */
    HAL_TIM_IC_Init(&g_timx_cap_chy_handle);

    timx_ic_cap_chy.ICPolarity = TIM_ICPOLARITY_RISING;                 /* �����ز��� */
    timx_ic_cap_chy.ICSelection = TIM_ICSELECTION_DIRECTTI;             /* ӳ�䵽TI1�� */
    timx_ic_cap_chy.ICPrescaler = TIM_ICPSC_DIV1;                       /* ���������Ƶ������Ƶ */
    timx_ic_cap_chy.ICFilter = 0;                                       /* ���������˲��������˲� */
    HAL_TIM_IC_ConfigChannel(&g_timx_cap_chy_handle, &timx_ic_cap_chy, TIM_CHANNEL_1);  /* ����TIM5ͨ��1 */

    __HAL_TIM_ENABLE_IT(&g_timx_cap_chy_handle, TIM_IT_UPDATE);         /* ʹ�ܸ����ж� */
    HAL_TIM_IC_Start_IT(&g_timx_cap_chy_handle, TIM_CHANNEL_1);         /* ��ʼ����TIM5��ͨ��1 */
}

/* ��ʱ�� ���벶�� MSP��ʼ������ */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5)                             /*����ͨ������*/
    {
        GPIO_InitTypeDef gpio_init_struct;
        __HAL_RCC_TIM5_CLK_ENABLE();                        /* ʹ��TIM5ʱ�� */
        __HAL_RCC_GPIOA_CLK_ENABLE();                       /* ��������IO��ʱ�� */

        gpio_init_struct.Pin = GPIO_PIN_0;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* ����������� */
        gpio_init_struct.Pull = GPIO_PULLDOWN;              /* ���� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* ���� */
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);

        HAL_NVIC_SetPriority(TIM5_IRQn, 1, 3);              /* ��ռ1�������ȼ�3 */
        HAL_NVIC_EnableIRQ(TIM5_IRQn);                      /* ����ITMx�ж� */
    }
}

/* ���벶��״̬(g_timxchy_cap_sta)
 * [7]  :0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
 * [6]  :0,��û���񵽸ߵ�ƽ;1,�Ѿ����񵽸ߵ�ƽ��.
 * [5:0]:����ߵ�ƽ������Ĵ���,������63��,���������ֵ = 63*65536 + 65535 = 4194303
 *       ע��:Ϊ��ͨ��,����Ĭ��ARR��CCRy����16λ�Ĵ���,����32λ�Ķ�ʱ��(��:TIM5),Ҳֻ��16λʹ��
 *       ��1us�ļ���Ƶ��,����ʱ��Ϊ:4194303 us, Լ4.19��
 *
 *      (˵��һ�£�����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
 */
uint8_t g_timxchy_cap_sta = 0;    /* ���벶��״̬ */
uint16_t g_timxchy_cap_val = 0;   /* ���벶��ֵ */


/* ��ʱ��5�жϷ����� */
void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_cap_chy_handle);  /* ��ʱ��HAL�⹲�ô����� */
}

/* ��ʱ�����벶���жϴ���ص����� */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5)
    {
        if ((g_timxchy_cap_sta & 0X80) == 0)                /* ��û�гɹ����� */
        {
            if (g_timxchy_cap_sta & 0X40)                   /* ����һ���½��� */
            {
                g_timxchy_cap_sta |= 0X80;                  /* ��ǳɹ�����һ�θߵ�ƽ���� */
                g_timxchy_cap_val = HAL_TIM_ReadCapturedValue(&g_timx_cap_chy_handle, TIM_CHANNEL_1);  /* ��ȡ��ǰ�Ĳ���ֵ */
                TIM_RESET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, TIM_CHANNEL_1);                      /* һ��Ҫ�����ԭ�������� */
                TIM_SET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING); /* ����TIM5ͨ��1�����ز��� */
            }
            else /* ��δ��ʼ,��һ�β��������� */
            {
                g_timxchy_cap_sta = 0;                              /* ��� */
                g_timxchy_cap_val = 0;
                g_timxchy_cap_sta |= 0X40;                          /* ��ǲ����������� */
                __HAL_TIM_DISABLE(&g_timx_cap_chy_handle);          /* �رն�ʱ��5 */
                __HAL_TIM_SET_COUNTER(&g_timx_cap_chy_handle, 0);   /* ��ʱ��5���������� */
                TIM_RESET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, TIM_CHANNEL_1);   /* һ��Ҫ�����ԭ�������ã��� */
                TIM_SET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING); /* ��ʱ��5ͨ��1����Ϊ�½��ز��� */
                __HAL_TIM_ENABLE(&g_timx_cap_chy_handle);           /* ʹ�ܶ�ʱ��5 */
            }
        }
    }
}

/* ��ʱ�������жϻص����� */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5)
    {
        if ((g_timxchy_cap_sta & 0X80) == 0)            /* ��δ�ɹ����� */
        {
            if (g_timxchy_cap_sta & 0X40)               /* �Ѿ����񵽸ߵ�ƽ�� */
            {
                if ((g_timxchy_cap_sta & 0X3F) == 0X3F) /* �ߵ�ƽ̫���� */
                {
                    TIM_RESET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, TIM_CHANNEL_1);                     /* һ��Ҫ�����ԭ�������� */
                    TIM_SET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);/* ����TIM5ͨ��1�����ز��� */
                    g_timxchy_cap_sta |= 0X80;          /* ��ǳɹ�������һ�� */
                    g_timxchy_cap_val = 0XFFFF;
                }
                else      /* �ۼƶ�ʱ��������� */
                {
                    g_timxchy_cap_sta++;
                }
            }
        }
    }
}




