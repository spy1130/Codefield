#ifndef __PERIPHERAL_INIT_H
#define __PERIPHERAL_INIT_H



#define QTI1_Pin   GPIO_Pin_0
#define QTI2_Pin   GPIO_Pin_1
#define QTI3_Pin   GPIO_Pin_2
#define QTI4_Pin   GPIO_Pin_3
#define QTI5_Pin   GPIO_Pin_4

#include <stdio.h>

#define s0 PCout(7)// PB5
#define led_xm PCout(6)// PB5

#define s1 PCout(10)// PE5	
#define s2 PCout(11)// PB5
#define s3 PCout(12)// PE5	
#define led PEout(15)// PB5//���ˣ�2019,09.29
#define out PEin(5)// PE5	

#define qti_1 PEin(0)// PB5
#define qti_2 PEin(1)// PE5	
#define qti_3 PEin(2)// PB5
#define qti_4 PEin(3)// PE5	
#define qti_5 PEin(4)// PB5
#define qti_6 PEin(6)// PB5
#define qti_7 PEin(7)// PB5

#define qti_8 PEin(8)// PB5
#define qti_9 PEin(10)// PB5
#define qti_10 PEin(12)// PB5
#define qti_11 PBin(0)// PB5






#define led_1 PCout(0)// PB5
#define led_2 PCout(1)// PE5	
#define led_3 PCout(2)// PB5
#define led_4 PCout(3)// PE5	
#define led_5 PCout(4)// PB5
#define led_6 PCout(5)// PB5
#define led_7 PAout(2)// PB5
#define led_8 PBout(1)// PB5
#define led_9 PDout(2)// PB5
#define led_10 PCout(13)// PB5
#define led_11 PAout(3)// PB5

void led_init(void);
	
void RCC_Configuration(void);

void GPIO_USART1_Configure(void);


void GPIO_PWM_Configuration(void);//������IO��


void GPIO_motor_Configure(void);//��е��IO����


void GPIO_QTI_Configure(void);//���⴫����IO����


void GPIO_ult_Configure(void);//������IO����


void GPIO_TCS230_1_Configure(void);//��ɫ������IO����

 void GPIO_TCS230_2_Configure(void);	//��ɫ������IO����


void GPIO_EXTI5_Configure(void);	//�ⲿ�ж�IO����


void EXTI9_5_Configure(void); //�ⲿ�ж���IO�ڹ�������


void NVIC_Configuration(void);  //�ж����� ��ע�������ù����������ⲿ�жϣ�

void GPIO_TIM3_Configure(void);

void TIM3_Configure(void);  //��ʱ���ж����ã�������ɫ������



void TIM4_Configure(void);//��ʱ��4���ã����ڳ�����������


void Tim1_Init(u16 CCR1_Val,u16 CCR2_Val,u16 CCR3_Val,u16 CCR4_Val);	//��е�֣�1��2��3��4�����Ӧ��ʱ��1��ӦPWMͨ��
 
void Tim2_Init(u16 CCR1_Val,u16 CCR2_Val,u16 CCR3_Val,u16 CCR4_Val);//��е�ֵ�����������Ӧ��ʱ��2 PWM1ͨ��


void USART1_Configuration(void);	//����1����

u8 QTI_State(u8 pin);//��ȡ����ֵ


void cfg_Init(void);//��ʼ������

/*******************************************************************************
* Function Name  : fputc
* Description    : Retargets the C library printf function to the USART.
*******************************************************************************/
int fputc(int ch, FILE *f);

#endif


