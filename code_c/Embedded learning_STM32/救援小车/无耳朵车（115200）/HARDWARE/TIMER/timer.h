#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
void TIM2_Int_Init(u16 arr,u16 psc);//TIM2_Int_Init
void TIM3_Int_Init(u16 arr,u16 psc);
void Tim4_Init(void);//TIM2_Int_Init

void TIM5_Int_Init(u16 arr,u16 psc);//TIM2_Int_Init  TIM6_Int_Init
void TIM6_Int_Init(u16 arr,u16 psc);//TIM2_Int_Init  
void TIM1_Int_Init(u16 arr,u16 psc);
void TIM8_Int_Init(u16 arr,u16 psc);

void TIM3_PWM_Init(u16 arr,u16 psc);

void display_led(void);

void dis_gy_25(void);

void  TIM7_Int_Init(u16 arr,u16 psc);


void key_scan(void);
#endif
