#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

void MySPI_Init(void)
{	//RCCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	//GPIO��
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//SPI����
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//˫��ȫ˫��
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8λ����֡
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//��λ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//��Ƶϵ��
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//ģʽ0������Ĭ�ϵ͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//��һ�����ؿ�ʼ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//���ģ��
	SPI_InitStructure.SPI_CRCPolynomial = 7;//Ĭ��
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
	
	MySPI_W_SS(1);//Ĭ�ϲ�ѡ�дӻ�
}

void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);//����ȡTXE��־λ������set�����ȴ�TXEΪ1��SPI_DRΪ��
	
	SPI_I2S_SendData(SPI1, ByteSend);//д��TDR
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);//�ȴ�RXNEΪ1����ʾ�յ�һ���ֽڣ�������ɣ���������ɣ���������
	
	return SPI_I2S_ReceiveData(SPI1);//��ȡDR
}
