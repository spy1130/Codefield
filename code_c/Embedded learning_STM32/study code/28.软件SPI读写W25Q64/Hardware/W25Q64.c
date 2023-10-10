#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

void W25Q64_Init(void)
{
	MySPI_Init();
}
//读ID
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_JEDEC_ID);//W25Q64_JEDEC_ID为0x9F
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//抛一个垃圾，25Q64_DUMMY_BYTE为0xFF
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//得到设备ID的高八位,25Q64_DUMMY_BYTE为0xFF
	*DID <<= 8;//运到高八位
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);//得到设备ID的低八位,25Q64_DUMMY_BYTE为0xFF
	MySPI_Stop();
}
//写
void W25Q64_WriteEnable(void)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);//0x06
	MySPI_Stop();
}
//1表示芯片忙，0表示芯片不忙
void W25Q64_WaitBusy(void)
{
	uint32_t Timeout;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);//发送读状态寄存器1的命令
	Timeout = 100000;
	while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)//返回值为状态寄存器1的值
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
	MySPI_Stop();
}
//写页编程
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);//发送运行页寄存器的命令
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);//连续发24位地址
	for (i = 0; i < Count; i ++)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}
//擦除
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);//发送运行擦除寄存器的命令
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);//将该24位地址的内容全变为1
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}
//读数据
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);//发送运行读寄存器的命令
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);//发送地址
	for (i = 0; i < Count; i ++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();
}
