#ifndef __SYS_H
#define __SYS_H	 

/**************************************************************************
 ��  �� ���������
 �Ա���ַ��https://shop119207236.taobao.com
 ΢�Ź��ںš���������ˡ�
 ��̨�ظ���ƽ��С��������ȡƽ��С��ȫ��DIY����
 ��̨�ظ������ӿ������ߡ�����ȡ���ӹ���ʦ�ر���������
 ��̨�ظ�������������ϡ�����ȡ����������ϰ�
 ֪���������� 
**************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "usart2.h"
#include "led.h"
#include "oled.h"
#include "key.h"
#include "exti.h"
#include "pwm.h"
#include "USB_PlugIn.h"
#include "timer.h"
#include "adc.h"
#include "timer.h"
#include "motor.h"
#include "control.h"
#include "encoder.h"
#include "mpu6050.h"
#include "mpuiic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "SR04.h"
#include "PS2.h"
////////////////////////////////////////////////////////////////////////////
#include <string.h> 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//�����Լ��ĵ��ѡ��ֻ����һ���������define��Ч
#define GA12N20
//#define GM25370
//#define TT130
//#define GB37520

#ifdef GB37520
#define BLC_KP 240
#define BLC_KD 0.75
#define SPD_KP 62
#define SPD_KI 0.31
#define TURN_KP -40
#define TURN_KD -0.6
#endif

#ifdef TT130
#define BLC_KP 360
#define BLC_KD 1.125
#define SPD_KP -72
#define SPD_KI -0.36
#define TURN_KP -40
#define TURN_KD -0.6
#endif

#ifdef GA12N20
#define BLC_KP -400
#define BLC_KD -0.75
#define SPD_KP -80
#define SPD_KI -0.4
#define TURN_KP -40
#define TURN_KD -0.6
#endif

#ifdef GM25370
#define BLC_KP 240
#define BLC_KD 0.75
#define SPD_KP -60
#define SPD_KI -0.3
#define TURN_KP -40
#define TURN_KD -0.6
#endif
//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

/////////////////////////////////////////////////////////////////
//Ex_NVIC_Configר�ö���
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6 

#define FTIR   1  //�½��ش���
#define RTIR   2  //�����ش���

//JTAGģʽ���ö���
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00	



extern float Voltage;  													//��ص�ѹ������صı���
extern float pitch,roll,yaw; 										//ŷ����
extern short aacx,aacy,aacz;										//���ٶȴ�����ԭʼ����
extern short gyrox,gyroy,gyroz;									//������ԭʼ����
extern u8 CTRL_MODE,Mode_Change;      			//ģʽ
extern int Uart_Receive;
extern int   Encoder_Left,Encoder_Right;        //���ұ��������������
extern int 	 Moto1,Moto2;										    //������������ո��������PWM
extern u8 Fore,Back,Left,Right;
extern u8 TkSensor;
extern float SR04_Distance;
extern int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY; //PS2��ر��� 
extern u8 PS2_Plugin;
extern float Target_Speed,Turn_Speed;	//�����ٶȣ�����/ƫ����

void NVIC_Configuration(void);//�ж����ȼ�����
void Tracking_Init(void);
void Mode_Init(void);
#endif

