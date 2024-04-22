/**
 ****************************************************************************************************
 * @file        rng.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-25
 * @brief       RNG(�����������) ��������
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
 * V1.0 20200325
 * ��һ�η���
 *
 ****************************************************************************************************
 */
 
#include "./BSP/RNG/rng.h"
#include "./SYSTEM/delay/delay.h"


RNG_HandleTypeDef rng_handle;  //RNG���

/**
 * @brief       ��ʼ��RNG
 * @param       ��
 * @retval      0,�ɹ�;1,ʧ��
 */
uint8_t rng_init(void)
{
    uint16_t retry = 0;

    rng_handle.Instance = RNG;   
    HAL_RNG_DeInit(&rng_handle);
    HAL_RNG_Init(&rng_handle);    /* ��ʼ��RNG */
    while (__HAL_RNG_GET_FLAG(&rng_handle, RNG_FLAG_DRDY) == RESET && retry < 10000) /* �ȴ�RNG׼������ */
    {
        retry++;
        delay_us(10);
    }
    if (retry >= 10000)
    {
        return 1; /* ��������������������� */
    }
    return 0;
}

/**
 * @brief       RNG�ײ�������ʱ��Դ���ú�ʹ��
 * @note        �˺����ᱻHAL_RNG_Init()����
 * @param       hrng:RNG���
 * @retval      ��
 */
void HAL_RNG_MspInit(RNG_HandleTypeDef *hrng)
{
    RCC_PeriphCLKInitTypeDef RNGClkInitStruct;

    /* ����RNGʱ��Դ��ѡ��PLL��ʱ��Ϊ480MHz */
    RNGClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;    /* ����RNGʱ��Դ */
    RNGClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_PLL;    /* RNGʱ��Դѡ��PLL */
    HAL_RCCEx_PeriphCLKConfig(&RNGClkInitStruct);

    __HAL_RCC_RNG_CLK_ENABLE();/* ʹ��RNGʱ�� */
}

/**
 * @brief       �õ������
 * @param       ��
 * @retval      ��ȡ���������(32bit)
 */
uint32_t rng_get_random_num(void)
{
    uint32_t randomnum;
    HAL_RNG_GenerateRandomNumber(&rng_handle, &randomnum);
    return randomnum;
}

/**
 * @brief       �õ�ĳ����Χ�ڵ������
 * @param       min,max: ��С,���ֵ.
 * @retval      �õ��������(rval),����:min<=rval<=max
 */
int rng_get_random_range(int min, int max)
{ 
    uint32_t randomnum;
    HAL_RNG_GenerateRandomNumber(&rng_handle, &randomnum);
    return randomnum%(max-min+1) + min;
}




