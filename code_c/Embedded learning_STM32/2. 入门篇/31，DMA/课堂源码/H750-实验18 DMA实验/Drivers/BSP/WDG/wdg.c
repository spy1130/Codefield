/**
 ****************************************************************************************************
 * @file        wdg.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.1
 * @date        2020-03-13
 * @brief       ���Ź� ��������
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
 * V1.0 20200312
 * ��һ�η���
 * V1.1 20200312
 * ����wwdg_init����
 *
 ****************************************************************************************************
 */

#include "./BSP/WDG/wdg.h"
#include "./BSP/LED/led.h"


IWDG_HandleTypeDef g_iwdg_handle; /* �������Ź���� */

/**
 * @brief       ��ʼ���������Ź� 
 * @param       prer: ��Ƶ��:0~7(ֻ�е�3λ��Ч!)
 *   @arg       ��Ƶ���� = 4 * 2^prer. �����ֵֻ����256!
 * @param       rlr: �Զ���װ��ֵ,0~0XFFF. 
 * @note        ʱ�����(���):Tout=((4 * 2^prer) * rlr) / 32 (ms). 
 * @retval      ��
 */
void iwdg_init(uint8_t prer, uint16_t rlr)
{
    g_iwdg_handle.Instance = IWDG1;
    g_iwdg_handle.Init.Prescaler = prer;               /* ����IWDG��Ƶϵ�� */
    g_iwdg_handle.Init.Reload = rlr;                   /* ��װ�� */
    g_iwdg_handle.Init.Window = IWDG_WINDOW_DISABLE;   /* �رմ��ڹ��� */
    HAL_IWDG_Init(&g_iwdg_handle);    
}
    
/**
 * @brief       ι�������Ź�
 * @param       ��
 * @retval      ��
 */
void iwdg_feed(void)
{   
    HAL_IWDG_Refresh(&g_iwdg_handle); /* ��װ�� */
}


/************************�����Ǵ��ڿ��Ź�����********************************/

WWDG_HandleTypeDef g_wwdg_handle;     //���ڿ��Ź����

/**
 * @brief       ��ʼ�����ڿ��Ź�
 * @param       tr: T[6:0],������ֵ
 * @param       tw: W[6:0],����ֵ
 * @note        fprer:��Ƶϵ����WDGTB��,��Χ:0~7,��ʾ2^WDGTB��Ƶ
 *              Fwwdg=PCLK3/(4096*2^fprer). һ��PCLK3=120Mhz
 *              ���ʱ��=(4096*2^fprer)*(tr-0X3F)/PCLK3
 *              ����fprer=4,tr=7f,PCLK3=120Mhz
 *              �����ʱ��=4096*16*64/120Mhz=34.95ms
 * @retval      ��
 */
void wwdg_init(uint8_t tr, uint8_t wr, uint32_t fprer)
{
    g_wwdg_handle.Instance = WWDG1;
    g_wwdg_handle.Init.Prescaler = fprer;  /* ���÷�Ƶϵ�� */
    g_wwdg_handle.Init.Window = wr;        /* ���ô���ֵ */
    g_wwdg_handle.Init.Counter = tr;       /* ���ü�����ֵ */
    g_wwdg_handle.Init.EWIMode = WWDG_EWI_ENABLE;/* ʹ�ܴ��ڿ��Ź���ǰ�����ж� */
    HAL_WWDG_Init(&g_wwdg_handle);        /* ��ʼ��WWDG */
}

/**
 * @brief       WWDG�ײ�����
 * @param       hwwdg:���ڿ��Ź����
 * @note        �˺����ᱻHAL_WWDG_Init()����
 * @retval      ��
 */
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg)
{
    __HAL_RCC_WWDG1_CLK_ENABLE();            /* ʹ�ܴ��ڿ��Ź�ʱ�� */
     
    HAL_NVIC_SetPriority(WWDG_IRQn,2,3);     /* ��ռ���ȼ�2�������ȼ�Ϊ3 */
    HAL_NVIC_EnableIRQ(WWDG_IRQn);           /* ʹ�ܴ��ڿ��Ź��ж� */
}

/**
 * @brief       ���ڿ��Ź��жϷ������
 * @param       ��
 * @retval      ��
 */
void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&g_wwdg_handle);
}

/**
 * @brief       ���ڿ��Ź��жϷ���ص�����
 * @param       ��
 * @note        �˺����ᱻHAL_WWDG_IRQHandler()����
 * @retval      ��
 */
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
    HAL_WWDG_Refresh(&g_wwdg_handle);/* ���´��ڿ��Ź�ֵ */
    LED1_TOGGLE();                   /* �̵���˸ */
}



















