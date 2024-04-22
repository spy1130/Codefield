/**
 ****************************************************************************************************
 * @file        lsens.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-28
 * @brief       ���������� ��������
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
 * V1.0 20200328
 * ��һ�η���
 ****************************************************************************************************
 */

#include "./BSP/LSENS/lsens.h"
#include "./SYSTEM/delay/delay.h"


extern ADC_HandleTypeDef  adc3_handle; /* ADC��� */

/**
 * @brief       ��ʼ������������
 * @param       ��
 * @retval      ��
 */
void lsens_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_GPIOC_CLK_ENABLE();                   /* ����GPIOCʱ�� */

    gpio_init_struct.Pin = LSENS_ADC3_CHX_GPIO_PIN; /* PC1 */
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;       /* ģ�� */
    gpio_init_struct.Pull = GPIO_NOPULL;            /* ���������� */
    HAL_GPIO_Init(LSENS_ADC3_CHX_GPIO_PORT, &gpio_init_struct);
    
    adc3_init();    /* ��ʼ��ADC3 */
}

/**
 * @brief       ��ȡ����������ֵ
 * @param       ��
 * @retval      0~100:0,�;100,����
 */
uint8_t lsens_get_val(void)
{
    uint32_t temp_val = 0;
    temp_val = adc3_get_result_average(adc3_handle, LSENS_ADC3_CHX, 10); /* ��ȡƽ��ֵ */
    temp_val /= 655;

    if (temp_val > 100)temp_val = 100;

    return (uint8_t)(100 - temp_val);
}












