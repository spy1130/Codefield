//#include "led.h"

////////////////////////////////////////////////////////////////////////////////////	 
////������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
////ALIENTEK ��ӢSTM32������
////LED��������	   
////����ԭ��@ALIENTEK
////������̳:www.openedv.com
////�޸�����:2012/9/2
////�汾��V1.0
////��Ȩ���У�����ؾ���
////Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
////All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////// 	   

#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   
/////////////////////////////////////////
void Delay_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PORTG��ʱ�� 
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//PORTG.11 �������
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOD,GPIO_Pin_8);    //���1
}











///////////////////////
//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
//	 //led pd7	
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //LED0-->PB.5 �˿�����
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//	 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
//	 GPIO_SetBits(GPIOD,GPIO_Pin_7);						 //PB.5 �����
//	
//	//trig pa4 pa5
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
//	 
//	//trig pd2
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //LED0-->PB.5 �˿�����
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//	 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
//	 
//	 //echo pa1  pa2  pa3
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;				 //LED0-->PB.5 �˿�����
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //�������
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
//	 
//	 //fire sense pb0,pb1
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;				 //LED0-->PB.5 �˿�����
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //�������
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//	 GPIO_Init(GPIOD, &GPIO_InitStructure);		

//	//motor pc5
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//	 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	 
	  //start key pc0
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PB.5 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	 GPIO_SetBits(GPIOA,GPIO_Pin_0); 						 //Pc0 ������pc0 ��λ1 ������ ��λ0 ������������Ҫ������������λ1��
	 
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 �˿�����, �������
//	 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
//	 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 ����� 

}


void check_io_ini(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
	//GPIOC  4,5,7��ʼ������
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //LED0-->PA.11 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //fukong����
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //LED0-->PA.11 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //fukong����
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //�����2#3#
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		//fukong����
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	
	
	
	//GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10���øߣ�����

}

void tlr230_s0_s1_ini(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
		
		
		
		//��Ȧ100�����ƶ˿�a6,a7
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2;				 //LED0-->PB.5 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	
	 //default s1,s0=0,1,mean *1,11 means *100;
		s0_1=1;
		s1_1=0;
		
		s0_2=1;
		s1_2=0;
		
		s0_3=1;
		s1_3=0;
		
		s0_4=1;
		s1_4=0;
		
		s0_5=1;
		s1_5=0;
		
		
		
		
	 
	 
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//tim4 etr pe0
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	 GPIO_Init(GPIOE, &GPIO_InitStructure); 
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//tim3 etr pd2 
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	 GPIO_Init(GPIOD, &GPIO_InitStructure); 
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//tim1 etr pa12
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	 GPIO_Init(GPIOA, &GPIO_InitStructure); 
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //tim8 etr pa0
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}



void step_motor_ini(void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
 	
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
		
		
		
		//GPIOD  3,
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//tim3 etr pd2 
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	 GPIO_Init(GPIOD, &GPIO_InitStructure); 
     GPIO_ResetBits(GPIOD,GPIO_Pin_3);//resetBits means pull down ;setBits means pull up
	//GPIOD  4,
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
	
	
	//GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10���øߣ�����
	
}
















