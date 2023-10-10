/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			search.c
** Descriptions:		�ṩ���⹦�ܵ��ڴ���Һ���
**						
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2011-4-4
** Version:				2.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include	"search.h"

/*******************************************************************************
* Function Name  : MemSearch
* Description    : ��ָ����С���ڴ�����������ݣ��ú������Բ����������͵�����
* Input          : - pvMem: ָ��ָ�����ڴ�����ָ��
*				   - u32MemSize: �������ڴ�����Ĵ�С
*				   - u32Data: ������������
*				   - u32DataSize: �����������ݴ�С
* Output         : None
* Return         : �ҵ��򷵻�λ��ָ�룬���򷵻�ֵΪNULL
* Attention		 : None
*******************************************************************************/
void *MemSearch(void *pvMem, INT32U u32MemSize, INT32U u32Data, INT32U u32DataSize)
{
	const INT8U		*__pubAddr = pvMem;	
	INT8S 			*__pbData = (INT8S *)&u32Data;
	INT32S 			i;
	BOOLEAN			__blIsMatch;
	
	do{
		__pubAddr = (INT8U*)MemChr((INT8S*)__pubAddr, *__pbData, ((INT8U *)pvMem + u32MemSize) - __pubAddr);
		if(__pubAddr != NULL)
		{
			__blIsMatch = 1;
			for(i=1; i<u32DataSize; i++)
			{
				if(*(__pubAddr + i) != *(__pbData + i))
				{
					__blIsMatch = 0;
					break;
				}
			}
		
			if(__blIsMatch)
				return (void *)__pubAddr;
		
			__pubAddr++;	
		}
		else
			break;		
	}while(1);
	
	return NULL;
}

/*******************************************************************************
* Function Name  : MemChr
* Description    : ��ָ�����ڴ���������ַ�
* Input          : - pbMem: ָ��ָ�����ڴ�����ָ��
*				   - bBYTE: ���������ַ�
*				   - u32MemSize: �ڴ��С
* Output         : None
* Return         : �����ҵ���ʵ��λ��
* Attention		 : None
*******************************************************************************/

INT8S *MemChr(INT8S *pbMem, INT8S bBYTE, INT32U u32MemSize)
{
	INT8S		*__pbNext = pbMem;
	INT32S		i;

	for(i=0; i<u32MemSize; i++)
	{
		if(*(__pbNext+i) == bBYTE)
			return (__pbNext + i);
	}
	
	return NULL;
}

/*******************************************************************************
* Function Name  : memstr
* Description    : ��ָ�����ڴ���������ַ���
* Input          : - pbMem: ָ��ָ�����ڴ�����ָ��
*				   - pbStr: ���������ַ���
*                  - u32StrSize: ���������ַ�����С
*				   - u32MemSize: �ڴ��С
* Output         : None
* Return         : �ҵ��򷵻�TRUE�����򷵻�FALSE
* Attention		 : None
*******************************************************************************/
BOOLEAN memstr(INT8S *pbMem, INT8S *pbStr, INT32U u32StrSize, INT32U u32MemSize)
{
	INT8S *__pbNext = pbMem;
	
	__pbNext = MemChr(__pbNext, (INT8S)pbStr[0], u32MemSize);
	while(__pbNext != NULL)
	{
		if(__pbNext != NULL)
		{
			if(strncmp( (void*)__pbNext, (void*)pbStr, u32StrSize) == 0)
				return 1;
			
			__pbNext += 1;
			__pbNext = MemChr(__pbNext, (INT8S)pbStr[0], (pbMem + u32MemSize) - __pbNext);
		}
	}
		
	return 0;
}

/*******************************************************************************
* Function Name  : memstrExt
* Description    : ��ָ�����ڴ���������ַ���
* Input          : - pbMem: ָ��ָ�����ڴ�����ָ��
*				   - pbStr: ���������ַ���
*                  - u32StrSize: ���������ַ�����С
*				   - u32MemSize: �ڴ��С
* Output         : None
* Return         : �ҵ������ַ������ڵ�λ��ָ�룬���򷵻�NULL
* Attention		 : None
*******************************************************************************/
INT8S *memstrExt(INT8S *pbMem, INT8S *pbStr, INT32U u32StrSize, INT32U u32MemSize)
{
	INT8S *__pbNext = pbMem;
	
	__pbNext = MemChr(__pbNext, (INT8S)pbStr[0], u32MemSize);
	while(__pbNext != NULL)
	{
		if(__pbNext != NULL)
		{
			if(strncmp( (void*)__pbNext, (void*)pbStr, u32StrSize) == 0)
				return __pbNext;
			
			__pbNext += 1;
			__pbNext = MemChr(__pbNext, (INT8S)pbStr[0], (pbMem + u32MemSize) - __pbNext);
		}
	}
		
	return NULL;
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
