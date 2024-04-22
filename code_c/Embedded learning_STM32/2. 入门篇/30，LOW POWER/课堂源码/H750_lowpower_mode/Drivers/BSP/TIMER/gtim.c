/**
 ****************************************************************************************************
 * @file        gtim.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.4
 * @date        2020-03-14
 * @brief       ͨ�ö�ʱ�� ��������
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
 * V1.0 20200314
 * ��һ�η���
 * V1.1 20200317
 * ����gtim_timx_pwm_chy_init��gtim_timx_comp_pwm_gpio_init����
 * V1.2 20200317
 * 1,����set_timx_comparey����
 * 2,����get_timx_capture����
 * 3,����gtim_timx_cap_chy_init����
 * 4,����gtim_timx_cap_chy_init����
 * V1.3 20200329
 * 1,֧���ⲿ�����������
 * 2,����gtim_timx_cnt_chy_init����
 * 3,����gtim_timx_cnt_chy_gpio_init����
 * 4,����gtim_timx_cnt_chy_get_count����
 * 5,����gtim_timx_cnt_chy_restart����
 * V1.4 20200329
 * 1,����gtim_timx_npwm_chy_init����
 * 2,����gtim_timx_npwm_chy_gpio_init����
 * 2,����gtim_timx_npwm_chy_set����
 *
 ****************************************************************************************************
 */

#include "./BSP/TIMER/gtim.h"
#include "./BSP/LED/led.h"


TIM_HandleTypeDef g_timx_handle;      /* ��ʱ��x��� */


/**
 * @brief       ͨ�ö�ʱ��TIMX��ʱ�жϳ�ʼ������
 * @note
 *              ͨ�ö�ʱ����ʱ������APB1,��D2PPRE1��2��Ƶ��ʱ��
 *              ͨ�ö�ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ120M, ���Զ�ʱ��ʱ�� = 240Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void gtim_timx_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX_INT_CLK_ENABLE();                                      /* ʹ��TIMxʱ�� */
    
    g_timx_handle.Instance = GTIM_TIMX_INT;                          /* ͨ�ö�ʱ��x */
    g_timx_handle.Init.Prescaler = psc;                              /* ��Ƶ */
    g_timx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;             /* ���ϼ����� */
    g_timx_handle.Init.Period = arr;                                 /* �Զ�װ��ֵ */
    g_timx_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;       /* ʱ�ӷ�Ƶ���� */
    HAL_TIM_Base_Init(&g_timx_handle);

    HAL_NVIC_SetPriority(GTIM_TIMX_INT_IRQn, 1, 3);  /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);          /* ����ITMx�ж� */

    HAL_TIM_Base_Start_IT(&g_timx_handle);           /* ʹ�ܶ�ʱ��x�Ͷ�ʱ��x�����ж� */
}

/**
 * @brief       ��ʱ���жϷ�����
 * @param       ��
 * @retval      ��
 */
void GTIM_TIMX_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_handle);
}

/*********************************ͨ�ö�ʱ��PWM�������*************************************/

TIM_HandleTypeDef g_timx_pwm_chy_handle;     /* ��ʱ��x��� */
TIM_OC_InitTypeDef g_timx_oc_pwm_chy_handle; /* ��ʱ�������� */

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y PWM��� ��ʼ��������ʹ��PWMģʽ1��
 * @note
 *              ͨ�ö�ʱ����ʱ������APB1,��D2PPRE1��2��Ƶ��ʱ��
 *              ͨ�ö�ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ120M, ���Զ�ʱ��ʱ�� = 240Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void gtim_timx_pwm_chy_init(uint16_t arr,uint16_t psc)
{
    gtim_timx_comp_pwm_gpio_init();
    GTIM_TIMX_PWM_CHY_CLK_ENABLE();
    
    g_timx_pwm_chy_handle.Instance = GTIM_TIMX_PWM;                    /* ��ʱ��x */
    g_timx_pwm_chy_handle.Init.Prescaler = psc;                        /* ��ʱ����Ƶ */
    g_timx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;       /* ���ϼ���ģʽ */
    g_timx_pwm_chy_handle.Init.Period = arr;                           /* �Զ���װ��ֵ */
    g_timx_pwm_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; /* ʱ�ӷ�Ƶ���� */
    HAL_TIM_PWM_Init(&g_timx_pwm_chy_handle);                          /* ��ʼ��PWM */
    
    g_timx_oc_pwm_chy_handle.OCMode = TIM_OCMODE_PWM1;                 /* ģʽѡ��PWM1 */
    g_timx_oc_pwm_chy_handle.Pulse = arr/2;                            /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
                                                                       /* Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50% */
    g_timx_oc_pwm_chy_handle.OCPolarity = TIM_OCPOLARITY_LOW;          /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle, &g_timx_oc_pwm_chy_handle, GTIM_TIMX_PWM_CHY);/* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle, GTIM_TIMX_PWM_CHY);       /* ����PWMͨ��1 */
}

/**
 * @brief       ͨ�ö�ʱ��PWM���ͨ�����ų�ʼ������
 * @param       htim:��ʱ�����
 * @note        �˺����ᱻgtim_timx_pwm_chy_init()����
 * @retval      ��
 */
void gtim_timx_comp_pwm_gpio_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE();                    /* ����ͨ��y��CPIOʱ�� */

    gpio_init_struct.Pin = GTIM_TIMX_PWM_CHY_GPIO_PIN;      /* ͨ��y��CPIO�� */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* ���� */
    gpio_init_struct.Alternate = GTIM_TIMX_PWM_CHY_GPIO_AF; /* ��ʱ��xͨ��y��CPIO�ڸ��� */
    HAL_GPIO_Init(GTIM_TIMX_PWM_CHY_GPIO_PORT, &gpio_init_struct);
}

/*********************************ͨ�ö�ʱ�����벶��ʵ�����*************************************/

/**
 * @brief       ����TIMͨ��1��ռ�ձ�
 * @param       compare:�Ƚ�ֵ
 * @retval      ��
 */
void set_timx_comparey(uint32_t compare)
{
    GTIM_TIMX_PWM_CHY_CCRX=compare;
}

/**
 * @brief       ��ȡtimx����/�ȽϼĴ���ֵ������Ĭ����TIM3��
 * @param       ��
 * @retval      ����TIM����/�ȽϼĴ���ֵ
 */
uint32_t get_timx_capture(void)
{
    return HAL_TIM_ReadCapturedValue(&g_timx_pwm_chy_handle,GTIM_TIMX_PWM_CHY);
}


TIM_HandleTypeDef g_timx_cap_chy_handler;      /* ��ʱ��x��� */
TIM_IC_InitTypeDef g_timx_ic_cap_chy_handler;

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y ���벶�� ��ʼ������
 * @note
 *              ͨ�ö�ʱ����ʱ������APB1,��D2PPRE1��2��Ƶ��ʱ��
 *              ͨ�ö�ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ120M, ���Զ�ʱ��ʱ�� = 240Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void gtim_timx_cap_chy_init(uint16_t arr, uint16_t psc)
{
    gtim_timx_cap_chy_gpio_init();
    
    g_timx_cap_chy_handler.Instance = GTIM_TIMX_CAP;                      /* ��ʱ��5 */
    g_timx_cap_chy_handler.Init.Prescaler = psc;                          /* ��ʱ����Ƶ */
    g_timx_cap_chy_handler.Init.CounterMode = TIM_COUNTERMODE_UP;         /* ���ϼ���ģʽ */
    g_timx_cap_chy_handler.Init.Period = arr;                             /* �Զ���װ��ֵ */
    g_timx_cap_chy_handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;   /* ʱ�ӷ�Ƶ���� */
    HAL_TIM_IC_Init(&g_timx_cap_chy_handler);
    
    g_timx_ic_cap_chy_handler.ICPolarity = TIM_ICPOLARITY_RISING;         /* �����ز��� */
    g_timx_ic_cap_chy_handler.ICSelection = TIM_ICSELECTION_DIRECTTI;     /* ӳ�䵽TI1�� */
    g_timx_ic_cap_chy_handler.ICPrescaler = TIM_ICPSC_DIV1;               /* ���������Ƶ������Ƶ */
    g_timx_ic_cap_chy_handler.ICFilter = 0;                               /* ���������˲��������˲� */
    HAL_TIM_IC_ConfigChannel(&g_timx_cap_chy_handler, &g_timx_ic_cap_chy_handler, GTIM_TIMX_CAP_CHY);//����TIM5ͨ��1 */
    HAL_TIM_IC_Start_IT(&g_timx_cap_chy_handler, GTIM_TIMX_CAP_CHY);      /* ��ʼ����TIM5��ͨ��1 */
    __HAL_TIM_ENABLE_IT(&g_timx_cap_chy_handler, TIM_IT_UPDATE);          /* ʹ�ܸ����ж� */
}

/**
 * @brief       ͨ�ö�ʱ�����벶�����ų�ʼ������
 * @param       htim:��ʱ�����
 * @note        �˺����ᱻgtim_timx_cap_chy_init()����
 * @retval      ��
 */
void gtim_timx_cap_chy_gpio_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    GTIM_TIMX_CAP_CHY_CLK_ENABLE();                               /* ʹ��TIMxʱ�� */
    GTIM_TIMX_CAP_CHY_GPIO_CLK_ENABLE();                          /* ����GPIOAʱ�� */

    gpio_init_struct.Pin = GTIM_TIMX_CAP_CHY_GPIO_PIN;            /* ���벶���GPIO�� */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                      /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                        /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;           /* ���� */
    gpio_init_struct.Alternate = GTIM_TIMX_CAP_CHY_GPIO_AF;       /* ����Ϊ����TIM5��ͨ��1 */
    HAL_GPIO_Init(GTIM_TIMX_CAP_CHY_GPIO_PORT, &gpio_init_struct);

    HAL_NVIC_SetPriority(GTIM_TIMX_CAP_IRQn,2,0);    /* �����ж����ȼ�����ռ���ȼ�2�������ȼ�0 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_CAP_IRQn);          /* ����ITMx�ж� */
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
uint8_t g_timxchy_cap_sta = 0;  /* ���벶��״̬ */
uint16_t g_timxchy_cap_val =0 ; /* ���벶��ֵ */


/**
 * @brief       ��ʱ���жϷ�����
 * @param       ��
 * @retval      ��
 */
void GTIM_TIMX_CAP_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_cap_chy_handler);/* ��ʱ�����ô����� */
}

/* �߼���ʱatim.c����������ص�������Ϊ�˱����ظ��������Σ���Ҫ����ȡ��ע�� */
/**
 * @brief       ��ʱ�����벶���жϴ���ص�����
 * @param       htim:��ʱ�����ָ��
 * @note        �ú�����HAL_TIM_IRQHandler�лᱻ����
 * @retval      ��
 */
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
//{
//    if ((g_timxchy_cap_sta & 0X80) == 0)         /* ��δ�ɹ����� */
//    {
//        if (g_timxchy_cap_sta & 0X40)            /* ����һ���½��� */
//        {
//            g_timxchy_cap_sta |= 0X80;           /* ��ǳɹ�����һ�θߵ�ƽ���� */
//            g_timxchy_cap_val = HAL_TIM_ReadCapturedValue(&g_timx_cap_chy_handler, GTIM_TIMX_CAP_CHY); /* ��ȡ��ǰ�Ĳ���ֵ */
//            TIM_RESET_CAPTUREPOLARITY(&g_timx_cap_chy_handler, GTIM_TIMX_CAP_CHY);                     /* һ��Ҫ�����ԭ�������� */
//            TIM_SET_CAPTUREPOLARITY(&g_timx_cap_chy_handler, GTIM_TIMX_CAP_CHY, TIM_ICPOLARITY_RISING);/* ����TIM5ͨ��1�����ز��� */
//        }
//        else                                     /* ��δ��ʼ,��һ�β��������� */
//        {
//            g_timxchy_cap_sta = 0;               /* ��� */
//            g_timxchy_cap_val = 0;
//            g_timxchy_cap_sta |= 0X40;           /* ��ǲ����������� */
//            __HAL_TIM_DISABLE(&g_timx_cap_chy_handler);  /* �رն�ʱ��5 */
//            __HAL_TIM_SET_COUNTER(&g_timx_cap_chy_handler,0);
//            TIM_RESET_CAPTUREPOLARITY(&g_timx_cap_chy_handler, GTIM_TIMX_CAP_CHY);   /* һ��Ҫ�����ԭ�������ã��� */
//            TIM_SET_CAPTUREPOLARITY(&g_timx_cap_chy_handler, GTIM_TIMX_CAP_CHY, TIM_ICPOLARITY_FALLING);/* ��ʱ��5ͨ��1����Ϊ�½��ز��� */
//            __HAL_TIM_ENABLE(&g_timx_cap_chy_handler);   /* ʹ�ܶ�ʱ��5 */
//        }
//    }
//}

/*********************************ͨ�ö�ʱ���������ʵ�����*************************************/

TIM_HandleTypeDef g_timx_cnt_chy_handler;      /* ��ʱ��x��� */

/* ��¼��ʱ�����������������, ���������������� */
uint32_t g_timxchy_cnt_ofcnt = 0 ;  /* ����������� */

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y ������� ��ʼ������
 * @note
 *              ������ѡ��ͨ�ö�ʱ����ʱ��ѡ��: �ⲿʱ��Դģʽ1(SMS[2:0] = 111)
 *              ����CNT�ļ���ʱ��Դ������ TIMX_CH1/CH2, ����ʵ���ⲿ�������(�������CH1/CH2)
 *
 *              ʱ�ӷ�Ƶ�� = psc, һ������Ϊ0, ��ʾÿһ��ʱ�Ӷ������һ��, ����߾���.
 *              ͨ����ȡCNT���������, �����򵥼���, ���Եõ���ǰ�ļ���ֵ, �Ӷ�ʵ���������
 *
 * @param       arr: �Զ���װֵ 
 * @retval      ��
 */
void gtim_timx_cnt_chy_init(uint16_t psc)
{
    TIM_SlaveConfigTypeDef  tim_slave_config_handle = {0};
    TIM_IC_InitTypeDef timx_ic_cnt_chy_handler = {0};

    gtim_timx_cnt_chy_gpio_init();
    
    g_timx_cnt_chy_handler.Instance = GTIM_TIMX_CNT;                      /* ��ʱ��x */
    g_timx_cnt_chy_handler.Init.Prescaler = psc;                          /* ��ʱ����Ƶ */
    g_timx_cnt_chy_handler.Init.CounterMode = TIM_COUNTERMODE_UP;         /* ���ϼ���ģʽ */
    g_timx_cnt_chy_handler.Init.Period = 65535;                           /* �Զ���װ��ֵ */
    g_timx_cnt_chy_handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;   /* ʱ�ӷ�Ƶ���� */
    g_timx_cnt_chy_handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_IC_Init(&g_timx_cnt_chy_handler);

    /* ��ģʽ���ⲿ����ģʽ1 */
    tim_slave_config_handle.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;            /* ��ģʽ���ⲿ����ģʽ1 */
    tim_slave_config_handle.InputTrigger = TIM_TS_TI1FP1;                   /* ���봥����ѡ�� TI1FP1(TIMX_CH1) ��Ϊ����Դ */
    tim_slave_config_handle.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;   /* �������ԣ������� */
    tim_slave_config_handle.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;   /* ����Ԥ��Ƶ���� */
    tim_slave_config_handle.TriggerFilter = 0x0;                            /* �˲��������в���Ҫ�κ��˲� */
    HAL_TIM_SlaveConfigSynchronization(&g_timx_cnt_chy_handler,&tim_slave_config_handle);

    /* �������벶��ģʽ */
    timx_ic_cnt_chy_handler.ICPolarity = TIM_ICPOLARITY_RISING;          /* �����ز��� */
    timx_ic_cnt_chy_handler.ICSelection = TIM_ICSELECTION_DIRECTTI;      /* ӳ�䵽TI1�� */
    timx_ic_cnt_chy_handler.ICPrescaler = TIM_ICPSC_DIV1;                /* ���������Ƶ������Ƶ */
    timx_ic_cnt_chy_handler.ICFilter = 0;                                /* ���������˲��������˲� */
    HAL_TIM_IC_ConfigChannel(&g_timx_cnt_chy_handler, &timx_ic_cnt_chy_handler, GTIM_TIMX_CNT_CHY);/* ����TIMxͨ��y */
    HAL_TIM_IC_Start_IT(&g_timx_cnt_chy_handler, GTIM_TIMX_CNT_CHY);     /* ��ʼ����TIMx��ͨ��y */
}

/**
 * @brief       ͨ�ö�ʱ���������ų�ʼ������
 * @param       htim:��ʱ�����
 * @note        �˺����ᱻgtim_timx_cap_chy_init()����
 * @retval      ��
 */
void gtim_timx_cnt_chy_gpio_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    GTIM_TIMX_CNT_CHY_CLK_ENABLE();                               /* ʹ��TIMxʱ�� */
    GTIM_TIMX_CNT_CHY_GPIO_CLK_ENABLE();                          /* ����GPIOAʱ�� */

    gpio_init_struct.Pin = GTIM_TIMX_CNT_CHY_GPIO_PIN;            /* ���벶���GPIO�� */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                      /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                        /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;           /* ���� */
    gpio_init_struct.Alternate = GTIM_TIMX_CNT_CHY_GPIO_AF;       /* ����Ϊ����TIMx��ͨ�� */
    HAL_GPIO_Init(GTIM_TIMX_CNT_CHY_GPIO_PORT, &gpio_init_struct);

    HAL_NVIC_SetPriority(GTIM_TIMX_CNT_IRQn,1,3);    /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_CNT_IRQn);          /* ����ITMx�ж� */
}

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y ��ȡ��ǰ����ֵ 
 * @param       ��
 * @retval      ��ǰ����ֵ
 */
uint32_t gtim_timx_cnt_chy_get_count(void)
{
    uint32_t count = 0;
    count = g_timxchy_cnt_ofcnt * 65536;/* �������������Ӧ�ļ���ֵ */
    count += __HAL_TIM_GET_COUNTER(&g_timx_cnt_chy_handler);        /* ���ϵ�ǰCNT��ֵ */
    return count;
}

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y ����������
 * @param       ��
 * @retval      ��ǰ����ֵ
 */
void gtim_timx_cnt_chy_restart(void)
{
    __HAL_TIM_DISABLE(&g_timx_cnt_chy_handler);           /* �رն�ʱ��TIMX */
    g_timxchy_cnt_ofcnt = 0;                              /* �ۼ������� */
    __HAL_TIM_SET_COUNTER(&g_timx_cnt_chy_handler, 0);    /* ���������� */
    __HAL_TIM_ENABLE(&g_timx_cnt_chy_handler);            /* ʹ�ܶ�ʱ��TIMX */
}

/**
 * @brief       ͨ�ö�ʱ��TIMX ������� �����жϷ�����
 * @param       ��
 * @retval      ��
 */
void GTIM_TIMX_CNT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_cnt_chy_handler);/* ��ʱ�����ô����� */
}

/*********************************ͨ�ö�ʱ�����ָ������PWM����*************************************/

TIM_HandleTypeDef g_timx_npwm_chy_handle;     /* ��ʱ��x��� */
TIM_OC_InitTypeDef g_timx_oc_npwm_chy_handle; /* ��ʱ�������� */

/* g_npwm_remain��ʾ��ǰ��ʣ�¶��ٸ�����Ҫ���� 
 * ÿ����෢��256������
 */
static uint32_t g_npwm_remain = 0;

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y ���ָ������PWM ��ʼ������
 * @note
 *              ͨ�ö�ʱ����ʱ������APB1,��D2PPRE1��2��Ƶ��ʱ��
 *              ͨ�ö�ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ120M, ���Զ�ʱ��ʱ�� = 240Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void gtim_timx_npwm_chy_init(uint16_t arr,uint16_t psc)
{
    gtim_timx_npwm_chy_gpio_init();
    GTIM_TIMX_NPWM_CHY_CLK_ENABLE();
    
    g_timx_npwm_chy_handle.Instance = GTIM_TIMX_NPWM;                     /* ��ʱ��x */
    g_timx_npwm_chy_handle.Init.Prescaler = psc;                          /* ��ʱ����Ƶ */
    g_timx_npwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;         /* ���ϼ���ģʽ */
    g_timx_npwm_chy_handle.Init.Period = arr;                             /* �Զ���װ��ֵ */
    g_timx_npwm_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;   /* ʱ�ӷ�Ƶ���� */
    HAL_TIM_PWM_Init(&g_timx_npwm_chy_handle);                            /* ��ʼ��PWM */
    
    g_timx_oc_npwm_chy_handle.OCMode = TIM_OCMODE_PWM1;                  /* ģʽѡ��PWM1 */
    g_timx_oc_npwm_chy_handle.Pulse = arr/2;                             /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
                                                                         /* Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50% */
    g_timx_oc_npwm_chy_handle.OCPolarity = TIM_OCPOLARITY_HIGH;          /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_timx_npwm_chy_handle, &g_timx_oc_npwm_chy_handle, GTIM_TIMX_NPWM_CHY);/* ����TIMxͨ��y */

    
    HAL_TIM_PWM_Start(&g_timx_npwm_chy_handle, GTIM_TIMX_NPWM_CHY);       /* ����PWMͨ��y */
    
    HAL_NVIC_SetPriority(GTIM_TIMX_NPWM_IRQn, 1, 3);  /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_NPWM_IRQn);          /* ����ITMx�ж� */

    HAL_TIM_Base_Start_IT(&g_timx_npwm_chy_handle);   /* ʹ�ܶ�ʱ��x�Ͷ�ʱ��x�����ж� */
   
}

/**
 * @brief       ͨ�ö�ʱ�����ָ������PWMͨ�����ų�ʼ������
 * @param       htim:��ʱ�����
 * @note        �˺����ᱻgtim_timx_npwm_chy_init()����
 * @retval      ��
 */
void gtim_timx_npwm_chy_gpio_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    GTIM_TIMX_NPWM_CHY_GPIO_CLK_ENABLE();                    /* ����ͨ��y��CPIOʱ�� */

    gpio_init_struct.Pin = GTIM_TIMX_NPWM_CHY_GPIO_PIN;      /* ͨ��y��CPIO�� */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                 /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;      /* ���� */
    gpio_init_struct.Alternate = GTIM_TIMX_NPWM_CHY_GPIO_AF; /* ��ʱ��xͨ��y��CPIO�ڸ��� */
    HAL_GPIO_Init(GTIM_TIMX_NPWM_CHY_GPIO_PORT, &gpio_init_struct);
}

/**
 * @brief       ͨ�ö�ʱ��TIMX NPWM����PWM����
 * @param       rcr: PWM�ĸ���, 1~2^32�η���
 * @retval      ��
 */
void gtim_timx_npwm_chy_set(uint32_t npwm)
{
    if (npwm == 0)return ;
    g_npwm_remain = npwm;               /* ����������� */
    GTIM_TIMX_NPWM->EGR |= 1 << 0;      /* ����һ�θ����¼�,���ж����洦��������� */
    GTIM_TIMX_NPWM->CR1 |= 1 << 0;      /* ʹ�ܶ�ʱ��TIMX */
}

/**
 * @brief       ��ʱ���жϷ�����
 * @param       ��
 * @retval      ��
 */
void GTIM_TIMX_NPWM_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_npwm_chy_handle);
}

/**
 * @brief       ��ʱ�������жϻص�����
 * @param       htim:��ʱ�����ָ��
 * @note        �˺����ᱻ��ʱ���жϺ�����ͬ���õ�
 * @retval      ��
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&g_timx_handle))           /* ͨ�ö�ʱ���ж�ʵ��ص�ִ�е����� */
    {
        LED1_TOGGLE();
    }

    /**********************************���벶��ʵ�鴦�����********************************/
    if (htim == (&g_timx_cap_chy_handler))
    {
        if ((g_timxchy_cap_sta & 0X80) == 0)               /* ��δ�ɹ����� */
        {
            if (g_timxchy_cap_sta & 0X40)                  /* �Ѿ����񵽸ߵ�ƽ�� */
            {
                if ((g_timxchy_cap_sta & 0X3F) == 0X3F)    /* �ߵ�ƽ̫���� */
                {
                    g_timxchy_cap_sta |= 0X80;             /* ��ǳɹ�������һ�� */
                    g_timxchy_cap_val = 0XFFFF;
                }
                else
                {
                    g_timxchy_cap_sta++;
                }
            }
        }
    }

    /**********************************�������ʵ�鴦�����********************************/
    if (htim == (&g_timx_cnt_chy_handler))
    {
        g_timxchy_cnt_ofcnt++;          /* �ۼ�������� */
    }

    /**********************************���ָ������PWM�������********************************/
    if (htim == (&g_timx_npwm_chy_handle)) /* ͨ�ö�ʱ�����ָ������PWMʵ��ص�ִ�е����� */
    {
        uint16_t npwm = 0;
        
        if (g_npwm_remain > 256)        /* ���д���256��������Ҫ���� */
        {
            g_npwm_remain=g_npwm_remain - 256;
            npwm = 256;
        }
        else if (g_npwm_remain % 256)   /* ����λ��������256��������Ҫ���� */
        {
            npwm = g_npwm_remain % 256; 
            g_npwm_remain = 0;          /* û�������� */
        }
        if (npwm)   /* ������Ҫ���� */
        { 
            GTIM_TIMX_NPWM->RCR = npwm - 1; /* �����ظ������Ĵ���ֵΪnpwm-1, ��npwm������ */
            GTIM_TIMX_NPWM->EGR |= 1 << 0;  /* ����һ�θ����¼�,�Ը���RCR�Ĵ��� */
            GTIM_TIMX_NPWM->CR1 |= 1 << 0;  /* ʹ�ܶ�ʱ��TIMX */
        }
        else
        { 
            GTIM_TIMX_NPWM->CR1 &= ~(1 << 0);   /* �رն�ʱ��TIMX */
        }
    }
}




































