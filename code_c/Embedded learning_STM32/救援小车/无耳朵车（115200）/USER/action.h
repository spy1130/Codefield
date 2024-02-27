#ifndef __ACTION_H
#define __ACTION_H


#include <stdio.h>
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define Turn_Left 1
#define Turn_Right 2
#define Not_Found 3


#define	Fan_On	GPIO_SetBits(GPIOC, GPIO_Pin_5);	 					
#define	Fan_Off	GPIO_ResetBits(GPIOC, GPIO_Pin_5); 

//#define right_speed_adjust	  58
#define right_speed_adjust	  53
#define left_speed_adjust	  55

#define blind_time1	  6
#define blind_time2	  6


#define ON  0
#define OFF 1

#define LED_L(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_3);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_3)

#define LED_F(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_5)
#define LED_R(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_4)   
					
					
#define left 0
#define right 1
#define forward 2
					
#define left_echo GPIO_Pin_2
#define left_trig GPIO_Pin_5

#define right_echo GPIO_Pin_3
#define right_trig GPIO_Pin_2

#define front_echo GPIO_Pin_1
#define front_trig GPIO_Pin_4
						
extern char i;
extern char mark;
extern int left_value;
extern int front_value;
extern int right_value;
extern int last_value;
extern int DistTheshold;
extern int DistTheshold1;
extern int roomNumber;

int GetDis_left(int echo,int trig);
int GetDis_right(int echo,int trig);
int GetDis_front(int echo,int trig);
void walkalone_left(void);
void walkalone_right(void);
void scanValue(void);
void FindMarks(u8 mark_temp,u8 base_wall) ;

void FindMarks1(u8 mark_temp,u8 base_wall) ;
void EnterRoom(u8 room);
void PutOutFire(u8 turn_direction);
void getOut(void);
void Out(u8 out_mark);
void checkThePath(void);
void walkalone_right_1(void);
void walkalone_right_2(void);
void walkalone_left_1(void);
void walkalone_left_2(void);
void find_fire_turn_right(void);
void find_fire_turn_left(void);
void find_fire_turn_back_left(void);
void find_fire_turn_back_right(void);

u8 ID_marks(void);

u8 ID_marks1(void); 
u32 findRoom(u32 roomNumber);

void car_stop(void);

#endif
