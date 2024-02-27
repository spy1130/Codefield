#ifndef __RUN_H
#define __RUN_H

#include "delay.h"
#include "cfg.h"
#include "stdbool.h"
#include "ult.h"
#include <string.h>
#include "main.h"
#include "run.h"
#include "tcs.h"
#include <math.h>

#define leftservo    GPIO_Pin_6
#define rightservo   GPIO_Pin_7

#define axis_forward 0
#define axis_turn_left 1
#define axis_turn_right 2

/*���ļ�Ϊ�����ֵ�������к��������������ֵ����ж���*/
//ע�������ֵ���������һ��������ͬ������������ת����ת�ĽǶ��ǲ�һ����
//��ֱǰ�����ˣ������ֵĸߵ�ƽʱ��Ҳ������ͬ
extern int led0pwmval;;

extern int pwm_num;
extern int speak_len,speak_i,s_i;

extern int speak_len1;//(sizeof(buf_xm)/sizeof(buf_xm[0]));
extern char buf1[30];//\[b1] ��ӭ���٣����
extern char car_head_flag;
extern char walk_ae_state;

extern 	 u8  gy_5_flag;
extern  short YAW,PITCH,ROLL;
extern  u8 rx_buff[20];
extern  u8 rx_end_flag;//1 is mean that serious port  recieve a good frame,0 mean that is recieving ;
extern  u8 rx_star_flag;//1 mean the 0xa5 has already recieved,0 mean that we are waiting for the beginning of series port

extern int pwm_num_distance;
extern float yaw_0;

extern char grasp_order[20];
extern char grasp_order_index;
extern char block_colour_order[20];
    
extern int a_point_top_down[30];
extern int a_point_top_up[30];
extern int k_num;
extern int servo_num_1_pwmval;

extern int count_point_a;
extern int count_point_c;
extern int count_point_e;

extern int count_stack_yellow;
extern int count_stack_white;
extern int count_stack_red;
extern int count_stack_black;
extern int count_stack_blue;


extern int count_stack_tempa;
extern int count_stack_tempb;
extern int count_stack_tempc;


extern int colour_tempa;
extern int colour_tempb;
extern int colour_tempc;

extern int put_down_bottle_param[20];

extern int put_down_middle_param[20];

extern int put_down_top_param[20];

extern int a_point_middle_down[30];

extern int a_point_bottle_down[30];

extern int turn_num;

extern int turn_left_flag;

unsigned int i=0;


int qti_2_flag=0x00;
int qti_3_flag=0x00;

int axis_turn_left_count=0x00;
int axis_turn_right_count=0x00;

int axis_dirrection=0x00;

int head_i=0;
int tail_i=0;
int car_desicion=0;

// ԭʼ��
//int matrix_decision_head_tail[5][5]={{0,3,-3,-7,-3},{-3,0,-7,-10,-7},{3,7,0,-3,3}, {7,10,3,0,7},{3,7,-3,-7,0}};
 //�Ż���
 //int matrix_decision_head_tail[5][5]={{3,3,-3,-7,0},{0,3,-7,-10,0},{3,7,-3,-7,0},{7,10,0,-3,0},{3,7,-3,-7,0}};

//˳������Ϊ19��21//19����ͷ��11�㷽��
//21����ͷ��1�㷽��������������Ҫ�ҹ�������̬����������С����Ҫ��ת��������̬
 int matrix_decision_head_tail[5][5]={{3,3,-3,-7,21},{21,3,-7,-10,21},{3,7,-3,-7,19},{7,10,19,-3,19},{3,7,-3,-7,0}};
	 
	
int matrix_decision_head[5]={0,3,-3,-7,-3};

int matrix_decision_tail[5]={0,-3,3,7,3};

int first_start_state_1=0x00;

int first_axis_qit=0;

char dirrection_array[4096]={0};
int dirrection_index=0x00;

float temp_180=0,temp_180_1=0;

void PulseOut(uint8_t pin,int speed) 
{
	GPIO_SetBits(GPIOC,pin );
	delay_us(speed);
	GPIO_ResetBits(GPIOC,pin);
}

void tiaoling(void)	//����
{
    
		while(1)
		{
		GPIO_SetBits(GPIOC, GPIO_Pin_6);
    delay_us(1500);
    GPIO_ResetBits(GPIOC,GPIO_Pin_6);
    delay_ms(20);
	
		GPIO_SetBits(GPIOC, GPIO_Pin_7);
		delay_us(1500);
		GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		delay_ms(20);
		}
}

void fastforward(void)//����ǰ��
{
//    GPIO_SetBits(GPIOC, GPIO_Pin_6);
//    delay_us(1550);
//    GPIO_ResetBits(GPIOC,GPIO_Pin_6);
//    	
//	GPIO_SetBits(GPIOC, GPIO_Pin_7);
//	delay_us(1450);
//	GPIO_ResetBits(GPIOC,GPIO_Pin_7);

//	delay_ms(20);	
	//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	led0pwmval=1500+50+1;//1550;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500-50;//1450;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	
}


void fastforward_xm(void)//����ǰ��
{
//    GPIO_SetBits(GPIOC, GPIO_Pin_6);
//    delay_us(1550);
//    GPIO_ResetBits(GPIOC,GPIO_Pin_6);
//    	
//	GPIO_SetBits(GPIOC, GPIO_Pin_7);
//	delay_us(1450);
//	GPIO_ResetBits(GPIOC,GPIO_Pin_7);

//	delay_ms(20);	
	//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	led0pwmval=1500+48;//1550;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500-50;//1450;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	
}

void slowforward(void)//����ǰ��
{
    GPIO_SetBits(GPIOC, GPIO_Pin_6);
    delay_us(1522);
    GPIO_ResetBits(GPIOC,GPIO_Pin_6);
    	
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	delay_us(1478);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);

//	delay_ms(20);	
}
//////////////////////
//
////////////////////////
/////////////////////
void car_left_turn(void)//��ת
{

//	
//	//-10��---30��;-7---20;3---10
//	led0pwmval=1500+35+5+2;//high mean low speed
//	 TIM_SetCompare3(TIM3,led0pwmval);
//	 
//	 led0pwmval=1500-50;//high mean low speed
//	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	//-10��---30��;-7---20;3---10
	led0pwmval=1500+42;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500-50-20;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
}



//////////////////////
//
////////////////////////
/////////////////////

void car_right_turn(void) //��ת
{
//	int i;
//    for(i=1;i<=2;i++)
//	{
//		GPIO_SetBits(GPIOC, GPIO_Pin_6);
//    	delay_us(1519);
//    	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
//    	
//		GPIO_SetBits(GPIOC, GPIO_Pin_7);
//		delay_us(1519);
//		GPIO_ResetBits(GPIOC,GPIO_Pin_7);

//		delay_ms(20);
//	}
	
	
	 led0pwmval=1500+70+50+20;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500-30-5;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
}


//////////////////////
//
////////////////////////
/////////////////////
void car_left_turn_big(void)//��ת
{
//	int i;
//    for(i=1;i<=2;i++)
//	{
//		GPIO_SetBits(GPIOC, GPIO_Pin_6);
//    	delay_us(1480);
//    	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
//    	
//		GPIO_SetBits(GPIOC, GPIO_Pin_7);
//		delay_us(1480);
//		GPIO_ResetBits(GPIOC,GPIO_Pin_7);

//		delay_ms(20);
//	}
	
	
	led0pwmval=1500+35+7;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500-50-30;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
}



//////////////////////
//
////////////////////////
/////////////////////



void car_right_turn_big(void) //��ת
{
//	int i;
//    for(i=1;i<=2;i++)
//	{
//		GPIO_SetBits(GPIOC, GPIO_Pin_6);
//    	delay_us(1519);
//    	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
//    	
//		GPIO_SetBits(GPIOC, GPIO_Pin_7);
//		delay_us(1519);
//		GPIO_ResetBits(GPIOC,GPIO_Pin_7);

//		delay_ms(20);
//	}
	
	
	 led0pwmval=1500+50;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500-30;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
}

void left_turn(void)//��ת
{
//	int i;
//    for(i=1;i<=2;i++)
//	{
//		GPIO_SetBits(GPIOC, GPIO_Pin_6);
//    	delay_us(1480);
//    	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
//    	
//		GPIO_SetBits(GPIOC, GPIO_Pin_7);
//		delay_us(1480);
//		GPIO_ResetBits(GPIOC,GPIO_Pin_7);

//		delay_ms(20);
//	}
	
	
	led0pwmval=1530;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1450;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
}


void right_turn(void) //��ת
{
//	int i;
//    for(i=1;i<=2;i++)
//	{
//		GPIO_SetBits(GPIOC, GPIO_Pin_6);
//    	delay_us(1519);
//    	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
//    	
//		GPIO_SetBits(GPIOC, GPIO_Pin_7);
//		delay_us(1519);
//		GPIO_ResetBits(GPIOC,GPIO_Pin_7);

//		delay_ms(20);
//	}
	
	
	 led0pwmval=1500+50;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500-30;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
}



void car_stop(void)
{
	//tim3 1# is right servo
	led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
}

void GoBack(void) //������
{
	PulseOut(leftservo,1480);
	PulseOut(rightservo,1520);
	delay_ms(20);
}

void stop(void)//ֹͣ
{
	int i;
	for(i=1;i<=5;i++)
	{
    	GPIO_SetBits(GPIOC, GPIO_Pin_6);
    	delay_us(1500);
    	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
    	
		GPIO_SetBits(GPIOC, GPIO_Pin_7);
		delay_us(1500);
		GPIO_ResetBits(GPIOC,GPIO_Pin_7);

		delay_ms(20);
	}	
}


void SpinLeft(void)// ����ת
{
	PulseOut(leftservo,1478);
	PulseOut(rightservo,1474);
	delay_ms(20);
}

void SpinRight(void)// ����ת
{
	PulseOut(leftservo,1530);
	PulseOut(rightservo,1524);
	delay_ms(20);
}

void TurnLeftAnyPulse(int pulses)//����תĳ������
{
	while(pulses--)
	{
		SpinLeft();
		delay_ms(2);
	}
}
void TurnRightAnyPulse(int pulses) //����תĳ������
{
	while(pulses--)
	{
		SpinRight();
		delay_ms(2);
	}
}

void Crossblk(int pulses)//����ǰ����������
{
	while(pulses--)
	{
		slowforward();
		delay_ms(2);
	}
}

bool IsTailQtiBlack(void)//�����������
{
	if((QTI_State(QTI5_Pin))==true)
	{
		delay_ms(2);
		if((QTI_State(QTI5_Pin))==true)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return true;
	}
}

/*
 * //����м���QTI�Ƿ��ں�����
 */
bool IsMLeftQtiBlack(void)
{
	if(QTI_State(QTI2_Pin)==false)
	{
		delay_ms(2);
		if(QTI_State(QTI2_Pin)==false)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
/*
 * //����м���QTI�Ƿ��ں�����
 */
bool IsMRightQtiBlack(void)
{
	if(QTI_State(QTI3_Pin)==false)
	{
		delay_ms(2);
		if(QTI_State(QTI3_Pin)==false)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
/*
 * //���������QTI�Ƿ��ں�����
 */
bool IsELeftQtiBlack(void)
{
	if(QTI_State(QTI1_Pin)==false)
	{
		delay_ms(2);
		if(QTI_State(QTI1_Pin)==false)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
/*
 * //���������QTI�Ƿ��ں�����
 */
bool IsERightQtiBlack(void)
{
	if(QTI_State(QTI4_Pin)==false)
	{
		delay_ms(2);
		if(QTI_State(QTI4_Pin)==false)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

			
//			if(qti_2==1 && qti_3==0)
//			{
//				left_turn();
//				return false;
//			}
//			if(qti_2==0 && qti_3==1)
//			{
//				right_turn();
//				return false;



///////////////////////////////////
//
//////////////////////////////////
void car_show_gy25(void)
{
	float tempxx=0;
	
	if(rx_end_flag==1)//PITCH,ROLL;
	{
		
		
		memset(buf1, 0,sizeof(buf1));
		if(YAW>=0)
		{
			tempxx=YAW/100.0;
			sprintf(buf1,"+%-5.2f",tempxx);
			
		}
		else
		{
			tempxx=YAW/100.0;
			sprintf(buf1,"%-5.2f",tempxx);
		}
		
		
		
		rx_end_flag=0;

	}
}			

//////////////////////////////////
//
////////////////////////////////
void car_return_strigt_turn_right_tail(void)
{
    led0pwmval=1500+20;//high mean low speed
	
	 TIM_SetCompare3(TIM3,led0pwmval);

	 led0pwmval=1500+20;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);
    
    
     pwm_num=0x00;
     while(pwm_num<=140)//6v ��ѹ 
    {
//        if(qti_8==0&&qti_9==1&&qti_10==0)
//        {
//            car_stop();
//            break;
//        }
//        while(qti_9==0);
//        while(qti_8==1);
		
		while(qti_9==0)
		{
			if(pwm_num>=120)
			{
				break;
			}
		}
		car_stop();
		
		 led0pwmval=1500+20;//high mean low speed
	
	 TIM_SetCompare3(TIM3,led0pwmval);

	 led0pwmval=1500+20;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);
    
		
		
        while(qti_8==1)
		{
			if(pwm_num>=135)
			{
				car_stop();
				break;
			}
		}
		
		
        car_stop();
        break;
    }

     car_stop();
    
}
//////////////////////////////////
//
////////////////////////////////
void car_return_strigt_turn_left_tail(void)
{
    car_stop();
	led0pwmval=1500-20;//high mean low speed
	
	 TIM_SetCompare3(TIM3,led0pwmval);

	 led0pwmval=1500-20;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);
    
    pwm_num=0x00;
	
		
	
	//while(pwm_num<=54);//5v ��ѹ
	while(pwm_num<=140)//6v ��ѹ 
    {
//        if(qti_8==0&&qti_9==1&&qti_10==0)
//        {
//           
//            break;
//        } 
		
        while(qti_9==0)
		{
			if(pwm_num>=120)
			{
				break;
			}
		}
		car_stop();
		
		 led0pwmval=1500+20;//high mean low speed
	
	 TIM_SetCompare3(TIM3,led0pwmval);

	 led0pwmval=1500+20;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);
		
        while(qti_10==1)
		{
			if(pwm_num>=135)
			{
				car_stop();
				break;
			}
		}
        
    }

     car_stop();
    
}
//////////////////////////////////
//
////////////////////////////////
void car_return_strigt_turn_right_head(void)
{
    led0pwmval=1500+20;//high mean low speed
	
	 TIM_SetCompare3(TIM3,led0pwmval);

	 led0pwmval=1500+20;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);
    
    
     pwm_num=0x00;
     while(pwm_num<=140)//6v ��ѹ 
    {
//        if(qti_1==0&&qti_2==1&&qti_3==0)
//        {
//            car_stop();
//            break;
//        }
		
		
//        while(qti_2==0);
//        while(qti_3==1);
		
		
		while(qti_2==0)
		{
			if(pwm_num>=120)
			{
				break;
			}
		}
		
		
        while(qti_3==1)
		{
			if(pwm_num>=135)
			{
				car_stop();
				break;
			}
		}
        car_stop();
        break;
    }

     car_stop();
    
}
//////////////////////////////////
//
////////////////////////////////
void car_return_strigt_turn_left_head(void)
{
    led0pwmval=1500-20;//high mean low speed
	
	 TIM_SetCompare3(TIM3,led0pwmval);

	 led0pwmval=1500-20;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);
    
    pwm_num=0x00;
	
		
	
	//while(pwm_num<=54);//5v ��ѹ
	while(pwm_num<=140)//6v ��ѹ 
    {
//        if(qti_1==0&&qti_2==1&&qti_3==0)
//        {
//            car_stop();
//            break;
//        }
		
		
//        while(qti_2==0);
//        while(qti_1==1);
		
		
		
		while(qti_2==0)
		{
			if(pwm_num>=120)
			{
				break;
			}
		}
		
		
        while(qti_1==1)
		{
			if(pwm_num>=135)
			{
				car_stop();
				break;
			}
		}
        car_stop();
        break;
    }

     car_stop();
    
}

//////////////////////////////////
//car_desicion��ʾС����һ��ִ�еĶ���
////////////////////////////////
void car_judge_decision_back_up(void)
{
	
	car_desicion=matrix_decision_head_tail[tail_i-1][head_i-1];
	
	dirrection_array[dirrection_index]=car_desicion;
	dirrection_index++;
	if(dirrection_index>=4095)
	{
		dirrection_index=0;
	}
	printf("H=%d,T=%d,D=%d\n", head_i,tail_i,car_desicion);
	
		
//		if(car_desicion==-10)
//		{
//			sprintf(buf1,"��10��");
//			
//			
//		}
//		
//		if(car_desicion==-7)
//		{
//			sprintf(buf1,"��7��");
//			
//			
//		}
//		
//		if(car_desicion==-3)
//		{
//			sprintf(buf1,"��4��");
//			
//			
//		}
//		
//		if(car_desicion==0)
//		{
//			sprintf(buf1,"ֱ");
//			
//			
//		}
//		
//		
//		if(car_desicion==10)
//		{
//			sprintf(buf1,"��10��");
//			
//			
//		}
//		
//		if(car_desicion==7)
//		{
//			sprintf(buf1,"��7��");
//			
//			
//		}
//		
//		if(car_desicion==3)
//		{
//			sprintf(buf1,"��4��");
//			
//			
//		}
		
		
		
		
//	}
//	
//	if(walk_ae_state==1||walk_ae_state==5)
//	{
//		car_desicion=matrix_decision_tail[tail_i];
//	}
//	
//	if(walk_ae_state==3)
//	{
//		car_desicion=matrix_decision_head[head_i];
//	}

}

//////////////////////////////////
//car_desicion��ʾС����һ��ִ�еĶ���
////////////////////////////////
void car_judge_decision(void)
{
	
	car_desicion=matrix_decision_head_tail[tail_i-1][head_i-1];
	
	dirrection_array[dirrection_index]=car_desicion;
	dirrection_index++;
	if(dirrection_index>=4095)
	{
		dirrection_index=0;
	}
	printf("H=%d,T=%d,D=%d\n", head_i,tail_i,car_desicion);
	
		
//		if(car_desicion==-10)
//		{
//			sprintf(buf1,"��10��");
//			
//			
//		}
//		
//		if(car_desicion==-7)
//		{
//			sprintf(buf1,"��7��");
//			
//			
//		}
//		
//		if(car_desicion==-3)
//		{
//			sprintf(buf1,"��4��");
//			
//			
//		}
//		
//		if(car_desicion==0)
//		{
//			sprintf(buf1,"ֱ");
//			
//			
//		}
//		
//		
//		if(car_desicion==10)
//		{
//			sprintf(buf1,"��10��");
//			
//			
//		}
//		
//		if(car_desicion==7)
//		{
//			sprintf(buf1,"��7��");
//			
//			
//		}
//		
//		if(car_desicion==3)
//		{
//			sprintf(buf1,"��4��");
//			
//			
//		}
		
		
		
		
//	}
//	
//	if(walk_ae_state==1||walk_ae_state==5)
//	{
//		car_desicion=matrix_decision_tail[tail_i];
//	}
//	
//	if(walk_ae_state==3)
//	{
//		car_desicion=matrix_decision_head[head_i];
//	}

}
//////////////////////////////////
//
////////////////////////////////
void car_fastforward(void)//����ǰ��
{
//    GPIO_SetBits(GPIOC, GPIO_Pin_6);
//    delay_us(1550);
//    GPIO_ResetBits(GPIOC,GPIO_Pin_6);
//    	
//	GPIO_SetBits(GPIOC, GPIO_Pin_7);
//	delay_us(1450);
//	GPIO_ResetBits(GPIOC,GPIO_Pin_7);

//	delay_ms(20);	
	//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	led0pwmval=1500+49;//1550;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500-50;//1450;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	
}	


////////////////////////////////////
/////
//////////////////////////////////

	
void delay_ms_xm1(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=5200;  //�Լ�����
      while(i--) ;    
   }
}

////////////////////////////////////
/////
//////////////////////////////////
car_special_run_backup_car(void)
{
	 led0pwmval=1500-50+5;//1550;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500+50;//1450;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	while(0==qti_5 && 0==qti_6 )
	{
		 led0pwmval=1500-50-15;//1550;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		 led0pwmval=1500+50;//1450;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
		
		 delay_ms_xm1(20);
	}
	car_stop();
	if(1==qti_5)
	{
		first_axis_qit=-1;
		//���ŵ�����֪��qti5Ϊ��
		while(1==qti_5  )
		{
			 led0pwmval=1500-50+5;//1550;//high mean low speed
			 TIM_SetCompare3(TIM3,led0pwmval);
			 
			 led0pwmval=1500+50;//1450;//high mean low speed
			 TIM_SetCompare4(TIM3,led0pwmval);	
			
			 delay_ms_xm1(20);
		}
		
		car_stop();
		car_return_strigt_turn_left_head();
		
	}
	
	//��ʾ�ұ�qit�Ӵ�����
	else
	{
		first_axis_qit=1;
		
		
		while(1==qti_6  )
		{
			 led0pwmval=1500-50+5;//1550;//high mean low speed
			 TIM_SetCompare3(TIM3,led0pwmval);
			 
			 led0pwmval=1500+50;//1450;//high mean low speed
			 TIM_SetCompare4(TIM3,led0pwmval);	
			
			 delay_ms_xm1(20);
		}
		
		car_stop();
		car_return_strigt_turn_right_head();
		
		
	}
	car_stop();

}

////////////////////////////////////
/////
//////////////////////////////////
car_special_run_forward_car(void)
{
	 led0pwmval=1500+50+15;//1550;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500-50;//1450;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	while(0==qti_5 && 0==qti_6 )
	{
		led0pwmval=1500+50+15;//1550;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		 led0pwmval=1500-50;//1450;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
		
		 delay_ms_xm1(20);
	}
	car_stop();
	if(1==qti_5)
	{
		first_axis_qit=-1;
		//���ŵ�����֪��qti5Ϊ��
		while(1==qti_5  )
		{
			 led0pwmval=1500+50+15;//1550;//high mean low speed
			 TIM_SetCompare3(TIM3,led0pwmval);
			 
			 led0pwmval=1500-50;//1450;//high mean low speed
			 TIM_SetCompare4(TIM3,led0pwmval);	
			
			 delay_ms_xm1(20);
		}
		
		car_stop();
		car_return_strigt_turn_right_tail();
		
	}
	
	//��ʾ�ұ�qit�Ӵ�����
	else
	{
		
		first_axis_qit=1;
		
		
		while(1==qti_6  )
		{
			 led0pwmval=1500+50+15;//1550;//high mean low speed
			 TIM_SetCompare3(TIM3,led0pwmval);
			 
			 led0pwmval=1500-50;//1450;//high mean low speed
			 TIM_SetCompare4(TIM3,led0pwmval);	
			
			 delay_ms_xm1(20);
		}
		
		car_stop();
		car_return_strigt_turn_left_tail();
		
		
	}
	car_stop();
}


////////////////////////////////////
/////
//////////////////////////////////
void car_position_test_head(void)
{
	if(0==qti_1 && 0==qti_2 && 0==qti_3)
	{
		sprintf(buf1,"��ͷȫ��");
		
		
		
		car_stop();
		
		if(1==qti_4)
		{
			
			
			car_left_turn();
		
			pwm_num=0;

			while(pwm_num<=10);//6v ��ѹ 
			
			car_stop();
			head_i=4;
			
			
			
		}
		
		else if(1==qti_5||1==qti_7)
		{
			car_left_turn();
		
			pwm_num=0;

			while(pwm_num<=10);//6v ��ѹ 
			
			car_stop();
			head_i=4;
		}
			
		else if(1==qti_6)
		{
			car_right_turn();
		
			pwm_num=0;

			while(pwm_num<=10);//6v ��ѹ 
			
			car_stop();
			head_i=2;
		}
		
		else
		{
			car_special_run_backup_car();
			
			car_stop();
			head_i=5;
		}
		
		
		
	}
	
	if(0==qti_1 && 0==qti_2 && 1==qti_3)
	{
		head_i=2;
		
////		sprintf(buf1,"��ͷ���");
////		
////		
//		
	}
	
	if(0==qti_1 && 1==qti_2 && 0==qti_3)
	{
		head_i=5;
//		sprintf(buf1,"��ͷֱ");
//		
//		
		
	}
	if(0==qti_1 && 1==qti_2 && 1==qti_3)
	{
		head_i=1;
//		sprintf(buf1,"��ͷ��С");
//		
//		
		
	}
	
	if(1==qti_1 && 0==qti_2 && 0==qti_3)
	{
		head_i=4;
//		sprintf(buf1,"��ͷ�Ҵ�");
//		
//		
		
	}
	
	if(1==qti_1 && 1==qti_2 && 0==qti_3)
	{
		head_i=3;
//		sprintf(buf1,"��ͷ��С");
//		
//		
		
	}
	
	if(1==qti_1 && 1==qti_2 && 1==qti_3)
	{
		head_i=5;
//		sprintf(buf1,"��ͷֱ");
//		
//		
		
	}
}

////////////////////////////////////
/////
//////////////////////////////////

//////////////////////////////////
void car_position_test_head_d_point(void)
{
	if(0==qti_1 && 0==qti_2 && 0==qti_3&& 1==qti_4)
	{
		sprintf(buf1,"��ͷȫ��");
		
		
		head_i=6;
		car_stop();
		
	}
    
   
	
	if(0==qti_1 && 0==qti_2 && 1==qti_3)
	{
		head_i=2;
		
//		sprintf(buf1,"��ͷ���");
//		
//		
		
	}
	
	if(0==qti_1 && 1==qti_2 && 0==qti_3)
	{
		head_i=5;
//		sprintf(buf1,"��ͷֱ");
//		
//		
		
	}
	if(0==qti_1 && 1==qti_2 && 1==qti_3)
	{
		head_i=1;
//		sprintf(buf1,"��ͷ��С");
//		
//		
		
	}
	
	if(1==qti_1 && 0==qti_2 && 0==qti_3)
	{
		head_i=4;
//		sprintf(buf1,"��ͷ�Ҵ�");
//		
//		
		
	}
	
	if(1==qti_1 && 1==qti_2 && 0==qti_3)
	{
		head_i=3;
//		sprintf(buf1,"��ͷ��С");
//		
//		
		
	}
	
	if(1==qti_1 && 1==qti_2 && 1==qti_3)
	{
		head_i=5;
//		sprintf(buf1,"��ͷֱ");
//		
//		
		
	}
}

///
void car_position_test_tail(void)
{
	//tail
	
	if(0==qti_8 && 0==qti_9 && 0==qti_10)
	{
		
		car_stop();
		
		if(1==qti_11)
		{
			
			car_right_turn();
		
			pwm_num=0;

			while(pwm_num<=10);//6v ��ѹ 
			
			car_stop();
			tail_i=4;
		}
		
		else if(1==qti_5||1==qti_7)
		{
			car_left_turn();
		
			pwm_num=0;

			while(pwm_num<=10);//6v ��ѹ 
			
			car_stop();
			tail_i=4;
		}
			
		else if(1==qti_6)
		{
			car_right_turn();
		
			pwm_num=0;

			while(pwm_num<=10);//6v ��ѹ 
			
			car_stop();
			head_i=2;
		}
		
		
		
		
		else
		{
			car_stop();
			car_special_run_forward_car();
			
			tail_i=5;
			
		}
		
		
//		sprintf(buf1,"��βȫ��");
//		
//		
		
	}
	
	
	if(0==qti_8 && 0==qti_9 && 1==qti_10)
	{
		tail_i=2;
//		sprintf(buf1,"��β���");
//		
//		
		
	}
	
	if(0==qti_8 && 1==qti_9 && 0==qti_10)
	{
		tail_i=5;
//		sprintf(buf1,"��βֱ");
//		
//		
		
	}
	
	if(0==qti_8 && 1==qti_9 && 1==qti_10)
	{
		tail_i=1;
//		sprintf(buf1,"��β��С");
//		
//		
		
	}
	
	if(1==qti_8 && 0==qti_9 && 0==qti_10)
	{
		tail_i=4;
//		sprintf(buf1,"��β�Ҵ�");
//		
//		
		
	}
	
	if(1==qti_8 && 1==qti_9 && 0==qti_10)
	{
		tail_i=3;
//		sprintf(buf1,"��β��С");
//		
//		
		
	}
	
	if(1==qti_8 && 1==qti_9 && 1==qti_10)
	{
		tail_i=5;
//		sprintf(buf1,"��βֱ");
//		
//		
		
	}
	
	
}

////////////////////////////////////
/////
//////////////////////////////////
////////////////////////////////////
/////
//////////////////////////////////
void car_position_test_head_all_white(void)
{
	if(0==qti_1 && 0==qti_2 && 0==qti_3)
	{
		sprintf(buf1,"��ͷȫ��");
		
		
		
		head_i=5;
		
	}
	
	if(0==qti_1 && 0==qti_2 && 1==qti_3)
	{
		head_i=2;
		
//		sprintf(buf1,"��ͷ���");
//		
//		
		
	}
	
	if(0==qti_1 && 1==qti_2 && 0==qti_3)
	{
		head_i=5;
//		sprintf(buf1,"��ͷֱ");
//		
//		
		
	}
	if(0==qti_1 && 1==qti_2 && 1==qti_3)
	{
		head_i=1;
//		sprintf(buf1,"��ͷ��С");
//		
//		
		
	}
	
	if(1==qti_1 && 0==qti_2 && 0==qti_3)
	{
		head_i=4;
//		sprintf(buf1,"��ͷ�Ҵ�");
//		
//		
		
	}
	
	if(1==qti_1 && 1==qti_2 && 0==qti_3)
	{
		head_i=3;
//		sprintf(buf1,"��ͷ��С");
//		
//		
		
	}
	
	if(1==qti_1 && 1==qti_2 && 1==qti_3)
	{
		head_i=5;
//		sprintf(buf1,"��ͷֱ");
//		
//		
		
	}
}

////////////////////////////////////
/////
//////////////////////////////////
void car_position_test_tail_all_white(void)
{
	//tail
	
	if(0==qti_8 && 0==qti_9 && 0==qti_10)
	{
	
		tail_i=5;
		
		
		sprintf(buf1,"��βȫ��");
		
		
		
	}
	
	
	if(0==qti_8 && 0==qti_9 && 1==qti_10)
	{
		tail_i=2;
//		sprintf(buf1,"��β���");
//		
//		
		
	}
	
	if(0==qti_8 && 1==qti_9 && 0==qti_10)
	{
		tail_i=5;
//		sprintf(buf1,"��βֱ");
//		
//		
		
	}
	
	if(0==qti_8 && 1==qti_9 && 1==qti_10)
	{
		tail_i=1;
//		sprintf(buf1,"��β��С");
//		
//		
		
	}
	
	if(1==qti_8 && 0==qti_9 && 0==qti_10)
	{
		tail_i=4;
//		sprintf(buf1,"��β�Ҵ�");
//		
//		
		
	}
	
	if(1==qti_8 && 1==qti_9 && 0==qti_10)
	{
		tail_i=3;
//		sprintf(buf1,"��β��С");
//		
//		
		
	}
	
	if(1==qti_8 && 1==qti_9 && 1==qti_10)
	{
		tail_i=5;
//		sprintf(buf1,"��βֱ");
//		
//		
		
	}
	
	
}

////////////////////////////////////
/////
//////////////////////////////////



////////////////////////////////////
/////
//////////////////////////////////
void car_position_test(void)
{
	
	car_position_test_head();
	car_position_test_tail();
	
	
}	

////////////////////////////////////
/////
//////////////////////////////////
void car_position_test_all_white(void)
{
	
	car_position_test_head_all_white();
	car_position_test_tail_all_white();
	
	
}	
////////////////////////////////////
/////
//////////////////////////////////

void car_excute_decision_back_up(void)
{
    if(car_desicion==-10)
	{
		car_left_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		//while(pwm_num<=10);//6v ��ѹ 
		while(pwm_num<=5);//6v ��ѹ 
		
		
		car_fastforward();
		
		
	}
	
	
	
	
	
	if(car_desicion==-7)
	{
		car_left_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		//while(pwm_num<=7);//6v ��ѹ 
		
		while(pwm_num<=4);//6v ��ѹ 
		
		
		car_fastforward();
	}
	
	
	
	
	
	if(car_desicion==-3)
	{
		car_left_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=3);//6v ��ѹ 
		
		
		car_fastforward();
	}
	
	if(car_desicion==0)
	{
		car_fastforward();
		delay_ms_xm1(20);
	}
	
	if(car_desicion==19)//19����ͷ��11�㷽��
	{
		car_fastforward();
		delay_ms_xm1(20);
	}
	
	
	if(car_desicion==21)//21����ͷ��1�㷽��
	{
		car_fastforward();
		delay_ms_xm1(20);
	}
	
	
	
	
	if(car_desicion==3)
	{
		car_right_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=3);//6v ��ѹ 
		
		
		car_fastforward();
		
		
	}
	
	if(car_desicion==7)
	{
		 car_right_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=4);//6v ��ѹ 
		
		
		car_fastforward();
	}
	
	if(car_desicion==10)
	{
		car_right_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=5);//6v ��ѹ 
		
		
		car_fastforward();
		
		
	}
    
    
    
}
//////////////////////////////////
//
////////////////////////////////
void turn_left_10()
{
	
	sprintf(buf1,"��ת45��");
	
	
	
	turn_num=turn_num-1;
    
     turn_left_flag=1;
    
	led0pwmval=1460+2;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1460-2;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	 pwm_num=0x00;
	
//	while(pwm_num<=40)//6v ��ѹ 
//	{
//		
//	}
//	
//	car_stop();
//	
//	delay_ms_xm1(100);

}
//////////////////////////////////
//
////////////////////////////////
void turn_left_45()
{
	
	sprintf(buf1,"��ת45��");
	
	
	
	turn_num=turn_num-1;
    
     turn_left_flag=1;
    
	led0pwmval=1460+2;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1460-2;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	 pwm_num=0x00;
	
	while(pwm_num<=40)//6v ��ѹ 
	{
		
	}
	
	car_stop();
	
	delay_ms_xm1(100);

}


void turn_left_135()
{
	
	sprintf(buf1,"��ת45��");
	
	
	
    turn_num=turn_num-3;
    
    turn_left_flag=1;
	
	led0pwmval=1460+2;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1460-2;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	 pwm_num=0x00;
	
	while(pwm_num<=112)//6v ��ѹ 
	{
		
	}
	
	car_stop();
	
	delay_ms_xm1(100);

}
////////////////
//
/////////////////

void turn_right_10(void)	   //��ת90��
{
	turn_num=turn_num+2;//�ҷ���ÿ��˳ʱ��תһȦ��6060����0.6�ȣ���ʱ��һȦ���ض�0.34�ȣ���������180��ʱƽ��
    ;
    sprintf(buf1,"��ת10��");
	
		
    
    turn_num=turn_num+2;
    turn_left_flag=0;
    
    
    led0pwmval=1540+2;//high mean low speed//2019.10.11�ҷ���С������ת90��ʱ�������������е���ת��
                      //�����е�ƫ��һ����ת���ʴˣ����ּ��٣����ּ��٣�����С�����������е���ת
	
		//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1540-2;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	pwm_num=0x00;
	
	
	
	//while(pwm_num<=97);//5v ��ѹ
//	while(pwm_num<40);//6v ��ѹ 
//	
//	 led0pwmval=1500;//high mean low speed
//	 TIM_SetCompare3(TIM3,led0pwmval);
//	 
//	 led0pwmval=1500;//high mean low speed
//	 TIM_SetCompare4(TIM3,led0pwmval);	
}
////////////////
//
/////////////////

void turn_right_45(void)	   //��ת90��
{
	turn_num=turn_num+2;//�ҷ���ÿ��˳ʱ��תһȦ��6060����0.6�ȣ���ʱ��һȦ���ض�0.34�ȣ���������180��ʱƽ��
    ;
    sprintf(buf1,"��ת45��");
	
		
    
    turn_num=turn_num+2;
    turn_left_flag=0;
    
    
    led0pwmval=1540+2;//high mean low speed//2019.10.11�ҷ���С������ת90��ʱ�������������е���ת��
                      //�����е�ƫ��һ����ת���ʴˣ����ּ��٣����ּ��٣�����С�����������е���ת
	
		//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1540-2;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	pwm_num=0x00;
	
	
	
	//while(pwm_num<=97);//5v ��ѹ
	while(pwm_num<47);//6v ��ѹ 
	
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
}
//////////////////////////////////
//
////////////////////////////////
void car_axis_recover(void)
{
    car_position_test_head();
    
    if (head_i==1)
    {
        //ת��45��
        turn_left_45();
        //����
        
       pwm_num=0;
         led0pwmval=1500-50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500+50-15;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=4);//6v ��ѹ 
        car_stop();
        
         //ת��45��
        turn_right_45();
        //ǰ��
        
       pwm_num=0;
         led0pwmval=1500+50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500-50+15;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=3);//6v ��ѹ 
        
        car_stop();
        car_stop();
        
        
        
    }
    else if (head_i==2)
    {
        //ת��45��
        turn_left_45();
        //����
        
       pwm_num=0;
         led0pwmval=1500-50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500+50-15;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=7);//6v ��ѹ 
        car_stop();
        
         //ת��45��
        turn_right_45();
        //ǰ��
        
       pwm_num=0;
         led0pwmval=1500+50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500-50+15;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=5);//6v ��ѹ 
        
        car_stop();
        car_stop();
        
        
        
    }
     else if (head_i==3)
    {
        //ת��45��
         turn_right_45();
        //����
        
       pwm_num=0;
         led0pwmval=1500-50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500+50-15;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=4);//6v ��ѹ 
        car_stop();
        
         //ת��45��
       turn_left_45();
        //ǰ��
        
       pwm_num=0;
         led0pwmval=1500+50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500-50+15;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=3);//6v ��ѹ 
        
        car_stop();
        car_stop();
        
        
        
    }
     else if (head_i==4)
    {
        
          //ת��45��
         turn_right_45();
        //����
        
       pwm_num=0;
         led0pwmval=1500-50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500+50+15;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=7);//6v ��ѹ 
        car_stop();
        
         //ת��45��
       turn_left_45();
        //ǰ��
        
       pwm_num=0;
         led0pwmval=1500+50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500-50+15;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=5);//6v ��ѹ 
        
        car_stop();
        car_stop();
    }
    
    
    
    
    
    
}

//////////////////////////////////
//
////////////////////////////////
void car_axis_recover_d_point(void)
{
    car_position_test_head_d_point();
    
    if (head_i==1)
    {
        
        
         //ת��45��
        turn_right_45();
        //ǰ��
        
       pwm_num=0;
         led0pwmval=1500+50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500-50+5;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=4);//6v ��ѹ 
        
         car_stop();
        //ת��45��
        turn_left_45();
        //����
        
       pwm_num=0;
         led0pwmval=1500-50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500+50;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=3);//6v ��ѹ 
       
        
        car_stop();
        car_stop();
        
        
        
    }
    else if (head_i==2)
    {
        //ת��45��
         turn_right_45();
        //ǰ��
        
       pwm_num=0;
         led0pwmval=1500+50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500-50+5;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=7);//6v ��ѹ 
        car_stop();
        
         //ת��45��
       
        turn_left_45();
        //����
        
       pwm_num=0;
         led0pwmval=1500-50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500+50;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=5);//6v ��ѹ 
        car_stop();
        car_stop();
        
        
        
    }
     else if (head_i==3)
    {
        //ת��45��
        turn_left_45();
        //ǰ��
        
       pwm_num=0;
         led0pwmval=1500+50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500-50+5;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=4);//6v ��ѹ 
        car_stop();
        
         //ת��45��
        turn_right_45();
        //����
        
       pwm_num=0;
         led0pwmval=1500-50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500+50;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=3);//6v ��ѹ 
        
        car_stop();
        car_stop();
        
        
        
    }
     else if (head_i==4)
    {
        
          //ת��45��
          turn_left_45();
        //ǰ��
        
       pwm_num=0;
         led0pwmval=1500+50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500-50+5;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=7);//6v ��ѹ 
        car_stop();
        
         //ת��45��
      turn_right_45();
        //����
        
       pwm_num=0;
         led0pwmval=1500-50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500+50;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=5);//6v ��ѹ 
        
        car_stop();
        car_stop();
    }
    
    else if (head_i==6)
    {
        
          //ת��45��
           turn_left_45();
        //ǰ��
        
       pwm_num=0;
         led0pwmval=1500+50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500-50+5;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=14);//6v ��ѹ 
        car_stop();
        
         //ת��45��
      turn_right_45();
        //����
        
       pwm_num=0;
         led0pwmval=1500-50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500+50;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=10);//6v ��ѹ
        
        car_stop();
        car_stop();
    }
    
    
    
    
    
    
    
    
}


////////////////////////////////////
/////
//////////////////////////////////

void car_excute_decision(void)
{
	
	
	if(car_desicion==-10)
	{
		car_left_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		//while(pwm_num<=10);//6v ��ѹ 
		while(pwm_num<=5);//6v ��ѹ 
		
		
		car_fastforward();
		
		
	}
	
	
	
	
	
	if(car_desicion==-7)
	{
		car_left_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		//while(pwm_num<=7);//6v ��ѹ 
		
		while(pwm_num<=4);//6v ��ѹ 
		
		
		car_fastforward();
	}
	
	
	
	
	
	if(car_desicion==-3)
	{
		car_left_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=3);//6v ��ѹ 
		
		
		car_fastforward();
	}
	
	if(car_desicion==0)
	{
		car_fastforward();
		delay_ms_xm1(20);
	}
	
	if(car_desicion==19)//19����ͷ��11�㷽��
	{
		car_fastforward();
		delay_ms_xm1(20);
	}
	
	
	if(car_desicion==21)//21����ͷ��1�㷽��
	{
		car_fastforward();
		delay_ms_xm1(20);
	}
	
	
	
	
	if(car_desicion==3)
	{
		car_right_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=3);//6v ��ѹ 
		
		
		car_fastforward();
		
		
	}
	
	if(car_desicion==7)
	{
		 car_right_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=4);//6v ��ѹ 
		
		
		car_fastforward();
	}
	
	if(car_desicion==10)
	{
		car_right_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=5);//6v ��ѹ 
		
		
		car_fastforward();
		
		
	}
	
	
}
//


//////////////////////////////////
//
////////////////////////////////
void car_from_star_to_2(void)
{
	walk_ae_state=0;
	while(1)
	{
		car_position_test();
		
		if(walk_ae_state==0)
		{
			car_judge_decision();
			car_excute_decision();
			
			if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
			{
				walk_ae_state=1;
				
				sprintf(buf1,"[s10]1");
			
			
        car_stop();
				
			}
			
			first_start_state_1=0x00;
			
		}
		
		
		else if(walk_ae_state==1)
		{
			if(first_start_state_1==0)
			{
				first_start_state_1=1;

				pwm_num_distance=0;
		
			}
			car_judge_decision();
			car_excute_decision();
			
			if(pwm_num_distance>=85+20)
			{
				walk_ae_state=2;
				sprintf(buf1,"[s10]2");
				
				
        car_stop();
				break;
				
			}
			
		}
		

		
	}
}


//////////////////////////////////
//
////////////////////////////////
void car_from_2_to_ae(void)
{
	
	walk_ae_state=0;
	while(1)
	{
		
		
		car_position_test();
		
		 if(1==qti_5 && 1==qti_6 && 1==qti_7)
        {
            walk_ae_state=2;
            sprintf(buf1,"[s10]4");
            
            
            break;
            
        }
			
        car_judge_decision();
        car_excute_decision();
        
       
            
       
		
		
		
	}
}
//////////////////////////////////
//
////////////////////////////////
void turn_left_90_with_6050_check(float des)
{
	float temp_90=0,temp_90_1=0;
	sprintf(buf1,"��ת90��");
	
	
    
    turn_num=turn_num-2;
     turn_left_flag=1;
	
	
	led0pwmval=1460+2;//2019.10.11�ҷ���С������ת90��ʱ�������������е���ת��
                      //�����е�ƫ��һ����ת���ʴˣ����ּ��٣����ּ��٣�����С�����������е���ת
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1460-2;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	 pwm_num=0x00;
	
	while(pwm_num<=77)//6v ��ѹ 
	{
		
	}
	
	car_stop();
	
	delay_ms_xm1(100);
	
	//����6050 �жϸո�ת��90���Ƕ��˻�������
	temp_90_1=YAW/100.0+180.0;
	temp_90=(yaw_0-180.0+des)-temp_90_1;
	while(temp_90>=1.0)
	{
		//����0������ûת��90�ȣ�����ת��87�ȣ���ǰͣ������.������ת��
		
		
			led0pwmval=1480+1;//1520;//1480;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		//tim3 2# is left servo
		 led0pwmval=1480-1;//1520;//1480;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
			
		delay_ms_xm1(20);
			
		temp_90_1=YAW/100.0+180.0;
		temp_90=(yaw_0-180.0+des)-temp_90_1;
	}	
		
		
		
		
		//С��0������ת��90�ȣ�����ת��92�ȣ�����ͣ������.�����ң�ת
		
	while(temp_90<=-1.0)
	{
		//����0������ûת��90�ȣ�����ת��87�ȣ���ǰͣ������.������ת��
		
		
		 led0pwmval=1520+1;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		//tim3 2# is left servo
		 led0pwmval=1520-1;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
			
		delay_ms_xm1(20);
		temp_90_1=YAW/100.0+180.0;
		temp_90=(yaw_0-180.0+des)-temp_90_1;

	}	
	
	car_stop();

	
}
//////////////////////////////////
//
////////////////////////////////
void turn_left_90_with_axis_recover_6050_check(float des)
{
	float temp_90=0,temp_90_1=0;
	sprintf(buf1,"��ת90��");
	
	
	
	
    car_position_test_head();
    
     //ת��45��
     turn_left_45();
    
    if (head_i==1)
    {
       
       
        //����
        
       pwm_num=0;
         led0pwmval=1500-50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500+50;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=4);//6v ��ѹ 
        car_stop();
        
         //ת��45��
        turn_left_45();
        //ǰ��
        
       pwm_num=0;
         led0pwmval=1500+50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500-45;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=3);//6v ��ѹ 
        
        car_stop();
        car_stop();
        
        
        
    }
    else if (head_i==2)
    {
       
        //����
        
       pwm_num=0;
         led0pwmval=1500-50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500+50;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=7);//6v ��ѹ 
        car_stop();
        
         //ת��45��
        turn_left_45();
        //ǰ��
        
       pwm_num=0;
         led0pwmval=1500+50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500-45;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        while(pwm_num<=5);//6v ��ѹ 
        
        car_stop();
        car_stop();
        
        
        
    }
     else if (head_i==3)
    {
           //ǰ��
        
     pwm_num=0;
     led0pwmval=1500+50;//1550;//high mean low speed
     TIM_SetCompare3(TIM3,led0pwmval);
     
     led0pwmval=1500-45;//1450;//high mean low speed
     while(pwm_num<=4);//6v ��ѹ 
     car_stop();
        
         //ת��45��
    turn_left_45();
    TIM_SetCompare4(TIM3,led0pwmval);	
    //����
    
    pwm_num=0;
     led0pwmval=1500-50;//1550;//high mean low speed
     TIM_SetCompare3(TIM3,led0pwmval);
     
     led0pwmval=1500+50;//1450;//high mean low speed
     TIM_SetCompare4(TIM3,led0pwmval);	

    while(pwm_num<=3);//6v ��ѹ 
    
    car_stop();
    car_stop();
        
        
        
    }
     else if (head_i==4)
    {
       //ǰ��
        
         pwm_num=0;
         led0pwmval=1500+50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500-45;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
      
        
        while(pwm_num<=7);//6v ��ѹ 
        car_stop();
        
         //ת��45��
       turn_left_45();
         //����
        
       pwm_num=0;
         led0pwmval=1500-50;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500+50;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	 
        
        while(pwm_num<=5);//6v ��ѹ 
        
        car_stop();
        car_stop();
    }
    
     else if (head_i==5)
    {
        
         //ת��45��
       turn_left_45();
       
        car_stop();
    }

    
    
	
	//����6050 �жϸո�ת��90���Ƕ��˻�������
	temp_90_1=YAW/100.0+180.0;
	temp_90=(yaw_0-180.0+des)-temp_90_1;
	while(temp_90>=1.0)
	{
		//����0������ûת��90�ȣ�����ת��87�ȣ���ǰͣ������.������ת��
		
		
			led0pwmval=1480;//1520;//1480;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		//tim3 2# is left servo
		 led0pwmval=1480;//1520;//1480;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
			
		delay_ms_xm1(20);
			
		temp_90_1=YAW/100.0+180.0;
		temp_90=(yaw_0-180.0+des)-temp_90_1;
	}	
		
		
		
		
		//С��0������ת��90�ȣ�����ת��92�ȣ�����ͣ������.�����ң�ת
		
	while(temp_90<=-1.0)
	{
		//����0������ûת��90�ȣ�����ת��87�ȣ���ǰͣ������.������ת��
		
		
		 led0pwmval=1520;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		//tim3 2# is left servo
		 led0pwmval=1520;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
			
		delay_ms_xm1(20);
		temp_90_1=YAW/100.0+180.0;
		temp_90=(yaw_0-180.0+des)-temp_90_1;

	}	
	
	car_stop();

	
}
//////////////////////////////////
//
////////////////////////////////
void mpu_6050_white(void)
{
    if(qti_1==0&&qti_2==0&&qti_3==0)
    {
        if (qti_4==0)
        {
              //ת��45��
            turn_left_45();
             //����
            
           pwm_num=0;
             led0pwmval=1500-50;//1550;//high mean low speed
             TIM_SetCompare3(TIM3,led0pwmval);
             
             led0pwmval=1500+50;//1450;//high mean low speed
             TIM_SetCompare4(TIM3,led0pwmval);
            
            while(pwm_num<=22);//6v ��ѹ 
            car_stop();
            
             //ת��45��
            turn_right_45();
            //ǰ��
            
           pwm_num=0;
             led0pwmval=1500+50;//1550;//high mean low speed
             TIM_SetCompare3(TIM3,led0pwmval);
             
             led0pwmval=1500-50+5;//1450;//high mean low speed
             TIM_SetCompare4(TIM3,led0pwmval);	
            
            while(pwm_num<=12);//6v ��ѹ 
            
            car_stop();
            car_stop();
            
            
            
        }
         else if (qti_4==1)
        {
            //ת��45��
            turn_left_45();
            //ǰ��
            
           pwm_num=0;
             led0pwmval=1500+50;//1550;//high mean low speed
             TIM_SetCompare3(TIM3,led0pwmval);
             
             led0pwmval=1500-50+5;//1450;//high mean low speed
             TIM_SetCompare4(TIM3,led0pwmval);	
            
            while(pwm_num<=23);//6v ��ѹ 
            car_stop();
            
             //ת��45��
            turn_right_45();
            //����
            
           pwm_num=0;
             led0pwmval=1500-50;//1550;//high mean low speed
             TIM_SetCompare3(TIM3,led0pwmval);
             
             led0pwmval=1500+50;//1450;//high mean low speed
             TIM_SetCompare4(TIM3,led0pwmval);	
            
            while(pwm_num<=20);//6v ��ѹ 
            
            car_stop();
            car_stop();
            
            
            
        }

    }
}





//////////////////////////////////
//des ��ָƫ��
////////////////////////////////
void mpu_6050_check(float des)
{
//	float temp_180=0,temp_180_1=0;
    
    if(des==0)
    {
        
           while(1)
           {
                temp_180_1=YAW/100.0+180.0-(yaw_0-180.0);
//               car_stop();
        
               if(temp_180_1<0)
               {
                   temp_180_1=temp_180_1+360.0;
               
                }
               
                
           
            
               if(0<=temp_180_1&&temp_180_1<180)
               {
                   //��ת
                     led0pwmval=1520;//high mean low speed
                     TIM_SetCompare3(TIM3,led0pwmval);
                     
                    //tim3 2# is left servo
                     led0pwmval=1520;//high mean low speed
                     TIM_SetCompare4(TIM3,led0pwmval);	
                }
               else
               {
                    //��ת  
                     led0pwmval=1480;//1520;//1480;//high mean low speed
                     TIM_SetCompare3(TIM3,led0pwmval);
                     
                    //tim3 2# is left servo
                     led0pwmval=1480;//1520;//1480;//high mean low speed
                     TIM_SetCompare4(TIM3,led0pwmval);	
                }
               
                 if(0<=temp_180_1&&temp_180_1<=1||365<=temp_180_1&&temp_180_1<365)
                 {
                     car_stop();
                     break;
                 }
               
                
           }
    }
    
    else
    {
        
        temp_180_1=YAW/100.0+180.0-(yaw_0-180.0);
        
        if(temp_180_1<0)
        {
            temp_180_1=temp_180_1+360;
        }
        
        temp_180=des-temp_180_1;
        while(temp_180>=1.0)
        {
            //����0������ûת��90�ȣ�����ת��87�ȣ���ǰͣ������.������ת��
            
            
             led0pwmval=1480;//1520;//1480;//high mean low speed
             TIM_SetCompare3(TIM3,led0pwmval);
             
            //tim3 2# is left servo
             led0pwmval=1480;//1520;//1480;//high mean low speed
             TIM_SetCompare4(TIM3,led0pwmval);	
                
            delay_ms_xm1(20);
            car_stop();	
            temp_180_1=YAW/100.0+180.0;
            temp_180=(yaw_0-180.0+des)-temp_180_1;
        }	
            
            
            
            
            //С��0������ת��90�ȣ�����ת��92�ȣ�����ͣ������.�����ң�ת
            
        while(temp_180<=-1.0)
        {
            //����0������ûת��90�ȣ�����ת��87�ȣ���ǰͣ������.������ת��
            
            
             led0pwmval=1520;//high mean low speed
             TIM_SetCompare3(TIM3,led0pwmval);
             
            //tim3 2# is left servo
             led0pwmval=1520;//high mean low speed
             TIM_SetCompare4(TIM3,led0pwmval);	
                
            delay_ms_xm1(20);
            
            car_stop();
            temp_180_1=YAW/100.0+180.0;
            temp_180=(yaw_0-180.0+des)-temp_180_1;

        }	
        
        
    }
	
	
	//����6050 �жϸո�ת��90���Ƕ��˻�������
//	temp_180_1=YAW/100.0+180.0;
//	temp_180=(yaw_0-180.0+des)-temp_180_1;
    
   
	
	car_stop();

	
}



//////////////////////////////////
//des ��ָƫ��
////////////////////////////////
void turn_left_180_with_6050_check(float des)
{
	
	sprintf(buf1,"ת180��");
	
	
	
	if(turn_num>=0)
    {
        //��ת
        
        
        turn_left_flag=1;
        
        turn_num=turn_num-4;
        
        led0pwmval=1460+2;//high mean low speed//2019.10.11�ҷ���С������ת90��ʱ�������������е���ת��
                      //�����е�ƫ��һ����ת���ʴˣ����ּ��٣����ּ��٣�����С�����������е���ת
         TIM_SetCompare3(TIM3,led0pwmval);
         
        //tim3 2# is left servo
         led0pwmval=1460-2;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
         pwm_num=0x00;
        
        //while(pwm_num<=95-5-3-2-0)//6v ��ѹ 
        while(pwm_num<=154)//6v ��ѹ 
        {
            
        }
        
        car_stop();
        
       
        
        delay_ms_xm1(100);
    }
    else
    {
        //��ת
        
        turn_left_flag=0;
         turn_num=turn_num+8; 
        
        led0pwmval=1540+2;//high mean low speed
	
		//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	
         TIM_SetCompare3(TIM3,led0pwmval);
         
        //tim3 2# is left servo
         led0pwmval=1540-2;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        pwm_num=0x00;
        
        
        
        //while(pwm_num<=97);//5v ��ѹ
        while(pwm_num<=159);//6v ��ѹ 
	
        car_stop();
    }
	
	
     mpu_6050_check(des);  
        
       
        
        
    
	
	
	car_stop();

	
}


//////////////////////////////////
//
////////////////////////////////
void turn_right_90_with_6050_check(float des)
{
	float temp_90=0,temp_90_1=0;
	
    turn_num=turn_num+4;//�ҷ���ÿ��˳ʱ��תһȦ��6060����0.6�ȣ���ʱ��һȦ���ض�0.34�ȣ���������180��ʱƽ��
    sprintf(buf1,"��ת90��");
	
	
    
    turn_num=turn_num+4;
    turn_left_flag=0;
	
	
	led0pwmval=1540+2;//high mean low speed//2019.10.11�ҷ���С������ת90��ʱ�������������е���ת��
                      //�����е�ƫ��һ����ת���ʴˣ����ּ��٣����ּ��٣�����С�����������е���ת
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1540-2;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	 pwm_num=0x00;
	
	while(pwm_num<=77)//6v ��ѹ 
	{
		
	}
	
	car_stop();
	
	delay_ms_xm1(100);
	
	//����6050 �жϸո�ת��90���Ƕ��˻�������
	temp_90_1=YAW/100.0+180.0;
	temp_90=(yaw_0-180.0+des)-temp_90_1;
	while(temp_90>=1.0)
	{
		//����0������ûת��90�ȣ�����ת��87�ȣ���ǰͣ������.������ת��
		
		
			led0pwmval=1480+1;//1520;//1480;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		//tim3 2# is left servo
		 led0pwmval=1480-1;//1520;//1480;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
			
		delay_ms_xm1(20);
		car_stop();	
		temp_90_1=YAW/100.0+180.0;
		temp_90=(yaw_0-180.0+des)-temp_90_1;
	}	
		
		car_stop();
		
		
		//С��0������ת��90�ȣ�����ת��92�ȣ�����ͣ������.�����ң�ת
		
	while(temp_90<=-1.0)
	{
		//����0������ûת��90�ȣ�����ת��87�ȣ���ǰͣ������.������ת��
		
		
		 led0pwmval=1540+2;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		//tim3 2# is left servo
		 led0pwmval=1540-2;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
			
		delay_ms_xm1(20);
        car_stop();
		temp_90_1=YAW/100.0+180.0;
		temp_90=(yaw_0-180.0+des)-temp_90_1;

	}	
	
	car_stop();

	
}

///////////////////////////////
//
///////////////////////////////
void car_pick_up_a_ponit_1_top(void)
{
	//��ȡA�㶥��ɫ�顣��ʼצ��λ��Ϊ 	Tim1_Init(1400,1532,2500,1000);//ֱ��ֵ	Tim2_Init(1500,1560,270,250);
	//��ץ��λ��Ϊ xxx��1553,1237,947,2100
	//��צע���������4�Ŷ����Ȼ�����2�Ŷ������3�Ŷ�������5�Ŷ��
	
	//��ʼ��צ��4�Ŷ���ӳ�ʼλ�ã�1000�����ĵ�956	
	sprintf(buf1,"���£�4�ż�");
	
		
	for(led0pwmval=a_point_top_up[4-1];led0pwmval>=a_point_top_down[4-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare4(TIM1,a_point_top_down[4-1]);
	
	delay_ms_xm1(100);
	//delay_ms(1000);
	
	sprintf(buf1,"2�ż�");
	
		
	//4�ž�λ��2�Ŷ���ӳ�ʼλ�ã�1532�����ĵ�1632
	k_num=0x00;
	for(led0pwmval=servo_2_orign_position;led0pwmval<=a_point_top_down[2-1];led0pwmval=led0pwmval+10)
    {
		
		
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=servo_1_orign_position-k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare2(TIM1,a_point_top_down[2-1]);
	TIM_SetCompare1(TIM1,a_point_top_down[1-1]);
	
	
	
	
	delay_ms(20);
	//delay_ms(1000);
	
	sprintf(buf1,"3�ż�");
	
		
	
	//2�ž�λ��3�Ŷ���ӳ�ʼλ�ã�2500�����ĵ�1290
	for(led0pwmval=servo_3_orign_position;led0pwmval>=a_point_top_down[3-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms_xm1(20);
    }
	
	TIM_SetCompare3(TIM1,a_point_top_down[3-1]);
	
	delay_ms(100);
		
	sprintf(buf1,"5�ż�");
	
		
	//3�ž�λ��5�Ŷ���ӳ�ʼλ�ã�1500�����ĵ�2100
	
	close_claw();
	delay_ms(100);
	
//	sprintf(buf1,"[s10]����д��������滹ûд������ͣ��");
//	
//		
//	car_stop();
//	while(1);
	
	
	
	
	////////////////////////
	//��ʼ̧��a�����
	////////////////////////////////
	
	
	
	//Ȼ��̧���е��
	sprintf(buf1,"̧��3�ż�");
	
			
	//3�Ŷ���ӳ�ʼλ�ã�1237�����ĵ�2500
	for(led0pwmval=a_point_top_down[3-1];led0pwmval<=a_point_top_up[3-1];led0pwmval=led0pwmval+20)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms_xm1(20);
    }
	
	TIM_SetCompare3(TIM1,a_point_top_up[3-1]);
	delay_ms(200);

	
	
	sprintf(buf1,"4�ż�");
	
	
	//4�Ŷ���ӳ�ʼλ�ã�947�����ĵ�1000
	
	for(led0pwmval=a_point_top_down[4-1];led0pwmval>=a_point_top_up[4-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare4(TIM1,a_point_top_up[4-1]);
	
	delay_ms(200);
	
	
	sprintf(buf1,"2�ż�");
	
	
	
	
	
	//2�Ŷ���ӳ�ʼλ�ã�1632�����ĵ�servo_2_orign_position
	//1�Ŷ���ӳ�ʼλ�ã�1343�����ĵ�servo_1_orign_position
	k_num=0x00;
	for(led0pwmval=a_point_top_down[2-1];led0pwmval>=a_point_top_up[2-1];led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=a_point_top_down[1-1]+k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		
		delay_ms_xm1(20);
    }
	
	
	TIM_SetCompare2(TIM1,a_point_top_up[2-1]);
	TIM_SetCompare1(TIM1,a_point_top_up[1-1]);
	
	delay_ms(200);
	
	open_claw_half();
	delay_ms_xm1(300);
	close_claw_half();
	
	
	
}

//////////////
//pick_up_a_ponit_1_2_3()
/////////////////
void car_pick_up_a_ponit_2_middle(void)
{
		//��ȡA�㶥��ɫ�顣��ʼצ��λ��Ϊ 	Tim1_Init(1400,1532,2500,1000);//ֱ��ֵ	Tim2_Init(1500,1560,270,250);
	//��ץ��λ��Ϊ xxx��1553,1237,947,2100
	//��צע���������4�Ŷ����Ȼ�����2�Ŷ������3�Ŷ�������5�Ŷ��
	
	//��ʼ��צ��4�Ŷ���ӳ�ʼλ�ã�1000�����ĵ�956	
	sprintf(buf1,"���£�4�ż�");
	
		
	for(led0pwmval=a_point_top_up[4-1];led0pwmval>=a_point_middle_down[4-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare4(TIM1,a_point_middle_down[4-1]);
	
	delay_ms_xm1(100);
	//delay_ms(1000);
	
	sprintf(buf1,"2�ż�");
	
		
	//4�ž�λ��2�Ŷ���ӳ�ʼλ�ã�1532�����ĵ�1632
	k_num=0x00;
	for(led0pwmval=servo_2_orign_position;led0pwmval<=a_point_middle_down[2-1];led0pwmval=led0pwmval+10)
    {
		
		
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=servo_1_orign_position-k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare2(TIM1,a_point_middle_down[2-1]);
	TIM_SetCompare1(TIM1,a_point_middle_down[1-1]);
	
	
	
	
	delay_ms(50);
	//delay_ms(1000);
	
	sprintf(buf1,"3�ż�");
	
		
	
	//2�ž�λ��3�Ŷ���ӳ�ʼλ�ã�2500�����ĵ�1290
	for(led0pwmval=servo_3_orign_position;led0pwmval>=a_point_middle_down[3-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms_xm1(20);
    }
	
	TIM_SetCompare3(TIM1,a_point_middle_down[3-1]);
	
	delay_ms(100);
		
	sprintf(buf1,"5�ż�");
	
		
	//3�ž�λ��5�Ŷ���ӳ�ʼλ�ã�1500�����ĵ�2100
	
	close_claw();
	
	delay_ms(100);
//	sprintf(buf1,"[s10]����д��������滹ûд������ͣ��");
//	
//		
//	car_stop();
//	while(1);
	
	
	
	
	////////////////////////
	//��ʼ̧��a�����
	////////////////////////////////
	
	
	
	//Ȼ��̧���е��
	sprintf(buf1,"̧��3�ż�");
	
			
	//3�Ŷ���ӳ�ʼλ�ã�1237�����ĵ�2500
	for(led0pwmval=a_point_middle_down[3-1];led0pwmval<=a_point_top_up[3-1];led0pwmval=led0pwmval+20)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms_xm1(20);
    }
	
	TIM_SetCompare3(TIM1,a_point_top_up[3-1]);
	delay_ms(200);

	
	
	sprintf(buf1,"4�ż�");
	
	
	//4�Ŷ���ӳ�ʼλ�ã�947�����ĵ�1000
	
	for(led0pwmval=a_point_middle_down[4-1];led0pwmval>=a_point_top_up[4-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare4(TIM1,a_point_top_up[4-1]);
	
	delay_ms(200);
	
	
	sprintf(buf1,"2�ż�");
	
	
	
	
	
	//2�Ŷ���ӳ�ʼλ�ã�1632�����ĵ�servo_2_orign_position
	//1�Ŷ���ӳ�ʼλ�ã�1343�����ĵ�servo_1_orign_position
	k_num=0x00;
	for(led0pwmval=a_point_middle_down[2-1];led0pwmval>=a_point_top_up[2-1];led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=a_point_middle_down[1-1]+k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		
		delay_ms_xm1(20);
    }
	
	
	TIM_SetCompare2(TIM1,a_point_top_up[2-1]);
	TIM_SetCompare1(TIM1,a_point_top_up[1-1]);
	
	delay_ms(200);
	
	open_claw_half();
	delay_ms_xm1(300);
	close_claw_half();
	
	
	



}


//////////////
//pick_up_a_ponit_1_2_3()
/////////////////
void car_pick_up_a_ponit_3_bottle(void)
{
			//��ȡA�㶥��ɫ�顣��ʼצ��λ��Ϊ 	Tim1_Init(1400,1532,2500,1000);//ֱ��ֵ	Tim2_Init(1500,1560,270,250);
	//��ץ��λ��Ϊ xxx��1553,1237,947,2100
	//��צע���������4�Ŷ����Ȼ�����2�Ŷ������3�Ŷ�������5�Ŷ��
	
	//��ʼ��צ��4�Ŷ���ӳ�ʼλ�ã�1000�����ĵ�956	
	sprintf(buf1,"���£�4�ż�");
	
		
	for(led0pwmval=a_point_top_up[4-1];led0pwmval>=a_point_bottle_down[4-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare4(TIM1,a_point_bottle_down[4-1]);
	
	delay_ms_xm1(100);
	//delay_ms(1000);
	
	sprintf(buf1,"2�ż�");
	
		
	//4�ž�λ��2�Ŷ���ӳ�ʼλ�ã�1532�����ĵ�1632
	k_num=0x00;
	for(led0pwmval=servo_2_orign_position;led0pwmval<=a_point_bottle_down[2-1];led0pwmval=led0pwmval+10)
    {
		
		
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=servo_1_orign_position-k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare2(TIM1,a_point_bottle_down[2-1]);
	TIM_SetCompare1(TIM1,a_point_bottle_down[1-1]);
	
	
	
	
	delay_ms(50);
	//delay_ms(1000);
	
	sprintf(buf1,"3�ż�");
	
		
	
	//2�ž�λ��3�Ŷ���ӳ�ʼλ�ã�2500�����ĵ�1290
	for(led0pwmval=servo_3_orign_position;led0pwmval>=a_point_bottle_down[3-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms_xm1(20);
    }
	
	TIM_SetCompare3(TIM1,a_point_bottle_down[3-1]);
	
	delay_ms(100);
		
	sprintf(buf1,"5�ż�");
	
		
	//3�ž�λ��5�Ŷ���ӳ�ʼλ�ã�1500�����ĵ�2100
	
	close_claw();
	delay_ms(100);
	
//	sprintf(buf1,"[s10]����д��������滹ûд������ͣ��");
//	
//		
//	car_stop();
//	while(1);
	
	
	
	
	////////////////////////
	//��ʼ̧��a�����
	////////////////////////////////
	
	
	
	//Ȼ��̧���е��
	sprintf(buf1,"̧��3�ż�");
	
			
	//3�Ŷ���ӳ�ʼλ�ã�1237�����ĵ�2500
	for(led0pwmval=a_point_bottle_down[3-1];led0pwmval<=a_point_top_up[3-1];led0pwmval=led0pwmval+20)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms_xm1(20);
    }
	
	TIM_SetCompare3(TIM1,a_point_top_up[3-1]);
	delay_ms(200);

	
	
	sprintf(buf1,"4�ż�");
	
	
	//4�Ŷ���ӳ�ʼλ�ã�947�����ĵ�1000
	
	for(led0pwmval=a_point_bottle_down[4-1];led0pwmval>=a_point_top_up[4-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare4(TIM1,a_point_top_up[4-1]);
	
	delay_ms(200);
	
	
	sprintf(buf1,"2�ż�");
	
	
	
	
	
	//2�Ŷ���ӳ�ʼλ�ã�1632�����ĵ�servo_2_orign_position
	//1�Ŷ���ӳ�ʼλ�ã�1343�����ĵ�servo_1_orign_position
	k_num=0x00;
	for(led0pwmval=a_point_bottle_down[2-1];led0pwmval>=a_point_top_up[2-1];led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=a_point_bottle_down[1-1]+k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		
		delay_ms_xm1(20);
    }
	
	
	TIM_SetCompare2(TIM1,a_point_top_up[2-1]);
	TIM_SetCompare1(TIM1,a_point_top_up[1-1]);
	
	delay_ms(200);
	
	open_claw_half();
	delay_ms_xm1(300);
	close_claw_half();
	
	
	





	
}
////////////////////////////////////
//
///////////////////////////////////////
//////////////////////////////////////
//
////////////////////////////////////////
void  car_pull_down_top(void)
{
    sprintf(buf1,"���£�4�ż�");
	
		
	for(led0pwmval=a_point_top_up[4-1];led0pwmval>=put_down_top_param[4-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare4(TIM1,put_down_top_param[4-1]);
	
	delay_ms_xm1(100);
	//delay_ms(1000);
	
	sprintf(buf1,"2�ż�");
	
		
	//4�ž�λ��2�Ŷ���ӳ�ʼλ�ã�1532�����ĵ�1632
	k_num=0x00;
	for(led0pwmval=a_point_top_up[2-1];led0pwmval<=put_down_top_param[2-1];led0pwmval=led0pwmval+10)
    {
		
		
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=a_point_top_up[1-1]-k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare2(TIM1,put_down_top_param[2-1]);
	TIM_SetCompare1(TIM1,put_down_top_param[1-1]);
	
	
	
	
	delay_ms(50);
	//delay_ms(1000);
	
	sprintf(buf1,"3�ż�");
	
		
	
	//2�ž�λ��3�Ŷ���ӳ�ʼλ�ã�2500�����ĵ�1290
	for(led0pwmval=servo_3_orign_position;led0pwmval>=put_down_top_param[3-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms_xm1(20);
    }
	
	TIM_SetCompare3(TIM1,put_down_top_param[3-1]);
	
	delay_ms(100);
		
	sprintf(buf1,"5�ż�");
	
		
	//3�ž�λ��5�Ŷ���ӳ�ʼλ�ã�1500�����ĵ�2100
	
	open_claw();
	
	
//	sprintf(buf1,"[s10]����д��������滹ûд������ͣ��");
//	
//		
//	car_stop();
//	while(1);
	
	
	
	
	////////////////////////
	//��ʼ̧��a�����
	////////////////////////////////
	
	
	
	//Ȼ��̧���е��
	sprintf(buf1,"̧��3�ż�");
	
			
	//3�Ŷ���ӳ�ʼλ�ã�1237�����ĵ�2500
	for(led0pwmval=put_down_top_param[3-1];led0pwmval<=a_point_top_up[3-1];led0pwmval=led0pwmval+20)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms_xm1(20);
    }
	
	TIM_SetCompare3(TIM1,a_point_top_up[3-1]);
	delay_ms(200);

	
	
	sprintf(buf1,"4�ż�");
	
	
	//4�Ŷ���ӳ�ʼλ�ã�947�����ĵ�1000
	
	for(led0pwmval=put_down_top_param[4-1];led0pwmval>=a_point_top_up[4-1];led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare4(TIM1,a_point_top_up[4-1]);
	
	delay_ms(200);
	
	
	sprintf(buf1,"2�ż�");
	
	
	
	
	
	//2�Ŷ���ӳ�ʼλ�ã�1632�����ĵ�servo_2_orign_position
	//1�Ŷ���ӳ�ʼλ�ã�1343�����ĵ�servo_1_orign_position
	k_num=0x00;
	for(led0pwmval=put_down_top_param[2-1];led0pwmval>=a_point_top_up[2-1];led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=put_down_top_param[1-1]+k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		
		delay_ms_xm1(20);
    }
	
	
	TIM_SetCompare2(TIM1,a_point_top_up[2-1]);
	TIM_SetCompare1(TIM1,a_point_top_up[1-1]);
	
	delay_ms(200);
	
	open_claw();
//	delay_ms_xm1(300);
//	close_claw();
}
//////////////////////////////////////
//
////////////////////////////////////////
void  car_pull_down_middle(void)
{
    sprintf(buf1,"���£�4�ż�");
	
		
	for(led0pwmval=a_point_top_up[4-1];led0pwmval>=put_down_middle_param[4-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare4(TIM1,put_down_middle_param[4-1]);
	
	delay_ms_xm1(100);
	//delay_ms(1000);
	
	sprintf(buf1,"2�ż�");
	
		
	//4�ž�λ��2�Ŷ���ӳ�ʼλ�ã�1532�����ĵ�1632
	k_num=0x00;
	for(led0pwmval=a_point_top_up[2-1];led0pwmval<=put_down_middle_param[2-1];led0pwmval=led0pwmval+10)
    {
		
		
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=a_point_top_up[1-1]-k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare2(TIM1,put_down_middle_param[2-1]);
	TIM_SetCompare1(TIM1,put_down_middle_param[1-1]);
	
	
	
	
	delay_ms(50);
	//delay_ms(1000);
	
	sprintf(buf1,"3�ż�");
	
		
	
	//2�ž�λ��3�Ŷ���ӳ�ʼλ�ã�2500�����ĵ�1290
	for(led0pwmval=servo_3_orign_position;led0pwmval>=put_down_middle_param[3-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms_xm1(20);
    }
	
	TIM_SetCompare3(TIM1,put_down_middle_param[3-1]);
	
	delay_ms(100);
		
	sprintf(buf1,"5�ż�");
	
		
	//3�ž�λ��5�Ŷ���ӳ�ʼλ�ã�1500�����ĵ�2100
	
	open_claw();
	
	
//	sprintf(buf1,"[s10]����д��������滹ûд������ͣ��");
//	
//		
//	car_stop();
//	while(1);
	
	
	
	
	////////////////////////
	//��ʼ̧��a�����
	////////////////////////////////
	
	
	
	//Ȼ��̧���е��
	sprintf(buf1,"̧��3�ż�");
	
			
	//3�Ŷ���ӳ�ʼλ�ã�1237�����ĵ�2500
	for(led0pwmval=put_down_middle_param[3-1];led0pwmval<=a_point_top_up[3-1];led0pwmval=led0pwmval+20)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms_xm1(20);
    }
	
	TIM_SetCompare3(TIM1,a_point_top_up[3-1]);
	delay_ms(200);

	
	
	sprintf(buf1,"4�ż�");
	
	
	//4�Ŷ���ӳ�ʼλ�ã�947�����ĵ�1000
	
	for(led0pwmval=put_down_middle_param[4-1];led0pwmval>=a_point_top_up[4-1];led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare4(TIM1,a_point_top_up[4-1]);
	
	delay_ms(200);
	
	
	sprintf(buf1,"2�ż�");
	
	
	
	
	
	//2�Ŷ���ӳ�ʼλ�ã�1632�����ĵ�servo_2_orign_position
	//1�Ŷ���ӳ�ʼλ�ã�1343�����ĵ�servo_1_orign_position
	k_num=0x00;
	for(led0pwmval=put_down_middle_param[2-1];led0pwmval>=a_point_top_up[2-1];led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=put_down_middle_param[1-1]+k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		
		delay_ms_xm1(20);
    }
	
	
	TIM_SetCompare2(TIM1,a_point_top_up[2-1]);
	TIM_SetCompare1(TIM1,a_point_top_up[1-1]);
	
	delay_ms(200);
	
	open_claw();
//	delay_ms_xm1(300);
//	close_claw();
}
//////////////////////////////////////
//
////////////////////////////////////////
void  car_pull_down_bottle(void)
{
    sprintf(buf1,"���£�4�ż�");
	
		
	for(led0pwmval=a_point_top_up[4-1];led0pwmval>=put_down_bottle_param[4-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare4(TIM1,put_down_bottle_param[4-1]);
	
	delay_ms_xm1(100);
	//delay_ms(1000);
	
	sprintf(buf1,"2�ż�");
	
		
	//4�ž�λ��2�Ŷ���ӳ�ʼλ�ã�1532�����ĵ�1632
	k_num=0x00;
	for(led0pwmval=a_point_top_up[2-1];led0pwmval<=put_down_bottle_param[2-1];led0pwmval=led0pwmval+10)
    {
		
		
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=a_point_top_up[1-1]-k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare2(TIM1,put_down_bottle_param[2-1]);
	TIM_SetCompare1(TIM1,put_down_bottle_param[1-1]);
	
	
	
	
	delay_ms(50);
	//delay_ms(1000);
	
	sprintf(buf1,"3�ż�");
	
		
	
	//2�ž�λ��3�Ŷ���ӳ�ʼλ�ã�2500�����ĵ�1290
	for(led0pwmval=a_point_top_up[3-1];led0pwmval>=put_down_bottle_param[3-1];led0pwmval=led0pwmval-20)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms_xm1(20);
    }
	
	TIM_SetCompare3(TIM1,put_down_bottle_param[3-1]);
	
	delay_ms(100);
		
	sprintf(buf1,"5�ż�");
	
		
	//3�ž�λ��5�Ŷ���ӳ�ʼλ�ã�1500�����ĵ�2100
	
	open_claw();
	
	
//	sprintf(buf1,"[s10]����д��������滹ûд������ͣ��");
//	
//		
//	car_stop();
//	while(1);
	
	
	
	
	////////////////////////
	//��ʼ̧��a�����
	////////////////////////////////
	
	
	
	//Ȼ��̧���е��
	sprintf(buf1,"̧��3�ż�");
	
			
	//3�Ŷ���ӳ�ʼλ�ã�1237�����ĵ�2500
	for(led0pwmval=put_down_bottle_param[3-1];led0pwmval<=a_point_top_up[3-1];led0pwmval=led0pwmval+20)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms_xm1(20);
    }
	
	TIM_SetCompare3(TIM1,a_point_top_up[3-1]);
	delay_ms(200);

	
	
	sprintf(buf1,"4�ż�");
	
	
	//4�Ŷ���ӳ�ʼλ�ã�947�����ĵ�1000
	
	for(led0pwmval=put_down_bottle_param[4-1];led0pwmval>=a_point_top_up[4-1];led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms_xm1(10);
    }
	
	TIM_SetCompare4(TIM1,a_point_top_up[4-1]);
	
	delay_ms(200);
	
	
	sprintf(buf1,"2�ż�");
	
	
	
	
	
	//2�Ŷ���ӳ�ʼλ�ã�1632�����ĵ�servo_2_orign_position
	//1�Ŷ���ӳ�ʼλ�ã�1343�����ĵ�servo_1_orign_position
	k_num=0x00;
	for(led0pwmval=put_down_bottle_param[2-1];led0pwmval>=a_point_top_up[2-1];led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=put_down_bottle_param[1-1]+k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		
		delay_ms_xm1(20);
    }
	
	
	TIM_SetCompare2(TIM1,a_point_top_up[2-1]);
	TIM_SetCompare1(TIM1,a_point_top_up[1-1]);
	
	delay_ms(200);
	
	open_claw();
//	delay_ms_xm1(300);
//	close_claw();
}
////////////////
//
/////////////////

void turn_right_90_1(void)		   //��ת90��
{
	turn_num=turn_num+4;//�ҷ���ÿ��˳ʱ��תһȦ��6060����0.6�ȣ���ʱ��һȦ���ض�0.34�ȣ���������180��ʱƽ��
    
    sprintf(buf1,"��ת90��");
	
		
    
    led0pwmval=1540;//high mean low speed
	
		//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1540;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	pwm_num=0x00;
	
	
	
	//while(pwm_num<=97);//5v ��ѹ
	while(pwm_num<=109);//6v ��ѹ 
	
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
}

/////////////////////
//
/////////////////////////
void car_yellow_pull_down_block(void)
{
    while(1)
    {
        car_position_test_all_white();
		
		       
        car_judge_decision();
		car_excute_decision();
        
        if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
        {
            
            
            sprintf(buf1,"[s10]β��");
            
            
            
            break;
            
        }
    }
        
        
        //��ʼ׼������� //С����λ��׼�������
        
    if(count_stack_yellow==0)
    {
        car_pull_down_bottle();
        count_stack_yellow=1;
        
    }
     else if(count_stack_yellow==1)
    {
        car_pull_down_middle();
        count_stack_yellow=2;
        
    }   
    
     else if(count_stack_yellow==2)
    {
        car_pull_down_top();
        count_stack_yellow=3;
        
    }   
        
        //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
    turn_left_180_with_6050_check(90.0);
    
    //׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
    
    walk_ae_state=0;
    while(1)
    {
        car_position_test_all_white();
        
        if(walk_ae_state==0)
        {
            car_judge_decision();
            car_excute_decision();
            
            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
            {
                walk_ae_state=1;
                
                sprintf(buf1,"[s10]ͷ��");
                
                
                
            }
            
            first_start_state_1=0x00;
            
        }
        
        
        else if(walk_ae_state==1)
        {
            if(first_start_state_1==0)
            {
                first_start_state_1=1;

                pwm_num_distance=0;
        
            }
            car_judge_decision();
            car_excute_decision();
            //ֱ��100���ף������غ�
            if(pwm_num_distance>=81)
            {
                walk_ae_state=2;
                sprintf(buf1,"[s10]100��");
                
                
                break;
                
            }
            
        }
        

        
    }
    //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
    turn_right_90_1();//turn_right_90_with_6050_check(0.0);
    mpu_6050_check(0);    
        //׼��ǰ��2������ֱ��ֱ��ͷ�ڣ�ԭ�ص�ͷ180
     if(0==qti_1 && 0==qti_2 && 0==qti_3)
    {
        mpu_6050_white();
    }     
    walk_ae_state=0;
    while(1)
    {
        car_position_test_all_white();
        
        if(walk_ae_state==0)
        {
            car_judge_decision();
            car_excute_decision();
            
            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
            {
                walk_ae_state=1;
                
                sprintf(buf1,"[s10]ͷ��");
                
                
                
                break;
                
            }
            
            first_start_state_1=0x00;
            
        }
        
        
    }
    //ԭ�ص�ͷ180�ȣ�ָ��180�ȷ���  
    turn_left_180_with_6050_check(90.0);    
     pwm_num_distance=0;    
    while(1)
    {
        car_position_test_all_white();
        
        
        car_judge_decision();
        car_excute_decision();
        //ֱ��10���ף��ܿ�����
        if(pwm_num_distance>=11)
        {
            walk_ae_state=2;
            sprintf(buf1,"[s10]2������");
            
            
            
            break;
            
        }
       
    }
    //��2������׼��������һ����飬  �������������1  
    grasp_order_index++;
        
        
			
    
        
        
        
    
}
//////////////////////
//
///////////////////
void car_yellow_not_pull_down_block(void)
{
    
    
    
}

////////////////////
//
/////////////////////


//////////////////////////
//
////////////////////////


///////////////////
//
/////////////////////////


//////////////



//////////////////////////////////
//
////////////////////////////////
void car_point_a_pull_up_down()
{
	
    //ԭ����ת90�ȣ�ָ��270�ȷ���  
    turn_left_90_with_6050_check(270.0);
	
	if(1==grasp_order[grasp_order_index])
	{
		car_pick_up_a_ponit_1_top();
	}
	
	else if(2==grasp_order[grasp_order_index])
	{
		
        car_pick_up_a_ponit_2_middle();
	}
	
	else if(3==grasp_order[grasp_order_index])
	{
		
        car_pick_up_a_ponit_3_bottle();
	}
    
    //�ж���ɫ��Ҳ����ֱ�ӷ�����ɫ�������飬��ֱ�ӷ�����ɫ��������
    if(yellow==block_colour_order[grasp_order_index])
    {
        if(count_point_a<=0)
        {
            car_yellow_pull_down_block();
        }
        else
        {
            car_yellow_not_pull_down_block();
        }
    }
    else if(white==block_colour_order[grasp_order_index])
	{
        
    }
    else if(red==block_colour_order[grasp_order_index])
	{
        
    }
    else if(black==block_colour_order[grasp_order_index])
	{
        
    }
    else if(blue==block_colour_order[grasp_order_index])
	{
        
    }
	
	
}

//////////////////////////////////
//
////////////////////////////////
void car_point_b_pull_up_down()
{
	
}

//////////////////////////////////
//
////////////////////////////////
void car_point_c_pull_up_down()
{
	
}

//////////////////////////////////
//
////////////////////////////////
void car_excute_decision_1(void)
{
	
	
	if(car_desicion==-10)
	{
		led0pwmval=1460;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		 led0pwmval=1460;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=10);//6v ��ѹ 
		
		
		car_fastforward();
		
		
	}
	
	
	
	
	
	if(car_desicion==-7)
	{
		led0pwmval=1460;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		 led0pwmval=1460;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=7);//6v ��ѹ 
		
		
		car_fastforward();
	}
	
	
	
	
	
	if(car_desicion==-3)
	{
		led0pwmval=1460;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		 led0pwmval=1460;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=3);//6v ��ѹ 
		
		
		car_fastforward();
	}
	
	if(car_desicion==0)
	{
		car_fastforward();
	}
	
	
	
	
	if(car_desicion==3)
	{
		 led0pwmval=1540;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		 led0pwmval=1540;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
		
		 pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=3);//6v ��ѹ 
		
		
		car_fastforward();
	}
	
	if(car_desicion==7)
	{
		 led0pwmval=1540;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		 led0pwmval=1540;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
		
		 pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=7);//6v ��ѹ 
		
		
		car_fastforward();
	}
	
	if(car_desicion==10)
	{
		  led0pwmval=1540;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		 led0pwmval=1540;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
		
		 pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=10);//6v ��ѹ 
		
		
		car_fastforward();
		
		
	}
	
	
}


////////////////////////////////////
/////
//////////////////////////////////

void car_excute_decision_2(void)
{
	
	
	if(car_desicion==-10)
	{
		car_left_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=30);//6v ��ѹ 
		
		
		car_fastforward();
		
		
	}
	
	
	
	
	
	if(car_desicion==-7)
	{
		car_left_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=20);//6v ��ѹ 
		
		
		car_fastforward();
	}
	
	
	
	
	
	if(car_desicion==-3)
	{
		car_left_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=10);//6v ��ѹ 
		
		
		car_fastforward();
	}
	
	if(car_desicion==0)
	{
		car_fastforward();
	}
	
	
	
	
	if(car_desicion==3)
	{
		car_right_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=10);//6v ��ѹ 
		
		
		car_fastforward();
		
		
	}
	
	if(car_desicion==7)
	{
		 car_right_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=20);//6v ��ѹ 
		
		
		car_fastforward();
	}
	
	if(car_desicion==10)
	{
		car_right_turn();
		
		pwm_num=0;
		
			
		
		//while(pwm_num<=54);//5v ��ѹ
		while(pwm_num<=30);//6v ��ѹ 
		
		
		car_fastforward();
		
		
	}
	
	
}
//////////////////////////////////
/////
//////////////////////////////////








void car_detec_position_decision_action(void)
{
	car_position_test();
	//��⳵�ڳ����㣬׼����˫���ǰ����֪����ͷ��������
	if(walk_ae_state==0)
	{
		car_judge_decision();
		car_excute_decision();
		
		if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
		{
			walk_ae_state=1;
			
			sprintf(buf1,"[s10]1");
		
		
			
		}
		
		first_start_state_1=0x00;
		
	}
	
	
	if(walk_ae_state==1)
	{
		if(first_start_state_1==0)
		{
			first_start_state_1=1;

			pwm_num_distance=0;
	
		}
		car_judge_decision();
		car_excute_decision();
		
		if(pwm_num_distance>=41)
		{
			walk_ae_state=2;
			sprintf(buf1,"[s10]2");
		
		
		}
		
	}
	
	if(walk_ae_state==2)
	{
		car_judge_decision();
		car_excute_decision();
		
		if(1==qti_1&& 1==qti_2 && 1==qti_3 && 1==qti_4)
		//if(1==qti_8&& 1==qti_9 && 1==qti_10 && 1==qti_11)
		{
			walk_ae_state=3;
			first_start_state_1=0;
			sprintf(buf1,"[s10]3");
		
		
			
		}
		first_start_state_1=0;
		
		
		
	}
	
	
//	if(walk_ae_state==3)
//	{
//		if(first_start_state_1==0)
//		{
//			first_start_state_1=1;

//			pwm_num_distance=0;
//	
//		}
//		car_judge_decision();
//		car_excute_decision();
//		
//		if(pwm_num_distance==31)
//		{
//			walk_ae_state=4;
//		}
//		
//		
//		
//	}
	
	
//	if(walk_ae_state==3)
//	{
//		car_judge_decision();
//		car_excute_decision();
//		
//		if(1==qti_1&& 1==qti_2 && 1==qti_3 && 1==qti_4)
//		{
//			walk_ae_state=4;
//			
//		}
//		first_start_state_1=0;
//		
//		
//		
//	}
	
	
//	if(walk_ae_state==3)
//	{
//		if(first_start_state_1==0)
//		{
//			first_start_state_1=1;

//			pwm_num_distance=0;
//	
//		}
//		car_judge_decision();
//		car_excute_decision();
//		
//		if(pwm_num_distance==31)
//		{
//			walk_ae_state=4;
//		}
//		
//		
//		
//	}
//	
	if(walk_ae_state==3)
	{
		car_judge_decision();
		car_excute_decision();
		
		
		first_start_state_1=0;
		
		
		
	}
}
	
	
	


////////////////////////////////////
/////
//////////////////////////////////
//car_position()

////////////////////////////////////
/////
//////////////////////////////////
//car_position()

////////////////////////////////////
/////
////////////////////////////////////
//car_position()



void car_action_judge(void)
{
	
	car_position_test();
	
	
	if(0==qti_5 && 0==qti_6)//�ɳ�ͷqti���ǳ�λqti����ת��
	{
		if(axis_dirrection!=axis_forward)
        {
            axis_dirrection=axis_forward;
            
            if(axis_dirrection==axis_turn_left)
            {
                if(walk_ae_state==0x00||walk_ae_state==0x02)
                {
                     car_return_strigt_turn_right_tail();
                }
                if(walk_ae_state==0x01)
                {
                     car_return_strigt_turn_right_head();
                }
            }
            else
            {
                if(walk_ae_state==0x00||walk_ae_state==0x02)
                {
                      car_return_strigt_turn_left_tail();
                }
                if(walk_ae_state==0x01)
                {
                     car_return_strigt_turn_left_head();
                }
            }
        }
        else
        {
            if(car_head_flag==0x00)
            {
                //��βqti�ж�ת��
                //�������������β��������010����ͷƫ����011����ͷ����ƫ��001����ͷƫ����110����ͷ����ƫ��100�����ⰸ��������111ȫ����Ҫ����ȫ����ȫ��������������Ȩ��ȫ�Ʋ�����ֱ�У�
                if(qti_8==0 && qti_9==1 && qti_10==0)//��ͷ������ȷ
                {
                    fastforward();
                    
                    
                    sprintf(buf1,"[s10]ֱ��");
                    
                    	
                }
                
                if(qti_8==0 && qti_9==1 && qti_10==1)//��Сƫ
                {
                    car_left_turn();
                    sprintf(buf1,"[s10]Lת");
                    
                    	
                }
                
                if(qti_8==0 && qti_9==0 && qti_10==1)//�Ҵ�ƫ
                {
                    car_left_turn_big();
                    sprintf(buf1,"[s10]Lת");
                    
                    	
                }
                
                
                if(qti_8==1 && qti_9==1 && qti_10==0)//��Сƫ
                {
                    car_right_turn();
                    sprintf(buf1,"[s10]��ת");
                    
                    	
                }
                
                if(qti_8==1 && qti_9==0 && qti_10==0)//���ƫ
                {
                    car_right_turn_big();
                    sprintf(buf1,"[s10]��ת");
                    
                    	
                }
                
                if(qti_8==1 && qti_9==1 && qti_10==1&&qti_11==1)//ȫ5����������Ȩ
                {
                    fastforward();
                    
                    sprintf(buf1,"[s10]��Ȩ");
                    
                    
                    
                    car_head_flag=1;
                    walk_ae_state=1;
                    
                }
                
                if(qti_8==1 && qti_9==1 && qti_10==1&&qti_11==0)
                {
                    fastforward();
                    
                    sprintf(buf1,"[s10]ֱ��");
                    
                    
                    
    //				car_head_flag=1;
                    
                }
                
                
                if(qti_8==0 && qti_9==0 && qti_10==0)//���ƫ
                {
                    
                    if((qti_1==0 && qti_2==1 && qti_3==0)||qti_1==1 && qti_2==1 && qti_3==1)//��ͷ������ȷ
                    {
                        fastforward();
                        sprintf(buf1,"[s10]ֱ��");
                        
                        
                    }
                    
                    if(qti_1==0 && qti_2==1 && qti_3==1)//��Сƫ
                    {
                        car_right_turn();
                        sprintf(buf1,"[s10]��ת");
                        
                        
                    }
                    
                    if(qti_1==0 && qti_2==0 && qti_3==1)//���ƫ
                    {
                        car_right_turn_big();
                        sprintf(buf1,"[s10]��ת");
                        
                        
                    }
                    
                    
                    if(qti_1==1 && qti_2==1 && qti_3==0)//��Сƫ
                    {
                        car_left_turn();
                        
                        sprintf(buf1,"[s10]Lת");
                        
                        
                    }
                    
                    if(qti_1==1 && qti_2==0 && qti_3==0)//�Ҵ�ƫ
                    {
                        car_left_turn_big();
                        sprintf(buf1,"[s10]Lת");
                        
                        
                    }
                    
                    
    //				car_right_turn_big();
    //				sprintf(buf1,"[s10]��ת");
    //				
    //					
                }
                

                
                
                
            }
            else//��ͷqti�ж�ת��
            {
			//��ͷqti�ж�ת��
			if(qti_1==0 && qti_2==1 && qti_3==0)//��ͷ������ȷ
			{
				fastforward();
				sprintf(buf1,"[s10]ֱ��");
				
				
			}
			
			if(qti_1==0 && qti_2==1 && qti_3==1)//��Сƫ
			{
				car_right_turn();
				sprintf(buf1,"[s10]��ת");
				
				
			}
			
			if(qti_1==0 && qti_2==0 && qti_3==1)//���ƫ
			{
				car_right_turn_big();
				sprintf(buf1,"[s10]��ת");
				
				
			}
			
			
			if(qti_1==1 && qti_2==1 && qti_3==0)//��Сƫ
			{
				car_left_turn();
				
				sprintf(buf1,"[s10]Lת");
				
				
			}
			
			if(qti_1==1 && qti_2==0 && qti_3==0)//�Ҵ�ƫ
			{
				car_left_turn_big();
				sprintf(buf1,"[s10]Lת");
				
				
			}
			
			if(qti_1==1 && qti_2==1 && qti_3==1&&qti_4==1)
			{
				fastforward();
				sprintf(buf1,"[s10]��Ȩ");
				
				
				
				car_head_flag=0;
				walk_ae_state=2;
				
			}
			
			if(qti_1==1 && qti_2==1 && qti_3==1&&qti_4==0)
			{
				fastforward();
				sprintf(buf1,"[s10]ֱ��");
				
				
				
				
			}
			
			if(qti_1==0 && qti_2==0 && qti_3==0)//��ͷ������ȷ
			{
				
				if((qti_8==0 && qti_9==1 && qti_10==0)||qti_8==1 && qti_9==1 && qti_10==1)//��ͷ������ȷ
				{
					fastforward();
					
					
					sprintf(buf1,"[s10]ֱ��");
					
						
				}
				
				if(qti_8==0 && qti_9==1 && qti_10==1)//��Сƫ
				{
					car_left_turn();
					sprintf(buf1,"[s10]Lת");
					
						
				}
				
				if(qti_8==0 && qti_9==0 && qti_10==1)//�Ҵ�ƫ
				{
					car_left_turn_big();
					sprintf(buf1,"[s10]Lת");
					
						
				}
				
				
				if(qti_8==1 && qti_9==1 && qti_10==0)//��Сƫ
				{
					car_right_turn();
					sprintf(buf1,"[s10]��ת");
					
						
				}
				
				if(qti_8==1 && qti_9==0 && qti_10==0)//���ƫ
				{
					car_right_turn_big();
					sprintf(buf1,"[s10]��ת");
					
						
				}
				
				
				
//				fastforward();
//				sprintf(buf1,"[s10]ֱ��");
//				
//				
			}
			
			
			
		}
	 
        }
        
        
    }
	else//ʹ������qti��������ת�䣬ֱ��
	{
		if(walk_ae_state==0x00)
		{
			if(qti_5==1&&qti_6==0)//��Сƫ
			{
				car_left_turn_big();
                
                axis_dirrection=axis_turn_left;
                
				sprintf(buf1,"[s10]Lת");
				
				
                
                axis_turn_right_count=0x00;
                axis_turn_left_count++;
                if(axis_turn_left_count>=0x0c)
                {
                    axis_turn_left_count=0;
                    car_return_strigt_turn_right_tail();
                }
                
				
			}
			
			if(qti_5==0&&qti_6==1)//��Сƫ
			{
				car_right_turn_big();
                axis_dirrection=axis_turn_right;
                
				sprintf(buf1,"[s10]��ת");
				
				
                
                axis_turn_right_count++;
                axis_turn_left_count=0x00;
                if(axis_turn_right_count>=0x0c)
                {
                    axis_turn_right_count=0;
                    car_return_strigt_turn_left_tail();
                }
			}
			
			if(qti_5==1&&qti_6==1)//ֱ��
			{
				fastforward();
                
                axis_dirrection=axis_forward;
                
                axis_turn_right_count=0x00;;
                axis_turn_left_count=0x00;
                
				sprintf(buf1,"[s10]ֱ��");
				
				
			}
			
			if(qti_8==1 && qti_9==1 && qti_10==1&&qti_11==1)//ȫ5����������Ȩ
			{
				
				car_head_flag=1;
				walk_ae_state=1;
				fastforward();
                axis_dirrection=axis_forward;
                
				sprintf(buf1,"[s10]��Ȩ");
				
				
				
			}
			
		}
		
		//���غ�---��ͷ��qti�ж�
		if(walk_ae_state==0x01)
		{
			if(qti_5==1&&qti_6==0)//��Сƫ
			{
				car_left_turn_big();
                axis_dirrection=axis_turn_left;
                
				sprintf(buf1,"[s10]Lת");
				
				
                
                axis_turn_right_count=0x00;
                axis_turn_left_count++;
                if(axis_turn_left_count>=0x0c)
                {
                    axis_turn_left_count=0;
                    car_return_strigt_turn_right_head();
                }
                
				
			}
			
			if(qti_5==0&&qti_6==1)//��Сƫ
			{
				car_right_turn_big();
                axis_dirrection=axis_turn_right;
                
				sprintf(buf1,"[s10]��ת");
				
				
                
                axis_turn_right_count++;
                axis_turn_left_count=0x00;
                if(axis_turn_right_count>=0x0c)
                {
                    axis_turn_right_count=0;
                    car_return_strigt_turn_left_head();
                }
			}
			
			if(qti_5==1&&qti_6==1)//ֱ��
			{
				fastforward();
                
                axis_dirrection=axis_forward;
                
                axis_turn_right_count=0x00;;
                axis_turn_left_count=0x00;
                
				sprintf(buf1,"[s10]ֱ��");
				
				
			}
			
			if(qti_1==1 && qti_2==1 && qti_3==1&&qti_4==1)
			{
				
				car_head_flag=0;
				walk_ae_state=2;
				fastforward();
                
                axis_dirrection=axis_forward;
				walk_ae_state=1;sprintf(buf1,"[s10]��Ȩ");
				
				
				
			}
			
		}
		
		
		if(walk_ae_state==0x02)
		{
			if(qti_5==1&&qti_6==0)//��Сƫ
			{
				car_left_turn_big();
                axis_dirrection=axis_turn_left;
                
				sprintf(buf1,"[s10]Lת");
				
				
                
                axis_turn_right_count=0x00;
                axis_turn_left_count++;
                if(axis_turn_left_count>=0x0c)
                {
                    axis_turn_left_count=0;
                    
                    car_return_strigt_turn_right_tail();
                }
                
				
			}
			
			if(qti_5==0&&qti_6==1)//��Сƫ
			{
				car_right_turn_big();
                axis_dirrection=axis_turn_right;
                
				sprintf(buf1,"[s10]��ת");
				
				
                
                axis_turn_right_count++;
                axis_turn_left_count=0x00;
                if(axis_turn_right_count>=0x0c)
                {
                    axis_turn_right_count=0;
                    car_return_strigt_turn_left_tail();
                }
			}
			
			if(qti_5==1&&qti_6==1)//ֱ��
			{
				fastforward();
                
                axis_dirrection=axis_forward;
                
                axis_turn_right_count=0x00;;
                axis_turn_left_count=0x00;
                
				sprintf(buf1,"[s10]ֱ��");
				
				
			}
			
			if(qti_5==1 && qti_6==1 && qti_7==1)
			{
				
				car_stop();
				sprintf(buf1,"[s10]����");
				
				
				
			}
			
		}
		
		
	}

}

/*
 * ǰ���м�QTIѭ��
 */
bool car_walk_to_middle(void)
{
	car_detec_position_decision_action();
	
	if(qti_5==1&&qti_6==1&&qti_7==1&&(walk_ae_state==0x05||walk_ae_state==0x04||walk_ae_state==0x03))
	{
		car_stop();
		delay_ms_xm1(100);
		return true;
	}
	else
	{
		delay_ms_xm1(20);
		//car_stop();
		return false;
	}
	
	
	
	
	
	
	
	
}



bool MoveMiddleQti(void)
{
	delay_ms(20);
//	if(QTI_State(QTI2_Pin)==true && QTI_State(QTI3_Pin)==true)
//	{
//		fastforward();
//	}
//	else if(QTI_State(QTI2_Pin)==true && QTI_State(QTI3_Pin)==false)
//	{
//		right_turn();
//	}
//	else if(QTI_State(QTI2_Pin)==false && QTI_State(QTI3_Pin)==true)
//	{
//		left_turn();
//	}
//	else if(QTI_State(QTI2_Pin)==false && QTI_State(QTI3_Pin)==false)
//	{
//		return true;
//	}
//	return false;
	if(qti_2==0 && qti_3==0)
	{
		fastforward();
			printf("2=0  3=0 \n" );
		return false;
	}
	if(qti_2==1 && qti_3==0)
	{
		left_turn();
		return false;
	}
	if(qti_2==0 && qti_3==1)
	{
		right_turn();
		return false;
	}
	if(qti_2==1 && qti_3==1)
	{
		
		
		car_stop();
		//��ֹ���ţ�ǰ��һЩ���ٴ��ж�
		while(pwm_num<3+3)
		{
			fastforward();
			     
		}
		car_stop();
		
		if(qti_2==1 && qti_3==1)
		{
		
			
			car_stop();
			return true;
		}
		else
		{
			return false;
		}
		
		
		
//		while(1);
		
	}
	return false;
	
	
	
}


bool MoveMiddleQti_qti7_stop(void)
{
	delay_ms(20);
//	if(QTI_State(QTI2_Pin)==true && QTI_State(QTI3_Pin)==true)
//	{
//		fastforward();
//	}
//	else if(QTI_State(QTI2_Pin)==true && QTI_State(QTI3_Pin)==false)
//	{
//		right_turn();
//	}
//	else if(QTI_State(QTI2_Pin)==false && QTI_State(QTI3_Pin)==true)
//	{
//		left_turn();
//	}
//	else if(QTI_State(QTI2_Pin)==false && QTI_State(QTI3_Pin)==false)
//	{
//		return true;
//	}
//	return false;
	if(qti_2==0 && qti_3==0)
	{
		fastforward();
			printf("2=0  3=0 \n" );
		
		
		//return false;
	}
	if(qti_2==1 && qti_3==0)
	{
		left_turn();
		//return false;
	}
	if(qti_2==0 && qti_3==1)
	{
		right_turn();
		//return false;
	}
	
	if(qti_5==0x00&&qti_7==0x00&&qti_6==0x00)
		{
			
			
			car_stop();
			//��ֹ���ţ�ǰ��һЩ���ٴ��ж�
	//		while(pwm_num<3+3)
	//		{
	//			fastforward();
	//			     
	//		}
	//		car_stop();
	//		
	//		if(qti_2==1 && qti_3==1)
	//		{
	//		
	//			
	//			car_stop();
	//			return true;
	//		}
	//		else
	//		{
	//			return false;
	//		}
	//		while(1);
			return true;
			
			
			
		}
	
	return false;
	
	
	
}

////////////////
//MoveEdgeQti
////////////////////


bool MoveEdgeQti(void)
{
//	if(QTI_State(QTI1_Pin)==true && QTI_State(QTI4_Pin)==true)
//	{
//		fastforward();
//	}
//	else if(QTI_State(QTI1_Pin)==true && QTI_State(QTI4_Pin)==false)
//	{
//		right_turn();
//	}
//	else if(QTI_State(QTI1_Pin)==false && QTI_State(QTI4_Pin)==true)
//	{
//		left_turn();
//	}
//	else if(QTI_State(QTI1_Pin)==false && QTI_State(QTI4_Pin)==false)
//	{
//		return true;
//	}
//	return false;
	
	delay_ms(22);//�ȴ�һ�����������ɣ�����ʱ����ͣ�ط���io�����ã�ͬʱ����ʱ�������ͣ�ؽ��������֪������ʲô����źá�2019.06.06 9:25
	
	if(qti_1==0 && qti_4==0)
	{
		if(qti_2==1 && qti_3==1)
		{
			fastforward_xm();//����λ���ҳ���ֱ
			
			//printf("    front=  %d",front_value )
			printf("2=1   3=1 \n" );
			
		}
		
		if(qti_2==1 && qti_3==0)
		{
			left_turn();
			printf("2=1   3=0 \n" );
			
		}
		if(qti_2==0 && qti_3==1)
		{
			right_turn();
				printf("2=0   3=1 \n" );
			
		}
		if(qti_2==0 && qti_3==0)
		{
			fastforward_xm();//����λ���ҳ���ֱ
				printf("2=0   3=0 \n" );
		}
		

		return false;	
	}
	if(qti_1==0 && qti_4==1)
	{
		right_turn();
		return false;
	}
	if(qti_1==1 && qti_4==0)
	{
		left_turn();
		return false;
	}
	
	
	//����4 �ں�4.1�� 4.2 ��4.3
	if(qti_1==1 && qti_4==1)
	{
		
		//�Ƿ���4.1
		if(qti_2==0 && qti_3==0)
		{
			car_stop();//����λ���ҳ���ֱ
			printf("2=0   3=0 \n" );
			return true;
		}	
		//�Ƿ���4.2
		if(qti_2==1 && qti_3==0)
		{
			//ԭ����ת ����1450 �Ҷ��1450
			printf("2=1   3=0 \n" );
			 pwm_num=0x00;
			while(!(qti_2==0 && qti_3==0))
			{
				 led0pwmval=1460;//high mean low speed
				 TIM_SetCompare3(TIM3,led0pwmval);
				 
				//tim3 2# is left servo
				 led0pwmval=1460;//high mean low speed
				 TIM_SetCompare4(TIM3,led0pwmval);

				if(pwm_num>20)
				{
					break;
					
				}
				
			}

			car_stop();
			

			return false;
		}
		
		//�Ƿ���4.3
		if(qti_2==0 && qti_3==1)
		{
			//ԭ����ת ����1550 �Ҷ��1550
			printf("2=0   3=1 \n" );
			 pwm_num=0x00;
			while(!(qti_2==0 && qti_3==0))
			{
				 led0pwmval=1550;//high mean low speed
				 TIM_SetCompare3(TIM3,led0pwmval);
				 
				//tim3 2# is left servo
				 led0pwmval=1550;//high mean low speed
				 TIM_SetCompare4(TIM3,led0pwmval);

				if(pwm_num>20)
				{
					break;
					
				}
				
			}

			car_stop();
			return false;
		}
	}
	
	return false;
	
}
///////////////////////////////
//
/////////////////////////////////


void walk_alone_MiddleQti(void)
{

	if(qti_2==0 && qti_3==0)
	{
		fastforward();
		printf("2=0   3=0 \n" );
		
	}
	if(qti_2==1 && qti_3==0)
	{
		left_turn();
		printf("2=1   3=0 \n" );
		
	}
	if(qti_2==0 && qti_3==1)
	{
		right_turn();
		printf("2=0   3=1 \n" );
		
	}
	if(qti_2==1 && qti_3==1)
	{
		
		printf("2=1   3=1 \n" );
		car_stop();
		//��ֹ���ţ�ǰ��һЩ���ٴ��ж�
		while(pwm_num<3+3)
		{
			fastforward();
			     
		}
		car_stop();
		
		if(qti_2==1 && qti_3==1)
		{

			car_stop();
			
		}
		

		
	}

}
////////////////
//
/////////////////

////////////////
//
/////////////////

////////////////
//
/////////////////

void turn_right_135()		   //��ת90��
{
	
   turn_num=turn_num+6;//�ҷ���ÿ��˳ʱ��תһȦ��6060����0.6�ȣ���ʱ��һȦ���ض�0.34�ȣ���������180��ʱƽ��
    
    led0pwmval=1540+2;//high mean low speed//2019.10.11�ҷ���С������ת90��ʱ�������������е���ת��
                      //�����е�ƫ��һ����ת���ʴˣ����ּ��٣����ּ��٣�����С�����������е���ת
	
		//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1540-2;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	pwm_num=0x00;
    turn_left_flag=0;
	
	sprintf(buf1,"��ת135��");
	
		
	
	//while(pwm_num<=97);//5v ��ѹ
	while(pwm_num<=119);//6v ��ѹ 
	
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
}

////////////////
//
/////////////////


////////////////
//
/////////////////

////////////////
//
/////////////////

////////////////
//
/////////////////

void turn_right_90(void)		   //��ת90��
{
	turn_num=turn_num+4;//�ҷ���ÿ��˳ʱ��תһȦ��6060����0.6�ȣ���ʱ��һȦ���ض�0.34�ȣ���������180��ʱƽ��
    
    sprintf(buf1,"��ת90��");
	
		
    
    
    turn_num=turn_num+4;
    turn_left_flag=0;
    
    led0pwmval=1540+2;//high mean low speed//2019.10.11�ҷ���С������ת90��ʱ�������������е���ת��
                      //�����е�ƫ��һ����ת���ʴˣ����ּ��٣����ּ��٣�����С�����������е���ת
	
		//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1540-2;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	pwm_num=0x00;
	
	
	
	//while(pwm_num<=97);//5v ��ѹ
	while(pwm_num<=78);//6v ��ѹ 
	
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
}



void turn_right_180(void)		   //��ת90��
{
	sprintf(buf1,"ת180��");
	
		
    
    
//      led0pwmval=1540;//high mean low speed
//	
//		//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
//	
//         TIM_SetCompare3(TIM3,led0pwmval);
//         
//        //tim3 2# is left servo
//         led0pwmval=1540;//high mean low speed
//         TIM_SetCompare4(TIM3,led0pwmval);	
//        
//        pwm_num=0x00;
//        
//        
//        
//        //while(pwm_num<=97);//5v ��ѹ
//        while(pwm_num<=213);//6v ��ѹ 
//	
//        car_stop();


    if(turn_num>=0)
    {
        
        turn_num=turn_num-4;
         turn_left_flag=1;
        
        led0pwmval=1460+2;//high mean low speed//2019.10.11�ҷ���С������ת90��ʱ�������������е���ת��
                      //�����е�ƫ��һ����ת���ʴˣ����ּ��٣����ּ��٣�����С�����������е���ת
         TIM_SetCompare3(TIM3,led0pwmval);
         
        //tim3 2# is left servo
         led0pwmval=1460-2;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
         pwm_num=0x00;
        
        //while(pwm_num<=95-5-3-2-0)//6v ��ѹ 
        while(pwm_num<=154)//6v ��ѹ 
        {
            
        }
        
        car_stop();
        
        delay_ms_xm1(100);
    }
    else
    {
         turn_num=turn_num+8; 
         turn_left_flag=0;
        
        led0pwmval=1540+2;//high mean low speed
	
		//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	
         TIM_SetCompare3(TIM3,led0pwmval);
         
        //tim3 2# is left servo
         led0pwmval=1540-2;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        pwm_num=0x00;
        
        
        
        //while(pwm_num<=97);//5v ��ѹ
        while(pwm_num<=159);//6v ��ѹ 
	
        car_stop();
    }	
}


void turn_right_90_qti2_3()			 //��ת90��
{
	//tim3 1# is right servo
	//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	led0pwmval=1540;//high mean low speed
	
		//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1540;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	pwm_num=0x00;
	
	sprintf(buf1,"��ת90��");
	
		
	
	//while(pwm_num<=97);//5v ��ѹ
	while(pwm_num<=109-28+10);//6v ��ѹ 
	
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
}



void turn_left_180(void)			 //��ת90��
{
	//tim3 1# is right servo
	//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	sprintf(buf1,"��ת90��");
	
	
	
//	turn_num=turn_num-4;
//    
//	led0pwmval=1460;//high mean low speed
//	 TIM_SetCompare3(TIM3,led0pwmval);
//	 
//	//tim3 2# is left servo
//	 led0pwmval=1460;//high mean low speed
//	 TIM_SetCompare4(TIM3,led0pwmval);	
//	
//	pwm_num=0x00;
//	
//		
//	
//	//while(pwm_num<=54);//5v ��ѹ
//	while(pwm_num<=170);//6v ��ѹ 
//	
//	 led0pwmval=1500;//high mean low speed
//	 TIM_SetCompare3(TIM3,led0pwmval);
//	 
//	 led0pwmval=1500;//high mean low speed
//	 TIM_SetCompare4(TIM3,led0pwmval);	

    if(turn_num>=0)
    {
        
        turn_num=turn_num-4;
        
         turn_left_flag=1;
        
        led0pwmval=1460+2;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
        //tim3 2# is left servo
         led0pwmval=1460-2;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
         pwm_num=0x00;
        
        //while(pwm_num<=95-5-3-2-0)//6v ��ѹ 
        while(pwm_num<=154)//6v ��ѹ 
        {
            
        }
        
        car_stop();
        
        delay_ms_xm1(100);
    }
    else
    {
         turn_num=turn_num+8; 
        
         turn_left_flag=0;
        
        led0pwmval=1540+2;//high mean low speed
	
		//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	
         TIM_SetCompare3(TIM3,led0pwmval);
         
        //tim3 2# is left servo
         led0pwmval=1540-2;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	
        
        pwm_num=0x00;
        
         while(pwm_num<=159)//6v ��ѹ 
        {
            
        }
        
        car_stop();



    }

	
}




void turn_left_90()			 //��ת90��
{
	//tim3 1# is right servo
	//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	sprintf(buf1,"��ת90��");
	
	
	
	turn_num=turn_num-2;
     turn_left_flag=1;
    
	led0pwmval=1460+2;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1460-2;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	pwm_num=0x00;
	
		
	
	//while(pwm_num<=54);//5v ��ѹ
	while(pwm_num<=77);//6v ��ѹ 
	
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
}





void turn_left_circle(void)
{
	led0pwmval=1460;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1460;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
}

void turn_left_circle_light(void)
{
	led0pwmval=1480;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1480;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
}

void turn_right_circle(void)
{
	led0pwmval=1550;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1550;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);
}
///////////////////////////////////////////
//
///////////////////////////////////////////


void turn_right_circle_light(void)
{
	led0pwmval=1520;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1520;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);
}
///////////////////////////////////////////
//
///////////////////////////////////////////


///////////////////////////////////////////
//
///////////////////////////////////////////




void turn_left_90_with_qti2_3_1(void)			 //��ת90��
{
	//tim3 1# is right servo
	//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	sprintf(buf1,"��ת90��");
	
	
	
	
	led0pwmval=1460;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1460;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	pwm_num=0x00;
	while(pwm_num<=95-5-3-2-0)//6v ��ѹ 
	{
		
	}
	
	car_stop();
	
	delay_ms(1000);
	delay_ms(1000);
	
		
	
	//while(pwm_num<=54);//5v ��ѹ
	qti_2_flag=0x00;
	qti_3_flag=0x00;
	while(pwm_num<=95-5-3-2-0)//6v ��ѹ 
	{
		if(qti_2==0x01)
		{
			qti_2_flag=0x01;
			sprintf(buf1,"2");
			
			
			printf("2\n");
		}
		
		if(qti_3==0x01)
		{
			qti_3_flag=0x01;
			qti_2_flag=0x00;// important must clear qti_2_flag when qti_3==0x01
			sprintf(buf1,"3");
			
			
			printf("3\n");
			
		}
	}
	
	car_stop();
	delay_ms(1000);
	delay_ms(1000);
	
	//�о��Ƿ�����Զ
	if((qti_2_flag==0x00))
	{
		
		sprintf(buf1,"����λ");
		
			 
		
		//��ת��qti2����ɫ����
		while(qti_2!=0x01)
		{
			turn_left_circle();
			
		}
		//ת��qti2����ɫ����
		while(qti_2==0x01)
		{
			turn_left_circle();
			
		}
		
		
		
	}
	
	//�ж��Ƿ�qti2���ں�ɫ������
	if((qti_2_flag==0x01)&&(qti_2==0x01))
	{
		sprintf(buf1,"����λ");
		
		
		//ת��qti2����ɫ����
		while(qti_2==0x01)
		{
			turn_left_circle();
			
		}

		
	}
	
	//�ж��Ƿ�qti3�Ƿ��ں�ɫ������
	if((qti_2_flag==0x01)&&(qti_3==0x01))
	{
		sprintf(buf1,"����λ");
		
		
		//ת��qti2����ɫ����
		while(qti_3==0x01)
		{
			turn_right_circle();
			
		}

		
	}
	
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
}
///////////////////////////////////////////
//
///////////////////////////////////////////


void turn_left_90_with_qti2_3_2(void)			 //��ת90��
{
	//tim3 1# is right servo
	//�����c6��c7tim3 pwm1 pwm2 �ջ��ˣ��ʴ˸�Ϊtim3 pwm3 pwm4 2018.10.22
	sprintf(buf1,"��ת90��");
	
	
	
	
	led0pwmval=1460;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1460;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	pwm_num=0x00;
	
		
	
	//while(pwm_num<=54);//5v ��ѹ
	
	qti_2_flag=0x00;
	qti_3_flag=0x00;
	while(pwm_num<=95-5-3-2-0)//6v ��ѹ 
	{
		if(qti_2==0x01)
		{
			qti_2_flag=0x01;
			sprintf(buf1,"2");
			
			
			printf("2\n");
		}
		
		if(qti_3==0x01)
		{
			qti_3_flag=0x01;
			qti_2_flag=0x01;
			sprintf(buf1,"3");
			
			
			printf("3\n");
		}
	}
	car_stop();
	delay_ms(1000);
	delay_ms(1000);
	
	//�о��Ƿ�����Զ
	if((qti_2_flag==0x00)&&(qti_3_flag==0x00))
	{
		
		sprintf(buf1,"����λ");
		
			 
		
		//��ת��qti2����ɫ����
		while(qti_2!=0x01)
		{
			turn_left_circle();
			
		}
		//ת��qti2����ɫ����
		while(qti_2==0x01)
		{
			turn_left_circle();
			
		}
		
		
		
	}
	
	//�ж��Ƿ�qti2���ں�ɫ������
	if((qti_2_flag==0x01)&&(qti_2==0x01))
	{
		sprintf(buf1,"����λ");
		
		
		//ת��qti2����ɫ����
		while(qti_2==0x01)
		{
			turn_left_circle();
			
		}
		car_stop();

		
	}
	
	//�ж��Ƿ�qti3�Ƿ��ں�ɫ������
	if((qti_2_flag==0x01)&&(qti_3==0x01))
	{
		sprintf(buf1,"����λ");
		
		
		//ת��qti2����ɫ����
		while(qti_3==0x01)
		{
			turn_right_circle();
			
		}
		car_stop();

		
	}
	
	
	
	
	
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
}

///////////////////////////////////////////
//
///////////////////////////////////////////































/*
 * ǰ���м�QTIѭ��
 */
bool MoveSlowQti(void)
{
	if(QTI_State(QTI2_Pin)==true && QTI_State(QTI3_Pin)==true)
	{
		PulseOut(leftservo,1545);
		PulseOut(rightservo,1465);
	}
	else if(QTI_State(QTI2_Pin)==true && QTI_State(QTI3_Pin)==false)
	{
		PulseOut(leftservo,1518);
		PulseOut(rightservo,1500);
	}
	else if(QTI_State(QTI2_Pin)==false&& QTI_State(QTI3_Pin)==true)
	{
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1484);
	}
	else if(QTI_State(QTI2_Pin)==false && QTI_State(QTI3_Pin)==false)
	{
		return true;
	}
	return false;
}

bool MoveSlowQti1(void)
{
	if(QTI_State(QTI2_Pin)==true && QTI_State(QTI3_Pin)==true)
	{
		PulseOut(leftservo,1540);
		PulseOut(rightservo,1460);
	}
	else if(QTI_State(QTI2_Pin)==true && QTI_State(QTI3_Pin)==false)
	{
		PulseOut(leftservo,1520);
		PulseOut(rightservo,1500);
	}
	else if(QTI_State(QTI2_Pin)==false && QTI_State(QTI3_Pin)==true)
	{
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1475);
	}
	else if(QTI_State(QTI2_Pin)==false && QTI_State(QTI3_Pin)==false)
	{
		return true;
	}
	return false;
}
/*
 * ǰ���м�QTIѭ��
 */
bool MoveQti(void)
{
	if(QTI_State(QTI2_Pin)==true && QTI_State(QTI3_Pin)==true)
	{
		PulseOut(leftservo,1560);
		PulseOut(rightservo,1440);
	}
	else if(QTI_State(QTI2_Pin)==true && QTI_State(QTI3_Pin)==false)
	{
		PulseOut(leftservo,1519);
		PulseOut(rightservo,1500);
	}
	else if(QTI_State(QTI2_Pin)==false && QTI_State(QTI3_Pin)==true)
	{
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1480);
	}
	else if(QTI_State(QTI2_Pin)==false && QTI_State(QTI3_Pin)==false)
	{
		return true;
	}
	return false;
}

bool MoveQti1(void)
{
	if(QTI_State(QTI2_Pin)==true && QTI_State(QTI3_Pin)==true)
	{
		PulseOut(leftservo,1535);
		PulseOut(rightservo,1465);
	}
	else if(QTI_State(QTI2_Pin)==true && QTI_State(QTI3_Pin)==false)
	{
		PulseOut(leftservo,1525);
		PulseOut(rightservo,1500);
	}
	else if(QTI_State(QTI2_Pin)==false && QTI_State(QTI3_Pin)==true)
	{
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1475);
	}
	else if(QTI_State(QTI2_Pin)==false&& QTI_State(QTI3_Pin)==false)
	{
		return true;
	}
	return false;
}


/*
 * ���Ӹ���QTIѭ��
 */


void Turn180(void)//��ͷ
{
	i=0;
	TurnRightAnyPulse(150);
	while(1)
	{
		SpinRight();
		if(IsMLeftQtiBlack())
		{
			delay_ms(2);
			if(IsMLeftQtiBlack())
			{
				break;
			}	
		}
	}
	stop();
}


void OCtoOA(void)  //�ж�ת��ʱ���Լ�ת���Ƿ�λ
{
	TurnLeftAnyPulse(105);
//	while(!IsELeftQtiBlack())
//	{
//		SpinLeft();
//	}
//	while(!(IsTailQtiBlack()&&IsMRightQtiBlack()))
//	{
//		SpinLeft();
//	}
//		stop();
}
void OCtoOE(void)
{
	TurnRightAnyPulse(90);
	
//	while(!IsERightQtiBlack())
//	{
//		SpinRight();
//	}
//	while(!(IsTailQtiBlack()&&IsMLeftQtiBlack()))
//	{
//		SpinRight();
//	}
//		stop();
}

void OCtoOD(void)
{
	TurnRightAnyPulse(22);
	while(1)
	{
		SpinRight();
		if(IsMRightQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	while(1)
	{
		SpinRight();
		if(IsMLeftQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	stop();
}
void ODtoOE(void)
{
	TurnRightAnyPulse(15);
	while(1)
	{
		SpinRight();
		if(IsMRightQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	while(1)
	{
		SpinRight();
		if(IsMLeftQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	stop();
}
void OCtoOB(void)
{
	TurnLeftAnyPulse(52);
//	while(1)
//	{
//		SpinLeft();
//		if(IsMLeftQtiBlack())
//		{
//			i++;
//			if(i==2)
//			{
//				i=0;
//				break;
//			}
//		}
//	}
//	while(1)
//	{
//		SpinLeft();
//		if(IsMRightQtiBlack())
//		{
//			i++;
//			if(i==2)
//			{
//				i=0;
//				break;
//			}
//		}
//	}
	stop();
}

void OCtoOB1(void)
{
	TurnLeftAnyPulse(45);
	while(1)
	{
		SpinLeft();
		if(IsMLeftQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	while(1)
	{
		SpinLeft();
		if(IsMRightQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	stop();
}
void OBtoOA(void)
{
	TurnLeftAnyPulse(52);
//	while(1)
//	{
//		SpinLeft();
//		if(IsMLeftQtiBlack())
//		{
//			i++;
//			if(i==2)
//			{
//				i=0;
//				break;
//			}
//		}
//	}
//	while(1)
//	{
//		SpinLeft();
//		if(IsMRightQtiBlack())
//		{
//			i++;
//			if(i==1)
//			{
//				i=0;
//				break;
//			}
//		}
//	}
	stop();
}
void OEtoOD(void)
{
	TurnLeftAnyPulse(49);
	while(1)
	{
		SpinLeft();
		if(IsMLeftQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	while(1)
	{
		SpinLeft();
		if(IsMRightQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	stop();
}

void OEtoOA(void)
{
	TurnLeftAnyPulse(186);
}

void ODtoOC(void)
{
	TurnLeftAnyPulse(49);
//	while(1)
//	{
//		SpinLeft();
//		if(IsMLeftQtiBlack())
//		{
//			i++;
//			if(i==2)
//			{
//				i=0;
//				break;
//			}
//		}
//	}
//	while(1)
//	{
//		SpinLeft();
//		if(IsMRightQtiBlack())
//		{
//			i++;
//			if(i==2)
//			{
//				i=0;
//				break;
//			}
//		}
//	}
	stop();
}

void FirstOtoMarkArea(void)//��һ�ε�Ŀ������ ��û�п���Ŀ������
{
	for(i=0;i<40;i++)
	{
		MoveMiddleQti();
	}
	stop();
	delay_ms(50);
	i=0;
	while(1)
	{
		MoveQti();
		delay_ms(20);
		if(!IsTailQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	stop();
	delay_ms(50);
	for(i=0;i<5;i++)
	{
		MoveQti();
		delay_ms(20);
	}
	for(i=0;i<1;i++)
	{
		MoveSlowQti();
		delay_ms(20);
	}
	
	stop();
}

void AOtoStartLine(void)
{
	while(1)
	{
		if(MoveMiddleQti())
		{
			fastforward();
		}
		if(IsERightQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	TurnRightAnyPulse(25);
	while(1)
	{
		SpinRight();
		if(IsMRightQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	while(1)
	{
		SpinRight();
		if(IsMLeftQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	stop();
}

void BOtoStartLine(void)
{

	
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	delay_ms(50);
	Crossblk(20);
	i=0;
	while(1)
	{
		fastforward();
		if(IsERightQtiBlack()||IsELeftQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	stop();
	Crossblk(60);
	delay_ms(20);
	TurnRightAnyPulse(8);
	stop();
	
}

void COtoStartLine(void)
{
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	Crossblk(15);
	while(!MoveEdgeQti())
	{
		MoveMiddleQti();
	}
	TurnLeftAnyPulse(10);
	Crossblk(40);
	stop();
}

void DOtoStartLine(void)
{

	
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	delay_ms(50);
	Crossblk(20);
	i=0;
	while(1)
	{
		fastforward();
		if(IsERightQtiBlack()||IsELeftQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	stop();
	Crossblk(20);
	delay_ms(20);
	TurnLeftAnyPulse(65);
	stop();
}
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //o �� ��ʱ��ŵ�a
/////////////////////////// ///////////////////////////        
void car_o_point_to_zero_2(void)
{
  //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///               ׼��ǰ��2������ֱ��ֱ��ͷ�ڣ�ԭ�ص�ͷ180
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
    walk_ae_state=0;
    while(1)
    {
        car_position_test_all_white();
        
        if(walk_ae_state==0)
        {
            car_judge_decision();
            car_excute_decision();
            
            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
            {
                walk_ae_state=1;
                
                sprintf(buf1,"[s10]ͷ��");
                
                
                 car_stop();
                break;
                
            }
            
            first_start_state_1=0x00;
            
        }
        
        
    }
    //ԭ�ص�ͷ180�ȣ�ָ��180�ȷ���  
    turn_left_180_with_6050_check(180.0);    
     mpu_6050_white();
     car_stop();
       //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ֱ��10���ף��ܿ�����
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
     pwm_num_distance=0;  
    
    while(1)
    {
        car_position_test_all_white();
        
        
        car_judge_decision();
        car_excute_decision();
        //ֱ��10���ף��ܿ�����
        if(pwm_num_distance>=11)
        {
            walk_ae_state=2;
            sprintf(buf1,"[s10]2������");
            
            
            
            break;
            
        }
       
    }
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///               ׼��ǰ��aeֱ�ߣ�ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            { 
              if(1==qti_5 && 1==qti_6 && 1==qti_7)
                {
                    walk_ae_state=1;
                    
                    sprintf(buf1,"[s10]���");
                    
                    
                     car_stop();
                    break;
                    
                }
                car_judge_decision();
                car_excute_decision();
                
               
                
                first_start_state_1=0x00;
                
            }
            
            
        } 
            
        car_stop();      
    
    
    
    
}
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //o �� ��ʱ��ŵ�a
/////////////////////////// ///////////////////////////        
void car_o_point_to_temp_a(void)
{
    if(0==qti_1 && 0==qti_2 && 0==qti_3)
    {
        mpu_6050_white();
    } 
    car_stop(); 
    
    //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///               ׼��ǰ��2������ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                {
                    walk_ae_state=1;
                    
                    sprintf(buf1,"[s10]ͷ��");
                    
                    
                     car_stop();
                    break;
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
        }
        mpu_6050_check(0);
       
       
        
         
        //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_tempa==0)
        {
            car_pull_down_bottle();
            count_stack_tempa=1;
            
        }
         else if(count_stack_tempa==1)
        {
            car_pull_down_middle();
            count_stack_tempa=2;
            
        }   
        
         else if(count_stack_tempa==2)
        {
            car_pull_down_top();
            count_stack_tempa=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��180�ȷ���  
        turn_left_180_with_6050_check(180.0);
        
        mpu_6050_white();
        car_stop();
        //
        //////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ֱ��10���ף��ܿ�����
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
         pwm_num_distance=0;  
        
        while(1)
        {
            car_position_test_all_white();
            
            
            car_judge_decision();
            car_excute_decision();
            //ֱ��10���ף��ܿ�����
            if(pwm_num_distance>=11)
            {
                walk_ae_state=2;
                sprintf(buf1,"[s10]2������");
                
                
                
                break;
                
            }
           
        }
        //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///               ׼��ǰ��aeֱ�ߣ�ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            { 
              if(1==qti_5 && 1==qti_6 && 1==qti_7)
                {
                    walk_ae_state=1;
                    
                    sprintf(buf1,"[s10]���");
                    
                    
                     car_stop();
                    break;
                    
                }
                car_judge_decision();
                car_excute_decision();
                
               
                
                first_start_state_1=0x00;
                
            }
            
            
        } 
            
        car_stop();  
}

//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              /o �� ��ʱ��ŵ�b
/////////////////////////// ///////////////////////////
void car_o_point_to_temp_b(void)
{
    if(0==qti_1 && 0==qti_2 && 0==qti_3)
    {
        mpu_6050_white();
    } 
    car_stop(); 
    
    
    //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///             ֱ��ֱ��ͷ�ڣ�
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                {
                    walk_ae_state=1;
                    
                    sprintf(buf1,"[s10]ͷ��");
                    
                    
                     car_stop();
                    break;
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
        }
        
        
        car_stop();     

        turn_right_90();
        mpu_6050_check(270);
            
        car_stop();

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_tempb==0)
        {
            car_pull_down_bottle();
            count_stack_tempb=1;
            
        }
         else if(count_stack_tempb==1)
        {
            car_pull_down_middle();
            count_stack_tempb=2;
            
        }   
        
         else if(count_stack_tempb==2)
        {
            car_pull_down_top();
            count_stack_tempb=3;
            
        } 
        car_stop();                    
            
            
        //    //ԭ����ת90�ȣ�ָ��180�ȷ��� 
       turn_right_90_with_6050_check(180.0);
        mpu_6050_white();
            //ԭ�ص�ͷ180�ȣ���ָ��180�ȷ���  
//        turn_left_180_with_6050_check(180.0);
        car_stop();
        //
        //////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ֱ��10���ף��ܿ�����
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
         pwm_num_distance=0;  
        
        while(1)
        {
            car_position_test_all_white();
            
            
            car_judge_decision();
            car_excute_decision();
            //ֱ��10���ף��ܿ�����
            if(pwm_num_distance>=11)
            {
                walk_ae_state=2;
                sprintf(buf1,"[s10]2������");
                
                
                
                break;
                
            }
           
        }
          car_stop();
 //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///               ׼��ǰ��aeֱ�ߣ�ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            { 
              if(1==qti_5 && 1==qti_6 && 1==qti_7)
                {
                    walk_ae_state=1;
                    
                    sprintf(buf1,"[s10]���");
                    
                    
                     car_stop();
                    break;
                    
                }
                car_judge_decision();
                car_excute_decision();
                
               
                
                first_start_state_1=0x00;
                
            }
            
            
        } 
            
        car_stop();   
    
}

//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //o �� ��ʱ��ŵ�c
/////////////////////////// ///////////////////////////
void car_o_point_to_temp_c(void)
{
    if(0==qti_1 && 0==qti_2 && 0==qti_3)
    {
        mpu_6050_white();
    } 
    car_stop(); 
    
    //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///            ֱ��ֱ��ͷ�ڣ�
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
      walk_ae_state=0;
      while(1)
      {
          car_position_test_all_white();
          
          if(walk_ae_state==0)
          {
              car_judge_decision();
              car_excute_decision();
              
              if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
              {
                  walk_ae_state=1;
                  
                  sprintf(buf1,"[s10]ͷ��");
                  
                  
                   car_stop();
                  break;
                  
              }
              
              first_start_state_1=0x00;
              
          }
          
          
      }
      
      
       car_stop();     

        turn_left_90();
        mpu_6050_check(90);
            
        car_stop();
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_tempc==0)
        {
            car_pull_down_bottle();
            count_stack_tempc=1;
            
        }
         else if(count_stack_tempc==1)
        {
            car_pull_down_middle();
            count_stack_tempc=2;
            
        }   
        
         else if(count_stack_tempc==2)
        {
            car_pull_down_top();
            count_stack_tempc=3;
            
        } 
        car_stop();                    
            
            
        //    //ԭ����ת90�ȣ�ָ��180�ȷ��� 
       turn_left_90_with_6050_check(180.0);
            //ԭ�ص�ͷ180�ȣ���ָ��180�ȷ���  
//        turn_left_180_with_6050_check(180.0);
        car_stop();
        //
        //////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ֱ��10���ף��ܿ�����
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
         pwm_num_distance=0;  
        
        while(1)
        {
            car_position_test_all_white();
            
            
            car_judge_decision();
            car_excute_decision();
            //ֱ��10���ף��ܿ�����
            if(pwm_num_distance>=11)
            {
                walk_ae_state=2;
                sprintf(buf1,"[s10]2������");
                
                
                
                break;
                
            }
           
        }
          car_stop();
 //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///               ׼��ǰ��aeֱ�ߣ�ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            { 
              if(1==qti_5 && 1==qti_6 && 1==qti_7)
                {
                    walk_ae_state=1;
                    
                    sprintf(buf1,"[s10]���");
                    
                    
                     car_stop();
                    break;
                    
                }
                car_judge_decision();
                car_excute_decision();
                
               
                
                first_start_state_1=0x00;
                
            }
            
            
        } 
            
        car_stop();   
}

//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //c �� ��ɫ���
/////////////////////////// ///////////////////////////

//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              /o �� ��ʱ��ŵ�b
/////////////////////////// ///////////////////////////
void car_o_point_to_temp_bo(void)
{
    if(0==qti_1 && 0==qti_2 && 0==qti_3)
    {
        mpu_6050_white();
    } 
    car_stop(); 
    
    
    //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///             ֱ��ֱ��ͷ�ڣ�
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                {
                    walk_ae_state=1;
                    
                    sprintf(buf1,"[s10]ͷ��");
                    
                    
                     car_stop();
                    break;
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
        }
        
        
          car_stop();       
            //ԭ�ص�ͷ180�ȣ���ָ��180�ȷ���  
        turn_left_180_with_6050_check(180.0);
        car_stop();
        //
        //////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ֱ��10���ף��ܿ�����
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
         pwm_num_distance=0;  
        
        while(1)
        {
            car_position_test_all_white();
            
            
            car_judge_decision();
            car_excute_decision();
            //ֱ��10���ף��ܿ�����
            if(pwm_num_distance>=11)
            {
                walk_ae_state=2;
                sprintf(buf1,"[s10]2������");
                
                
                
                break;
                
            }
           
        }
          car_stop();
       //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///               ׼��ǰ��aeֱ�ߣ�ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_5 && 1==qti_6 && 1==qti_7)
                {
                    walk_ae_state=1;
                    
                    sprintf(buf1,"[s10]���");
                    
                    
                     car_stop();
                    break;
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
        } 
            
        car_stop();   
        
            
        //    //ԭ����ת90�ȣ�ָ��279�ȷ��� 
            turn_left_135();
            mpu_6050_check(315);
            
              car_stop();

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_tempb==0)
        {
            car_pull_down_bottle();
            count_stack_tempb=1;
            
        }
         else if(count_stack_tempb==1)
        {
            car_pull_down_middle();
            count_stack_tempb=2;
            
        }   
        
         else if(count_stack_tempb==2)
        {
            car_pull_down_top();
            count_stack_tempb=3;
            
        } 
        car_stop();                    
            
            
        //    //ԭ����ת90�ȣ�ָ��180�ȷ��� 
            turn_right_90_with_6050_check(180.0);
            
              car_stop();  
}

//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //o �� ��ʱ��ŵ�c
/////////////////////////// ///////////////////////////
void car_o_point_to_temp_co(void)
{
    if(0==qti_1 && 0==qti_2 && 0==qti_3)
    {
        mpu_6050_white();
    } 
    car_stop(); 
    
    //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///            ֱ��ֱ��ͷ�ڣ�
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
      walk_ae_state=0;
      while(1)
      {
          car_position_test_all_white();
          
          if(walk_ae_state==0)
          {
              car_judge_decision();
              car_excute_decision();
              
              if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
              {
                  walk_ae_state=1;
                  
                  sprintf(buf1,"[s10]ͷ��");
                  
                  
                   car_stop();
                  break;
                  
              }
              
              first_start_state_1=0x00;
              
          }
          
          
      }
      
      
        car_stop();       
          //ԭ�ص�ͷ180�ȣ���ָ��180�ȷ���  
      turn_left_180_with_6050_check(180.0);
      car_stop();
      //
      //////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ֱ��10���ף��ܿ�����
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
       pwm_num_distance=0;  
      
      while(1)
      {
          car_position_test_all_white();
          
          
          car_judge_decision();
          car_excute_decision();
          //ֱ��10���ף��ܿ�����
          if(pwm_num_distance>=11)
          {
              walk_ae_state=2;
              sprintf(buf1,"[s10]2������");
              
              
              
              break;
              
          }
         
      }
        car_stop();
     //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///               ׼��ǰ��aeֱ�ߣ�ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
      walk_ae_state=0;
      while(1)
      {
          car_position_test_all_white();
          
          if(walk_ae_state==0)
          {
              car_judge_decision();
              car_excute_decision();
              
              if(1==qti_5 && 1==qti_6 && 1==qti_7)
              {
                  walk_ae_state=1;
                  
                  sprintf(buf1,"[s10]���");
                  
                  
                   car_stop();
                  break;
                  
              }
              
              first_start_state_1=0x00;
              
          }
          
          
      } 
          
      car_stop();   
      
   //    //ԭ����ת90�ȣ�ָ��180�ȷ��� 
      turn_right_135();//(90.0);
//        turn_left_135();
      mpu_6050_check(45);
            car_stop();

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
      if(count_stack_tempc==0)
      {
          car_pull_down_bottle();
          count_stack_tempc=1;
          
      }
       else if(count_stack_tempc==1)
      {
          car_pull_down_middle();
          count_stack_tempc=2;
          
      }   
      
       else if(count_stack_tempc==2)
      {
          car_pull_down_top();
          count_stack_tempc=3;
          
      } 
      car_stop();                    
          
          
    //    //ԭ����ת90�ȣ�ָ��279�ȷ��� 
          turn_left_90_with_6050_check(180.0);
          
            car_stop();  
}

//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //c �� ��ɫ���
//
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //c �� ��ɫ���
/////////////////////////// ///////////////////////////



//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //c �� ��ɫ���
/////////////////////////// ///////////////////////////



//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //c �� ��ɫ���
/////////////////////////// ///////////////////////////



//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //c �� ��ɫ���
/////////////////////////// ///////////////////////////





//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //c �� ��ɫ���
/////////////////////////// ///////////////////////////










//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //c �� ��ɫ���
/////////////////////////// ///////////////////////////







//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //c �� ��ɫ���
/////////////////////////// ///////////////////////////
void car_c_ponit_yellow(void)
{
     //��ʱ��С��ͷ��ɫ�飬��ͷ180��
    if(count_point_a<=1)
    {
//                    car_yellow_pull_down_block();
        turn_left_90_with_axis_recover_6050_check(270.0);
        //ֱ�ӷŻ�ɫ��飬ǰ����βȫ��
         while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                
                
                sprintf(buf1,"[s10]β��");
                
                
                car_stop();
                break;
                
            }
        }
         car_stop();    
            
          mpu_6050_check(270);  
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_yellow==0)
        {
            car_pull_down_bottle();
            count_stack_yellow=1;
            
        }
         else if(count_stack_yellow==1)
        {
            car_pull_down_middle();
            count_stack_yellow=2;
            
        }   
        
         else if(count_stack_yellow==2)
        {
            car_pull_down_top();
            count_stack_yellow=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        turn_left_180_with_6050_check(90.0);
        car_stop();
        //
                               //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]ͷ��");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                if(first_start_state_1==0)
                {
                    first_start_state_1=1;

                    pwm_num_distance=0;
            
                }
                car_judge_decision();
                car_excute_decision();
                //ֱ��100���ף������غ�
                if(pwm_num_distance>=55)
                {
                    walk_ae_state=2;
                    sprintf(buf1,"[s10]55��");
                    
                    
                    car_stop();
                    break;
                    
                }
                
            }
            
           
        }
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_right_90();//        turn_right_90_with_6050_check(0.0);
        mpu_6050_check(0);
         if(0==qti_1 && 0==qti_2 && 0==qti_3)
        {
            mpu_6050_white();
        } 
        car_stop();   
        
        car_o_point_to_zero_2();

        //��2������׼��������һ����飬  �������������1  
        grasp_order_index++;
        count_point_c--;
        
        
        
        
    }
    
////////////////////////////////
/////////////////////////////////
//////////////////////////////////////                
//���������ϰ�������    ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////                  
    
    
    else
    {
        //car_yellow_not_pull_down_block();


/////////////////////////////////
//////////////////////////////////////                
//a��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////     
        
        if(colour_tempa==other||colour_tempa==block_colour_order[grasp_order_index])
        {
            colour_tempa=yellow;
            count_stack_tempa=0x00;
            
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� 
            turn_left_180();
            mpu_6050_check(0);
            car_stop();   
            car_o_point_to_temp_a();
           //��2������׼��������һ����飬  �������������1  
            grasp_order_index++;
            count_point_c--;
            

            
        }
        
 //////////////////////////
//////////////////////////////////////                
//b��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////     
        else if(colour_tempb==other||colour_tempb==block_colour_order[grasp_order_index])
        {
            colour_tempb=yellow;
            count_stack_tempb=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
            turn_left_180();//turn_left_180_with_6050_check(0);
            mpu_6050_check(0);
            
            car_stop();   
          
            car_o_point_to_temp_b();
          
     
          grasp_order_index++;
          count_point_c--;

            
            
        }
        
        
 
       
          /////////////////////////////////
//////////////////////////////////////                
//c��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////   
        else
        {
            
            
            colour_tempc=yellow;
            count_stack_tempc=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
            turn_left_180();//            turn_left_180_with_6050_check(0);
            mpu_6050_check(0);
            car_stop();   
          
            car_o_point_to_temp_c();
          
        
            grasp_order_index++;
            count_point_c--;
            

            
        }
     
    }

    
    
    
}

/////////////////////////////////////////////
//
///////////////////////////////////////////////
void car_c_point_white(void)
{ //��ʱ��С��ͷ��ɫ�飬��ͷ180��
    
    //��ת45��
    turn_left_45();
    
    
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��b�㣬ж����β�״�����ָ���㣬ж��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
     walk_ae_state=0;
    while(1)
    {
        car_position_test_all_white();
        
        if(walk_ae_state==0)
        {
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_1 && 0==qti_2 && 0==qti_3 && 0==qti_4)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]ͷ��");
                
                
                
            }
            
            first_start_state_1=0x00;
            
        }
        
        
        else if(walk_ae_state==1)
        {
            
            car_judge_decision();
            car_excute_decision();
            //ֱ��100���ף������غ�
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]β��");
                
                
                
                break;
                
            }
            
        }

    }

    
    //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_white==0)
        {
            car_pull_down_bottle();
            count_stack_white=1;
            
        }
         else if(count_stack_white==1)
        {
            car_pull_down_middle();
            count_stack_white=2;
            
        }   
        
         else if(count_stack_white==2)
        {
            car_pull_down_top();
            count_stack_white=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        turn_left_180_with_6050_check(45.0);
        car_stop();
        //
                               //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת45��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_5 && 1==qti_6 && 1==qti_7)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]���");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                if(first_start_state_1==0)
                {
                    first_start_state_1=1;

                    pwm_num_distance=0;
            
                }
                car_judge_decision();
                car_excute_decision();
                //ֱ��100���ף������غ�
                if(pwm_num_distance>=20)
                {
                    walk_ae_state=2;
                    sprintf(buf1,"[s10]20��");
                    
                    
                    car_stop();
                    break;
                    
                }
                
            }
            

            
        }
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_right_45();
        mpu_6050_check(0);
         if(0==qti_1 && 0==qti_2 && 0==qti_3)
        {
            mpu_6050_white();
        } 
        
        car_stop();  
          
         car_o_point_to_zero_2();

        //��2������׼��������һ����飬  �������������1  
        grasp_order_index++;
        count_point_c--;

    
}
///////////////////////////////////////////
//
////////////////////////////////////////////
void car_c_point_red(void)
{ //��ʱ��С��ͷ��ɫ�飬��ͷ180��
    
    if(count_point_c<=1)
    {
//                    car_yellow_pull_down_block();
////                    turn_right_90_with_6050_check(180.0);
        //ֱ�ӷź�ɫ��飬ǰ����βȫ��
         while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                
                
                sprintf(buf1,"[s10]β��");
                
                
                car_stop();
                break;
                
            }
        }
         car_stop();    
            
          mpu_6050_check(180);  
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_red==0)
        {
            car_pull_down_bottle();
            count_stack_red=1;
            
        }
         else if(count_stack_red==1)
        {
            car_pull_down_middle();
            count_stack_red=2;
            
        }   
        
         else if(count_stack_red==2)
        {
            car_pull_down_top();
            count_stack_red=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
            turn_left_180();//        turn_left_180_with_6050_check(0.0);
            mpu_6050_check(0);
            car_stop();
        //
                               //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_8&& 1==qti_9 && 1==qti_10 && 1==qti_11)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]β��");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                
                car_judge_decision();
                car_excute_decision();
             
                if(1==qti_1&& 1==qti_2 && 1==qti_3 && 1==qti_4)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]ͷ��");
                    
                    
                    break;
                    
                }
                
            }
            

            
        }
       
        //ԭ�ص�ͷ180�ȣ�ָ��180�ȷ���  
        turn_left_180_with_6050_check(180.0);    
        
         car_stop();
           //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ֱ��10���ף��ܿ�����
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
         pwm_num_distance=0;  
        
        while(1)
        {
            car_position_test_all_white();
            
            
            car_judge_decision();
            car_excute_decision();
            //ֱ��10���ף��ܿ�����
            if(pwm_num_distance>=11)
            {
                walk_ae_state=2;
                sprintf(buf1,"[s10]2������");
                
                
                
                break;
                
            }
           
        }
        //��2������׼��������һ����飬  �������������1  
        grasp_order_index++;
        count_point_c--;
        
        
        
    }
    

         

    else
    {
         if(colour_tempa==other||colour_tempa==block_colour_order[grasp_order_index])
        {
            colour_tempa=red;
            count_stack_tempa=0x00;          
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� 
            turn_left_180();//            turn_left_180_with_6050_check(0.0);
            mpu_6050_check(0);
            car_stop();  
            car_o_point_to_temp_a();

            //��2������׼��������һ����飬  �������������1  
            grasp_order_index++;
            count_point_c--;
   
 }
        


 /////////////////////////////////
//////////////////////////////////////                
//b��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////     
        else if(colour_tempb==other||colour_tempb==block_colour_order[grasp_order_index])
        {
            colour_tempb=red;
            count_stack_tempb=0x00;
            
                         //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
            turn_left_180();//            turn_left_180_with_6050_check(0);
            mpu_6050_check(0);
            car_stop();  
            car_o_point_to_temp_b();
          
      
            grasp_order_index++;
            
            count_point_c--;;
            
            
        }

/////////////////////////////////
//////////////////////////////////////                
//c��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////   
        else
        {
            
            
            colour_tempc=red;
            count_stack_tempc=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
            turn_left_180();//            turn_left_180_with_6050_check(0);
            mpu_6050_check(0);
            car_stop();   
            car_o_point_to_temp_c();

        
            grasp_order_index++;
            count_point_c--;

            
        }

    }
  
}
/////////////////////////////////////////////
//
//////////////////////////////////////////////
void car_c_point_black(void)
{
      //��ʱ��С��ͷ��ɫ�飬��ͷ180��           
                //��ת135��
    turn_right_45();
   
    mpu_6050_check(135);
    car_axis_recover_d_point();
    
    
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��d�㣬ж����β�״�����ָ���㣬ж��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
     walk_ae_state=0;
    while(1)
    {
        car_position_test_all_white();
        
        if(walk_ae_state==0)
        {
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_1 && 0==qti_2 && 0==qti_3 && 0==qti_4)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]ͷ��");
                
                
                
            }
            
            first_start_state_1=0x00;
            
        }
        
        
        else if(walk_ae_state==1)
        {
            
            car_judge_decision();
            car_excute_decision();
            //ֱ��100���ף������غ�
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]β��");
                
                
                
                break;
                
            }
            
        }

    }

    
    //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_black==0)
        {
            car_pull_down_bottle();
            count_stack_black=1;
            
        }
         else if(count_stack_black==1)
        {
            car_pull_down_middle();
            count_stack_black=2;
            
        }   
        
         else if(count_stack_black==2)
        {
            car_pull_down_top();
            count_stack_black=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        turn_left_180_with_6050_check(315.0);
        car_stop();
        //
                               //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת45��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_5 && 1==qti_6 )
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]���");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                if(first_start_state_1==0)
                {
                    first_start_state_1=1;

                    pwm_num_distance=0;
            
                }
                car_judge_decision();
                car_excute_decision();
                //ֱ��100���ף������غ�
                if(pwm_num_distance>=14)
                {
                    walk_ae_state=2;
                    sprintf(buf1,"[s10]14��");
                    
                    
                    car_stop();
                    break;
                    
                }
                
            }
            

            
        }
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_left_45();
        mpu_6050_check(0);
        if(0==qti_1 && 0==qti_2 && 0==qti_3)
        {
            mpu_6050_white();
        } 
        car_stop();  
          
        car_o_point_to_zero_2();

        //��2������׼��������һ����飬  �������������1  
        grasp_order_index++;
        count_point_c--;
    
    

}
/////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////
void car_c_point_blue(void)
{
    //��ʱ��С��ͷ��ɫ�飬��ͷ180��
               
    if(count_point_e<=1)
    {
//                    car_yellow_pull_down_block();
        turn_right_90_with_6050_check(90.0);
//                    turn_right_90_with_6050_check(90.0);
        //ֱ�ӷ���ɫ��飬ǰ����βȫ��
         while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                
                
                sprintf(buf1,"[s10]β��");
                
                
                car_stop();
                break;
                
            }
        }
         car_stop();    
            
          mpu_6050_check(90);  
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_blue==0)
        {
            car_pull_down_bottle();
            count_stack_blue=1;
            
        }
         else if(count_stack_blue==1)
        {
            car_pull_down_middle();
            count_stack_blue=2;
            
        }   
        
         else if(count_stack_blue==2)
        {
            car_pull_down_top();
            count_stack_blue=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        //turn_right_180_with_6050_check(270.0);
        turn_right_180();
        mpu_6050_check(270.0);
        
        car_stop();
        //
        


        //////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]ͷ��");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                if(first_start_state_1==0)
                {
                    first_start_state_1=1;

                    pwm_num_distance=0;
            
                }
                car_judge_decision();
                car_excute_decision();
                //ֱ��100���ף������غ�
                if(pwm_num_distance>=49)
                {
                    walk_ae_state=2;
                    sprintf(buf1,"[s10]49��");
                    
                    
                    car_stop();
                    break;
                    
                }
                
            }
            
     
        }
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_left_90();//turn_left_90_with_6050_check(0.0);
        mpu_6050_check(0);
        
         if(0==qti_1 && 0==qti_2 && 0==qti_3)
        {
            mpu_6050_white();
        } 
        car_stop();  
        
        car_o_point_to_zero_2();

        //��2������׼��������һ����飬  �������������1  
        grasp_order_index++;
        count_point_c--;


//////////////////////////////////////////////////////

    }
    

         

    else
    {
         if(colour_tempa==other||colour_tempa==block_colour_order[grasp_order_index])
        {
            colour_tempa=blue;
            count_stack_tempa=0x00;
            
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� 
            turn_left_180();//            turn_left_180_with_6050_check(0.0);
            mpu_6050_check(0);
            car_stop(); 
            car_o_point_to_temp_a();

        //��2������׼��������һ����飬  �������������1  
            grasp_order_index++;
            count_point_c--;

 }
        


 /////////////////////////////////
//////////////////////////////////////                
//b��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////     
        else if(colour_tempb==other||colour_tempb==block_colour_order[grasp_order_index])
        {
            colour_tempb=blue;
            count_stack_tempb=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
            turn_left_90();//            turn_left_90_with_6050_check(0);
            mpu_6050_check(0);   
            car_stop(); 
            car_o_point_to_temp_b();

        
            grasp_order_index++;
            
            count_point_c--;
            
            
        }
/////////////////////////////////
//////////////////////////////////////                
//c��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////   
        else
        {
            
            
            colour_tempc=blue;
            count_stack_tempc=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
        turn_left_180();//            turn_left_90_with_6050_check(0);
        mpu_6050_check(0);    
        car_stop();  
        car_o_point_to_temp_c();          

        grasp_order_index++;
        count_point_c--;  

            
        }
     

    
    

    }
  

    


}
///////////////////////////////////////////////
//
//////////////////////////////////////////////
void car_a_point_yellow(void)
{
    //��ʱ��С��ͷ��ɫ�飬��ͷ270��
    if(count_point_a<=1)
    {
//                    car_yellow_pull_down_block();
        
        //ֱ�ӷŻ�ɫ��飬ǰ����βȫ��
         while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                
                
                sprintf(buf1,"[s10]β��");
                
                
                car_stop();
                break;
                
            }
        }
         car_stop();    
            
          mpu_6050_check(270); 
          mpu_6050_white();
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_yellow==0)
        {
            car_pull_down_bottle();
            count_stack_yellow=1;
            
        }
         else if(count_stack_yellow==1)
        {
            car_pull_down_middle();
            count_stack_yellow=2;
            
        }   
        
         else if(count_stack_yellow==2)
        {
            car_pull_down_top();
            count_stack_yellow=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        turn_left_180_with_6050_check(90.0);
        car_stop();
        //
                               //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]ͷ��");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                if(first_start_state_1==0)
                {
                    first_start_state_1=1;

                    pwm_num_distance=0;
            
                }
                car_judge_decision();
                car_excute_decision();
                //ֱ��100���ף������غ�
                if(pwm_num_distance>=55)
                {
                    walk_ae_state=2;
                    sprintf(buf1,"[s10]55��");
                    
                    
                    car_stop();
                    break;
                    
                }
                
            }
            
        }
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_right_90();//        turn_right_90_with_6050_check(0.0);
        mpu_6050_check(0);
        if(0==qti_1 && 0==qti_2 && 0==qti_3)
        {
            mpu_6050_white();
        } 
        car_stop();  
        
        car_o_point_to_zero_2();
       

        grasp_order_index++;
        count_point_a--;
        
        
        
        
    }
    
////////////////////////////////
/////////////////////////////////
//////////////////////////////////////                
//���������ϰ�������    ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////                  
    
    
    else
    {
/////////////////////////////////
//////////////////////////////////////                
//a��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////     
        
        if(colour_tempa==other||colour_tempa==block_colour_order[grasp_order_index])
        {
            colour_tempa=yellow;
            count_stack_tempa=0x00;
            
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� 
        turn_left_90();//            turn_left_90_with_6050_check(0.0);
            
        mpu_6050_check(0); 
        mpu_6050_white();
            
        car_stop();  
            
        car_o_point_to_temp_a();

        //��2������׼��������һ����飬  �������������1  
        grasp_order_index++;
        count_point_a--;

            
 }


 /////////////////////////////////
//////////////////////////////////////                
//b��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////     
        else if(colour_tempb==other||colour_tempb==block_colour_order[grasp_order_index])
        {
            colour_tempb=yellow;
            count_stack_tempb=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
        turn_left_90();//            turn_left_90_with_6050_check(0);
        mpu_6050_check(0); 
        mpu_6050_white();
          
        car_stop();   
        car_o_point_to_temp_b();
        
          grasp_order_index++;
          count_point_a--;

            
            
        }
        

        
          /////////////////////////////////
//////////////////////////////////////                
//c��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////   
        else
        {
            
            
            colour_tempc=yellow;
            count_stack_tempc=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
        turn_left_90();//            turn_left_90_with_6050_check(0);
        mpu_6050_check(0);   
        mpu_6050_white();
          
        car_stop();   
            
        car_o_point_to_temp_c();

        
         grasp_order_index++;
         count_point_a--;
            

            
        }
     
    }
} 

//////////////////////////////////////////////
//
/////////////////////////////////////////////
void car_a_point_white(void)
{
//��ʱ��С��ͷ��ɫ�飬��ͷ270��               
    //��ת45��
    turn_right_45();
     


    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
    ///              ׼��ǰ��b�㣬ж����β�״�����ָ���㣬ж��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    ////////////////////////////////////////////////////// 
     walk_ae_state=0;
    while(1)
    {
        car_position_test_all_white();
        
        if(walk_ae_state==0)
        {
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_1 && 0==qti_2 && 0==qti_3 && 0==qti_4)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]ͷ��");
                
                
                
            }
            
            first_start_state_1=0x00;
            
        }
        
        
        else if(walk_ae_state==1)
        {
            
            car_judge_decision();
            car_excute_decision();
            //ֱ��100���ף������غ�
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]β��");
                
                
                
                break;
                
            }
            
        }

    }


    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
    ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
        if(count_stack_white==0)
        {
            car_pull_down_bottle();
            count_stack_white=1;
            
        }
         else if(count_stack_white==1)
        {
            car_pull_down_middle();
            count_stack_white=2;
            
        }   
        
         else if(count_stack_white==2)
        {
            car_pull_down_top();
            count_stack_white=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        turn_left_180_with_6050_check(45.0);
         
        car_stop();
        //
                               //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
    ///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת45��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    ////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_5 && 1==qti_6 && 1==qti_7)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]���");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                if(first_start_state_1==0)
                {
                    first_start_state_1=1;

                    pwm_num_distance=0;
            
                }
                car_judge_decision();
                car_excute_decision();
                //ֱ��100���ף������غ�
                if(pwm_num_distance>=20)
                {
                    walk_ae_state=2;
                    sprintf(buf1,"[s10]20��");
                    
                    
                    car_stop();
                    break;
                    
                }
                
            }
            

            
        }
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_right_45();
        mpu_6050_check(0);
        if(0==qti_1 && 0==qti_2 && 0==qti_3)
        {
            mpu_6050_white();
        } 
        car_stop();   
        
        car_o_point_to_zero_2();
    
        grasp_order_index++;
        count_point_a--;

        
        
        
        
        
        
        
        
        
        
    }
            


//////////////////////////////////////////////
//
/////////////////////////////////////////////
void car_a_point_red(void)
{
//��ʱ��С��ͷ��ɫ�飬��ͷ270��
    if(count_point_c<=1)
    {
    //                    car_yellow_pull_down_block();
        turn_right_90_with_6050_check(180.0);
        //ֱ�ӷź�ɫ��飬ǰ����βȫ��
         while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                
                
                sprintf(buf1,"[s10]β��");
                
                
                car_stop();
                break;
                
            }
        }
         car_stop();    
            
          mpu_6050_check(180);  
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
    ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
        if(count_stack_red==0)
        {
            car_pull_down_bottle();
            count_stack_red=1;
            
        }
         else if(count_stack_red==1)
        {
            car_pull_down_middle();
            count_stack_red=2;
            
        }   
        
         else if(count_stack_red==2)
        {
            car_pull_down_top();
            count_stack_red=3;
            
        } 
        car_stop();                                
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
            turn_left_180();//        turn_left_180_with_6050_check(0.0);
        mpu_6050_check(0);
        
        car_stop();
        //
                               //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
    ///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    ////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_8&& 1==qti_9 && 1==qti_10 && 1==qti_11)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]β��");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                
                car_judge_decision();
                car_excute_decision();
             
                if(1==qti_1&& 1==qti_2 && 1==qti_3 && 1==qti_4)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]ͷ��");
                    
                    
                    break;
                    
                }
                
            }
            

            
        }
       
        //ԭ�ص�ͷ180�ȣ�ָ��180�ȷ���  
        turn_left_180_with_6050_check(180.0);    
         mpu_6050_white();
         car_stop();
        
        
           //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
    ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
         pwm_num_distance=0;  
        
        while(1)
        {
            car_position_test_all_white();
            
            
            car_judge_decision();
            car_excute_decision();
            //ֱ��10���ף��ܿ�����
            if(pwm_num_distance>=11)
            {
                walk_ae_state=2;
                sprintf(buf1,"[s10]2������");
                
                
                
                break;
                
            }
           
        }
        //��2������׼��������һ����飬  �������������1  
        grasp_order_index++;
        count_point_a--;
        
        
        
    }


         

    else
    {
         if(colour_tempa==other||colour_tempa==block_colour_order[grasp_order_index])
        {
            colour_tempa=red;
            count_stack_tempa=0x00;
            
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� 
        turn_left_90();//            turn_left_90_with_6050_check(0.0);
        mpu_6050_check(0);
       
          
        car_stop();   
          
        car_o_point_to_temp_a();
          
        grasp_order_index++;
        count_point_a--;
            

            
    }
        


    /////////////////////////////////
    //////////////////////////////////////                
    //b��ʱ��ŵ���Դ�� ////////////////////////////////
    /////////////////////////////////
    //////////////////////////////////////     
        else if(colour_tempb==other||colour_tempb==block_colour_order[grasp_order_index])
        {
            colour_tempb=red;
            count_stack_tempb=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
        turn_left_90();//            turn_left_90_with_6050_check(0);
        mpu_6050_check(0);    
        car_stop();   
          
        car_o_point_to_temp_b();  
    
        
          grasp_order_index++;
            
           count_point_a--;;
            
            
        }
        

          /////////////////////////////////
    //////////////////////////////////////                
    //c��ʱ��ŵ���Դ�� ////////////////////////////////
    /////////////////////////////////
    //////////////////////////////////////   
        else
        {
            
            
            colour_tempc=red;
            count_stack_tempc=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
        turn_left_90();//            turn_left_90_with_6050_check(0);
        mpu_6050_check(0);    
        car_stop();   
          
        car_o_point_to_temp_c();
        
        grasp_order_index++;
        count_point_a--;

            
        }
     




    }

}

            

//////////////////////////////////////////////
//
/////////////////////////////////////////////
void car_a_point_black(void)
{
    //��ʱ��С��ͷ��ɫ�飬��ͷ270��
    //��ת135��
    turn_right_135();
   
    mpu_6050_check(135);
//    car_axis_recover_d_point();
    
    
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��d�㣬ж����β�״�����ָ���㣬ж��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
     walk_ae_state=0;
    while(1)
    {
        car_position_test_all_white();
        
        if(walk_ae_state==0)
        {
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_1 && 0==qti_2 && 0==qti_3 && 0==qti_4)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]ͷ��");
                
                
                
            }
            
            first_start_state_1=0x00;
            
        }
        
        
        else if(walk_ae_state==1)
        {
            
            car_judge_decision();
            car_excute_decision();
            //ֱ��100���ף������غ�
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]β��");
                
                
                
                break;
                
            }
            
        }

    }

    
    //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_black==0)
        {
            car_pull_down_bottle();
            count_stack_black=1;
            
        }
         else if(count_stack_black==1)
        {
            car_pull_down_middle();
            count_stack_black=2;
            
        }   
        
         else if(count_stack_black==2)
        {
            car_pull_down_top();
            count_stack_black=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        turn_left_180_with_6050_check(315.0);
        car_stop();
        //
                               //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת45��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_5 && 1==qti_6 && 1==qti_7)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]���");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                if(first_start_state_1==0)
                {
                    first_start_state_1=1;

                    pwm_num_distance=0;
            
                }
                car_judge_decision();
                car_excute_decision();
                //ֱ��100���ף������غ�
                if(pwm_num_distance>=14)
                {
                    walk_ae_state=2;
                    sprintf(buf1,"[s10]14��");
                    
                    
                    car_stop();
                    break;
                    
                }
                
            }
            

            
        }
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_left_45();
        mpu_6050_check(0);
         if(0==qti_1 && 0==qti_2 && 0==qti_3)
        {
            mpu_6050_white();
        } 
        car_stop();   
        
        car_o_point_to_zero_2();

        //��2������׼��������һ����飬  �������������1  
        grasp_order_index++;
        count_point_a--;
    
    

}
            


 //////////////////////////////////////////////
//
/////////////////////////////////////////////
void car_a_point_blue(void)
{
   //��ʱ��С��ͷ��ɫ�飬��ͷ270��
    if(count_point_e<=1)
    {
//                    car_yellow_pull_down_block();
        turn_left_180_with_6050_check(90.0);
       
        //ֱ�ӷ���ɫ��飬ǰ����βȫ��
         while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                
                
                sprintf(buf1,"[s10]β��");
                
                
                car_stop();
                break;
                
            }
        }
         car_stop();    
            
          mpu_6050_check(90);  
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_blue==0)
        {
            car_pull_down_bottle();
            count_stack_blue=1;
            
        }
         else if(count_stack_blue==1)
        {
            car_pull_down_middle();
            count_stack_blue=2;
            
        }   
        
         else if(count_stack_blue==2)
        {
            car_pull_down_top();
            count_stack_blue=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        turn_left_180_with_6050_check(270.0);
        car_stop();
        //
        


        //////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]ͷ��");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                if(first_start_state_1==0)
                {
                    first_start_state_1=1;

                    pwm_num_distance=0;
            
                }
                car_judge_decision();
                car_excute_decision();
                //ֱ��100���ף������غ�
                if(pwm_num_distance>=49)
                {
                    walk_ae_state=2;
                    sprintf(buf1,"[s10]49��");
                    
                    
                    car_stop();
                    break;
                    
                }
                
            }
            
            
        }
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_left_90();//        turn_left_90_with_6050_check(0.0);
        mpu_6050_check(0);
         if(0==qti_1 && 0==qti_2 && 0==qti_3)
        {
            mpu_6050_white();
        } 
        car_stop();  
          
        car_o_point_to_zero_2();

        grasp_order_index++;
        count_point_a--;


//////////////////////////////////////////////////////

    }
    

         

    else
    {
         if(colour_tempa==other||colour_tempa==block_colour_order[grasp_order_index])
        {
            colour_tempa=blue;
            count_stack_tempa=0x00;
            
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� 
        turn_left_90();//            turn_left_90_with_6050_check(0.0);
        mpu_6050_check(0);    
        car_stop();   
          
        car_o_point_to_temp_a();

        //��2������׼��������һ����飬  �������������1  
        grasp_order_index++;
        count_point_a--;
            

            
 }
        


 /////////////////////////////////
//////////////////////////////////////                
//b��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////     
        else if(colour_tempb==other||colour_tempb==block_colour_order[grasp_order_index])
        {
            colour_tempb=blue;
            count_stack_tempb=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
        turn_left_90();//            turn_left_90_with_6050_check(0);
        mpu_6050_check(0);    
        car_stop();   
          
        car_o_point_to_temp_b();

          grasp_order_index++;
            
            count_point_a--;
            
            
        }
        
     /////////////////////////////////
//////////////////////////////////////                
//c��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////   
        else
        {
            
            
            colour_tempc=blue;
            count_stack_tempc=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
        turn_left_90();//            turn_left_90_with_6050_check(0);
        mpu_6050_check(0);    
        car_stop();   
          
        car_o_point_to_temp_c();
          
        grasp_order_index++;
        count_point_a--;  

            
        }
     

    
    

    }
  


}
                
        


//////////////////////////////////////////////
//
/////////////////////////////////////////////
void car_e_point_yellow(void)
{
     //��ʱ��С��ͷ��ɫ�飬��ͷ90��  
    if(count_point_a<=1)
    {
//                    car_yellow_pull_down_block();
        
        //ֱ�ӷŻ�ɫ��飬��ͷ��ǰ����βȫ��
        turn_left_180_with_6050_check(270);
         while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                
                
                sprintf(buf1,"[s10]β��");
                
                
                car_stop();
                break;
                
            }
        }
         car_stop();    
            
          mpu_6050_check(270);  
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_yellow==0)
        {
            car_pull_down_bottle();
            count_stack_yellow=1;
            
        }
         else if(count_stack_yellow==1)
        {
            car_pull_down_middle();
            count_stack_yellow=2;
            
        }   
        
         else if(count_stack_yellow==2)
        {
            car_pull_down_top();
            count_stack_yellow=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        turn_left_180_with_6050_check(90.0);
        car_stop();
        //
                               //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
           
            car_judge_decision();
            car_excute_decision();
            
            if(1==qti_5 && 1==qti_6 && 1==qti_7)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]���");
                
                
                break;
                
            }
                
                first_start_state_1=0x00;

        }
        
        
                
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_right_90();//        turn_right_90_with_6050_check(0.0);
        mpu_6050_check(0);
        car_stop(); 

        car_axis_recover_d_point();
         car_o_point_to_zero_2();

        //��2������׼��������һ����飬  �������������1  
        grasp_order_index++;
        count_point_e--;
        
        
        
        
    }
    
////////////////////////////////
/////////////////////////////////
//////////////////////////////////////                
//���������ϰ�������    ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////                  
    
    
    else
    {
        //car_yellow_not_pull_down_block();


/////////////////////////////////
//////////////////////////////////////                
//a��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////     
        
        if(colour_tempa==other||colour_tempa==block_colour_order[grasp_order_index])
        {
            colour_tempa=yellow;
            count_stack_tempa=0x00;
            
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� 
            turn_right_90();//            turn_right_90_with_6050_check(0.0);
            mpu_6050_check(0);        
            car_stop(); 
            car_o_point_to_temp_a();
        
        //��2������׼��������һ����飬  �������������1  
        grasp_order_index++;
        count_point_e--;
            

            
        }

 /////////////////////////////////
//////////////////////////////////////                
//b��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////     
        else if(colour_tempb==other||colour_tempb==block_colour_order[grasp_order_index])
        {
            colour_tempb=yellow;
            count_stack_tempb=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
            turn_right_90();//            turn_right_90_with_6050_check(0);
            mpu_6050_check(0);        
            car_stop(); 
          
            car_o_point_to_temp_b();

            grasp_order_index++;
            count_point_e--;

            
        }
/////////////////////////////////
//////////////////////////////////////                
//c��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////   
        else
        {
            
            
            colour_tempc=yellow;
            count_stack_tempc=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
            turn_right_90();//            turn_right_90_with_6050_check(0);
            mpu_6050_check(0);       
            car_stop(); 
            car_o_point_to_temp_c();          
 
        
            grasp_order_index++;
            count_point_e--;
            

            
        }
     
    }
} 


            
//////////////////////////////////////////////
//
/////////////////////////////////////////////
void car_e_point_white(void)
{
 //��ʱ��С��ͷ��ɫ�飬��ͷ90��                 
    //��ת45��
    turn_left_135();
    
    
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��b�㣬ж����β�״�����ָ���㣬ж��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
     walk_ae_state=0;
    while(1)
    {
        car_position_test_all_white();
        
        if(walk_ae_state==0)
        {
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_1 && 0==qti_2 && 0==qti_3 && 0==qti_4)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]ͷ��");
                
                
                
                        }
                        
                        first_start_state_1=0x00;
                        
                    }
                    
                    
            else if(walk_ae_state==1)
            {
                
                car_judge_decision();
                car_excute_decision();
                //ֱ��100���ף������غ�
                if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]β��");
                    
                    
                    
                    break;
                    
                }
                
            }
   
     }
            
                
                //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
      if(count_stack_white==0)
      {
          car_pull_down_bottle();
          count_stack_white=1;
          
      }
       else if(count_stack_white==1)
      {
          car_pull_down_middle();
          count_stack_white=2;
          
      }   
      
       else if(count_stack_white==2)
      {
          car_pull_down_top();
          count_stack_white=3;
          
      } 
      car_stop();                    
          
          //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
      turn_left_180_with_6050_check(45.0);
      car_stop();
      //
                             //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת45��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
      walk_ae_state=0;
      while(1)
      {
          car_position_test_all_white();
          
          if(walk_ae_state==0)
          {
              car_judge_decision();
              car_excute_decision();
              
              if(1==qti_5 && 1==qti_6 && 1==qti_7)
              {
                  walk_ae_state=1;
                  car_stop();
                  sprintf(buf1,"[s10]���");
                  
                  
                  
              }
              
              first_start_state_1=0x00;
              
          }
          
          
          else if(walk_ae_state==1)
          {
              if(first_start_state_1==0)
              {
                  first_start_state_1=1;

                  pwm_num_distance=0;
          
              }
              car_judge_decision();
              car_excute_decision();
              //ֱ��100���ף������غ�
              if(pwm_num_distance>=15)
              {
                  walk_ae_state=2;
                  sprintf(buf1,"[s10]15��");
                  
                  
                  car_stop();
                  break;
                  
              }
              
          }
          

          
      }
      //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
      turn_right_45();
      mpu_6050_check(0);
      if(0==qti_1 && 0==qti_2 && 0==qti_3)
      {
          mpu_6050_white();
      }       
      car_stop(); 
      
       car_o_point_to_zero_2();

      //��2������׼��������һ����飬  �������������1  
      grasp_order_index++;
      count_point_e--;

                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
            }
            
//////////////////////////////////////////////
//
/////////////////////////////////////////////
void car_e_point_red(void)
{
      //��ʱ��С��ͷ��ɫ�飬��ͷ90��             
    if(count_point_c<=1)
    {
//                    car_yellow_pull_down_block();
        turn_left_90_with_6050_check(180.0);
        //ֱ�ӷź�ɫ��飬ǰ����βȫ��
         while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                
                
                sprintf(buf1,"[s10]β��");
                
                
                car_stop();
                break;
                
            }
        }
         car_stop();    
            
          mpu_6050_check(180);  
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_red==0)
        {
            car_pull_down_bottle();
            count_stack_red=1;
            
        }
         else if(count_stack_red==1)
        {
            car_pull_down_middle();
            count_stack_red=2;
            
        }   
        
         else if(count_stack_red==2)
        {
            car_pull_down_top();
            count_stack_red=3;
            
        } 
        car_stop();                    
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
            turn_left_180();//        turn_left_180_with_6050_check(0.0);
        mpu_6050_check(0);
        car_stop();
        //
                               //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_8&& 1==qti_9 && 1==qti_10 && 1==qti_11)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]β��");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                
                car_judge_decision();
                car_excute_decision();
             
                if(1==qti_1&& 1==qti_2 && 1==qti_3 && 1==qti_4)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]ͷ��");
                    
                    
                    break;
                    
                }
                
            }
            

            
        }
       
        //ԭ�ص�ͷ180�ȣ�ָ��180�ȷ���  
        turn_left_180_with_6050_check(180.0);    
        
         car_stop();
           //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ֱ��10���ף��ܿ�����
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
         pwm_num_distance=0;  
        
        while(1)
        {
            car_position_test_all_white();
            
            
            car_judge_decision();
            car_excute_decision();
            //ֱ��10���ף��ܿ�����
            if(pwm_num_distance>=11)
            {
                walk_ae_state=2;
                sprintf(buf1,"[s10]2������");
                
                
                
                break;
                
            }
           
        }
        //��2������׼��������һ����飬  �������������1  
        grasp_order_index++;
        count_point_e--;
        
        
        
    }
    

         

    else
    {
         if(colour_tempa==other||colour_tempa==block_colour_order[grasp_order_index])
        {
            colour_tempa=red;
            count_stack_tempa=0x00;
            
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� 
            turn_right_90();//            turn_right_90_with_6050_check(0.0);
            mpu_6050_check(0);        
            car_stop();   
          
            car_o_point_to_temp_a();

        //��2������׼��������һ����飬  �������������1  
            grasp_order_index++;
            count_point_e--;
            

            
          }
        


 /////////////////////////////////
//////////////////////////////////////                
//b��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////     
        else if(colour_tempb==other||colour_tempb==block_colour_order[grasp_order_index])
        {
            colour_tempb=red;
            count_stack_tempb=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
            turn_right_90();//            turn_right_90_with_6050_check(0);
            mpu_6050_check(0);        
            car_stop();   
          
            car_o_point_to_temp_b();

        
            grasp_order_index++;
            
            count_point_e--;;
            
            
        }
 /////////////////////////////////
//////////////////////////////////////                
//c��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////   
        else
        {
            
            
            colour_tempc=red;
            count_stack_tempc=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
            turn_right_90();//            turn_right_90_with_6050_check(0);
            mpu_6050_check(0);        
            car_stop(); 

            car_o_point_to_temp_c();

        
            grasp_order_index++;
            count_point_e--;

            
        }
    }
  
}

            
//////////////////////////////////////////////
//
/////////////////////////////////////////////
void car_e_point_black(void)
{
     //��ʱ��С��ͷ��ɫ�飬��ͷ90��           
    //��ת135��
    turn_left_45();
   
    mpu_6050_check(135);
    car_axis_recover_d_point();
    
    
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��d�㣬ж����β�״�����ָ���㣬ж��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
     walk_ae_state=0;
    while(1)
    {
        car_position_test_all_white();
        
        if(walk_ae_state==0)
        {
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_1 && 0==qti_2 && 0==qti_3 && 0==qti_4)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]ͷ��");
                
                
                
            }
            
            first_start_state_1=0x00;
            
        }
        
        
        else if(walk_ae_state==1)
        {
            
            car_judge_decision();
            car_excute_decision();
            //ֱ��100���ף������غ�
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]β��");
                
                
                
                break;
                
            }
            
        }

    }

    
    //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_black==0)
        {
            car_pull_down_bottle();
            count_stack_black=1;
            
        }
         else if(count_stack_black==1)
        {
            car_pull_down_middle();
            count_stack_black=2;
            
        }   
        
         else if(count_stack_black==2)
        {
            car_pull_down_top();
            count_stack_black=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        turn_left_180_with_6050_check(315.0);
        car_stop();
        //
                               //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת45��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_5 && 1==qti_6 && 1==qti_7)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]���");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                if(first_start_state_1==0)
                {
                    first_start_state_1=1;

                    pwm_num_distance=0;
            
                }
                car_judge_decision();
                car_excute_decision();
                //ֱ��100���ף������غ�
                if(pwm_num_distance>=15)
                {
                    walk_ae_state=2;
                    sprintf(buf1,"[s10]15��");
                    
                    
                    car_stop();
                    break;
                    
                }
                
            }
            

            
        }
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_left_45();
        mpu_6050_check(0);
        if(0==qti_1 && 0==qti_2 && 0==qti_3)
        {
            mpu_6050_white();
        }
        
        
        car_stop();  
        
        car_o_point_to_zero_2();
        

        //��2������׼��������һ����飬  �������������1  
        grasp_order_index++;
        count_point_e--;
    
    

}
            
//////////////////////////////////////////////
//
/////////////////////////////////////////////
void car_e_point_blue(void)
{
     //��ʱ��С��ͷ��ɫ�飬��ͷ90��             
    if(count_point_e<=1)
    {
//                    car_yellow_pull_down_block();
        
        //ֱ�ӷ���ɫ��飬ǰ����βȫ��
         while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                
                
                sprintf(buf1,"[s10]β��");
                
                
                car_stop();
                break;
                
            }
        }
         car_stop();    
            
          mpu_6050_check(90);  
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_blue==0)
        {
            car_pull_down_bottle();
            count_stack_blue=1;
            
        }
         else if(count_stack_blue==1)
        {
            car_pull_down_middle();
            count_stack_blue=2;
            
        }   
        
         else if(count_stack_blue==2)
        {
            car_pull_down_top();
            count_stack_blue=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        turn_left_180_with_6050_check(270.0);
        car_stop();
        //
        


        //////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
           
            car_judge_decision();
            car_excute_decision();
            
            if(1==qti_5 && 1==qti_6 && 1==qti_7)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]���");
                
                
                break;
            }
            
            first_start_state_1=0x00;
                
          
                       
        }
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_left_90();//        turn_left_90_with_6050_check(0.0);
        mpu_6050_check(0);
        car_stop(); 
          
        car_o_point_to_zero_2();

        //��2������׼��������һ����飬  �������������1  
        grasp_order_index++;
        count_point_e--;


//////////////////////////////////////////////////////

    }
    

         

    else
    {
         if(colour_tempa==other||colour_tempa==block_colour_order[grasp_order_index])
        {
            colour_tempa=blue;
            count_stack_tempa=0x00;
            
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� 
            turn_right_90();//            turn_right_90_with_6050_check(0.0);
            mpu_6050_check(0);        
            car_stop(); 

            car_o_point_to_temp_a();
        //��2������׼��������һ����飬  �������������1  
            grasp_order_index++;
            count_point_e--;
            

            
      }
        


 /////////////////////////////////
//////////////////////////////////////                
//b��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////     
        else if(colour_tempb==other||colour_tempb==block_colour_order[grasp_order_index])
        {
            colour_tempb=blue;
            count_stack_tempb=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
            turn_right_90();//            turn_right_90_with_6050_check(0);
            mpu_6050_check(0);        
            car_stop();  
            
            car_o_point_to_temp_b();

        
            grasp_order_index++;
            
            count_point_e--;
            
            
        }
        
        
        
        
        
        
        

        

          /////////////////////////////////
//////////////////////////////////////                
//c��ʱ��ŵ���Դ�� ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////   
        else
        {
            
            
            colour_tempc=blue;
            count_stack_tempc=0x00;
            
                 
        //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
            turn_right_90();//            turn_right_90_with_6050_check(0);
            mpu_6050_check(0);        
            car_stop();  

            car_o_point_to_temp_c();

        
            grasp_order_index++;
            count_point_e--;  

            
        }
        
    }


}
            

//////////////////////////////////////////////
//
/////////////////////////////////////////////
void car_o_point_yellow(void)
{
      turn_left_90_with_axis_recover_6050_check(270.0);
        //ֱ�ӷŻ�ɫ��飬ǰ����βȫ��
         while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                
                
                sprintf(buf1,"[s10]β��");
                
                
                car_stop();
                break;
                
            }
        }
         car_stop();    
            
          mpu_6050_check(270);  
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_yellow==0)
        {
            car_pull_down_bottle();
            count_stack_yellow=1;
            
        }
         else if(count_stack_yellow==1)
        {
            car_pull_down_middle();
            count_stack_yellow=2;
            
        }   
        
         else if(count_stack_yellow==2)
        {
            car_pull_down_top();
            count_stack_yellow=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        turn_left_180_with_6050_check(90.0);
        car_stop();
        //
                               //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]ͷ��");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                if(first_start_state_1==0)
                {
                    first_start_state_1=1;

                    pwm_num_distance=0;
            
                }
                car_judge_decision();
                car_excute_decision();
                //ֱ��100���ף������غ�
                if(pwm_num_distance>=55)
                {
                    walk_ae_state=2;
                    sprintf(buf1,"[s10]55��");
                    
                    
                    car_stop();
                    break;
                    
                }
                
            }
            
            
        }
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_right_90();//        turn_right_90_with_6050_check(0.0);
        mpu_6050_check(0);
         if(0==qti_1 && 0==qti_2 && 0==qti_3)
        {
            mpu_6050_white();
        } 
        car_stop(); 
        
        while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
            {
                
                
                sprintf(buf1,"[s10]ͷ��");
                
                
                car_stop();
                break;
                
            }
        }
        
        
          
        //car_o_point_to_zero_2();

        //��2������׼��������һ����飬  �������������1  
//        grasp_order_index++;
        
}
//////////////////////////////////////////////
//
/////////////////////////////////////////////
  void car_o_point_white(void)
{
    turn_left_45();


    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
    ///              ׼��ǰ��b�㣬ж����β�״�����ָ���㣬ж��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    ////////////////////////////////////////////////////// 
     walk_ae_state=0;
    while(1)
    {
        car_position_test_all_white();
        
        if(walk_ae_state==0)
        {
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_1 && 0==qti_2 && 0==qti_3 && 0==qti_4)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]ͷ��");
                
                
                
            }
            
            first_start_state_1=0x00;
            
        }
        
        
        else if(walk_ae_state==1)
        {
            
            car_judge_decision();
            car_excute_decision();
            //ֱ��100���ף������غ�
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]β��");
                
                
                
                break;
                
            }
            
        }

    }


    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
    ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
        if(count_stack_white==0)
        {
            car_pull_down_bottle();
            count_stack_white=1;
            
        }
         else if(count_stack_white==1)
        {
            car_pull_down_middle();
            count_stack_white=2;
            
        }   
        
         else if(count_stack_white==2)
        {
            car_pull_down_top();
            count_stack_white=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        turn_left_180_with_6050_check(45.0);
        car_stop();
        //
                               //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
    ///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת45��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    ////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_5 && 1==qti_6 && 1==qti_7)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]���");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                if(first_start_state_1==0)
                {
                    first_start_state_1=1;

                    pwm_num_distance=0;
            
                }
                car_judge_decision();
                car_excute_decision();
                //ֱ��100���ף������غ�
                if(pwm_num_distance>=20)
                {
                    walk_ae_state=2;
                    sprintf(buf1,"[s10]20��");
                    
                    
                    car_stop();
                    break;
                    
                }
                
            }
            

            
        }
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_right_45();
        mpu_6050_check(0);
         if(0==qti_1 && 0==qti_2 && 0==qti_3)
        {
            mpu_6050_white();
        } 
        car_stop();   
        
//        car_o_point_to_zero_2();
         while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
            {
                
                
                sprintf(buf1,"[s10]ͷ��");
                
                
                car_stop();
                break;
                
            }
        }
 
    
}          
//////////////////////////////////////////////
//
/////////////////////////////////////////////
  void car_o_point_red(void)
{
     while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                
                
                sprintf(buf1,"[s10]β��");
                
                
                car_stop();
                break;
                
            }
        }
         car_stop();    
            
          mpu_6050_check(180);  
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
    ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
        if(count_stack_red==0)
        {
            car_pull_down_bottle();
            count_stack_red=1;
            
        }
         else if(count_stack_red==1)
        {
            car_pull_down_middle();
            count_stack_red=2;
            
        }   
        
         else if(count_stack_red==2)
        {
            car_pull_down_top();
            count_stack_red=3;
            
        } 
        car_stop();                                
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
            turn_left_180();//        turn_left_180_with_6050_check(0.0);
        mpu_6050_check(0);
        car_stop();
        //
                               //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
    ///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    ////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_8&& 1==qti_9 && 1==qti_10 && 1==qti_11)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]β��");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                
                car_judge_decision();
                car_excute_decision();
             
                if(1==qti_1&& 1==qti_2 && 1==qti_3 && 1==qti_4)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]ͷ��");
                    
                    
                    break;
                    
                }
                
            }
            

            
        }
       
//        //ԭ�ص�ͷ180�ȣ�ָ��180�ȷ���  
//        turn_left_180_with_6050_check(180.0);    
//        
//         car_stop();
//           //////////////////////////////////////////////////////
//    //////////////////////////////////////////////////////                    
//    //////////////////////////////////////////////////////
//    ///              ֱ��10���ף��ܿ�����
//    /////////////////////////// /////////////////////////// 
//    //////////////////////////////////////////////////////
//    //////////////////////////////////////////////////////                    
//         pwm_num_distance=0;  
//        
//        while(1)
//        {
//            car_position_test_all_white();
//            
//            
//            car_judge_decision();
//            car_excute_decision();
//            //ֱ��10���ף��ܿ�����
//            if(pwm_num_distance>=11)
//            {
//                walk_ae_state=2;
//                sprintf(buf1,"[s10]2������");
//                
//                
//                
//                break;
//                
//            }
//           
//        }
//        
//        //////////////////////////////////////////////////////
////////////////////////////////////////////////////////                    
////////////////////////////////////////////////////////
/////               ׼��ǰ��aeֱ�ߣ�ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
///////////////////////////// /////////////////////////// 
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////                    
//        walk_ae_state=0;
//        while(1)
//        {
//            car_position_test_all_white();
//            
//            if(walk_ae_state==0)
//            { 
//              if(1==qti_5 && 1==qti_6 && 1==qti_7)
//                {
//                    walk_ae_state=1;
//                    
//                    sprintf(buf1,"[s10]���");
//                    
//                    
//                     car_stop();
//                    break;
//                    
//                }
//                car_judge_decision();
//                car_excute_decision();
//                
//               
//                
//                first_start_state_1=0x00;
//                
//            }
//            
//            
//        } 
            
        car_stop();  
}
//////////////////////////////////////////////
//
/////////////////////////////////////////////
  void car_o_point_black(void)
{
     //��ת135��
    turn_right_45();
   
    mpu_6050_check(135);
    car_axis_recover_d_point();
    
    
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��d�㣬ж����β�״�����ָ���㣬ж��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
     walk_ae_state=0;
    while(1)
    {
        car_position_test_all_white();
        
        if(walk_ae_state==0)
        {
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_1 && 0==qti_2 && 0==qti_3 && 0==qti_4)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]ͷ��");
                
                
                
            }
            
            first_start_state_1=0x00;
            
        }
        
        
        else if(walk_ae_state==1)
        {
            
            car_judge_decision();
            car_excute_decision();
            //ֱ��100���ף������غ�
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                walk_ae_state=1;
                car_stop();
                sprintf(buf1,"[s10]β��");
                
                
                
                break;
                
            }
            
        }

    }

    
    //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_black==0)
        {
            car_pull_down_bottle();
            count_stack_black=1;
            
        }
         else if(count_stack_black==1)
        {
            car_pull_down_middle();
            count_stack_black=2;
            
        }   
        
         else if(count_stack_black==2)
        {
            car_pull_down_top();
            count_stack_black=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        turn_left_180_with_6050_check(315.0);
        car_stop();
        //
                               //////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת45��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_5 && 1==qti_6 && 1==qti_7)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]���");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                if(first_start_state_1==0)
                {
                    first_start_state_1=1;

                    pwm_num_distance=0;
            
                }
                car_judge_decision();
                car_excute_decision();
                //ֱ��100���ף������غ�
                if(pwm_num_distance>=14)
                {
                    walk_ae_state=2;
                    sprintf(buf1,"[s10]14��");
                    
                    
                    car_stop();
                    break;
                    
                }
                
            }
            

            
        }
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_left_45();
        mpu_6050_check(0);
         if(0==qti_1 && 0==qti_2 && 0==qti_3)
        {
            mpu_6050_white();
        } 
        car_stop();  

//          car_o_point_to_zero_2();
         while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
            {
                
                
                sprintf(buf1,"[s10]ͷ��");
                
                
                car_stop();
                break;
                
            }
        }

        //��2������׼������
} 
//////////////////////////////////////////////
//
/////////////////////////////////////////////
  void car_o_point_blue(void)
{
//                    car_yellow_pull_down_block();
        
        turn_right_90_with_6050_check(90.0);
        //ֱ�ӷ���ɫ��飬ǰ����βȫ��
         while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
            {
                
                
                sprintf(buf1,"[s10]β��");
                
                
                car_stop();
                break;
                
            }
        }
         car_stop();    
            
          mpu_6050_check(90);  
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              //��ʼ׼������� //С����λ��׼�������
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////                         
        if(count_stack_blue==0)
        {
            car_pull_down_bottle();
            count_stack_blue=1;
            
        }
         else if(count_stack_blue==1)
        {
            car_pull_down_middle();
            count_stack_blue=2;
            
        }   
        
         else if(count_stack_blue==2)
        {
            car_pull_down_top();
            count_stack_blue=3;
            
        } 
        car_stop();                    
            
            //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
        turn_left_180_with_6050_check(270.0);
        car_stop();
        //
        


        //////////////////////////////////////////////////////                    
//////////////////////////////////////////////////////
///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
/////////////////////////// /////////////////////////// 
//////////////////////////////////////////////////////
////////////////////////////////////////////////////// 
        walk_ae_state=0;
        while(1)
        {
            car_position_test_all_white();
            
            if(walk_ae_state==0)
            {
                car_judge_decision();
                car_excute_decision();
                
                if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                {
                    walk_ae_state=1;
                    car_stop();
                    sprintf(buf1,"[s10]ͷ��");
                    
                    
                    
                }
                
                first_start_state_1=0x00;
                
            }
            
            
            else if(walk_ae_state==1)
            {
                if(first_start_state_1==0)
                {
                    first_start_state_1=1;

                    pwm_num_distance=0;
            
                }
                car_judge_decision();
                car_excute_decision();
                //ֱ��100���ף������غ�
                if(pwm_num_distance>=49)
                {
                    walk_ae_state=2;
                    sprintf(buf1,"[s10]49��");
                    
                    
                    car_stop();
                    break;
                    
                }
                
            }
            
            
        }
        //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
        turn_left_90();//        turn_left_90_with_6050_check(0.0);
        mpu_6050_check(0);
         if(0==qti_1 && 0==qti_2 && 0==qti_3)
        {
            mpu_6050_white();
        } 
        car_stop();   
        
//        car_o_point_to_zero_2();
         while(1)
        {
            car_position_test_all_white();
            
                   
            car_judge_decision();
            car_excute_decision();
            
            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
            {
                
                
                sprintf(buf1,"[s10]ͷ��");
                
                
                car_stop();
                break;
                
            }
        }




//////////////////////////////////////////////////////

    }
    

         

    

//////////////////////////////////////////////
//
/////////////////////////////////////////////
void EOtoStartLine(void)
{
	while(1)
	{
		if(MoveMiddleQti())
		{
			fastforward();
		}
		if(IsELeftQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	TurnLeftAnyPulse(25);
	while(1)
	{
		SpinLeft();
		if(IsMLeftQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	while(1)
	{
		SpinLeft();
		if(IsMRightQtiBlack())
		{
			i++;
			if(i==2)
			{
				i=0;
				break;
			}
		}
	}
	stop();
}



#endif
