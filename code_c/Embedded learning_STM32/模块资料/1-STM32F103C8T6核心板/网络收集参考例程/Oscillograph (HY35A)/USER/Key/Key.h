/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               key.h
** Descriptions:            The key application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2011-3-16
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef __KEY_H
#define __KEY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define BUTTON_FILTER_TIME 	5		/* �����˲�ʱ��50ms ��λ10ms ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼� */
#define BUTTON_LONG_TIME 	100		/* ����1�룬��Ϊ�����¼� */
#define KEY_FIFO_SIZE	    20

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t Count;			        /* �˲��������� */
	uint8_t FilterTime;		        /* �˲�ʱ��(���255,��ʾ2550ms) */
	uint16_t LongCount;		        /* ���������� */
	uint16_t LongTime;		        /* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			        /* ������ǰ״̬�����»��ǵ��� */
	uint8_t KeyCodeUp;		        /* ��������ļ�ֵ����, 0��ʾ����ⰴ������ */
	uint8_t KeyCodeDown;	        /* �������µļ�ֵ����, 0��ʾ����ⰴ������ */
	uint8_t KeyCodeLong; 	        /* ���������ļ�ֵ����, 0��ʾ����ⳤ�� */
}BUTTON_T;

/* �����ֵ���� */
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	/* ��Ҫ���USER�������¼��ͳ����¼� */
	KEY_DOWN_USER,			/* User������ */
	KEY_UP_USER,			/* User������ */
	KEY_HOLD_USER,			/* User������ */

	KEY_DOWN_WAKEUP,		/* WakeUp������ */
	KEY_DOWN_TAMPER,		/* Tamper������ */

	KEY_DOWN_JOY_UP,		/* ҡ��UP������ */
	KEY_DOWN_JOY_DOWN,		/* ҡ��DOWN������ */
	KEY_DOWN_JOY_LEFT,		/* ҡ��LEFT������ */
	KEY_DOWN_JOY_RIGHT,		/* ҡ��RIGHT������ */
	KEY_DOWN_JOY_OK			/* ҡ��OK������ */
}KEY_ENUM;

/*	����FIFO�õ����� */
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;	                /* ��������ָ�� */
	uint8_t Write;	                /* ������дָ�� */
}KEY_FIFO_T;

/* Private function prototypes -----------------------------------------------*/
void KeyPro(void);
uint8_t GetKey(void);
void Key_Initializtion(void);
void PutKey(uint8_t _KeyCode);

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/



