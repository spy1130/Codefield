#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//����ADCCLK
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//GPIO����Ч����ֹGPIO��ģ���ѹ��ɸ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//��ʼ��ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//��ʹ���ⲿ������ʹ���������
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//����
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);//����
	//ADCУ׼
	ADC_ResetCalibration(ADC1);
	//�������
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);//���ǲ���һֱ��1����1��һֱУ׼
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);//�Ƿ�������
}
//����ת������ȡ���
uint16_t AD_GetValue(uint8_t ADC_Channel)
{	ADC_RegularChannelConfig(ADC1,  ADC_Channel, 1, ADC_SampleTime_55Cycles5);//2.ͨ��0��4.55.5��ADC��������
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//���ϳ�ʼ��
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//�������Ƿ�ת�����
	return ADC_GetConversionValue(ADC1);//ȡ���
}
