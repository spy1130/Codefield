#include "Tim.h"

void TIM_Config(uint16_t psc,uint16_t arr)
{
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);//��ʼ��
    TIM_TimeBaseStructure.TIM_Prescaler = psc;     //Ԥ��Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x00; //���벶�񣬶��ٵ������źŽ��в���һ��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;//�������ظ�����
	TIM_TimeBaseStructure.TIM_Period = arr;//�Զ���װֵ
	
	
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);//����TIME�������Ϊ����ģʽ
}
