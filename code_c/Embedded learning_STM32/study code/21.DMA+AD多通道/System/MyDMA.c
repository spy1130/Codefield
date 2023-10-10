#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	MyDMA_Size = Size;
	//RCC开启DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//配置DMA
	DMA_InitTypeDef DMA_InitStructure;
	//外设
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;//起始地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//数据宽度(字节）
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//地址是否自增
	//存储器
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;//起始地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//数据宽度(字节）
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向，外设作为数据源
	DMA_InitStructure.DMA_BufferSize = Size;//传输计数器
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//不自动重装
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;//软件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//配置到DMA1的通道1
	//失能DMA
	DMA_Cmd(DMA1_Channel1, DISABLE);
}
/*
DMA工作的3个条件：
1.size计数器>0
2.触发源有触发信号
3.DMA使能
*/
//DMA传输函数
void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);//赋值前必须先让DMA失能
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);//给DMA重新赋值
	DMA_Cmd(DMA1_Channel1, ENABLE);//使能DMA
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//判断是否转运完成
	DMA_ClearFlag(DMA1_FLAG_TC1);//清除标志位
}
