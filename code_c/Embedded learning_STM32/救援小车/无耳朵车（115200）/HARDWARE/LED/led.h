#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PDout(7)// PB5
#define LED1 PEout(5)// PE5	


#define s0_1 PAout(2)// PA6
#define s1_1 PAout(3)// PA7	

#define s0_2 PAout(4)// PA6
#define s1_2 PAout(5)// PA7	

#define s0_3 PAout(6)// PA6
#define s1_3 PAout(7)// PA7	

#define s0_4 PCout(0)// PA7	
#define s1_4 PCout(1)// PA7	

#define s0_5 PCout(2)// PA7	
#define s1_5 PCout(3)// PA7	

#define LED3 PAout(8)// PA8	

#define light_check_1 PAin(11)// PA11
#define light_check_2 PCin(6)// PC6
#define light_check_3 PCin(7)// PC7

#define motor_in PDin(3)// PA7	
#define motor_out PDout(4)// PA7

#define key_in PAin(1)// PA0 

///////////////
#define LeftFire PDin(10)// PA7	//LeftFire==0&&RightFire
#define RightFire PDin(11)// PA7

void LED_Init(void);//��ʼ��
void check_io_ini(void);
void tlr230_s0_s1_ini(void);
void step_motor_ini(void);
void Delay_Init(void);
		 				    
#endif
