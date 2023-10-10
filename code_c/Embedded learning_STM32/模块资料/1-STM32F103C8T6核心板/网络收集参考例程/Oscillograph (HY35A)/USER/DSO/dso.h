/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               DSO.h
** Descriptions:            The DSO application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2011-2-20
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

#ifndef __DSO_H 
#define __DSO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADC2_DR_Address    ((uint32_t)0x4001284C)
#define ADC3_DR_Address    ((uint32_t)0x40013C4C)

#define Horizontal         0x00
#define Vertical           0x01

#define	SAMPLE_COUNT	   (4 * 1024) 	/* ������� ������������� */

/* Private typedef -----------------------------------------------------------*/
	
/* ʾ������ص����ݽṹ */
typedef struct
{
  uint16_t Ch1Buf[SAMPLE_COUNT];	 /* ͨ��1���ݻ����� */
  uint16_t Ch2Buf[SAMPLE_COUNT];	 /* ͨ��2���ݻ����� */

  uint32_t TimeBaseIdHold;			 /* ��ͣʱ��ʱ�� */

  uint32_t TimeBaseId;		    	 /* ʱ������ ���ɵõ� us��λ��ʱ�� */
  uint32_t SampleFreq;			     /* ����Ƶ�� ��λHz */
  uint32_t TimeBase;				 /* ʱ�����ɵõ� us��λ��ʱ�� */

  uint8_t  Ch1AttId;				 /* CH1 ˥���������� */
  uint8_t  Ch2AttId;				 /* CH2 ˥���������� */
  float    Ch1Attenuation; 		     /* ����1����˥��ϵ��(ԭʼ����x10���ٳ��������)  */
  float    Ch2Attenuation; 		     /* ����2����˥��ϵ��(ԭʼ����x10���ٳ��������)  */
  uint16_t Ch1VScale;				 /* ͨ��1��ֱ�ֶ�ֵmV��λ */
  uint16_t Ch2VScale;				 /* ͨ��2��ֱ�ֶ�ֵmV��λ */

  uint32_t TriggerLevel;			 /* ������ƽ(ADC��������Ƚ�ֵ) */
  uint32_t TriggerUpEdge;			 /* 1��ʾ����ģʽ�����أ�0��ʾ������ */

  uint8_t Ch1DC;					 /* 0�ǽ��� 1��ʾCH1��ֱ�� */
  uint8_t Ch2DC;					 /* 0�ǽ��� 1��ʾCH2��ֱ�� */

  int16_t Ch1VOffset;				 /* ͨ��1 GND��λ��, ����Ϊ���� */
  int16_t Ch2VOffset;				 /* ͨ��1 GND��λ��, ����Ϊ���� */

  uint8_t ActiveCH;				     /* ��ǰ����ͨ�� 1��ʾCH1, 2��ʾCH2 */
  uint8_t AdjustMode;				 /* ��ǰ����ģʽ��0��ʾ���ڷ��ȣ�1��ʾ����ƫ�� */

  /* ʹ��2����������ɲ��εĲ��������� */
  int16_t xCh1Buf1[310];		     /* �������ݣ��������� */
  int16_t yCh1Buf1[310];		     /* �������ݣ��������� */
  int16_t xCh1Buf2[310];		     /* �������ݣ��������� */
  int16_t yCh1Buf2[310];		     /* �������ݣ��������� */

  int16_t xCh2Buf1[310];		     /* �������ݣ��������� */
  int16_t yCh2Buf1[310];		     /* �������ݣ��������� */
  int16_t xCh2Buf2[310];		     /* �������ݣ��������� */
  int16_t yCh2Buf2[310];	 	     /* �������ݣ��������� */
  uint8_t BufUsed;			         /* 0��ʾ��ǰ��Buf1�� 1��ʾ��Buf2 */

  uint8_t HoldEn;				     /* ���βɼ���ͣ��־ 1��ʾ��ͣ��0��ʾ���� */
}
DSO;

typedef struct 
{
   int16_t * CH1x;
   int16_t * CH1y;
   int16_t * CH2x;
   int16_t * CH2y;
} 
PARAM;
	
/* ����֧�ֵĲ���Ƶ�� */
typedef enum
{
	SR_1K = 0,	
}
SAMPLE_RATE_E;

/* ������״̬�� */
enum
{
	S_HELP		= 0,	/* ����ҳ�� */
	S_RUN		= 1,	/* �ɼ�״̬ */
	S_HOLD		= 2		/* ��ͣ״̬ */
};

/* Private function prototypes -----------------------------------------------*/
void DSO_SampleRate(uint32_t freq);
void DSO_Initializtion(void);
void Display_Help(void);
void DSO_Help(void);
void Display_Frame(void);
void Display_CH1_Wave(void);
void Display_CH2_Wave(void);

void Display_CH_Info(void);
void Display_DSO(void);
void DSO_Configuration(void);
void Inc_SampleFreq(void);
void Dec_SampleFreq(void);
void Adjust_DSO(uint8_t ch, uint8_t mode);
void Stop_ADC(void);
void Start_ADC(void);
void DSO_Run(void);
void _DrawDSO(void) ;
void _Draw(void * p);
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


