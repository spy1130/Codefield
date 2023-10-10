#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//配置ADCCLK
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//GPIO口无效，防止GPIO对模拟电压造成干扰
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);//2.通道0；4.55.5个ADC采样周期
	//初始化ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//不使用外部触发，使用软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//单次
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);//开启
	//ADC校准
	ADC_ResetCalibration(ADC1);
	//检验过程
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);//看是不是一直是1，是1就一直校准
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);//是否检验完成
}
//启动转换，获取结果
uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//不断初始化
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//规则组是否转换完成
	return ADC_GetConversionValue(ADC1);//取结果
}
