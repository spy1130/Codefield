#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

void MySPI_Init(void)
{	//RCC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	//GPIO口
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
	
	
	//SPI配置
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主机
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线全双工
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8位数据帧
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//高位先行
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//分频系数
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//模式0，空闲默认低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//第一个边沿开始采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//软件模拟
	SPI_InitStructure.SPI_CRCPolynomial = 7;//默认
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
	
	MySPI_W_SS(1);//默认不选中从机
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
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);//（读取TXE标志位不等于set），等待TXE为1，SPI_DR为空
	
	SPI_I2S_SendData(SPI1, ByteSend);//写入TDR
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);//等待RXNE为1，表示收到一个字节，发送完成，即接受完成（交换）。
	
	return SPI_I2S_ReceiveData(SPI1);//读取DR
}
