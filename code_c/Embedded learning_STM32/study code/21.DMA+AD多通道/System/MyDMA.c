#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	MyDMA_Size = Size;
	//RCC����DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//����DMA
	DMA_InitTypeDef DMA_InitStructure;
	//����
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;//��ʼ��ַ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���ݿ��(�ֽڣ�
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//��ַ�Ƿ�����
	//�洢��
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;//��ʼ��ַ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//���ݿ��(�ֽڣ�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//���䷽��������Ϊ����Դ
	DMA_InitStructure.DMA_BufferSize = Size;//���������
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//���Զ���װ
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;//�������
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//���ȼ�
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//���õ�DMA1��ͨ��1
	//ʧ��DMA
	DMA_Cmd(DMA1_Channel1, DISABLE);
}
/*
DMA������3��������
1.size������>0
2.����Դ�д����ź�
3.DMAʹ��
*/
//DMA���亯��
void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);//��ֵǰ��������DMAʧ��
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);//��DMA���¸�ֵ
	DMA_Cmd(DMA1_Channel1, ENABLE);//ʹ��DMA
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//�ж��Ƿ�ת�����
	DMA_ClearFlag(DMA1_FLAG_TC1);//�����־λ
}
