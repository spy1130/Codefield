#include "timer.h"
#include "led.h"
#include "usart.h"
#include "stdio.h"
#include <string.h>
#include "main.h"
#include "cfg.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTE��ӢSTM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,����TIM3_PWM_Init������
//2,����LED0_PWM_VAL�궨�壬����TIM3_CH2����									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
volatile int counter_t2=0;//1#
volatile int counter_t3=0;//2#
volatile int counter_t4=0;//3#
volatile int counter_t1=0;//4#
volatile int counter_t8=0;//5#

volatile int over_flow_t2=0;
volatile int over_flow_t3=0;
volatile int over_flow_t4=0;
volatile int over_flow_t1=0;
volatile int over_flow_t8=0;

volatile int black_frequency_1=0;
volatile int black_frequency_2=0;
volatile int black_frequency_3=0;
volatile int black_frequency_4=0;
volatile int black_frequency_5=0;

volatile int display_flag=0x00;




extern int tim5_interruput_flag;
extern int tim7_interruput_flag;

extern  short YAW,PITCH,ROLL;
extern  u8 rx_buff[20];
extern  u8 rx_end_flag;//1 is mean that serious port  recieve a good frame,0 mean that is recieving ;
extern  u8 rx_star_flag;//1 mean the 0xa5 has already recieved,0 mean that we are waiting for the beginning of series port

extern  int colour_flag;

int motor_in1=0x00;
int motor_in2=0x00;
int motor_in3=0x00;
int pwm_num=0x00;
int room_2_pwm_num=0x00;

int pwm_num_distance=0x00;

int motor_in_old=0x01;

int motor_count=0x00;
int key_count=0x00;
int key_value=0x00;
int key_last_value=0x00;

int counter_50ms=0x00;
int time_1s_flag=0x01;

char new_key_flag=0x00;

char temp_1=0x00;
char temp_2=0x00;
char temp_3=0x00;
char temp_4=0x00;
char temp_5=0x00;
char temp_6=0x00;
char temp_7=0x00;
char temp_8=0x00;
char temp_9=0x00;
char temp_10=0x00;
char temp_11=0x00;


int tim5_over_count=0x00;
int tim7_over_count=0x00;

float yaw_0=0;










void speak_context_2(u8 buf_xm[],int speak_len )
{
	//speak_len=(sizeof(buf_xm)/sizeof(buf_xm[0]));
	int Send_Count,speak_i;
	char DataScope_OutPut_Buffer[20];
	Send_Count = speak_len+5;

	DataScope_OutPut_Buffer[0]=0xfd;
	DataScope_OutPut_Buffer[1]=0x00;
	DataScope_OutPut_Buffer[2]=speak_len+2;
	
	DataScope_OutPut_Buffer[3]=0x01;
	DataScope_OutPut_Buffer[4]=0x00;//01 gbk //00 gb2312
	
	
	//˵������
		
	for(speak_i=0;speak_i<speak_len;speak_i++)
	{
		DataScope_OutPut_Buffer[speak_i+5]=buf_xm[speak_i];
	}
	
	

	for( speak_i = 0 ; speak_i < Send_Count; speak_i++) 
	{
		while((USART2->SR&0X40)==0);  
		USART2->DR = DataScope_OutPut_Buffer[speak_i]; 
	}
}



//��ʱ��2����1#ͨ������ 
void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	
	TIM_Cmd(TIM2, DISABLE);  //ʹ��TIM4 DISABLE
	
	TIM_SetCounter(TIM2, 0);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
	
	 
}
/////////////////////




////////////////
//��ʱ��2�����������
void TIM2_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
				//LED0=!LED0;
				key_scan();
				TIM_SetCounter(TIM2, 0);
				TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
			
				TIM_Cmd(TIM2, ENABLE);
			  
			
		}
}







////��ʱ��3����2#ͨ������
void TIM3_Int_Init(u16 arr,u16 psc)
{
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

   //??TIMER3 ����Ϊ������
//   TIM_DeInit(TIM3);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ


	TIM_ETRClockMode2Config(TIM3, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
	TIM_SetCounter(TIM3, 0);
	

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	
	TIM_Cmd(TIM3, ENABLE);
							 
}
//
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer3��ӳ��  TIM3_CH2->PC7   TIM3_CH1->PC6    
 
	//�����c6��c7 tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	//�����c6��c7 tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	//�����c6��c7 tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	
   //���ø�����Ϊ�����������,���TIM3 CH1��PWM���岨��	GPIOC.8->ch1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO
	
	//���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOC.9->ch2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO
	
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	
	
	
	//��ʼ��TIM3 Channel 3 PWMģʽ
	//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse=900;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	
	//��ʼ��TIM3 Channel 4 PWMģʽ	 
		//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
//	TIM_OCInitStructure.TIM_Pulse=900;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���,д��ccr��ֵ������Ч�����enable����������ʱ���ccrֵ�����£��Ȳ���������Ч

	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	

}
//��ʱ��3�����������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
				TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
				
				TIM_SetCounter(TIM3, 0);
				TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
			
				TIM_Cmd(TIM3, ENABLE);
			
				pwm_num++;
				
				pwm_num_distance++;
				
				room_2_pwm_num++;
			 
			
		}
}






////��ʱ��4����3#ͨ������
void Tim4_Init(void)
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

   //??TIMER3 ����Ϊ������
//   TIM_DeInit(TIM4);
	
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ARRPreloadConfig(TIM4,DISABLE);  
	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4,DISABLE); 
							 
}
//��ʱ4�����������

////////////
//
////////////////
void TIM4_Cap_Init(unsigned int arr,unsigned int psc)
{ 
        //��Щ����һЩ�ṹ���������ȥ����һ���⺯���ֲ�һ����֪�� �ܼ򵥵� 
		  GPIO_InitTypeDef GPIO_InitStructure;
		  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		  TIM_ICInitTypeDef  TIM4_ICInitStructure;
		  NVIC_InitTypeDef NVIC_InitStructure;


        //�������ʱ��ʹ���� ��ʹ��TIM4 ��GPIOB
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
			GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //Timer3��ӳ��  TIM3_CH2->PC7   TIM3_CH1->PC6    

        //���벶׽����Ȼ�˿�Ҫ����Ϊ����ģʽ��
			GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
			GPIO_Init(GPIOD, &GPIO_InitStructure);
//			GPIO_ResetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);

			TIM_TimeBaseStructure.TIM_Period = arr;  //����������ü���ֵ�����ֵ����������Ĵ����ܼƶ��ٸ�����һ�����ó�0XFFFF
			TIM_TimeBaseStructure.TIM_Prescaler =psc;  //������������Ĵ�����Ƶ�ʵķ�Ƶϵ�� ��Ƶ��ֵ=psc+1 ������Ҫ2��Ƶ�Ļ���psc= 1���Դ�����
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷָ� ������һЩ����ã��������ǲ���Ӱ�� 
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //�������ϼ��� 
			TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);  //��ʼ��
			  
			TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1;             //ѡ��ͨ��1
			TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //�����ش��������½��ش���������
			TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TT1��
			TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;        //���������Ƶ ����Ƶ
			TIM4_ICInitStructure.TIM_ICFilter = 0x01;                        //�˲������� �Ȳ���
			TIM_ICInit(TIM4, &TIM4_ICInitStructure);

			TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2;               
			TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 
			TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
			TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;       
			TIM4_ICInitStructure.TIM_ICFilter = 0x01; 
			TIM_ICInit(TIM4, &TIM4_ICInitStructure);

			TIM4_ICInitStructure.TIM_Channel = TIM_Channel_3;                
			TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;  
			TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
			TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;       
			TIM4_ICInitStructure.TIM_ICFilter = 0x01;    
			TIM_ICInit(TIM4, &TIM4_ICInitStructure);



			//�ж�����
			NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�����ж� ��ʵ�������淢�������������غ��½��ش����¼�
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ�жϵȼ�
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //���жϵȼ�
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ж�ͨ����
			NVIC_Init(&NVIC_InitStructure);  //��ʼ�� 

			TIM_ITConfig(TIM4,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_Update,ENABLE);//ʹ�ܸ����ж�

					TIM_Cmd(TIM4,ENABLE ); //ʹ�ܶ�ʱ��TIM4
			}


////////////
void TIM4_IRQHandler(void)   //TIM3�ж�
{
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
				//LED0=!LED0;
				TIM_SetCounter(TIM4, 0);
				TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
			
				TIM_Cmd(TIM4, ENABLE);
				over_flow_t4++;
			
		}
		
		if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) //CH1ͨ����PB6�����������¼�
	   { 
		 /* ������� **/
		}

	  if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET) //CH2ͨ����PB7�����������¼�
	   { 
		 /* ������� **/
		}


	   if(TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET) //CH3ͨ����PB8�����������¼�
	   {  
		   /* ������� **/
		}

}







//��ʱ��5����̶�����ʱ������
void TIM5_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	
	TIM_Cmd(TIM5, DISABLE);  //ʹ��TIM4 DISABLE

	TIM_SetCounter(TIM5, 0);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	TIM_Cmd(TIM5, ENABLE);
	
	
	
}
//��ʱ��5�жϷ������ 0.1s period
void TIM5_IRQHandler(void)   //TIM5�ж�
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
			
			TIM_Cmd(TIM5, DISABLE);  //ʹ��TIM5 DISABLE		
				
//			counter_t2=TIM_GetCounter(TIM2);
//			black_frequency_1=20*(counter_t2+over_flow_t2*65539);
			
			
			led_xm=!led_xm;
				
			tim5_over_count++; 
			
			tim5_interruput_flag=0x01;
			
			TIM_SetCounter(TIM5, 0);
			TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
			TIM_Cmd(TIM5, ENABLE);
			
		
		}
}






void TIM6_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	
	TIM_Cmd(TIM6, DISABLE);  //ʹ��TIM4 DISABLE
	
	
	
	
	
	
	TIM_SetCounter(TIM6, 0);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	TIM_Cmd(TIM6, ENABLE);
							 
}


void  TIM7_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //ʱ��ʹ��
	
	TIM_DeInit( TIM7);//��λTIM3��ʱ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	//TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_ClearFlag(TIM7,TIM_FLAG_Update);
//	TIM_Cmd(TIM7, DISABLE);  //ʹ��TIM4 DISABLE
	
	
	
	
	
	
	TIM_SetCounter(TIM7, 0);
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	TIM_Cmd(TIM7, ENABLE);
							 
}









void display_led(void)
{
//	char temp_1_a=0x00;
//	char temp_2_a=0x00;
//	char temp_3_a=0x00;
//	char temp_4_a=0x00;
//	char temp_5_a=0x00;
//	char temp_6_a=0x00;
//	char temp_7_a=0x00;
//	char temp_8_a=0x00;
//	char temp_9_a=0x00;

	temp_1=qti_1;
	temp_2=qti_2;
	temp_3=qti_3;
	temp_4=qti_4;
	temp_5=qti_5;
	temp_6=qti_6;
	temp_7=qti_7;
	temp_8=qti_8;
	temp_9=qti_9;
	temp_10=qti_10;
	temp_11=qti_11;
	
//	if(temp_1==0x01)
//	{
//		temp_1_a=0x00;
//	}
//	if(temp_1==0x00)
//	{
//		temp_1_a=0x01;
//	}
//	
//	if(temp_2==0x01)
//	{
//		temp_2_a=0x00;
//	}
//	if(temp_2==0x00)
//	{
//		temp_2_a=0x01;
//	}
//	
//	if(temp_3==0x01)
//	{
//		temp_3_a=0x00;
//	}
//	if(temp_3==0x00)
//	{
//		temp_3_a=0x01;
//	}
//	
//	if(temp_4==0x01)
//	{
//		temp_4_a=0x00;
//	}
//	if(temp_4==0x00)
//	{
//		temp_4_a=0x01;
//	}
//	
//	if(temp_5==0x01)
//	{
//		temp_5_a=0x00;
//	}
//	if(temp_5==0x00)
//	{
//		temp_5_a=0x01;
//	}
//	
//	if(temp_6==0x01)
//	{
//		temp_6_a=0x00;
//	}
//	if(temp_6==0x00)
//	{
//		temp_6_a=0x01;
//	}
//	
//	if(temp_7==0x01)
//	{
//		temp_7_a=0x00;
//	}
//	if(temp_7==0x00)
//	{
//		temp_7_a=0x01;
//	}
//	
//	
//	if(temp_8==0x01)
//	{
//		temp_8_a=0x00;
//	}
//	if(temp_8==0x00)
//	{
//		temp_8_a=0x01;
//	}
//	
//	
//	if(temp_9==0x01)
//	{
//		temp_9_a=0x00;
//	}
//	if(temp_9==0x00)
//	{
//		temp_9_a=0x01;
//	}
	
	
	
//	led_1=temp_1_a;
//	led_2=temp_2_a;
//	led_3=temp_3_a;
//	led_4=temp_4_a;
//	led_5=temp_5_a;
//	led_6=temp_6_a;
//	led_7=temp_7_a;
//	led_8=temp_8_a;
//	led_9=temp_9_a;
	
	led_1=temp_1;
	led_2=temp_2;
	led_3=temp_3;
	led_4=temp_4;
	led_5=temp_5;
	led_6=temp_6;
	led_7=temp_7;
	led_8=temp_8;
	led_9=temp_9;
	led_10=temp_10;
	led_11=temp_11;


}
///////////////////////////////////////////////////
//
//////////////////////////////////////////////
void show_gy25(void)
{
	float tempxx=0;
	char buf1_xm[10];
	
	if(rx_end_flag==1)//PITCH,ROLL;
	{
		
		
		memset(buf1_xm, 0,sizeof(buf1_xm));
		if(YAW>=0)
		{
			tempxx=YAW/100.0+180;
			sprintf(buf1_xm,"+%-5.2f",tempxx);
			
		}
		else
		{
			tempxx=YAW/100.0+180;
			sprintf(buf1_xm,"%-5.2f",tempxx);
		}		
		
		
		rx_end_flag=0;

	}
}


/////////////////////////////////////////
//
/////////////////////////////////////////
	
//��ʱ��6�жϷ������ 0.1s period
void TIM6_IRQHandler(void)   //TIM5�ж�
{
	
	
	
	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
//			LED3=!LED3;
			TIM_Cmd(TIM6, DISABLE);  //ʹ��TIM5 DISABLE		
			

			if(colour_flag==0)
			{
				display_led();
			
				 key_scan();
				
				show_gy25();
				
				
				while((USART2->SR&0X40)==0);  
				USART2->DR = 0xa5; 
				
				while((USART2->SR&0X40)==0);  
				USART2->DR = 0x51; 
			}
			
	
			
			TIM_SetCounter(TIM6, 0);
			TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
			TIM_Cmd(TIM6, ENABLE);
			
			
		
		}
}


/////////////////////////////////////////
//
/////////////////////////////////////////
	
//��ʱ��6�жϷ������ 0.1s period
void TIM7_IRQHandler(void)   //TIM5�ж�
{
	
	
	
	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
//			LED3=!LED3;
			TIM_Cmd(TIM7, DISABLE);  //ʹ��TIM5 DISABLE		
			

			
			led_xm=!led_xm;
				
			tim7_over_count++; 
			
			
			tim7_interruput_flag=0x01;
			
	
			
			TIM_SetCounter(TIM7, 0);
			TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
			TIM_Cmd(TIM7, ENABLE);
			
			
		
		}
}
////////////////////////////////
//
////////////////////////////////
//
///////////////////////////////////
void key_scan(void)
{
	char buf1[30]={"�Լ� "};//\[b1] ��ӭ���٣����

//////////////
//foward photo or backward photo
//////////////////////
	key_value=key_in;
   switch (key_count)
   {
   	case 0:
		
		if(key_value==0x01)
		{
			key_count=0x00;
		}
		else
		{
			key_count=0x01;
		}
		
   		break;
   	case 1:
		if(key_value==0x01)
		{
			key_count=0x00;
		}
		else
		{
			key_count=0x02;
		}
   		break;
	case 2:
		if(key_value==0x01)
		{
			key_count=0x00;
		}
		else
		{
			new_key_flag=0x01;
			sprintf(buf1,"��ʼ");
			key_count=0x03;
			
			
			yaw_0=YAW/100.0+180;
		}
		
		
   		break;
		
	case 3:
		if(key_value==0x01)
		{
			key_count=0x00;
		}
		
		
		
   		break;
   	default:
		key_count=0x00;
		
   		break;
   }
	
	
}





////�߼���ʱ��1����4#ͨ������
void TIM1_Int_Init(u16 arr,u16 psc)
{
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��


//   TIM_DeInit(TIM1);

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseStructure.TIM_RepetitionCounter=0; 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ


	TIM_ETRClockMode2Config(TIM1, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
	TIM_SetCounter(TIM1, 0);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM1�ж�,��������ж�
	
	TIM_Cmd(TIM1, ENABLE);
							 
}
//��ʱ��4�����������
void TIM1_UP_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
				TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
				//LED0=!LED0;
				TIM_SetCounter(TIM1, 0);
				TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
			
				TIM_Cmd(TIM1, ENABLE);
			  over_flow_t1++;
			
		}
}


////�߼���ʱ��8����5#ͨ������
void TIM8_Int_Init(u16 arr,u16 psc)
{
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //ʱ��ʹ��

//  TIM_DeInit(TIM8);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ


	TIM_ETRClockMode2Config(TIM8, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
	TIM_SetCounter(TIM8, 0);


	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	
	TIM_Cmd(TIM8, ENABLE);
							 
}
//��ʱ��8�����������
void TIM8_UP_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
				TIM_ClearITPendingBit(TIM8, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
				//LED0=!LED0;
				TIM_SetCounter(TIM8, 0);
				TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
			
				TIM_Cmd(TIM8, ENABLE);
			  over_flow_t8++;
			
		}
}
































