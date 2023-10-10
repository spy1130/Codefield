/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			lwipconfig.h
** Descriptions:		lwip����ͷ�ļ�
**
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2011-3-1
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef __LWIPCONFIG_H
#define __LWIPCONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* �����MAC��ַ */   
#define emacETHADDR0   0x00
#define emacETHADDR1   0x00
#define emacETHADDR2   0x00
#define emacETHADDR3   0x00
#define emacETHADDR4   0x00
#define emacETHADDR5   0x01
 
/* �����IP��ַ */
#define	 emacIPADDR0  192
#define	 emacIPADDR1  168
#define	 emacIPADDR2  0
#define	 emacIPADDR3  100

/* ��������ص�ַ */
#define emacGATEWAY_ADDR0  192
#define emacGATEWAY_ADDR1  168
#define emacGATEWAY_ADDR2  0
#define emacGATEWAY_ADDR3  1

/* ����������ַ */
#define emacNET_MASK0  255
#define emacNET_MASK1  255
#define emacNET_MASK2  255
#define emacNET_MASK3  0

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
