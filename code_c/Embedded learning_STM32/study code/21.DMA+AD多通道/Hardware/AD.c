#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[4];

void AD_Init(void)
{	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//配置ADCCLK
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//RCC开启DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//GPIO口无效，防止GPIO对模拟电压造成干扰
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//菜单
	ADC_RegularChannelConfig(ADC1,  ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);//2.通道0；4.55.5个ADC采样周期
	ADC_RegularChannelConfig(ADC1,  ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);//2.通道0；4.55.5个ADC采样周期
	ADC_RegularChannelConfig(ADC1,  ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);//2.通道0；4.55.5个ADC采样周期
	ADC_RegularChannelConfig(ADC1,  ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);//2.通道0；4.55.5个ADC采样周期
	
	//初始化ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//不使用外部触发，使用软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续扫描
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描
	ADC_InitStructure.ADC_NbrOfChannel = 4;//4个菜
	ADC_Init(ADC1, &ADC_InitStructure);
	
		
	//配置DMA
	DMA_InitTypeDef DMA_InitStructure;
	//外设
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//起始地址ADC1
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度(16位）
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//地址否自增
	//存储器
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;//起始地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据宽度(字节）
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//地址自增
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向，外设作为数据源
	DMA_InitStructure.DMA_BufferSize = 4;//传输计数器
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//硬件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//配置到DMA1的通道1
	//使能DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);
	ADC_DMACmd(ADC1,ENABLE);//使能两者间的通道
	ADC_Cmd(ADC1, ENABLE);//开启
	//ADC校准
	ADC_ResetCalibration(ADC1);
	//检验过程
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);//看是不是一直是1，是1就一直校准
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);//是否检验完成
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//不断初始化
}
////启动转换，获取结果
//uint16_t AD_GetValue(void)
//{	DMA_Cmd(DMA1_Channel1, DISABLE);//赋值前必须先让DMA失能
//	DMA_SetCurrDataCounter(DMA1_Channel1,4);//给DMA重新赋值
//	DMA_Cmd(DMA1_Channel1, ENABLE);//使能DMA
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//不断初始化
//	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//判断是否转运完成
//	DMA_ClearFlag(DMA1_FLAG_TC1);//清除标志位
//}
//	
