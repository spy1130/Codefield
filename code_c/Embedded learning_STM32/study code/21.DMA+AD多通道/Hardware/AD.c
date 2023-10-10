#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[4];

void AD_Init(void)
{	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//����ADCCLK
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//RCC����DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//GPIO����Ч����ֹGPIO��ģ���ѹ��ɸ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//�˵�
	ADC_RegularChannelConfig(ADC1,  ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);//2.ͨ��0��4.55.5��ADC��������
	ADC_RegularChannelConfig(ADC1,  ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);//2.ͨ��0��4.55.5��ADC��������
	ADC_RegularChannelConfig(ADC1,  ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);//2.ͨ��0��4.55.5��ADC��������
	ADC_RegularChannelConfig(ADC1,  ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);//2.ͨ��0��4.55.5��ADC��������
	
	//��ʼ��ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//��ʹ���ⲿ������ʹ���������
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ɨ��
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��
	ADC_InitStructure.ADC_NbrOfChannel = 4;//4����
	ADC_Init(ADC1, &ADC_InitStructure);
	
		
	//����DMA
	DMA_InitTypeDef DMA_InitStructure;
	//����
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//��ʼ��ַADC1
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ��(16λ��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//��ַ������
	//�洢��
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;//��ʼ��ַ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���ݿ��(�ֽڣ�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//��ַ����
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//���䷽��������Ϊ����Դ
	DMA_InitStructure.DMA_BufferSize = 4;//���������
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//Ӳ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//���ȼ�
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//���õ�DMA1��ͨ��1
	//ʹ��DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);
	ADC_DMACmd(ADC1,ENABLE);//ʹ�����߼��ͨ��
	ADC_Cmd(ADC1, ENABLE);//����
	//ADCУ׼
	ADC_ResetCalibration(ADC1);
	//�������
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);//���ǲ���һֱ��1����1��һֱУ׼
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);//�Ƿ�������
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//���ϳ�ʼ��
}
////����ת������ȡ���
//uint16_t AD_GetValue(void)
//{	DMA_Cmd(DMA1_Channel1, DISABLE);//��ֵǰ��������DMAʧ��
//	DMA_SetCurrDataCounter(DMA1_Channel1,4);//��DMA���¸�ֵ
//	DMA_Cmd(DMA1_Channel1, ENABLE);//ʹ��DMA
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//���ϳ�ʼ��
//	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//�ж��Ƿ�ת�����
//	DMA_ClearFlag(DMA1_FLAG_TC1);//�����־λ
//}
//	
