#include "encoder.h"

/**************************************************************************
��    �ߣ��������
�Ա���ַ��https://shop119207236.taobao.com
**************************************************************************/
/**************************************************************************
�������ܣ���TIM2��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ�ܶ�ʱ��4��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PB�˿�ʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM2,0);
  TIM_Cmd(TIM2, ENABLE); 
}
/**************************************************************************
�������ܣ���TIM3��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM3(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//ʹ�ܶ�ʱ��4��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PB�˿�ʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM3,0);
  TIM_Cmd(TIM3, ENABLE); 
}

/**************************************************************************
�������ܣ���λʱ���ȡ����������
��ڲ�������ʱ��
����  ֵ���ٶ�ֵ
**************************************************************************/
int Read_Encoder(u8 TIMX)
{
   int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
		 case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	
		 default: Encoder_TIM=0;
	 }
	return Encoder_TIM;
}


