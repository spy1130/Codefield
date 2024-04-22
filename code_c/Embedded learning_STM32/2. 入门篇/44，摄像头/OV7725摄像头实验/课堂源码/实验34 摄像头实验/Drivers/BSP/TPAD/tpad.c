/**
 ****************************************************************************************************
 * @file        tpad.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-21
 * @brief       ���ݴ�������(TPAD) ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200421
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/TPAD/tpad.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"

/******************************************************************************************/
/* ���ص�ʱ��(û���ְ���),��������Ҫ��ʱ��
 * ���ֵӦ����ÿ�ο�����ʱ�򱻳�ʼ��һ��
 */
volatile uint16_t g_tpad_default_val = 0;   /* ���ص�ʱ��(û���ְ���),��������Ҫ��ʱ�� */


/*��ʱ��������ز���*/
static TIM_HandleTypeDef g_timx_cap_chy_handler; /* ��ʱ��x��� */
static TIM_IC_InitTypeDef g_timx_ic_cap_chy_handler;


/**
 * @brief       ��ʼ����������
 * @param       psc     : ��Ƶϵ��(ֵԽС, Խ����, ��СֵΪ: 1)
 * @retval      0, ��ʼ���ɹ�; 1, ��ʼ��ʧ��;
 */
uint8_t tpad_init(uint16_t psc)
{
    uint16_t buf[10];
    uint16_t temp;
    uint8_t j, i;
    tpad_timx_cap_init(TPAD_ARR_MAX_VAL, psc - 1);  /* ��72 / (psc - 1) Mhz��Ƶ�ʼ��� */

    for (i = 0; i < 10; i++)    /* ������ȡ10�� */
    {
        buf[i] = tpad_get_val();
        delay_ms(10);
    }

    for (i = 0; i < 9; i++)     /* ���� */
    {
        for (j = i + 1; j < 10; j++)
        {
            if (buf[i] > buf[j])/* �������� */
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }

    temp = 0;

    for (i = 2; i < 8; i++)     /* ȡ�м��6�����ݽ���ƽ�� */
    {
        temp += buf[i];
    }
    
    g_tpad_default_val = temp / 6;
    printf("g_tpad_default_val:%d\r\n", g_tpad_default_val);

    if (g_tpad_default_val > TPAD_ARR_MAX_VAL / 2)
    {
        return 1;   /* ��ʼ����������TPAD_ARR_MAX_VAL/2����ֵ,������! */
    }

    return 0;
}

/**
 * @brief       ��λTPAD
 *   @note      ���ǽ�TPAD����������һ������, ����ָ����/������ʱ��ֵ�б仯
 *              �ú�����GPIO���ó��������, Ȼ�����0, ���зŵ�, Ȼ��������
 *              GPIOΪ��������, �ȴ��ⲿ������������
 * @param       ��
 * @retval      ��
 */
static void tpad_reset(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = TPAD_GPIO_PIN;                               /* ���벶���GPIO�� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;                        /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                                /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;                    /* ���� */
    HAL_GPIO_Init(TPAD_GPIO_PORT, &gpio_init_struct);

    HAL_GPIO_WritePin(TPAD_GPIO_PORT, TPAD_GPIO_PIN, GPIO_PIN_RESET);   /* TPAD�������0, �ŵ� */

    delay_ms(5);

    g_timx_cap_chy_handler.Instance->SR = 0;                            /* ������ */
    g_timx_cap_chy_handler.Instance->CNT = 0;                           /* ���� */

    gpio_init_struct.Pin = TPAD_GPIO_PIN;             /* ���벶���GPIO�� */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;          /* ����������� */
    gpio_init_struct.Pull = GPIO_NOPULL;              /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;  /* ���� */
    HAL_GPIO_Init(TPAD_GPIO_PORT, &gpio_init_struct); /* TPAD���Ÿ������� */
}

/**
 * @brief       �õ���ʱ������ֵ
 *   @note      �����ʱ, ��ֱ�ӷ��ض�ʱ���ļ���ֵ
 *              ���Ƕ��峬ʱʱ��Ϊ: TPAD_ARR_MAX_VAL - 500
 * @param       ��
 * @retval      ����ֵ/����ֵ����ʱ������·��أ�
 */
static uint16_t tpad_get_val(void)
{
    uint32_t flag = (TPAD_TIMX_CAP_CHY== TIM_CHANNEL_1)?TIM_FLAG_CC1:\
                    (TPAD_TIMX_CAP_CHY== TIM_CHANNEL_2)?TIM_FLAG_CC2:\
                    (TPAD_TIMX_CAP_CHY== TIM_CHANNEL_3)?TIM_FLAG_CC3:TIM_FLAG_CC4;
    
    tpad_reset();
    while (__HAL_TIM_GET_FLAG(&g_timx_cap_chy_handler ,flag) == RESET) /* �ȴ�ͨ��CHY���������� */
    {
        if (g_timx_cap_chy_handler.Instance->CNT > TPAD_ARR_MAX_VAL - 500)
        {
            return g_timx_cap_chy_handler.Instance->CNT;  /* ��ʱ��,ֱ�ӷ���CNT��ֵ */
        }
    }

    return TPAD_TIMX_CAP_CHY_CCRX;      /* ���ز���/�Ƚ�ֵ */
}

/**
 * @brief       ��ȡn��, ȡ���ֵ
 * @param       n       ��������ȡ�Ĵ���
 * @retval      n�ζ������������������ֵ
 */
static uint16_t tpad_get_maxval(uint8_t n)
{
    uint16_t temp = 0;
    uint16_t maxval = 0;

    while (n--)
    {
        temp = tpad_get_val();  /* �õ�һ��ֵ */

        if (temp > maxval)maxval = temp;
    }

    return maxval;
}

/**
 * @brief       ɨ�败������
 * @param       mode    ��ɨ��ģʽ
 *   @arg       0, ��֧����������(����һ�α����ɿ����ܰ���һ��);
 *   @arg       1, ֧����������(����һֱ����)
 * @retval      0, û�а���; 1, �а���;
 */
uint8_t tpad_scan(uint8_t mode)
{
    static uint8_t keyen = 0;   /* 0, ���Կ�ʼ���;  >0, �����ܿ�ʼ���; */
    uint8_t res = 0;
    uint8_t sample = 3;         /* Ĭ�ϲ�������Ϊ3�� */
    uint16_t rval;

    if (mode)
    {
        sample = 6;     /* ֧��������ʱ�����ò�������Ϊ6�� */
        keyen = 0;      /* ֧������, ÿ�ε��øú��������Լ�� */
    }

    rval = tpad_get_maxval(sample);

    if (rval > (g_tpad_default_val + TPAD_GATE_VAL))/* ����tpad_default_val+TPAD_GATE_VAL,��Ч */
    {
        if (keyen == 0)
        {
            res = 1;    /* keyen==0, ��Ч */
        }

        //printf("r:%d\r\n", rval);   /* �������ֵ, ���Ե�ʱ����õ� */
        keyen = 3;      /* ����Ҫ�ٹ�3��֮����ܰ�����Ч */
    }

    if (keyen)keyen--;

    return res;
}

/**
 * @brief       �����������벶������
 * @param       arr     ���Զ���װֵ
 * @param       psc     ��ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
static void tpad_timx_cap_init(uint16_t arr, uint16_t psc)
{
    TPAD_GPIO_CLK_ENABLE();                                             /* TPAD���� ʱ��ʹ�� */
    TPAD_TIMX_CAP_CHY_CLK_ENABLE();                                     /* ��ʱ�� ʱ��ʹ�� */

    GPIO_InitTypeDef gpio_init_struct;
    TPAD_GPIO_CLK_ENABLE();                                             /* TPAD���� ʱ��ʹ�� */
    TPAD_TIMX_CAP_CHY_CLK_ENABLE();                                     /* ��ʱ�� ʱ��ʹ�� */

    gpio_init_struct.Pin = TPAD_GPIO_PIN;                               /* ���벶���GPIO�� */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                            /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                              /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;                    /* ���� */
    HAL_GPIO_Init(TPAD_GPIO_PORT, &gpio_init_struct);                   /* TPAD���Ÿ������� */

    g_timx_cap_chy_handler.Instance = TPAD_TIMX_CAP;                    /* ��ʱ��5 */
    g_timx_cap_chy_handler.Init.Prescaler = psc;                        /* ��ʱ����Ƶ */
    g_timx_cap_chy_handler.Init.CounterMode = TIM_COUNTERMODE_UP;       /* ���ϼ���ģʽ */
    g_timx_cap_chy_handler.Init.Period = arr;                           /* �Զ���װ��ֵ */
    g_timx_cap_chy_handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; /* ʱ�ӷ�Ƶ���� */
    HAL_TIM_IC_Init(&g_timx_cap_chy_handler);

    g_timx_ic_cap_chy_handler.ICPolarity = TIM_ICPOLARITY_RISING;                                     /* �����ز��� */
    g_timx_ic_cap_chy_handler.ICSelection = TIM_ICSELECTION_DIRECTTI;                                 /* ӳ�䵽TI1�� */
    g_timx_ic_cap_chy_handler.ICPrescaler = TIM_ICPSC_DIV1;                                           /* ���������Ƶ������Ƶ */
    g_timx_ic_cap_chy_handler.ICFilter = 0;                                                           /* ���������˲��������˲� */
    HAL_TIM_IC_ConfigChannel(&g_timx_cap_chy_handler, &g_timx_ic_cap_chy_handler, TPAD_TIMX_CAP_CHY); /* ����TIM5ͨ��2 */
    HAL_TIM_IC_Start(&g_timx_cap_chy_handler,TPAD_TIMX_CAP_CHY);    /* ʹ�����벶��Ͷ�ʱ�� */

}



