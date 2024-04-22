#include "dma.h"
#include "delay.h"
extern u16 SineWave[4096]; 
extern u16 nums;
void DMA_Config()
{                  
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//开启DMA2时钟
	DMA_Cmd(DMA2_Channel3,DISABLE);
//	delay_ms(5);                /* 等待DMA时钟稳定 */
    DMA_StructInit( &DMA_InitStructure);        //DMA结构体初始化
	
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//从寄存器读数据
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//宽度为半字
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//宽度为半字
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级中
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//关闭内存到内存模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环发送模式
	DMA_InitStructure.DMA_BufferSize = nums;//传输数据个数，采样点个数
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DAC->DHR12R1;//外设地址为DAC通道1的地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SineWave;//待搬运数据地址
	
	DMA_Init(DMA2_Channel3, &DMA_InitStructure);//初始化　
	DMA_Cmd(DMA2_Channel3,ENABLE);  	
}
