/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               dso.c
** Descriptions:            The dso application function
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

/* Includes ------------------------------------------------------------------*/
#include "dso.h"
#include "key.h"
#include "glcd.h"
#include "SimHei.c"
#include <includes.h>            


/* Private define ------------------------------------------------------------*/
#define DSO_VER	 "V1.0"	  /* ��ǰ�汾 */
#define ATT_COUNT	6	  /* ˥��������g_DSO.Ch1Attenuation = 23 ����˥������: (1) 28 === 1V  (1:1) */


/* Private variables ---------------------------------------------------------*/

/* ʱ��ֶȱ� g_DSO.TimeBaseId��Ϊ�������� */
const uint32_t g_TimeTable[] =
{
	20, 	50, 	100, 
	200,	500,	1000,
	2000,	5000,	10000,
	20000,	50000,	100000,
	200000,	500000,	1000000		
};


const float g_AttTable[ATT_COUNT][2] =
{
	/* {����,ÿ����ѹ}  1:1 */
	{327.6,	5000},
	{130.8,	2000},
	{64.8,	1000},
	{32.4,	500},
	{12,	200},
	{6,	    100},
};

/* 
   ʾ����ͨ��1ͬʱ���ӵ�CPU��PC0��PC1��PC2
   PC0 = ADC1_IN10	
   PC1 = ADC2_IN11
   PC2 = ADC3_IN12

   ʾ����ͨ��2���ӵ�CPU��PC3
   PC1 = ADC2_IN13

   ����ͨ��1��ģ���źţ�����ͬʱ����3��ADC���н�������������Ͽ���ʵ��3Mbps�������� 

   ͨ��2ֻ������1��ADC���в�����
   ֧��ͨ��1��ͨ��2����ͬ��������
*/

extern OS_EVENT *DSOHelp;
extern OS_EVENT *DSORun;
extern OS_EVENT *LCDRefresh;
DSO    g_DSO;


/*******************************************************************************
* Function Name  : DSO_Initializtion
* Description    : ��ʾ����ͨ��1��ͨ��2���г�ʼ�����á���Ҫ���GPIO�����á�
*                  ADC�����á���ʱ���������Լ�DMA�����á�
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DSO_Initializtion(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef	ADC_InitStructure;	
  GPIO_InitTypeDef GPIO_InitStructure; 

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC	| RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* ����DMA1_1 ����CH1 */
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  DMA_DeInit(DMA1_Channel1);		                                            /* ��λDMA1�Ĵ�����ȱʡ״̬ */
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	                /* ѡ��ADC1�����ݼĴ�����ΪԴ */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_DSO.Ch1Buf;	            /* Ŀ���ַ */
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	                        /* ����DMA���䷽������(ADC)��ΪԴ */
  DMA_InitStructure.DMA_BufferSize = SAMPLE_COUNT;	                            /* ���û�������С */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	            /* �����ַ������ */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	                    /* �洢����ַ��Ҫ���� */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	/* ѡ�����贫�䵥λ��16bit */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	        /* ѡ���ڴ洫�䵥λ��16bit */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	                                /* ����ѭ��ģʽ   */
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;	                        /* ѡ��DMA���ȼ� */
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	                                /* DMA�������ͣ������ڴ浽�ڴ� */
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);

  /* ����DMA2_5 ����CH2 */

  /* Enable DMA2 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

  DMA_DeInit(DMA2_Channel5);		                                            /* ��λDMA2�Ĵ�����ȱʡ״̬ */
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_DR_Address;	                /* ѡ��ADC3�����ݼĴ�����ΪԴ */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_DSO.Ch2Buf;	            /* Ŀ���ַ */
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	                        /* ����DMA���䷽������(ADC)��ΪԴ */
  DMA_InitStructure.DMA_BufferSize = SAMPLE_COUNT;	                            /* ���û�������С */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	            /* �����ַ������ */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	                    /* �洢����ַ��Ҫ���� */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	/* ѡ�����贫�䵥λ��16bit */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	        /* ѡ���ڴ洫�䵥λ��16bit */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	                                /* ����ѭ��ģʽ   */
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	                        /* ѡ��DMA���ȼ� */
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	                                /* DMA�������ͣ������ڴ浽�ڴ� */
  DMA_Init(DMA2_Channel5, &DMA_InitStructure);

  /* Enable DMA2 channel5 */
  DMA_Cmd(DMA2_Channel5, ENABLE);

  /* ����ADC1  */
	
  /* ADCCLK = PCLK2/2 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div2);     /* ADCCLK = PCLK2/4 = 18MHz */

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		                /* ����ת����ֹ */
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC3;	    /* ѡ��TIM1��CC3������ */
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	                /* �����Ҷ���,��λΪ0 */
  ADC_InitStructure.ADC_NbrOfChannel = 1;	                                /* 1��ͨ�� */
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channels configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_1Cycles5);

  /* Enable ADC1 external trigger conversion */
  ADC_ExternalTrigConvCmd(ADC1, ENABLE);

  /* ʹ�� ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* ʹ�� ADC1 ��λУ׼�Ĵ��� */
  ADC_ResetCalibration(ADC1);
  /* ��⸴λУ׼�Ĵ��� */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* ��ʼ ADC1 У׼ */
  ADC_StartCalibration(ADC1);
  /* �ȴ�У׼���� */
  while(ADC_GetCalibrationStatus(ADC1));

  /* ����ADC3  */
    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		                /* ����ת����ֹ */
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC3;	    /* ѡ��TIM1��CC3������ */
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	                /* �����Ҷ���,��λΪ0 */
  ADC_InitStructure.ADC_NbrOfChannel = 1;	                                /* 1��ͨ�� */
  ADC_Init(ADC3, &ADC_InitStructure);

  /* ADC1 regular channels configuration */
  ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 1, ADC_SampleTime_1Cycles5);

  /* Enable ADC3 external trigger conversion */
  ADC_ExternalTrigConvCmd(ADC3, ENABLE);

  /* ʹ�� ADC3 */
  ADC_Cmd(ADC3, ENABLE);

  /* Enable ADC3 DMA */
  ADC_DMACmd(ADC3, ENABLE);

  /* ʹ�� ADC3 ��λУ׼�Ĵ��� */
  ADC_ResetCalibration(ADC3);
  /* ��⸴λУ׼�Ĵ��� */
  while(ADC_GetResetCalibrationStatus(ADC3));

  /* ��ʼ ADC3 У׼ */
  ADC_StartCalibration(ADC3);
  /* �ȴ�У׼���� */
  while(ADC_GetCalibrationStatus(ADC3));

  /* ���ò���������ʱ����ʹ��TIM1 CC1 */
  DSO_SampleRate(g_DSO.SampleFreq);	/* �޸Ĳ���Ƶ��(����ʱ100K) */
}

/*******************************************************************************
* Function Name  : DSO_SampleRate
* Description    : �޸Ĳ���Ƶ��
* Input          : - freq: ����Ƶ�� ��λHz 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DSO_SampleRate(uint32_t freq)
{
  /* ���ö�ʱ��1��3ͨ��CC3��ΪADC1�Ĵ���Դ */	  
  uint16_t Period;	
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  TIM_Cmd(TIM1, DISABLE);

  //g_DSO.SampleRate = freq;	/* ������Ƶ�ʱ��浽ȫ�������� */
	
  /* ����Ƶ�ʼ��㹫ʽ : period = 72000000 / freq ; 1200 == 60KHz ����Ƶ�� */
  Period = 72000000 / freq;		

  /* Enable TIM1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = Period;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0;        /* ����Ƶ�� = 72000 000 / 18 = 4000 000	 */
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* TIM1 channel1 configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
  TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period / 2; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);

  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);

  /* TIM1 main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);  
}

/*******************************************************************************
* Function Name  : Display_Help
* Description    : ��ʾhelp����
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Display_Help(void)
{
  uint16_t y;
  uint16_t LineCap;

  GUI_SetFont(&GUI_FontHZ_SimHei); 

  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();

  GUI_SetColor(GUI_WHITE);

  LineCap = 20; /* �м�� */

  y = 4;	  	
  GUI_DispStringAt( "HY-RedBull V3.0 ����ʾ������ʾ����" , 24, y ) ;
  y += LineCap;
  GUI_DispStringAt( "www.PowerMCU.com" , 98, y - 4 ) ;
  y += LineCap;
  GUI_DispStringAt( "������ʾ" , 130, y - 4 ) ;	
  y += LineCap;
  GUI_DispStringAt( "TAMPER�� = �л�ͨ������(CH1��CH2)" , 5, y );
  y += LineCap;
  GUI_DispStringAt( "WAKEUP�� = ��ʾ�������˳�����" , 5, y );
  y += LineCap;
  GUI_DispStringAt( "USER  �� = ��ͣ��ʵʱ����" , 5, y );
  y += LineCap;
  GUI_DispStringAt( "ҡ���ϼ� = �Ŵ��δ�ֱ���Ȼ������ƶ�" , 5, y );
  y += LineCap;
  GUI_DispStringAt( "ҡ���¼� = ��С���δ�ֱ���Ȼ������ƶ�" , 5, y );
  y += LineCap;
  GUI_DispStringAt( "ҡ����� = ˮƽչ����" , 5, y );
  y += LineCap;
  GUI_DispStringAt( "ҡ���Ҽ� = ˮƽ��С����" , 5, y );
  y += LineCap;
  GUI_DispStringAt( "ҡ��OK�� = �л�ҡ�˵���ģʽ(���Ȼ�λ��)" , 5, y );
  y += LineCap;
}

/*******************************************************************************
* Function Name  : DSO_Help
* Description    : ��ʾhelp����
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DSO_Help(void)
{
  uint8_t key;

  Display_Help();
  for(;;)
  {		  
    key = GetKey(); /* ��ȡ����������0��ʾ�м����� */
    if (key > 0)
	{
       /* �м����� */
	   switch (key)
	   {
		  case KEY_DOWN_JOY_LEFT:	 /* ҡ��LEFT������ */
		  case KEY_DOWN_JOY_RIGHT:   /* ҡ��RIGHT������ */
		  case KEY_DOWN_WAKEUP:
		  case KEY_DOWN_USER:
		  case KEY_DOWN_JOY_OK:	     /* ҡ��OK�� */
		       OSSemPost(DSORun);	 /* �����ź��� �˳�,����ȫ������״̬*/
			   return;
		  case KEY_DOWN_JOY_UP:	     /* ҡ��UP������ */
			   break;
		  case KEY_DOWN_JOY_DOWN:	 /* ҡ��DOWN������ */
			   break;
		  default:
			   break;
	   }
	} 
  }
}

/*******************************************************************************
* Function Name  : Display_Frame
* Description    : ��ʾ���δ��ڵı߿�Ϳ̶���
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Display_Frame(void)
{
  uint16_t x, y;

  GUI_SetColor(GUI_WHITE); 
  GUI_SetBkColor(GUI_BLACK);
	
  /* ����һ��ʵ�߾��ο� x, y, h, w */
  GUI_DrawRect(9, 19, 9 + 302, 19 + 202);

  /* ˢ�� */
  GUI_ClearRect(9 + 1, 19 + 1, 9 + 302 - 1 , 19 + 202 -1 );
						   
  /* ���ƴ�ֱ�̶ȵ� */
  for (x = 0; x < 13; x++)
  {
	 for (y = 0; y < 41; y++)
	 {
		 GUI_DrawPixel(10 + (x * 25), 20 + (y * 5));
	 }
  }

  /* ����ˮƽ�̶ȵ� */
  for (y = 0; y < 9; y++)
  {
	 for (x = 0; x < 61; x++)
	 {
		 GUI_DrawPixel(10 + (x * 5), 20 + (y * 25));
	 }
  }

  /* ���ƴ�ֱ���Ŀ̶ȵ� */
  for (y = 0; y < 41; y++)
  {	 
	 GUI_DrawPixel(9 + (6 * 25), 20 + (y * 5));
	 GUI_DrawPixel(11 + (6 * 25), 20 + (y * 5));
  }

  /* ����ˮƽ���Ŀ̶ȵ� */
  for (x = 0; x < 61; x++)
  {	 
	 GUI_DrawPixel(10 + (x * 5), 19 + (4 * 25));
	 GUI_DrawPixel(10 + (x * 5), 21 + (4 * 25) );
  }
}

/*******************************************************************************
* Function Name  : LCD_DrawPoints
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_DrawPoints( uint16_t *x, uint16_t *y, uint16_t Size )
{
	uint16_t i;

	for (i = 0 ; i < Size - 1; i++)
	{
		GUI_DrawLine(x[i], y[i], x[i + 1], y[i + 1] );
	}
}

/*******************************************************************************
* Function Name  : Display_CH_Info
* Description    : ��ʾͨ����Ϣ
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	
void Display_CH_Info(void)
{
	char buf[32];   /* �ַ���ʾ������ */

	GUI_SetFont(&GUI_FontHZ_SimHei); 

	/* ��ʾʾ��������汾 */
	GUI_SetColor(GUI_WHITE);   
	GUI_SetBkColor(GUI_BLACK);		
	GUI_DispStringAt( DSO_VER, 10, 2 );

	/* ��ʾͨ��1��Ϣ */
	if (g_DSO.Ch1DC == 1)
	{
		strcpy(buf, "CH1 DC ");
	}
	else
	{
		strcpy(buf, "CH1 DC ");
	}

	if (g_DSO.Ch1VScale >= 1000)
	{
		sprintf(&buf[7], "%d.00V", g_DSO.Ch1VScale / 1000);
	}
	else
	{
		sprintf(&buf[7], "%dmV", g_DSO.Ch1VScale);
	}

	if (g_DSO.ActiveCH == 1)
	{
		GUI_SetColor(GUI_WHITE);  
		GUI_SetBkColor(GUI_MAGENTA);	
	}
	else
	{
		GUI_SetColor(GUI_WHITE);   
		GUI_SetBkColor(GUI_BLACK);		
	}

	GUI_DispStringAt( buf, 10, 224 );

	/* ��ʾͨ��2��Ϣ */
	if (g_DSO.Ch2DC == 1)
	{
		strcpy(buf, "CH2 DC ");
	}
	else
	{
		strcpy(buf, "CH2 DC ");
	}

	if (g_DSO.Ch2VScale >= 1000)
	{
		sprintf(&buf[7], "%d.00V", g_DSO.Ch2VScale / 1000);
	}
	else
	{
		sprintf(&buf[7], "%dmV", g_DSO.Ch2VScale);
	}
	if (g_DSO.ActiveCH == 2)
	{
		GUI_SetColor(GUI_WHITE);   
		GUI_SetBkColor(GUI_MAGENTA);
	}
	else
	{
		GUI_SetColor(GUI_WHITE);  
		GUI_SetBkColor(GUI_BLACK);	
	}
	GUI_DispStringAt( buf, 120, 224 );

	/* ��ʾʱ�� */
	GUI_SetColor(GUI_WHITE);      /* ʱ����ɫ */
	GUI_SetBkColor(GUI_BLACK);	 

	if (g_DSO.TimeBase < 1000)
	{
		sprintf(buf, "Time %3dus", g_DSO.TimeBase);
	}
	else if (g_DSO.TimeBase < 1000000)
	{
		sprintf(buf, "Time %3dms", g_DSO.TimeBase / 1000);
	}
	else
	{
		sprintf(buf, "Time %3ds ", g_DSO.TimeBase / 1000000);	
	}
	GUI_DispStringAt( buf, 230, 224 );


	/* ��ʾ����ģʽ */
	GUI_SetColor(GUI_WHITE);    /* ���� */
	GUI_SetBkColor(GUI_BLACK);	/* ���� */
	if (g_DSO.AdjustMode == 1)
	{
		GUI_DispStringAt((uint8_t *)"����λ��", 245, 2 );
	}
	else
	{
		GUI_DispStringAt((uint8_t *)"���ڷ���", 245, 2 );
	}

	sprintf(buf, "����Ƶ��:%7dHz",	g_DSO.SampleFreq);
	GUI_DispStringAt( buf, 75, 2 );
}

/*******************************************************************************
* Function Name  : DSO_Configuration
* Description    : ��ʼ������
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DSO_Configuration(void)
{
  g_DSO.Ch1Attenuation = 23;	/* ����ȱʡ˥��ϵ�� */
  g_DSO.Ch2Attenuation = 23;	/* ����ȱʡ˥��ϵ�� */
  g_DSO.Ch1VScale = 1000;	    /* ȱʡ��1V */
  g_DSO.Ch2VScale = 1000;	    /* ȱʡ��1V */

  g_DSO.Ch1VOffset = 70; 	    /* ͨ��1 GND��λ�� */
  g_DSO.Ch2VOffset = 170;       /* ͨ��2 GND��λ�� */

  g_DSO.ActiveCH = 1;		    /* ȱʡ��CH1 */
  g_DSO.AdjustMode = 1;	        /* ȱʡ�ǵ��ڴ�ֱƫ�� �����л���2���ڷ��� */

  g_DSO.HoldEn = 0;

  g_DSO.TimeBaseId = 0;									 
  g_DSO.TimeBase = 	g_TimeTable[g_DSO.TimeBaseId];
  g_DSO.SampleFreq = 25000000 / g_DSO.TimeBase;


  g_DSO.Ch1AttId = 2;
  g_DSO.Ch1Attenuation = g_AttTable[g_DSO.Ch1AttId][0];
  g_DSO.Ch1VScale =  g_AttTable[g_DSO.Ch1AttId][1];

  g_DSO.Ch2AttId = 2;
  g_DSO.Ch2Attenuation = g_AttTable[g_DSO.Ch2AttId][0];
  g_DSO.Ch2VScale =  g_AttTable[g_DSO.Ch2AttId][1];
}

/*******************************************************************************
* Function Name  : Inc_SampleFreq
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Inc_SampleFreq(void)
{
  if (g_DSO.TimeBaseId < (sizeof(g_TimeTable) / 4) - 1)
  {
	 g_DSO.TimeBaseId++;
  }
													 
  g_DSO.TimeBase = 	g_TimeTable[g_DSO.TimeBaseId];
  g_DSO.SampleFreq = 25000000 / g_DSO.TimeBase;

  DSO_SampleRate(g_DSO.SampleFreq);	/* �ı����Ƶ�� */
}

/*******************************************************************************
* Function Name  : Dec_SampleFreq
* Description    : ���Ͳ���Ƶ�ʣ��� 1-2-5
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Dec_SampleFreq(void)
{
  if (g_DSO.TimeBaseId > 0)
  {
	 g_DSO.TimeBaseId--;
  }													 
  g_DSO.TimeBase = g_TimeTable[g_DSO.TimeBaseId];
  g_DSO.SampleFreq = 25000000 / g_DSO.TimeBase;

  DSO_SampleRate(g_DSO.SampleFreq);	/* �ı����Ƶ�� */
}

/*******************************************************************************
* Function Name  : Adjust_DSO
* Description    : ����˥������ 1-2-5 
* Input          : - ch: ͨ���ţ�1��2
*                  - mode: 0 ���� 1����
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Adjust_DSO(uint8_t ch, uint8_t mode)
{

  if (ch == 1)
  {
	 if (mode == 0) /* ���� */
	 {
		 if (g_DSO.Ch1AttId > 0)
		 {
			 g_DSO.Ch1AttId--;
		 }
	 }
	 else		    /* ���� */
	 {
		 if (g_DSO.Ch1AttId < ATT_COUNT - 1)
		 {
			 g_DSO.Ch1AttId++;
		 }
	 }
	 g_DSO.Ch1Attenuation = g_AttTable[g_DSO.Ch1AttId][0];
	 g_DSO.Ch1VScale =  g_AttTable[g_DSO.Ch1AttId][1];
  }
  else if (ch == 2)
  {
	 if (mode == 0) /* ���� */
	 {
		 if (g_DSO.Ch2AttId > 0)
		 {
			 g_DSO.Ch2AttId--;
		 }
	 }
	 else		    /* ���� */
	 {
		 if (g_DSO.Ch2AttId < ATT_COUNT - 1)
		 {
			 g_DSO.Ch2AttId++;
		 }
	 }
	 g_DSO.Ch2Attenuation = g_AttTable[g_DSO.Ch2AttId][0];
	 g_DSO.Ch2VScale =  g_AttTable[g_DSO.Ch2AttId][1];
  }
}

/*******************************************************************************
* Function Name  : Stop_ADC
* Description    : ��ͣADC����
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Stop_ADC(void)
{
  TIM_Cmd(TIM1, DISABLE);

  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, DISABLE);
	
  /* Enable DMA2 channel5 */
  DMA_Cmd(DMA2_Channel5, DISABLE);
}

/*******************************************************************************
* Function Name  : Start_ADC
* Description    : ��ʼADC����
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Start_ADC(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  DMA_DeInit(DMA1_Channel1);		                                            /* ��λDMA1�Ĵ�����ȱʡ״̬ */
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	                /* ѡ��ADC1�����ݼĴ�����ΪԴ */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_DSO.Ch1Buf;	            /* Ŀ���ַ */
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	                        /* ����DMA���䷽������(ADC)��ΪԴ */
  DMA_InitStructure.DMA_BufferSize = SAMPLE_COUNT;	                            /* ���û�������С */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	            /* �����ַ������ */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	                    /* �洢����ַ��Ҫ���� */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	/* ѡ�����贫�䵥λ��16bit */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	        /* ѡ���ڴ洫�䵥λ��16bit */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	                                /* ����ѭ��ģʽ   */
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;	                        /* ѡ��DMA���ȼ� */
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	                                /* DMA�������ͣ������ڴ浽�ڴ� */
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  DMA_DeInit(DMA2_Channel5);		                                            /* ��λDMA2�Ĵ�����ȱʡ״̬ */
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_DR_Address;	                /* ѡ��ADC1�����ݼĴ�����ΪԴ */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_DSO.Ch2Buf;	            /* Ŀ���ַ */
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	                        /* ����DMA���䷽������(ADC)��ΪԴ */
  DMA_InitStructure.DMA_BufferSize = SAMPLE_COUNT;	                            /* ���û�������С */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	            /* �����ַ������ */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	                    /* �洢����ַ��Ҫ���� */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	/* ѡ�����贫�䵥λ��16bit */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	        /* ѡ���ڴ洫�䵥λ��16bit */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	                                /* ����ѭ��ģʽ   */
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;	                        /* ѡ��DMA���ȼ� */
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	                                /* DMA�������ͣ������ڴ浽�ڴ� */
  DMA_Init(DMA2_Channel5, &DMA_InitStructure);

  /* Enable DMA2 channel5 */
  DMA_Cmd(DMA1_Channel1, ENABLE);

  /* Enable DMA2 channel5 */
  DMA_Cmd(DMA2_Channel5, ENABLE);

  TIM_Cmd(TIM1, ENABLE);
}

/*******************************************************************************
* Function Name  : DSO_Run
* Description    : DSOȫ������״̬
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DSO_Run(void)
{
  INT8U   err;
  uint8_t key;

  DSO_Configuration();	 /* ��ʼ��ʾ�������� */
  DSO_Initializtion();	 /* ����ʾ�����õ������裺ADC,TIM,DMA�� */

  for(;;)
  {
     OSSemPend(DSORun,10,&err);	/* ��ȡ�ź��� ��ʱ10���� */
	 if( err == OS_ERR_NONE )
	 {
        GUI_SetBkColor(GUI_BLACK);
        GUI_Clear();		   
	    GUI_SetColor(GUI_WHITE);   
	 }
     /* ��ȡ���� ����0��ʾ�м����� */
	 key = GetKey();
	 if (key > 0)
	 {
		/* �м����� */
		switch (key)
		{
		   case  KEY_DOWN_TAMPER:	        /* TAMPER�� ͨ��ѡ��(CH1��CH2) */
				 if (g_DSO.ActiveCH == 1)
				 {
					 g_DSO.ActiveCH = 2;
				 }
				 else
				 {
					 g_DSO.ActiveCH = 1;
				 }
				 OSSemPost(LCDRefresh);	    /* �����ź��� ����ˢ��LCD */
				 break;

		   case  KEY_DOWN_WAKEUP:	        /* WAKEUP�� ����ģʽѡ��(���Ȼ��ߴ�ֱƫ��) */
				 OSSemPost(DSOHelp);	    /* �����ź��� �˳� ����ȫ������״̬ */
				 break;

		   case  KEY_DOWN_USER:	            /* USER�� ��ʱ��ʵʱ */
				 if ( g_DSO.HoldEn == 0 )
				 {
					 g_DSO.HoldEn = 1;
					 /* ������ͣʱ��ʱ�� Ϊ��ˮƽ��չ�� */
					 g_DSO.TimeBaseIdHold = g_DSO.TimeBaseId;	
                     Stop_ADC();
				 }
				 else
				 {
					 g_DSO.HoldEn = 0;
					 Start_ADC();				
				 }
				 OSSemPost(LCDRefresh);	    /* �����ź��� ����ˢ��LCD */
				 break;

		   case  KEY_DOWN_JOY_LEFT:	        /* ҡ��LEFT������ */
				 if (g_DSO.HoldEn == 0)
				 {
					 Dec_SampleFreq();	    /* �ݼ�����Ƶ�� */
				     OSSemPost(LCDRefresh); /* �����ź��� ����ˢ��LCD */
				 }
				 else
				 {
				     /* ����ˮƽ�ƶ� ������ */
				 }
				 break;

		   case  KEY_DOWN_JOY_RIGHT:	    /* ҡ��RIGHT������ */
				 if (g_DSO.HoldEn == 0)
				 {
					 Inc_SampleFreq();  	/* ��������Ƶ�� */					
				     OSSemPost(LCDRefresh); /* �����ź��� ����ˢ��LCD */
				 }
				 else
				 {
					 /* ����ˮƽ�ƶ� ������ */
				 }
				 break;

		   case  KEY_DOWN_JOY_OK:	        /* ҡ��OK�� */
				 if (g_DSO.AdjustMode == 0)
				 {
					 g_DSO.AdjustMode = 1;
				 }
				 else
				 {
					 g_DSO.AdjustMode = 0;
				 }
				 OSSemPost(LCDRefresh);     /* �����ź��� ����ˢ��LCD */
				 break;

		   case  KEY_DOWN_JOY_UP:	        /* ҡ��UP������ */
				 if (g_DSO.ActiveCH == 1) 	/* ��ǰ�������CH1 */
				 {
					if (g_DSO.AdjustMode == 0)	/* ���ڷ��ȷŴ��� */
					{
						Adjust_DSO(1, 1);
					}
					else 	/* ��������ƫ�� */
					{
						g_DSO.Ch1VOffset -= 5;
					}
				 }
				 else	/* ��ǰ�������CH2 */
				 {
					 if (g_DSO.AdjustMode == 0)	/* ���ڷ��ȷŴ��� */
					 {
						Adjust_DSO(2, 1);
					 }
					 else 	/* ��������ƫ�� */
					 {
						g_DSO.Ch2VOffset -= 5;
					 }
				 }
				 OSSemPost(LCDRefresh); /* �����ź��� ����ˢ��LCD */
				 break;

		   case  KEY_DOWN_JOY_DOWN:		/* ҡ��DOWN������ */
				 if (g_DSO.ActiveCH == 1) 	/* ��ǰ�������CH1 */
				 {
					if (g_DSO.AdjustMode == 0)	/* ���ڷ��ȷŴ��� */
					{
						Adjust_DSO(1, 0);
					}
					else 	/* ��������ƫ�� */
					{
						g_DSO.Ch1VOffset += 5;
					}
				 }
				 else	/* ��ǰ�������CH2 */
				 {
					if (g_DSO.AdjustMode == 0)	/* ���ڷ��ȷŴ��� */
					{
							Adjust_DSO(2, 0);
					}
					else 	/* ��������ƫ�� */
					{
							g_DSO.Ch2VOffset += 5;
					}
				  }
				 OSSemPost(LCDRefresh); /* �����ź��� ����ˢ��LCD */
				 break;

			default:
				 break;
		 }
	  }
  }
}

/*******************************************************************************
* Function Name  : Display_DSO
* Description    : ˢ����������
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/		
void Display_DSO(void)
{
	Display_CH_Info();	/* ��ʾͨ����Ϣ(���ȣ�ʱ�䵵λ) */
    _DrawDSO();
}

/*******************************************************************************
* Function Name  : _Draw
* Description    : �ص�����
* Input          : - p: ����
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void _Draw(void * p)
{
  PARAM * pParam = (PARAM *)p;
  Display_Frame();	/* ���ƿ̶ȿ� */
  
  GUI_SetColor(GUI_LIGHTYELLOW);     
  LCD_DrawPoints( (void*)pParam->CH1x, (void*)pParam->CH1y, 300 );
  GUI_SetColor(GUI_LIGHTMAGENTA);     
  LCD_DrawPoints( (void*)pParam->CH2x, (void*)pParam->CH2y, 300 );
}

/*******************************************************************************
* Function Name  : _DrawDSO
* Description    : ��ʾ����ͼ��
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void _DrawDSO(void) 
{
  PARAM Param;	
  int16_t index;	
  int16_t iTemp;

  GUI_RECT Rect =  {  9, 19, 9 + 302, 19 + 202 };

  if (g_DSO.BufUsed == 0)
  {
	 g_DSO.BufUsed = 1;
  }
  else
  {
	 g_DSO.BufUsed = 0;
  }

  if (g_DSO.BufUsed == 0)
  {
	 Param.CH1x = g_DSO.xCh1Buf1;
	 Param.CH1y = g_DSO.yCh1Buf1;
	 Param.CH2x = g_DSO.xCh2Buf1;
	 Param.CH2y = g_DSO.yCh2Buf1;
  }
  else
  {
	 Param.CH1x = g_DSO.xCh1Buf2;
	 Param.CH1y = g_DSO.yCh1Buf2;
	 Param.CH2x = g_DSO.xCh2Buf2;
	 Param.CH2y = g_DSO.yCh2Buf2;
  }

  /* CH1ͨ��, ����0Vʱ��ADC����ֵ = 2070 */
  for (index = 0; index < 300; index++)
  {										 
	 Param.CH1x[index] = Param.CH2x[index] = 10 + index;
	 iTemp = g_DSO.Ch1VOffset - (int16_t)(( g_DSO.Ch1Buf[index + 1] - 2070 ) * 10) / g_DSO.Ch1Attenuation;

	 if (iTemp > 220)
	 {
		 iTemp = 220;
	 }
	 else if (iTemp < 20)
	 {
		 iTemp = 20;
	 }
	 Param.CH1y[index] = iTemp;

	 iTemp = g_DSO.Ch2VOffset - (int16_t)(( g_DSO.Ch2Buf[index + 1] - 2070 ) * 10) / g_DSO.Ch2Attenuation;

	 if (iTemp > 220)
	 {
		 iTemp = 220;
	 }
	 else if (iTemp < 20)
	 {
		 iTemp = 20;
	 }
	 Param.CH2y[index] = iTemp;
  }

  GUI_MEMDEV_Draw(&Rect, _Draw, &Param, 0, 0);
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


