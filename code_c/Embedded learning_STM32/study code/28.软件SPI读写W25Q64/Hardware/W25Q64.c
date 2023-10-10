#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

void W25Q64_Init(void)
{
	MySPI_Init();
}
//��ID
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_JEDEC_ID);//W25Q64_JEDEC_IDΪ0x9F
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//��һ��������25Q64_DUMMY_BYTEΪ0xFF
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//�õ��豸ID�ĸ߰�λ,25Q64_DUMMY_BYTEΪ0xFF
	*DID <<= 8;//�˵��߰�λ
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);//�õ��豸ID�ĵͰ�λ,25Q64_DUMMY_BYTEΪ0xFF
	MySPI_Stop();
}
//д
void W25Q64_WriteEnable(void)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);//0x06
	MySPI_Stop();
}
//1��ʾоƬæ��0��ʾоƬ��æ
void W25Q64_WaitBusy(void)
{
	uint32_t Timeout;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);//���Ͷ�״̬�Ĵ���1������
	Timeout = 100000;
	while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)//����ֵΪ״̬�Ĵ���1��ֵ
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
	MySPI_Stop();
}
//дҳ���
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);//��������ҳ�Ĵ���������
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);//������24λ��ַ
	for (i = 0; i < Count; i ++)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}
//����
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);//�������в����Ĵ���������
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);//����24λ��ַ������ȫ��Ϊ1
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}
//������
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);//�������ж��Ĵ���������
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);//���͵�ַ
	for (i = 0; i < Count; i ++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();
}
