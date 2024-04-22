/**
 ****************************************************************************************************
 * @file        gtim.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.3
 * @date        2020-04-20
 * @brief       ͨ�ö�ʱ�� ��������
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
 * V1.0 20200420
 * ��һ�η���
 * V1.1 20200420
 * ����gtim_timx_pwm_chy_init����
 * V1.2 20200420
 * 1,����gtim_timx_cap_chy_init����
 * V1.3 20200420
 * 1,֧���ⲿ�����������
 * 2,����gtim_timx_cnt_chy_init,gtim_timx_cnt_chy_get_count��gtim_timx_cnt_chy_restart��������  
 *
 ****************************************************************************************************
 */

#include "./BSP/TIMER/gtim.h"
#include "./BSP/LED/led.h"

/**
 * @brief       ͨ�ö�ʱ��TIMX�жϷ�����
 * @param       ��
 * @retval      ��
 */
void GTIM_TIMX_INT_IRQHandler(void)
{ 
    if (GTIM_TIMX_INT->SR & 0X0001)   /* ����ж� */
    {
        LED1_TOGGLE();
    }

    GTIM_TIMX_INT->SR &= ~(1 << 0); /* ����жϱ�־λ */
} 

/**
 * @brief       ͨ�ö�ʱ��TIMX��ʱ�жϳ�ʼ������
 * @note
 *              ͨ�ö�ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ͨ�ö�ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ36M, ���Զ�ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void gtim_timx_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX_INT_CLK_ENABLE();
    GTIM_TIMX_INT->ARR = arr;           /* �趨�������Զ���װֵ */
    GTIM_TIMX_INT->PSC = psc;           /* ����Ԥ��Ƶ��  */
    GTIM_TIMX_INT->DIER |= 1 << 0;      /* ��������ж� */
    GTIM_TIMX_INT->CR1 |= 1 << 0;       /* ʹ�ܶ�ʱ��TIMX */
    sys_nvic_init(1, 3, GTIM_TIMX_INT_IRQn, 2); /* ��ռ1�������ȼ�3����2 */
}

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y PWM��� ��ʼ��������ʹ��PWMģʽ1��
 * @note
 *              ͨ�ö�ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ͨ�ö�ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ36M, ���Զ�ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void gtim_timx_pwm_chy_init(uint16_t arr, uint16_t psc)
{
    uint8_t chy = GTIM_TIMX_PWM_CHY;
    GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE();    /* TIMX ͨ�� IO��ʱ��ʹ�� */
    GTIM_TIMX_PWM_CHY_CLK_ENABLE();         /* TIMX ʱ��ʹ�� */

    sys_gpio_set(GTIM_TIMX_PWM_CHY_GPIO_PORT, GTIM_TIMX_PWM_CHY_GPIO_PIN,
                 SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);    /* TIMX PWM CHY ����ģʽ���� */

    GTIM_TIMX_PWM_CHY_GPIO_REMAP();         /* IO��REMAP����, �Ǳ���! */

    GTIM_TIMX_PWM->ARR = arr;       /* �趨�������Զ���װֵ */
    GTIM_TIMX_PWM->PSC = psc;       /* ����Ԥ��Ƶ��  */
    GTIM_TIMX_PWM->BDTR |= 1 << 15; /* ʹ��MOEλ(��TIM1/8 �д˼Ĵ���,��������MOE�������PWM), ����ͨ�ö�ʱ��, ���
                                     * �Ĵ�������Ч��, ��������/�����ò���Ӱ����, Ϊ�˼�������ͳһ�ĳ�����MOEλ
                                     */

    if (chy <= 2)
    {
        GTIM_TIMX_PWM->CCMR1 |= 6 << (4 + 8 * (chy - 1));   /* CH1/2 PWMģʽ1 */
        GTIM_TIMX_PWM->CCMR1 |= 1 << (3 + 8 * (chy - 1));   /* CH1/2 Ԥװ��ʹ�� */
    }
    else if (chy <= 4)
    {
        GTIM_TIMX_PWM->CCMR2 |= 6 << (4 + 8 * (chy - 3));   /* CH3/4 PWMģʽ1 */
        GTIM_TIMX_PWM->CCMR2 |= 1 << (3 + 8 * (chy - 3));   /* CH3/4 Ԥװ��ʹ�� */
    }

    GTIM_TIMX_PWM->CCER |= 1 << (4 * (chy - 1));        /* OCy ���ʹ�� */
    GTIM_TIMX_PWM->CCER |= 1 << (1 + 4 * (chy - 1));    /* OCy �͵�ƽ��Ч */
    GTIM_TIMX_PWM->CR1 |= 1 << 7;   /* ARPEʹ�� */
    GTIM_TIMX_PWM->CR1 |= 1 << 0;   /* ʹ�ܶ�ʱ��TIMX */
}

/* ���벶��״̬(g_timxchy_cap_sta)
 * [7]  :0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
 * [6]  :0,��û���񵽸ߵ�ƽ;1,�Ѿ����񵽸ߵ�ƽ��.
 * [5:0]:����ߵ�ƽ������Ĵ���,������63��,���������ֵ = 63*65536 + 65535 = 4194303
 *       ע��:Ϊ��ͨ��,����Ĭ��ARR��CCRy����16λ�Ĵ���,����32λ�Ķ�ʱ��(��:TIM5),Ҳֻ��16λʹ��
 *       ��1us�ļ���Ƶ��,����ʱ��Ϊ:4194303 us, Լ4.19��
 */
uint8_t g_timxchy_cap_sta = 0;  /* ���벶��״̬ */
uint32_t g_timxchy_cap_val =0 ; /* ���벶��ֵ */

/**
 * @brief       ͨ�ö�ʱ��TIMX ���벶�� �жϷ�����
 * @param       ��
 * @retval      ��
 */
void GTIM_TIMX_CAP_IRQHandler(void)
{
    uint16_t tsr = GTIM_TIMX_CAP->SR;       /* ��ȡ�ж�״̬ */
    uint8_t chy = GTIM_TIMX_CAP_CHY;        /* ��Ҫ�����ͨ�� */

    if ((g_timxchy_cap_sta & 0X80) == 0)    /* ��δ�ɹ����� */
    {
        if (tsr & (1 << 0)) /* ����ж� */
        {
            if (g_timxchy_cap_sta & 0X40)   /* �Ѿ����񵽸ߵ�ƽ�� */
            {
                if ((g_timxchy_cap_sta & 0X3F) == 0X3F) /* �ߵ�ƽ̫���� */
                {
                    g_timxchy_cap_sta |= 0X80;  /* ��ǳɹ�������һ�� */
                    g_timxchy_cap_val = 0XFFFF;
                }
                else    /* �������ۼӸߵ�ƽ���� */
                {
                    g_timxchy_cap_sta++;   
                }
            }
        }

        if (tsr & (1 << chy))   /* ͨ��y �����˲����¼� */
        {
            if (g_timxchy_cap_sta & 0X40)   /* ����һ���½��� */
            {
                g_timxchy_cap_sta |= 0X80;  /* ��ǳɹ�����һ�θߵ�ƽ���� */
                g_timxchy_cap_val = GTIM_TIMX_CAP_CHY_CCRX; /* ��ȡ��ǰ�Ĳ���ֵ. */
                
                GTIM_TIMX_CAP->CCER &= ~(1 << (1 + 4 * (chy - 1)));/* CCyP = 0 ����Ϊ�����ز��� */ 
            }
            else    /* ��δ��ʼ,��һ�β��������� */
            {
                g_timxchy_cap_val = 0;
                g_timxchy_cap_sta = 0X40;   /* ��ǲ����������� */
                GTIM_TIMX_CAP->CNT = 0;     /* ��������� */ 
                GTIM_TIMX_CAP->CCER |= 1 << (1 + 4 * (chy - 1));    /* CCyP = 1 ����Ϊ�½��ز��� */ 
            }
        }
    }

    GTIM_TIMX_CAP->SR = 0;     /* ��������жϱ�־λ */
}

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y ���벶�� ��ʼ������
 * @note
 *              ͨ�ö�ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ͨ�ö�ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ36M, ���Զ�ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void gtim_timx_cap_chy_init(uint16_t arr, uint16_t psc)
{
    uint8_t chy = GTIM_TIMX_CAP_CHY;
    GTIM_TIMX_CAP_CHY_GPIO_CLK_ENABLE();   /* TIMX ͨ��IO��ʱ��ʹ�� */
    GTIM_TIMX_CAP_CHY_CLK_ENABLE();        /* TIMX ʱ��ʹ�� */

    sys_gpio_set(GTIM_TIMX_CAP_CHY_GPIO_PORT, GTIM_TIMX_CAP_CHY_GPIO_PIN,
                 SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PD);    /* TIMX PWM CHY ���ù��� ���� */

    GTIM_TIMX_CAP->ARR = arr;       /* �趨�������Զ���װֵ */
    GTIM_TIMX_CAP->PSC = psc;       /* ����Ԥ��Ƶ��  */

    if (chy <= 2)
    {
        GTIM_TIMX_CAP->CCMR1 |= 1 << 8 * (chy - 1);        /* CCyS[1:0]   = 01 ѡ������� IC1/2ӳ�䵽TI1/2�� */
        GTIM_TIMX_CAP->CCMR1 |= 0 << (2 + 8 * (chy - 1));  /* ICyPSC[1:0] = 00 ���벶�񲻷�Ƶ,ȫ���� */
        GTIM_TIMX_CAP->CCMR1 |= 0 << (4 + 8 * (chy - 1));  /* ICyF[3:0]   = 00 ������˲� ���˲� */
    }
    else if (chy <= 4)
    {
        GTIM_TIMX_CAP->CCMR2 |= 1 << 8 * (chy - 3);        /* CCyS[1:0]   = 01 ѡ������� IC3/4ӳ�䵽TI3/4�� */
        GTIM_TIMX_CAP->CCMR2 |= 0 << (2 + 8 * (chy - 3));  /* ICyPSC[1:0] = 00 ���벶�񲻷�Ƶ,ȫ���� */
        GTIM_TIMX_CAP->CCMR2 |= 0 << (4 + 8 * (chy - 3));  /* ICyF[3:0]   = 00 ������˲� ���˲� */
    }

    GTIM_TIMX_CAP->CCER |= 1 << (4 * (chy - 1));       /* CCyE = 1 ���벶��ʹ�� */
    GTIM_TIMX_CAP->CCER |= 0 << (1 + 4 * (chy - 1));   /* CCyP = 0 ���������� ,ע��:CCyNPʹ��Ĭ��ֵ0 */

    GTIM_TIMX_CAP->EGR  |= 1 << 0;  /* ������Ʋ��������¼�,ʹд��PSC��ֵ������Ч,���򽫻�Ҫ�ȵ���ʱ������Ż���Ч */
    GTIM_TIMX_CAP->DIER |= 1 << 1;  /* �������ж� */
    GTIM_TIMX_CAP->DIER |= 1 << 0;  /* ��������ж� */
    GTIM_TIMX_CAP->CR1  |= 1 << 0;  /* ʹ�ܶ�ʱ��TIMX */

    sys_nvic_init(1, 3, GTIM_TIMX_CAP_IRQn, 2);/* ��ռ1�������ȼ�3����2 */
}

/* ��¼��ʱ�����������������, ���������������� */
uint32_t g_timxchy_cnt_ofcnt = 0 ;  /* ����������� */

/**
 * @brief       ͨ�ö�ʱ��TIMX ������� �����жϷ�����
 * @param       ��
 * @retval      ��
 */
void GTIM_TIMX_CNT_IRQHandler(void)
{ 
    if (GTIM_TIMX_CNT->SR & (1 << 0))   /* ����ж� */
    {
        g_timxchy_cnt_ofcnt++;          /* �ۼ�������� */
    } 
    GTIM_TIMX_CNT->SR = 0;              /* ��������жϱ�־λ */
}

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
    uint8_t chy = GTIM_TIMX_CNT_CHY;
    GTIM_TIMX_CNT_CHY_GPIO_CLK_ENABLE();   /* TIMX ͨ��IO��ʱ��ʹ�� */
    GTIM_TIMX_CNT_CHY_CLK_ENABLE();        /* TIMX ʱ��ʹ�� */

    sys_gpio_set(GTIM_TIMX_CNT_CHY_GPIO_PORT, GTIM_TIMX_CNT_CHY_GPIO_PIN,
                 SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PD);    /* TIMX PWM CHY ���ù��� ���� */

    GTIM_TIMX_CNT->ARR = 65535; /* �趨�������Զ���װֵΪ��� */
    GTIM_TIMX_CNT->PSC = psc;   /* ����Ԥ��Ƶ��  */

    GTIM_TIMX_CNT->CCMR1 |= 1 << 8 * (chy - 1);        /* CCyS[1:0]   = 01 ѡ������� IC1/2ӳ�䵽TI1/2�� */
    GTIM_TIMX_CNT->CCMR1 |= 0 << (2 + 8 * (chy - 1));  /* ICyPSC[1:0] = 00 ���벶�񲻷�Ƶ,ȫ���� */
    GTIM_TIMX_CNT->CCMR1 |= 0 << (4 + 8 * (chy - 1));  /* ICyF[3:0]   = 00 ������˲� ���˲� */
    
    GTIM_TIMX_CNT->CCER |= 1 << (4 * (chy - 1));       /* CCyE = 1 ���벶��ʹ�� */
    GTIM_TIMX_CNT->CCER |= 0 << (1 + 4 * (chy - 1));   /* CCyP = 0 ����������,�������ؼ��� ,ע��:CCyNPʹ��Ĭ��ֵ0 */

    GTIM_TIMX_CNT->SMCR |= (4 + chy) << 4;  /* TS[4:0] = 5/6  ����ѡ��: 5,TI1FP1(TIMX_CH1);  6,TI2FP2(TIMX_CH2); */
    GTIM_TIMX_CNT->SMCR |= 7 << 0;          /* SMS[2:0] = 7   ��ģʽѡ��: �ⲿʱ��ģʽ1 */

    
    GTIM_TIMX_CNT->EGR  |= 1 << 0;  /* ������Ʋ��������¼�,ʹд��PSC��ֵ������Ч,���򽫻�Ҫ�ȵ���ʱ������Ż���Ч */
    GTIM_TIMX_CNT->DIER |= 1 << 0;  /* ��������ж� */
    GTIM_TIMX_CNT->CR1  |= 1 << 0;  /* ʹ�ܶ�ʱ��TIMX */

    sys_nvic_init(1, 3, GTIM_TIMX_CNT_IRQn, 2);/* ��ռ1�������ȼ�3����2 */
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
    count += GTIM_TIMX_CNT->CNT;        /* ���ϵ�ǰCNT��ֵ */
    return count;
}

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y ����������
 * @param       ��
 * @retval      ��ǰ����ֵ
 */
void gtim_timx_cnt_chy_restart(void)
{
    GTIM_TIMX_CNT->CR1  &= (1 << 0);    /* �رն�ʱ��TIMX */
    g_timxchy_cnt_ofcnt = 0;            /* �ۼ������� */
    GTIM_TIMX_CNT->CNT = 0;             /* ���������� */
    GTIM_TIMX_CNT->CR1  |= 1 << 0;      /* ʹ�ܶ�ʱ��TIMX */
}















