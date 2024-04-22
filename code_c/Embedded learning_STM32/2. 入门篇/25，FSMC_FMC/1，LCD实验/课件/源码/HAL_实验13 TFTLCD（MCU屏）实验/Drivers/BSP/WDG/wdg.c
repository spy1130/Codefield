/**
 ****************************************************************************************************
 * @file        wdg.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.1
 * @date        2020-04-20
 * @brief       ���Ź� ��������
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
 * ��Ӵ��ڿ��Ź���ش���
 *
 ****************************************************************************************************
 */

#include "./BSP/WDG/wdg.h"
#include "./BSP/LED/led.h"

/**
 * @brief       ��ʼ���������Ź� 
 * @param       prer: ��Ƶ��:0~7(ֻ�е�3λ��Ч!)
 *   @arg       ��Ƶ���� = 4 * 2^prer. �����ֵֻ����256!
 * @param       rlr: �Զ���װ��ֵ,0~0XFFF. 
 * @note        ʱ�����(���):Tout=((4 * 2^prer) * rlr) / 40 (ms). 
 * @retval      ��
 */
void iwdg_init(uint8_t prer, uint16_t rlr)
{
    IWDG->KR = 0X5555;  /* ʹ�ܶ�IWDG->PR��IWDG->RLR��д */
    IWDG->PR = prer;    /* ���÷�Ƶϵ�� */
    IWDG->RLR = rlr;    /* �Ӽ��ؼĴ��� IWDG->RLR */
    IWDG->KR = 0XAAAA;  /* reload */
    IWDG->KR = 0XCCCC;  /* ʹ�ܿ��Ź� */
}

/**
 * @brief       ι�������Ź�
 * @param       ��
 * @retval      ��
 */
void iwdg_feed(void)
{
    IWDG->KR = 0XAAAA;  /* reload */
}

/* ����WWDG������������ֵ,Ĭ��Ϊ���. */
uint8_t g_wwdg_cnt = 0x7f;

/**
 * @brief       ��ʼ�����ڿ��Ź�
 * @param       tr: T[6:0],������ֵ
 * @param       tw: W[6:0],����ֵ
 * @note        fprer:��Ƶϵ����WDGTB��,��Χ:0~7,��ʾ2^WDGTB��Ƶ
 *              Fwwdg=PCLK1/(4096*2^fprer). һ��PCLK1=36Mhz
 *              ���ʱ��=(4096*2^fprer)*(tr-0X3F)/PCLK1
 *              ����fprer=4,tr=7f,PCLK1=36Mhz
 *              �����ʱ��=4096*16*64/36Mhz=116.51ms
 * @retval      ��
 */
void wwdg_init(uint8_t tr, uint8_t wr, uint8_t fprer)
{
    RCC->APB1ENR |= 1 << 11;        /* ʹ��wwdgʱ�� */
    g_wwdg_cnt = tr & g_wwdg_cnt;   /* ��ʼ��WWDG_CNT. */
    WWDG->CFR |= fprer << 7;        /* PCLK1/4096�ٳ�2^fprer */
    WWDG->CFR &= 0XFF80;
    WWDG->CFR |= wr;                /* �趨����ֵ */
    WWDG->CR |= g_wwdg_cnt;         /* �趨������ֵ */
    WWDG->CR |= 1 << 7;             /* �������Ź� */
    sys_nvic_init(2, 3, WWDG_IRQn, 2); /* ��ռ2�������ȼ�3����2 */
    WWDG->SR = 0X00;                /* �����ǰ�����жϱ�־λ */
    WWDG->CFR |= 1 << 9;            /* ʹ����ǰ�����ж� */
}

/**
 * @brief       ������WWDG��������ֵ
 * @param       ��
 * @retval      ��
 */
void wwdg_set_counter(uint8_t cnt)
{
    WWDG->CR = (cnt & 0x7F);   /* ������7λ������ */
}

/**
 * @brief       ���ڿ��Ź��жϷ������
 * @param       ��
 * @retval      ��
 */
void WWDG_IRQHandler(void)
{  
    if (WWDG->SR & 0X01)               /* ���ж��Ƿ�����WWDG��ǰ�����ж� */
    {
        WWDG->SR = 0X00;               /* �����ǰ�����жϱ�־λ */
        wwdg_set_counter(g_wwdg_cnt);   /* ���贰�ڿ��Ź���ֵ! */
        LED1_TOGGLE();                  /* �̵���˸ */
    }
}















