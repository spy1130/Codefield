#include "./BSP/TIMER/atim.h"


TIM_HandleTypeDef g_timx_comp_pwm_handle;       /* ��ʱ��x��� */

/* �߼���ʱ�� ����Ƚ�ģʽ ��ʼ������ */
void atim_timx_comp_pwm_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_comp_pwm = {0};
    
    g_timx_comp_pwm_handle.Instance = TIM8;                       /* ��ʱ��8 */
    g_timx_comp_pwm_handle.Init.Prescaler = psc  ;                /* ��ʱ����Ƶ */
    g_timx_comp_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP; /* ��������ģʽ */
    g_timx_comp_pwm_handle.Init.Period = arr;                     /* �Զ���װ��ֵ */
    HAL_TIM_OC_Init(&g_timx_comp_pwm_handle);                     /* ����Ƚ�ģʽ��ʼ�� */

    timx_oc_comp_pwm.OCMode = TIM_OCMODE_TOGGLE;
    timx_oc_comp_pwm.OCPolarity = TIM_OCPOLARITY_HIGH;
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_1);
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_2);
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_3);
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_4);
    
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_1);
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_2);
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_3);
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_4);
    
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_1);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_2);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_3);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_4);
}

/* ��ʱ�� ����Ƚ� MSP��ʼ������ */
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM8)
    {
        GPIO_InitTypeDef gpio_init_struct;

        __HAL_RCC_TIM8_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_6;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_NOPULL;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOC, &gpio_init_struct);

        gpio_init_struct.Pin = GPIO_PIN_7;
        HAL_GPIO_Init(GPIOC, &gpio_init_struct);

        gpio_init_struct.Pin = GPIO_PIN_8;
        HAL_GPIO_Init(GPIOC, &gpio_init_struct);

        gpio_init_struct.Pin = GPIO_PIN_9;
        HAL_GPIO_Init(GPIOC, &gpio_init_struct);
    }
}






