

#include "stm32f10x.h"
#include <stdio.h>
#include "cfg.h"
#include "delay.h"

/*���ļ�Ϊ����������*/

#define Trig_Write_1()     GPIO_SetBits(GPIOD,GPIO_Pin_10) //д1
#define Trig_Write_0()     GPIO_ResetBits(GPIOD,GPIO_Pin_10) //д0
#define Echo_Read()				GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) //����ƽ
volatile u16 times=0;  //����
u16 dis;

u16 Get_Distance(void) //���������
{
	u16 distance=0;
	
//	TIM_SetCounter(TIM4,0);//�������
    TIM4_Configure();
	
	Trig_Write_1();//10us��������
	delay_us(10);
	Trig_Write_0();
	
	while(Echo_Read() == 0);//�ȴ�������
	TIM_Cmd(TIM4, ENABLE);//������ʱ��
	while(Echo_Read() == 1);//�ȴ��½���
	TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��
	distance = TIM_GetCounter(TIM4);//��ȡ������ֵ����λ1us
	
//	dis = distance/60.085;//ת���ɾ���
	dis = distance/5.88;
	return dis;
}





