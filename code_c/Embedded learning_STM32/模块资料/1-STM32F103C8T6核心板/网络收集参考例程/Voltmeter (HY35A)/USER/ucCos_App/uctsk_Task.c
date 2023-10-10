/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               uctsk_Task.c
** Descriptions:            The uctsk_Task application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-24
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
#include <includes.h> 
#include "SimHei.c"

/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
/* 100�������һ�β������� */
#define	 SAMPLE_COUNT	   (1000) 	 /* ������� ������������� */
#define  countof(Obj)      (sizeof(Obj) / sizeof(Obj[0]))
#define  DEG2RAD           (3.1415926f / 180)

#define  MAG 3

/* Private typedef -----------------------------------------------------------*/
typedef struct 
{
  GUI_AUTODEV_INFO AutoDevInfo; /* Information about what has to be displayed */
  GUI_POINT aPoints[7];         /* Polygon data */
  int8_t AD_value;
  int8_t limit;  
  float Angle;
}PARAM;

/* Private variables ---------------------------------------------------------*/	
static  OS_STK         App_Task_ADC_Stk	         [APP_TASK_ADC_STK_SIZE];
static  OS_STK         App_Task_Blink_Stk        [APP_TASK_BLINK_STK_SIZE];
static  OS_STK         App_Task_VOLTMETER_Stk	 [APP_TASK_VOLTMETER_STK_SIZE];

static const GUI_POINT _aNeedle[] = 
{
  { MAG * ( 0), ( MAG * 2 ) * (  0 + 125 - 58 )},
  { MAG * (-3), ( MAG * 2 ) * (-15 + 125 - 58)},
  { MAG * (-3), ( MAG * 2 ) * (-65 + 125 - 58)},
  { MAG * ( 3), ( MAG * 2 ) * (-65 + 125 - 58)},
  { MAG * ( 3), ( MAG * 2 ) * (-15 + 125 - 58)},
};

float AD_value;
uint16_t ADC_Buffer[SAMPLE_COUNT];	 /* ADC���ݻ����� */

/* Private function prototypes -----------------------------------------------*/
static void uctsk_Blink            (void);
static void uctsk_VoltMeter        (void); 
static void uctsk_ADC              (void);
static void LED_GPIO_Configuration (void);
static void ADC_Initializtion      (void);
static void ADC_SampleRate         (uint32_t freq);
static void Stop_ADC               (void);
static void Start_ADC              (void);
 
void  App_UCGUI_TaskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; 

	os_err = OSTaskCreate((void (*)(void *)) uctsk_Blink,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_Task_Blink_Stk[APP_TASK_BLINK_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_BLINK_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_BLINK_PRIO, "Task LED Blink", &os_err);
	#endif

	os_err = OSTaskCreate((void (*)(void *)) uctsk_VoltMeter,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_Task_VOLTMETER_Stk[APP_TASK_VOLTMETER_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_VOLTMETER_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_VOLTMETER_PRIO, "Task VoltMeter", &os_err);
	#endif

	os_err = OSTaskCreate((void (*)(void *)) uctsk_ADC,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_Task_ADC_Stk[APP_TASK_ADC_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_ADC_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_ADC_PRIO, "Task ADC", &os_err);
	#endif

}	

static void _Draw(void * p) 
{
  PARAM * pParam = (PARAM *)p;
  uint8_t   index;
  int x , y;
  float a;
  char ac[4];
  /* Fixed background */
  if (pParam->AutoDevInfo.DrawFixed) 
  {
    GUI_Clear();
    GUI_SetPenSize(8);
	GUI_SetColor(GUI_RED);
	GUI_AA_DrawArc( MAG * 160 , MAG * 150, MAG * 140, MAG * 140, -30 , -6);
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_DrawArc( MAG * 160 , MAG * 150, MAG * 140, MAG * 140, -6 , 6 * 24 -6 );
	GUI_SetColor(GUI_GREEN);
	GUI_AA_DrawArc( MAG * 160 , MAG * 150, MAG * 140, MAG * 140, 6 * 24 -6 , 210 );
    for( index = 0; index < 21; index++ )
    {
      a = ( -30 + index * 12 ) * DEG2RAD;
	  x = -128 * cos(a) + 160;
	  y = -128 * sin(a) + 150;
	  if( index % 2 == 0 )
	  {																						 
	     GUI_SetPenSize(8);
	  }
	  else
	  {
	     GUI_SetPenSize(4);
	  }
	  GUI_SetColor(GUI_WHITE);
	  GUI_DrawPoint( x, y );
    }
  }
  /* Moving needle */
  GUI_SetColor(GUI_WHITE);
  GUI_AA_FillPolygon(pParam->aPoints, 
                     countof(_aNeedle), 
                     MAG * 160, 
                     MAG * 150);

  GUI_SetColor(GUI_RED);
  if( pParam->limit )
  {
    GUI_DispStringHCenterAt( "+" , 160 - 20, 175 );   
  }
  else
  {
    GUI_DispStringHCenterAt( "-" , 160 - 20, 175 );   
  }
  sprintf( ac, "%d", pParam->AD_value / 10 );
  GUI_DispStringHCenterAt( ac , 160 - 7, 175 );

  sprintf( ac, "%d", pParam->AD_value % 10 );
  GUI_DispStringHCenterAt( ac , 160 + 15, 175 );

  /* Fixed foreground */
  if (pParam->AutoDevInfo.DrawFixed) 
  {	
    GUI_SetPenSize(1);
    GUI_SetColor(GUI_RED);
    GUI_SetTextMode(GUI_TM_TRANS);  
	GUI_SetFont(&GUI_FontHZ_SimHei_48);
    GUI_DispStringHCenterAt("V", 160, 110 );
    GUI_AA_DrawArc( MAG * 160 , MAG * 110, MAG * 30, MAG * 30, 0 , 360 );
    GUI_SetFont(&GUI_Font24B_ASCII);
	GUI_DispStringHCenterAt( "." , 160 + 4, 175 );
    /* ����һ��ʵ�߾��ο� x, y, h, w */
    GUI_DrawRect( 160 - 28, 175 - 12 , 160 + 24, 175 + 10);

    for( index = 0; index < 21; index++ )
    {
	  if( index % 2 == 0 )
	  {
         a = ( -30 + index * 12 ) * DEG2RAD;
	     x = -111 * cos(a) + 160;
	     y = -111 * sin(a) + 150;
	     sprintf( ac, "%d", index / 2 );
	     GUI_SetTextAlign( GUI_TA_VCENTER );
	     GUI_DispStringHCenterAt( ac, x, y );
	  }
	}
  }
}

static void uctsk_VoltMeter (void) 
{ 
  static float ADCvalue;
  PARAM Param;            /* Parameters for drawing routine */
  GUI_AUTODEV AutoDev;    /* Object for banding memory device */

  GUI_Init();
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  /* Enable high resolution for antialiasing */
  GUI_AA_EnableHiRes();
  GUI_AA_SetFactor(MAG);
  /* Create GUI_AUTODEV-objects */
  GUI_MEMDEV_CreateAuto(&AutoDev);

  for(;;)
  { 
	
	 if( AD_value > 0 )	  /* ����ѹ */
	 {
	    Param.limit = 1;			    
	 }
     else				  /* ����ѹ */		
	 {	
	    Param.limit = 0;										
        AD_value = fabs(AD_value);		      	  								    
	 }

	 if( AD_value - ADCvalue > 0.2 )	 //	ADCvalue < abs( AD_value )
	 {
		ADCvalue += 0.2;
	 }
	 else if( AD_value - ADCvalue > 0.1 )
	 {
	    ADCvalue += 0.1;
	 } 												

	 else if( AD_value - ADCvalue < -0.2 )	  //	 ADCvalue > abs( AD_value )
	 {
	    ADCvalue -= 0.2;
	 }
     else if( AD_value - ADCvalue < -0.1 )
	 {
	    ADCvalue -= 0.1;
	 }
  
	 if( ADCvalue >= 10 ) /* �������� */
	 {
	    ADCvalue = 9.9;
	 }
	 	
	 /* 6��λ�ÿ�ʼ ��ʱ�뷽�� */
     Param.Angle = ( 300 - (ADCvalue * 24) ) * DEG2RAD;
	 Param.AD_value = ( ADCvalue * 10 );

     GUI_RotatePolygon(Param.aPoints, _aNeedle, countof(_aNeedle), Param.Angle);
     GUI_MEMDEV_DrawAuto(&AutoDev, &Param.AutoDevInfo, _Draw, &Param);
	 OSTimeDlyHMSM(0, 0, 0, 30);	 /* 30 MS  */
  }
}						


float myround( float num, int i ) 
{
  return ( int ) ( num * pow(10,i) + 0.5 ) / 
         (float) ( pow(10,i) );
} 


static void uctsk_ADC (void) 
{ 
  uint16_t index;
  ADC_Initializtion();
  for(;;)
  { 
    Stop_ADC();
	AD_value = 0;
	for( index = 0; index < SAMPLE_COUNT; index++ )
	{
	   AD_value += ADC_Buffer[index];
	}  
	AD_value = AD_value / SAMPLE_COUNT;
	AD_value = AD_value - 2048;
    AD_value = AD_value / 1650 * 10 ; /* ת���ɵ�ѹ */ 
    AD_value = myround( AD_value, 1); /* �������� */
	Start_ADC();
    OSTimeDlyHMSM(0, 0, 0, 100);	  /* 100 MS  */ 	
  }
}						

static void uctsk_Blink (void) 
{   
    LED_GPIO_Configuration();              
    for(;;)
   	{     	
	  /*====LED-ON=======*/
	  GPIO_SetBits(GPIOF , GPIO_Pin_6);
	  GPIO_SetBits(GPIOF , GPIO_Pin_7);
	  GPIO_SetBits(GPIOF , GPIO_Pin_8);
	  GPIO_SetBits(GPIOF , GPIO_Pin_9);
	  OSTimeDlyHMSM(0, 0, 0, 500);	  /* 500 MS  */ 
	  /*====LED-OFF=======*/ 
	  GPIO_ResetBits(GPIOF , GPIO_Pin_6);
	  GPIO_ResetBits(GPIOF , GPIO_Pin_7);
	  GPIO_ResetBits(GPIOF , GPIO_Pin_8);
	  GPIO_ResetBits(GPIOF , GPIO_Pin_9);
      OSTimeDlyHMSM(0, 0, 0, 500);	  /* 500 MS  */	 
    }
}

/*******************************************************************************
* Function Name  : LED_GPIO_Configuration
* Description    : Configure GPIO Pin
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LED_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF , ENABLE); 						 
/**
 *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
 */					 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : Start_ADC
* Description    : ��ʼADC����
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void Start_ADC(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  DMA_DeInit(DMA1_Channel1);		                                            /* ��λDMA1�Ĵ�����ȱʡ״̬ */
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	                /* ѡ��ADC1�����ݼĴ�����ΪԴ */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Buffer;	                /* Ŀ���ַ */
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

  /* Enable DMA2 channel5 */
  DMA_Cmd(DMA1_Channel1, ENABLE);

  TIM_Cmd(TIM1, ENABLE);
}	
									
/*******************************************************************************
* Function Name  : Stop_ADC
* Description    : ��ͣADC����
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void Stop_ADC(void)
{
  TIM_Cmd(TIM1, DISABLE);

  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, DISABLE);
}

/*******************************************************************************
* Function Name  : ADC_SampleRate
* Description    : �޸Ĳ���Ƶ��
* Input          : - freq: ����Ƶ�� ��λHz 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void ADC_SampleRate(uint32_t freq)
{
  /* ���ö�ʱ��1��3ͨ��CC3��ΪADC1�Ĵ���Դ */	  
  uint16_t Period;	
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  TIM_Cmd(TIM1, DISABLE);
	
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
* Function Name  : DSO_Initializtion
* Description    : GPIO�����á�ADC�����á���ʱ���������Լ�DMA�����á�               			 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void ADC_Initializtion(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;	
  GPIO_InitTypeDef GPIO_InitStructure; 

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* ����DMA1_1 ����CH1 */
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  DMA_DeInit(DMA1_Channel1);		                                            /* ��λDMA1�Ĵ�����ȱʡ״̬ */
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	                /* ѡ��ADC1�����ݼĴ�����ΪԴ */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Buffer;	                /* Ŀ���ַ */
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

  /* ���ò���������ʱ����ʹ��TIM1 CC3 */
  ADC_SampleRate( 10000 );	 /* 10K����Ƶ�� */
}	

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
