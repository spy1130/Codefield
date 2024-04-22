/**
 ****************************************************************************************************
 * @file        adc3.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-28
 * @brief       ADC3(�����ڲ��¶ȴ�����) ��������
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

#include "./BSP/ADC/adc3.h"
#include "./SYSTEM/delay/delay.h"


ADC_HandleTypeDef adc3_handle;  /* ADC��� */

/**
 * @brief       ADC3��ʼ������
 *   @note      ������ר����֧��ADC3, ��ADC1/2���ֿ���, ����ʹ��
 *              ����ʹ��16λ����, ADC����ʱ��=32M, ת��ʱ��Ϊ:�������� + 8.5��ADC����
 *              ��������������: 810.5, ��ת��ʱ�� = 819��ADC���� = 25.6us
 * @param       ��
 * @retval      ��
 */
void adc3_init(void)
{
    adc3_handle.Instance = ADC3;
    adc3_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;             /* 2��Ƶ��ADCCLK=PER_CK/2=64/2 = 32MHZ */
    adc3_handle.Init.Resolution = ADC_RESOLUTION_16B;                       /* 16λģʽ */
    adc3_handle.Init.ScanConvMode = DISABLE;                                /* ��ɨ��ģʽ */
    adc3_handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;                    /* �ر�EOC�ж� */
    adc3_handle.Init.LowPowerAutoWait = DISABLE;                            /* �Զ��͹��Ĺر� */
    adc3_handle.Init.ContinuousConvMode = DISABLE;                          /* �ر�����ת�� */
    adc3_handle.Init.NbrOfConversion = 1;                                   /* 1��ת���ڹ��������� Ҳ����ֻת����������1 */
    adc3_handle.Init.DiscontinuousConvMode = DISABLE;                       /* ��ֹ����������ģʽ */
    adc3_handle.Init.NbrOfDiscConversion = 0;                               /* ����������ͨ����Ϊ0 */
    adc3_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;                 /* ������� */
    adc3_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;  /* ʹ��������� */
    adc3_handle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;                    /* ���µ����ݵ�����ֱ�Ӹ��ǵ������� */
    adc3_handle.Init.OversamplingMode = DISABLE;                            /* �������ر� */
    adc3_handle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;      /* ����ͨ�������ݽ���������DR�Ĵ������� */
    HAL_ADC_Init(&adc3_handle);                                             /* ��ʼ�� */

    HAL_ADCEx_Calibration_Start(&adc3_handle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED); /* ADCУ׼ */
}

/**
 * @brief       ADC�ײ�����,ʱ��ʹ��
 * @param       hadc:ADC���
 * @note        �˺����ᱻHAL_ADC_Init()����
 * @retval      ��
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    RCC_PeriphCLKInitTypeDef rcc_periph_clk_handle;

    __HAL_RCC_ADC3_CLK_ENABLE();            /* ʹ��ADC3ʱ�� */

    rcc_periph_clk_handle.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    rcc_periph_clk_handle.AdcClockSelection = RCC_ADCCLKSOURCE_CLKP;
    HAL_RCCEx_PeriphCLKConfig(&rcc_periph_clk_handle);
}

/**
 * @brief       ���ADCת����Ľ��
 * @param       ch: ͨ����, ADC_CHANNEL_0~ADC_CHANNEL_19
 * @retval      ��
 */
uint32_t adc3_get_result(ADC_HandleTypeDef adc_handle, uint32_t ch)
{
    ADC_ChannelConfTypeDef adc_ch_conf;

    adc_ch_conf.Channel = ch;                              /* ͨ�� */
    adc_ch_conf.Rank = ADC_REGULAR_RANK_1;                 /* 1������ */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_810CYCLES_5; /* ����ʱ�� */
    adc_ch_conf.SingleDiff = ADC_SINGLE_ENDED;             /* ���߲ɼ� */
    adc_ch_conf.OffsetNumber = ADC_OFFSET_NONE;
    adc_ch_conf.Offset=0;   
    HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);      /* ͨ������ */

    HAL_ADC_Start(&adc_handle);                            /* ����ADC */

    HAL_ADC_PollForConversion(&adc_handle, 10);            /* ��ѯת�� */
    return HAL_ADC_GetValue(&adc_handle);  /* �������һ��ADC�������ת����� */
}

/**
 * @brief       ��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ��
 * @param       ch      : ͨ����, 0~19
 * @param       times   : ��ȡ����
 * @retval      ͨ��ch��times��ת�����ƽ��ֵ
 */
uint32_t adc3_get_result_average(ADC_HandleTypeDef adc_handle, uint32_t ch, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++)   /* ��ȡtimes������ */
    {
        temp_val += adc3_get_result(adc_handle, ch);
        delay_ms(5);
    }

    return temp_val / times;      /* ����ƽ��ֵ */
}

/**
 * @brief       ��ȡ�ڲ��¶ȴ������¶�ֵ
 * @param       ��
 * @retval      �¶�ֵ(������100��,��λ:��.)
 */
short adc3_get_temperature(void)
{
    uint32_t adcx;
    short result;
    double temperature;
    float temp = 0;
    uint16_t ts_cal1, ts_cal2;
    
    ts_cal1 = *(volatile uint16_t *)(0X1FF1E820);          /* ��ȡTS_CAL1 */
    ts_cal2 = *(volatile uint16_t *)(0X1FF1E840);          /* ��ȡTS_CAL2 */
    temp = (float)((110.0 - 30.0) / (ts_cal2 - ts_cal1));  /* ��ȡ�������� */

    adcx = adc3_get_result_average(adc3_handle, ADC3_TEMPSENSOR_CHX, 10); /* ��ȡ�ڲ��¶ȴ�����ͨ��,10��ȡƽ�� */
    temperature = (float)(temp * (adcx - ts_cal1) + 30);   /* �����¶� */

    result = temperature *= 100;                           /* ����100��. */
    return result;
}
















