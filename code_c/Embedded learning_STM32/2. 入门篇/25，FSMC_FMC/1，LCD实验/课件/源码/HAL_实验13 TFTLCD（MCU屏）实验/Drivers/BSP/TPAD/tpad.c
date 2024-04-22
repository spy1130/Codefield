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


/* ���ص�ʱ��(û���ְ���),��������Ҫ��ʱ��
 * ���ֵӦ����ÿ�ο�����ʱ�򱻳�ʼ��һ��
 */
volatile uint16_t g_tpad_default_val = 0;   /* ���ص�ʱ��(û���ְ���),��������Ҫ��ʱ�� */

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
    sys_gpio_set(TPAD_GPIO_PORT, TPAD_GPIO_PIN,
                 SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_MID, SYS_GPIO_PUPD_PU);   /* TPAD����������� */

    sys_gpio_pin_set(TPAD_GPIO_PORT, TPAD_GPIO_PIN, 0); /* TPAD�������0, �ŵ� */
    
    delay_ms(5);
    
    TPAD_TIMX_CAP->SR = 0;      /* ������ */
    TPAD_TIMX_CAP->CNT = 0;     /* ���� */

    sys_gpio_set(TPAD_GPIO_PORT, TPAD_GPIO_PIN,
                 SYS_GPIO_MODE_IN, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_MID, SYS_GPIO_PUPD_NONE);  /* TPAD���Ÿ������� */
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
    tpad_reset();

    while ((TPAD_TIMX_CAP->SR & (1 << TPAD_TIMX_CAP_CHY)) == 0) /* �ȴ�ͨ��CHY���������� */
    {
        if (TPAD_TIMX_CAP->CNT > TPAD_ARR_MAX_VAL - 500)
        {
            return TPAD_TIMX_CAP->CNT;  /* ��ʱ��,ֱ�ӷ���CNT��ֵ */
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
 * @brief       ��ʱ�����벶������
 * @param       arr     ���Զ���װֵ
 * @param       psc     ��ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
static void tpad_timx_cap_init(uint16_t arr, uint16_t psc)
{
    uint8_t chy = TPAD_TIMX_CAP_CHY;
    TPAD_GPIO_CLK_ENABLE();         /* TPAD���� ʱ��ʹ�� */
    TPAD_TIMX_CAP_CHY_CLK_ENABLE(); /* ��ʱ�� ʱ��ʹ�� */

    sys_gpio_set(TPAD_GPIO_PORT, TPAD_GPIO_PIN,
                 SYS_GPIO_MODE_IN, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_MID, SYS_GPIO_PUPD_NONE);  /* TPAD���Ÿ������� */

    TPAD_TIMX_CAP->ARR = arr;       /* �趨�������Զ���װֵ */
    TPAD_TIMX_CAP->PSC = psc;       /* ����Ԥ��Ƶ�� */

    if (chy <= 2)
    {
        TPAD_TIMX_CAP->CCMR1 |= 1 << 8 * (chy - 1);         /* CCyS[1:0]   = 01 ѡ������� IC1/2ӳ�䵽TI1/2�� */
        TPAD_TIMX_CAP->CCMR1 |= 0 << (2 + 8 * (chy - 1));   /* ICyPSC[1:0] = 00 ���벶�񲻷�Ƶ,ȫ���� */
        TPAD_TIMX_CAP->CCMR1 |= 0 << (4 + 8 * (chy - 1));   /* ICyF[3:0]   = 00 ������˲� ���˲� */
    }
    else if (chy <= 4)
    {
        TPAD_TIMX_CAP->CCMR2 |= 1 << 8 * (chy - 3);         /* CCyS[1:0]   = 01 ѡ������� IC3/4ӳ�䵽TI3/4�� */
        TPAD_TIMX_CAP->CCMR2 |= 0 << (2 + 8 * (chy - 3));   /* ICyPSC[1:0] = 00 ���벶�񲻷�Ƶ,ȫ���� */
        TPAD_TIMX_CAP->CCMR2 |= 0 << (4 + 8 * (chy - 3));   /* ICyF[3:0]   = 00 ������˲� ���˲� */
    }

    TPAD_TIMX_CAP->CCER |= 1 << (4 * (chy - 1));        /* CCyE = 1 ���벶��ʹ�� */
    TPAD_TIMX_CAP->CCER |= 0 << (1 + 4 * (chy - 1));    /* CCyP = 0 ���������� ,ע��:CCyNPʹ��Ĭ��ֵ0 */
    
    TPAD_TIMX_CAP->CR1  |= 1 << 0;  /* ʹ�ܶ�ʱ��TIMX */
}













