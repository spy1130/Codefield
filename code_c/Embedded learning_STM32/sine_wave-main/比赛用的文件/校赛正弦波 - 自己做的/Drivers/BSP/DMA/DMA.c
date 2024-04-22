#include "dma.h"
#include "delay.h"
extern u16 SineWave[4096]; 
extern u16 nums;
void DMA_Config()
{                  
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//����DMA2ʱ��
	DMA_Cmd(DMA2_Channel3,DISABLE);
//	delay_ms(5);                /* �ȴ�DMAʱ���ȶ� */
    DMA_StructInit( &DMA_InitStructure);        //DMA�ṹ���ʼ��
	
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//�ӼĴ���������
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���Ϊ����
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���Ϊ����
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//���ȼ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//�ر��ڴ浽�ڴ�ģʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ������ģʽ
	DMA_InitStructure.DMA_BufferSize = nums;//�������ݸ��������������
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DAC->DHR12R1;//�����ַΪDACͨ��1�ĵ�ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SineWave;//���������ݵ�ַ
	
	DMA_Init(DMA2_Channel3, &DMA_InitStructure);//��ʼ����
	DMA_Cmd(DMA2_Channel3,ENABLE);  	
}
