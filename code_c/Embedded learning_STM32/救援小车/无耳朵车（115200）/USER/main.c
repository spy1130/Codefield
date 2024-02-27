#include "stm32f10x.h"
#include "run.h"
#include "delay.h"
#include "cfg.h"
#include <stdio.h>
#include "stdbool.h"
#include "ult.h"
#include "motor.h"
#include "tcs.h"
#include "banyun1.h"
#include "CorrectDirection.h"
#include "usart.h"
#include "timer.h"

#include "DataScope_DP.h"
#include "tcs.h"
#include "run.h"
#include <string.h>
#include <math.h>




#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键3(WK_UP) 

 

#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	  2	//KEY1按下
#define WKUP_PRES   3	//KEY_UP按下(即WK_UP/KEY_UP)

//#define servo_1_orign_position 1400
//#define servo_2_orign_position 1575

#define servo_1_orign_position 1500
#define servo_2_orign_position 1500
//*******************************************************************
//#define servo_3_orign_position 2048
#define servo_3_orign_position 1872
#define servo_3_final_position 1024 
//*******************************************************************
#define servo_4_orign_position 1350
#define servo_5_orign_position 500


#define front_echo GPIO_Pin_11
#define front_trig GPIO_Pin_10


extern u16 dis;
extern uint8_t EPnum;

extern char func;

extern int colour;

extern int count_tcs230;
extern int tim5_interruput_flag;

extern int tim7_interruput_flag;
extern int pwm_num;



extern 	 u8  gy_5_flag;
extern  short YAW,PITCH,ROLL;
extern  u8 rx_buff[20];
extern  u8 rx_end_flag;//1 is mean that serious port  recieve a good frame,0 mean that is recieving ;
extern  u8 rx_star_flag;//1 mean the 0xa5 has already recieved,0 mean that we are waiting for the beginning of series port

extern char new_key_flag;

extern int head_i;
extern int tail_i;
extern int car_desicion;


extern u8 rx_buff_1[20];
extern u8 rx_end_flag_1;

extern int colour_flag;
extern int first_start_state_1;

extern int pwm_num_distance;

char grasp_order[20]={ 1,2,3,
                       4,5,6,
                       7,8,9,
                       10,11,12
                       };
char grasp_order_index=0x09;
					  
//char block_colour_order[20]={yellow,white,red,black,blue,}
char block_colour_order[20]={blue,red,yellow,
							 white,yellow,red,
							 yellow,red,white};
char block_colour_order_index=0x00;
							 
							 
u16 pcolor[3]={0,0,0};//时间比例因子


//uint8_t APnum=3;// A点叠放的色块个数
//uint8_t BPnum=3;
//uint8_t CPnum=3;
//uint8_t DPnum=3;
//uint8_t EPnum=3;

uint8_t yellownum=0;
uint8_t whitenum=0;//白区当前色块个数
uint8_t rednum=0;
uint8_t blacknum=0;
uint8_t bluenum=0;

int count_point_a=0x03;
int count_point_c=0x03;
int count_point_e=0x03;

int count_stack_yellow=0x00;
int count_stack_white=0x00;
int count_stack_red=0x00;
int count_stack_black=0x00;
int count_stack_blue=0x00;


int count_stack_tempa=0x03;
int count_stack_tempb=0x01;
int count_stack_tempc=0x01;

int colour_tempa=red;
int colour_tempb=blue;
int colour_tempc=yellow;

int tempa=0x00;

uint8_t remainsum=8;

int speak_len1=0x00;//(sizeof(buf_xm)/sizeof(buf_xm[0]));

char buf1[30]={"自检 "};//\[b1] 欢迎光临，请进

int str_num=0x00;

//char DataScope_OutPut_Buffer[20];

short temperature=0x00;

int Send_Count=0x00;
int led0pwmval;
int servo_num_1_pwmval=0x00;
int k_num=0x00;

int bolck_num=0x00;

char buf1[30];//\[b1] 欢迎光临，请进



int front_dis=0x00;
int left_dis=0x00;
int right_dis=0x00;

char temp_1_c;
char temp_2_c;

char temp_3_c;

char temp_4_c;

char temp_5_c;
char temp_6_c;
char temp_7_c;
char temp_8_c;
char temp_9_c;
char temp_10_c;
char temp_11_c;


char car_head_flag=0x00;
char walk_ae_state=0x00;

int a_point_top_down[30]={1500-110,1500+110,1360,800,1416};

int a_point_middle_down[30]={1500-310,1500+310,1450,780,1416};

int a_point_bottle_down[30]={1500-530,1500+530,1530,710,1416};

int a_point_top_up[30]={1500,1500,2500,500,1024};

int put_down_bottle_param[20]={1500-590,1500+590,1600,690};

int put_down_middle_param[20]={1500-470,1500+470,1590,660};

int put_down_top_param[20]={1500-330,1500+330,1650,830};

int turn_num=0x00;


int turn_left_flag=0x00;
//unsigned long dis=0,mindis=65535;
//char cnts1=0,cnts2=0;
////////////////////////////////////////////////////
//channel  1
///////////////////////////
///////////////////////////

void delay_ms_xm(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=5200;  //自己定义
      while(i--) ;    
   }
}
//////////////////////
//
////////////////////
int GetDis_front(int echo,int trig)
{
//    int dis=0;
//	int count;
//	char time_i=0;
//	//TIM_Cmd(TIM4,DISABLE); 
//	Tim4_Init();
//	GPIO_ResetBits(GPIOD,echo);
//	for(time_i=1;time_i<4;time_i++)
//	{
//	
//	
//		GPIO_SetBits(GPIOD,trig);		          //trig置高 发出10us的高电平信号 
//		delay_us(20); 
//		GPIO_ResetBits(GPIOD,trig);  
//				  
//		while(GPIO_ReadInputDataBit(GPIOD, echo) == 0);

//		TIM_SetCounter(TIM4,0)	;
//		TIM_Cmd(TIM4,ENABLE);    //开启计数器
//		count = TIM_GetCounter(TIM4);//获取计数器值
//	       //开启定时器开始计时
//		while(GPIO_ReadInputDataBit(GPIOD, echo));   //等待echo置低
//		TIM_Cmd(TIM4,DISABLE);   //关闭计数器
//	
//		count = TIM_GetCounter(TIM4);//获取计数器值
//		//dis = count/60.034   ;//转换为距离,精确度为cm		  // 若dis=count/60.034	,dis的精确度为cm
//		dis = dis+count/6.0034   ;//转换为距离,精确度为mm

//		
//		printf("    front=  %d",count )	;
//		delay_ms(30);	 


//	}
//	 	dis=dis/(time_i-1);
//		return dis;

        return 12;



}
//////////////////////
//
////////////////////


void NoFirstOtoMarkArea(void)
{
	int i;
	while(1)
	{
		
		MoveQti();
		delay_ms(20);
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(20);
		dis = Get_Distance();
		if(dis<190 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}
	stop();
	while(1)
	{
		MoveSlowQti();
		delay_ms(20);  
		PulseOut(leftservo,1500);
		PulseOut(rightservo,1500);
		delay_ms(20);
		dis = Get_Distance();
		if(dis<163 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}

	if(IsMLeftQtiBlack())
	{
		while(IsMLeftQtiBlack())
		{
			SpinLeft();
		}

	}
	else if(IsMRightQtiBlack())
	{
		while(IsMRightQtiBlack())
		{
			SpinRight();
		}
	}
	stop();
}
//////////////////////
//
////////////////////


void judge_turn_more_or_less(void)
{
	
	sprintf(buf1,"[s10]调整");
	
	
	
	if(qti_2==1 && qti_3==0)//车体左转90度后，发现转少了，没有转够90度
	{
		//原地左转 左舵机1450 右舵机1450
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
		

		
	}
		
		//是
	if(qti_2==0 && qti_3==1)//车体左转90度后，发现转多了，超过90度，往回转，即右转，直至回正
	{
		//原地右转 左舵机1550 右舵机1550
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
		
	}
}
//////////////////////
//
////////////////////
void close_claw_half(void)
{
	for(led0pwmval=1050;led0pwmval<=1450;led0pwmval=led0pwmval+40)
    {
		TIM_SetCompare1(TIM2,led0pwmval);
		delay_ms(10);
    }
	
	TIM_SetCompare1(TIM2,1450);
	delay_ms(120);
}
//////////////////////
//
////////////////////
void open_claw_half(void)
{
  for(led0pwmval=1450;led0pwmval>=1050;led0pwmval=led0pwmval-40)
    {
		TIM_SetCompare1(TIM2,led0pwmval);
		delay_ms(10);
    }
	
	TIM_SetCompare1(TIM2,1050);
	delay_ms(120);
}
//////////////////////
//
////////////////////
void close_claw(void)
{
	for(led0pwmval=1600;led0pwmval<=1900;led0pwmval=led0pwmval+40)
    {
		TIM_SetCompare1(TIM2,led0pwmval);
		delay_ms(10);
    }
	
	TIM_SetCompare1(TIM2,1900);
	delay_ms(120);
}

//////////////////////
//
////////////////////
void open_claw(void)
{
	for(led0pwmval=1900;led0pwmval>=1600;led0pwmval=led0pwmval-40)
    {
		TIM_SetCompare1(TIM2,led0pwmval);
		delay_ms(10);
    }
	
	TIM_SetCompare1(TIM2,1600);
	delay_ms(120);
}

	
void put_down_a_ponit_1_bottom(void)
{
	
	//开始下爪，4号舵机从初始位置（1000），改到763
	
	sprintf(buf1,"[s10]放下，4号夹");
	
	
	
	for(led0pwmval=1000;led0pwmval>=766;led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms(10);
    }
	
	TIM_SetCompare4(TIM1,766);
	
	delay_ms(1000);
	delay_ms(1000);
	
	sprintf(buf1,"[s10]2号夹");
	
	
	//4号就位，2号舵机从初始位置（1575），改到2060  +485
	//        1号舵机从初始位置（1400），改到915   -485
	k_num=0x00;
	for(led0pwmval=1500;led0pwmval<=2060;led0pwmval=led0pwmval+10)
    {
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=servo_1_orign_position-k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		delay_ms(20);
    }
	
	TIM_SetCompare2(TIM1,2060);
	TIM_SetCompare1(TIM1,915);
	
	delay_ms(1000);
	delay_ms(1000);
	
	sprintf(buf1,"[s10]3号夹");
	
	
	//2号就位，3号舵机从初始位置（2500），改到1316
	for(led0pwmval=2500;led0pwmval>=1426;led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms(30);
    }
	
	TIM_SetCompare3(TIM1,1426);
	delay_ms(1000);
	delay_ms(1000);
	
	
	sprintf(buf1,"[s10]5号夹");
	
	
	//3号就位，5号舵机从初始位置（2100），改到1500
	
	for(led0pwmval=2100;led0pwmval>=1500;led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare1(TIM2,led0pwmval);
		delay_ms(10);
    }
	
	TIM_SetCompare1(TIM2,1500);
	
	
	//然后抬起机械臂
	
	
	sprintf(buf1,"[s10]抬起，3号夹");
	
	
	//3号就位，5号舵机从初始位置（2100），改到1500
	//3号舵机从初始位置（1237），改到2500
	for(led0pwmval=1388;led0pwmval<=2500;led0pwmval=led0pwmval+10)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms(30);
    }
	
	TIM_SetCompare3(TIM1,2500);
	delay_ms(1000);
	delay_ms(1000);
	
	
	
	//4号舵机从初始位置（947），改到1000
	sprintf(buf1,"[s10]4号夹");
	
	
	
	for(led0pwmval=696;led0pwmval<=1000;led0pwmval=led0pwmval+10)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms(10);
    }
	
	TIM_SetCompare4(TIM1,1000);
	delay_ms(1000);
	delay_ms(1000);
	
	sprintf(buf1,"[s10]2号夹");
	
	
	//2号舵机从初始位置（2060），改到1575
	//1号舵机从初始位置（915），改到1400
	k_num=0x00;
	for(led0pwmval=2060;led0pwmval>=1575;led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=915+k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		delay_ms(10);
    }
	
	TIM_SetCompare2(TIM1,1575);
	
	TIM_SetCompare1(TIM1,1400);
	delay_ms(1000);
	delay_ms(1000);
	
}

//////////////
//put_down_a_ponit_2()
/////////////////////////
void put_down_a_ponit_2_middle(void)
{
	
	//开始下爪，4号舵机从初始位置（1000），改到763
	sprintf(buf1,"[s10]放下，4号夹");
	
	
	for(led0pwmval=1000;led0pwmval>=690;led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms(10);
    }
	
	TIM_SetCompare4(TIM1,690);
	
	delay_ms(1000);
	delay_ms(1000);
	
	
	//4号就位，2号舵机从初始位置（1575），改到1888  +313
	//        1号舵机从初始位置（1400），改到1087  -313
	sprintf(buf1,"[s10]2号夹");
	
	
	k_num=0x00;
	for(led0pwmval=servo_2_orign_position;led0pwmval<=1888;led0pwmval=led0pwmval+10)
    {
		TIM_SetCompare2(TIM1,led0pwmval);
		
		
		servo_num_1_pwmval=servo_1_orign_position-k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		delay_ms(20);
    }
	
	TIM_SetCompare2(TIM1,1888);
	TIM_SetCompare1(TIM1,1087);
	
	delay_ms(1000);
	delay_ms(1000);
	
	
	//2号就位，3号舵机从初始位置（2500），改到1316
	sprintf(buf1,"[s10]3号夹");
	
	
	for(led0pwmval=2500;led0pwmval>=1388;led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms(30);
    }
	
	TIM_SetCompare3(TIM1,1316);
	delay_ms(1000);
	delay_ms(1000);
	
	
	
	//3号就位，5号舵机从初始位置（2100），改到1500
	sprintf(buf1,"[s10]5号夹");
	
	
	
	for(led0pwmval=2100;led0pwmval>=1500;led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare1(TIM2,led0pwmval);
		delay_ms(10);
    }
	
	TIM_SetCompare1(TIM2,1500);
	
	//然后抬起机械臂
	
	
	sprintf(buf1,"[s10]抬起，3号夹");
	
	
	//3号舵机从初始位置（1237），改到2500
	for(led0pwmval=1237;led0pwmval<=2500;led0pwmval=led0pwmval+10)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms(30);
    }
	
	TIM_SetCompare3(TIM1,2500);
	delay_ms(1000);
	delay_ms(1000);
	
	
	
	//4号舵机从初始位置（947），改到1000
	sprintf(buf1,"[s10]4号夹");
	
	
	for(led0pwmval=947;led0pwmval<=1000;led0pwmval=led0pwmval+10)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms(10);
    }
	
	TIM_SetCompare4(TIM1,1000);
	
	delay_ms(1000);
	delay_ms(1000);
	
	//2号舵机从初始位置（1888），改到1575
	//1号舵机从初始位置（1087），改到1400
	sprintf(buf1,"[s10]2号夹");
	
	
	
	k_num=0x00;
	for(led0pwmval=1888;led0pwmval>=servo_2_orign_position;led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare2(TIM1,led0pwmval);
		
		
		servo_num_1_pwmval=1087+k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		delay_ms(10);
    }
	
	TIM_SetCompare2(TIM1,servo_2_orign_position);
	TIM_SetCompare1(TIM1,servo_1_orign_position);
	
	delay_ms(1000);
	delay_ms(1000);
	
}





//////////////
//put_down_a_ponit_3()
/////////////////////////
void put_down_a_ponit_3_top(void)
{
	
	//开始下爪，4号舵机从初始位置（1000），改到763
	sprintf(buf1,"[s10]放下，4号夹");
	
	
	for(led0pwmval=1000;led0pwmval>=947;led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms(10);
    }
	
	TIM_SetCompare4(TIM1,947);
	
	delay_ms(1000);
	delay_ms(1000);
	
	
	//4号就位，2号舵机从初始位置（1575），改到1650  +75
	//        1号舵机从初始位置（1400），改到1325  -75
	sprintf(buf1,"[s10]2号夹");
	
	
	k_num=0x00;
	for(led0pwmval=servo_2_orign_position;led0pwmval<=1650;led0pwmval=led0pwmval+10)
    {
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=servo_1_orign_position-k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		delay_ms(10);
    }
	
	TIM_SetCompare2(TIM1,1650);
	TIM_SetCompare1(TIM1,1325);
	
	delay_ms(1000);
	delay_ms(1000);
	
	
	//2号就位，3号舵机从初始位置（2500），改到1316
	sprintf(buf1,"[s10]3号夹");
	
	
	for(led0pwmval=2500;led0pwmval>=1300;led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms(30);
    }
	
	TIM_SetCompare3(TIM1,1300);
	delay_ms(1000);
	delay_ms(1000);
	
	
	
	//3号就位，5号舵机从初始位置（2100），改到1500
	sprintf(buf1,"[s10]5号夹");
	
	
	for(led0pwmval=2100;led0pwmval>=1500;led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare1(TIM2,led0pwmval);
		delay_ms(10);
    }
	
	TIM_SetCompare1(TIM2,1500);
	
	//然后抬起机械臂
	
	
	sprintf(buf1,"[s10]抬起，3号夹");
	
	
	//3号舵机从初始位置（1237），改到2500
	for(led0pwmval=1300;led0pwmval<=2500;led0pwmval=led0pwmval+10)
    {
		TIM_SetCompare3(TIM1,led0pwmval);
		delay_ms(30);
    }
	
	TIM_SetCompare3(TIM1,2500);
	delay_ms(1000);
	delay_ms(1000);
	
	
	
	//4号舵机从初始位置（947），改到1000
	sprintf(buf1,"[s10]4号夹");
	
	
	for(led0pwmval=947;led0pwmval<=1000;led0pwmval=led0pwmval+10)
    {
		TIM_SetCompare4(TIM1,led0pwmval);
		delay_ms(10);
    }
	
	TIM_SetCompare4(TIM1,1000);
	
	delay_ms(1000);
	delay_ms(1000);
	
	//2号舵机从初始位置（1650），改到1575
	//1号舵机从初始位置（1325），改到1400
	sprintf(buf1,"[s10]2号夹");
	
	
	for(led0pwmval=1650;led0pwmval>=servo_2_orign_position;led0pwmval=led0pwmval-10)
    {
		TIM_SetCompare2(TIM1,led0pwmval);
		
		servo_num_1_pwmval=1325+k_num*10;
		
		k_num++;
		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
		
		
		delay_ms(10);
    }
	
	TIM_SetCompare2(TIM1,servo_2_orign_position);
	TIM_SetCompare1(TIM1,servo_1_orign_position);
	delay_ms(1000);
	delay_ms(1000);
	
}
//////////////
//
/////////////////////////

void display_leda(void)


{
	temp_1_c=qti_1;
	temp_2_c=qti_2;
	temp_3_c=qti_3;
	temp_4_c=qti_4;
	temp_5_c=qti_5;
	temp_6_c=qti_6;
	temp_7_c=qti_7;
	temp_8_c=qti_8;
	temp_9_c=qti_9;
	temp_10_c=qti_10;
	temp_11_c=qti_11;
	
	
	led_1=temp_1_c;
	led_2=temp_2_c;
	led_3=temp_3_c;
	led_4=temp_4_c;
	led_5=temp_5_c;
	led_6=temp_6_c;
	led_7=temp_7_c;
	led_8=temp_8_c;
	led_9=temp_9_c;
	led_10=temp_10_c;
	led_11=temp_11_c;

}

void use_qti5_6_7_end_adjust(void)
{
	sprintf(buf1,"[s10]最后");
	
	
	
	
	
		//原地左转 左舵机1450 右舵机1450
		pwm_num=0x00;
		while(!(qti_5==0&&qti_7==1&&qti_6==0))
		{
//			 led0pwmval=1460;//high mean low speed
//			 TIM_SetCompare3(TIM3,led0pwmval);
//			 
//			//tim3 2# is left servo
//			 led0pwmval=1460;//high mean low speed
//			 TIM_SetCompare4(TIM3,led0pwmval);
			
//			turn_left_circle();
//			turn_right_circle();
//			sprintf(buf1,"0");
//			
//			
//			printf("5-0\n");
			
			display_leda();

			if(pwm_num>40)
			{
				break;
				
			}
			
			if(qti_5==0&&qti_7==1&&qti_6==1)
			{
				turn_left_circle_light();
			}
			
			if(qti_5==0&&qti_7==0&&qti_6==1)
			{
				turn_left_circle_light();
			}
			
//			if(qti_5==0&&qti_7==0&&qti_6==0)
//			{
//				turn_left_circle();
//			}
			
			
			if(qti_5==0&&qti_7==0&&qti_6==0)
			{
				turn_right_circle_light();
			}
			
			if(qti_5==1&&qti_7==0&&qti_6==0)
			{
				turn_right_circle_light();
			}
			
			if(qti_5==1&&qti_7==1&&qti_6==0)
			{
				turn_right_circle_light();
			}
			
//			
			
		}

		car_stop();
		display_leda();
		
		sprintf(buf1,"1");
		
		
		printf("5-1\n");
		

		
	
		
		//是
//	if(qti_2==0 && qti_3==1)//车体左转90度后，发现转多了，超过90度，往回转，即右转，直至回正
//	{
//		//原地右转 左舵机1550 右舵机1550
//		 pwm_num=0x00;
//		while(!(qti_2==0 && qti_3==0))
//		{
//			 led0pwmval=1550;//high mean low speed
//			 TIM_SetCompare3(TIM3,led0pwmval);
//			 
//			//tim3 2# is left servo
//			 led0pwmval=1550;//high mean low speed
//			 TIM_SetCompare4(TIM3,led0pwmval);

//			if(pwm_num>20)
//			{
//				break;
//				
//			}
//			
//		}

//		car_stop();
//		
//	}
}
	


////////////////////////////
//
//////////////////////////
void StartRun(void)
{
	bolck_num=0x04;
	car_head_flag=0x00;
	walk_ae_state=0x00;
	//goto B_ponit;//
	for(bolck_num=1;bolck_num<=9;bolck_num++)
	{
		
		car_stop();
		//goto aaa;
		while(!car_walk_to_middle());//到达A,E线边缘
		car_stop();
		
//		sprintf(buf1,"[s10]程序写到这里，下面还没写，所以停车");
//		
//		
//		car_stop();
//		while(1);
		
		sprintf(buf1,"[s10]5号6号到达边缘");
		
		
		
		car_stop();
		delay_ms(1000);
//		delay_ms(1000);

//		while(!MoveEdgeQti());//qti_1,qti_4到达A,E线边缘
//		
//		car_stop();
//		sprintf(buf1,"[s10]1号4号到达边缘");
//		
//		
//		car_stop();
//		delay_ms(1000);
//		delay_ms(1000);
		//往前冲6mm，使轮子轴线与A,E线中心重和
//		pwm_num=0x00;//pwm_num每加1，小车前进0.2毫米
//		while(pwm_num<3+3+2-4+3)
//		{
//			fastforward_xm();
//			     
//		}
//		car_stop();
//		
//		sprintf(buf1,"[s10]轴心重合");
//		
//		
//		
//		car_stop();
//		delay_ms(1000);
////		delay_ms(1000);
		
//		sprintf(buf1,"[s10]左转90度");
//		
//		
		
		//turn_left_90_with_qti2_3_1();
		//turn_left_90_with_6050_check();
		
		
		
		
//开始夹取a点最顶端物体
		if(bolck_num==0x01)
		{
//			pick_up_a_ponit_1_top();
			
			

//			sprintf(buf1,"[s8]开始测量颜色");
//			
//			
//			delay_ms(1000);
//			delay_ms(1000);
			colourjudge(pcolor);
		
			delay_ms_xm(500);
			sprintf(buf1,"[s10]程序写到这里，下面还没写，所以停车");
			
			
			car_stop();
			while(1);
		
		}
		
		if(bolck_num==0x02)
		{
//			pick_up_a_ponit_2_middle();
			
			sprintf(buf1,"[s8]开始测量颜色");
			
			
			delay_ms(1000);
			delay_ms(1000);
			colourjudge(pcolor);
		}
		
		if(bolck_num==0x03)
		{
//			pick_up_a_ponit_3_bottom();
			
			sprintf(buf1,"[s8]开始测量颜色");
			
			
			delay_ms(1000);
			delay_ms(1000);
			colourjudge(pcolor);
		}
		
		
		
		sprintf(buf1,"[s10]程序写到这里，下面还没写，所以停车");
		
		
		car_stop();
//		while(1); 
		
		car_stop();
		delay_ms(1000);
		delay_ms(1000);
		
		
		
//aaa:		
		//右转90度
		turn_right_90_qti2_3();
		car_stop();
		delay_ms(1000);
		delay_ms(1000);
		
//		judge_turn_more_or_less();
//		car_stop();
//		delay_ms(1000);
//		delay_ms(1000);
		
		use_qti5_6_7_end_adjust();
		delay_ms(1000);
		delay_ms(1000);
		//巡线前进，知道qti7发现白色停车点
		
		while(!MoveMiddleQti_qti7_stop());//到达A,E线边缘
//		MoveMiddleQti_qti7_stop();
		
		car_stop();
		delay_ms(1000);
		delay_ms(1000);
		
		put_down_a_ponit_1_bottom();
		car_stop();
		delay_ms(1000);
		delay_ms(1000);
		
//		while(1);
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		//左转90度
		turn_left_90_with_qti2_3_2();
		//判断转多还是转少
		car_stop();
		delay_ms(1000);
		delay_ms(1000);
		
		judge_turn_more_or_less();
		car_stop();
		delay_ms(1000);
		delay_ms(1000);
		
		use_qti5_6_7_end_adjust();
		//夹取A点顶部色块，然后抬起机械臂
		car_stop();
		delay_ms(1000);
		delay_ms(1000);
		
		//码垛第一个
		while(!MoveMiddleQti());//将色块搬回起点，到达起点边缘停车
		car_stop();
		
			
		//放下a点第一个（即顶部）色块,将其变为起点区底部色块
		if(bolck_num==0x01)
		{
			put_down_a_ponit_1_bottom();
		}
		
		
		if(bolck_num==0x02)
		{
			put_down_a_ponit_2_middle();
			
		}
		
		if(bolck_num==0x03)
		{
			put_down_a_ponit_3_top();
		}
		
	
				
		//原地掉头
		car_stop();
		
		
		turn_left_90();
		car_stop();
		delay_ms(1000);
		delay_ms(1000);
		
		
		turn_left_90_with_qti2_3_2();
		car_stop();
		delay_ms(1000);
		delay_ms(1000);
		//判断转多还是转少
		judge_turn_more_or_less();
		
		delay_ms(1000);
		delay_ms(1000);
		
		
		
		
		
	}
	
	
//B_ponit:	
	bolck_num=4;
	sprintf(buf1,"[s13]开始B任务");
	
	
	
	
//	    turn_left_90_with_qti2_3_2();
//		//判断转多还是转少
//		car_stop();
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		
//		judge_turn_more_or_less();
//		car_stop();
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
		
//	while(1)
//	{
//		front_dis= GetDis_front(front_echo,front_trig);
//		delay_ms(300);
//		
//		 sprintf(buf1,"%d",front_dis);
//		
//		
//		
//	}
	
	
	
	//开始抓取c点的3个码垛
	for(bolck_num=4;bolck_num<=6;bolck_num++)
	{
		while(!MoveMiddleQti());//到达A,E线边缘
		car_stop();
		
		sprintf(buf1,"[s10]2号3号到达边缘");
		
		
		
		delay_ms(1000);
		delay_ms(1000);

		while(!MoveEdgeQti());//qti_1,qti_4到达A,E线边缘
		
		car_stop();
		sprintf(buf1,"[s10]1号4号到达边缘");
		
		
		delay_ms(1000);
		delay_ms(1000);
		//往前冲6mm，使轮子轴线与A,E线中心重和
		pwm_num=0x00;//pwm_num每加1，小车前进0.2毫米
		while(pwm_num<3+3+2-4)
		{
			fastforward();
			
		}
		car_stop();
		
		sprintf(buf1,"[s10]轴心重合");
		
		
		
		delay_ms(1000);
		delay_ms(1000);
		
	
		//夹取A点顶部色块，然后抬起机械臂

//		if(bolck_num==0x04)
//		{
//			pick_up_a_ponit_1_top();
//		}
//		
//		if(bolck_num==0x05)
//		{
//			pick_up_a_ponit_2_middle();
//		}
//		
//		if(bolck_num==0x06)
//		{
//			pick_up_a_ponit_3_bottom();
//		}
		car_stop();
		
		//左转90度
		turn_left_90();
		car_stop();
		
		delay_ms(1000);
		delay_ms(1000);
		
		//左转90度
		turn_left_90_with_qti2_3_2();
		car_stop();
		judge_turn_more_or_less();
		delay_ms(1000);
		delay_ms(1000);
		
		//判断转多还是转少
		judge_turn_more_or_less();
		
	    
		



		//码垛第一个
		//放下a点第一个（即顶部）色块,将其变为起点区底部色块
		if(bolck_num==0x04)
		{
			//将色块搬回起点，到达距离第一个码垛5厘米处停车
			
			while(1)//
			{
				front_dis= GetDis_front(front_echo,front_trig);
				if(front_dis<=190)
				{
					
					car_stop();
					delay_ms(1000);
					delay_ms(1000);
					
					break;
				}
				
				walk_alone_MiddleQti();
				
				
			}
			//距离小于180毫米，开始放置物品a
			
			put_down_a_ponit_1_bottom();
		}
		
		
		if(bolck_num==0x05)
		{
			
			while(1)//
			{
				front_dis= GetDis_front(front_echo,front_trig);
				if(front_dis<=80)
				{
					
					car_stop();
					delay_ms(1000);
					delay_ms(1000);
					
					break;
				}
				walk_alone_MiddleQti();
				
			}
			
			put_down_a_ponit_2_middle();
			
		}
		
		if(bolck_num==0x06)
		{
			while(1)//
			{
				front_dis= GetDis_front(front_echo,front_trig);
				if(front_dis<=80)
				{
					
					car_stop();
					delay_ms(1000);
					delay_ms(1000);
					
					break;
				}
				
				walk_alone_MiddleQti();
				
			}
			
			put_down_a_ponit_3_top();
		}
		
		
		//原地掉头
		
		turn_left_90();
		delay_ms(1000);
		delay_ms(1000);
		
		
		turn_left_90();
		
		delay_ms(1000);
		delay_ms(1000);
		//判断转多还是转少
		judge_turn_more_or_less();
		
		delay_ms(1000);
		delay_ms(1000);
		
		
		
		
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	car_stop();
	while(1);
	
	//Crossblk(50);
}

//////////////////////////////////////////
//
/////////////////////////////////////////
void start_run_new(void)
{
    
    count_point_a=0x00;
    count_point_c=0x03;
    count_point_e=0x03;

    count_stack_yellow=0x00;
    count_stack_white=0x00;
    count_stack_red=0x00;
    count_stack_black=0x00;
    count_stack_blue=0x00;


    count_stack_tempa=0x00;
    count_stack_tempb=0x00;
    count_stack_tempc=0x00;
	
	grasp_order_index=0x00;
    
	
	car_from_star_to_2();
    car_stop();
    car_stop();
    
    while(1)
    {
        car_from_2_to_ae();//
        car_stop();
	
        if(1<=grasp_order[grasp_order_index]&&grasp_order[grasp_order_index]<=3)
        {
            //car_point_a_pull_up_down();
            
                         //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///     原地左转90度，指向270度方向,抓取物体
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
             //  
            turn_left_90_with_6050_check(270.0);
             car_stop();
            
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
             car_stop();
             //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///     判断颜色，也可以直接访问颜色序列数组，我直接访问颜色序列数组
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////     
           
            if(yellow==block_colour_order[grasp_order_index])
            {
                if(count_point_a<=0)
                {
//                    car_yellow_pull_down_block();
                    
                    //直接放黄色物块，前进，尾全白
                     while(1)
                    {
                        car_position_test_all_white();
                        
                               
                        car_judge_decision();
                        car_excute_decision();
                        
                        if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
                        {
                            
                            
                            sprintf(buf1,"[s10]尾白");
                            
                            
                            car_stop();
                            break;
                            
                        }
                    }
                     car_stop();    
                        
                       
 //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        //物体放下完毕，原地调头180度，，指向90度方向  
                    turn_left_180_with_6050_check(90.0);
                    car_stop();
                    //
                                           //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              准备前往o点，并且轴心停在o点，然后右转90度
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
                                sprintf(buf1,"[s10]头黑");
                                
                                
                                
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
                            //直行100毫米，轴心重合
                            if(pwm_num_distance>=49)
                            {
                                walk_ae_state=2;
                                sprintf(buf1,"[s10]49到");
                                
                                
                                car_stop();
                                break;
                                
                            }
                            
                        }
                        
                        
                    }
                    //    //原地右转90度，指向0度方向  
                    turn_right_90();//                    turn_right_90_with_6050_check(0.0);
                    mpu_6050_check(0);
                     if(0==qti_1 && 0==qti_2 && 0==qti_3)
                    {
                        mpu_6050_white();
                    } 
                    car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               准备前往2号区域，直行直到头黑，原地调头180
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
                                
                                sprintf(buf1,"[s10]头黑");
                                
                                
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    //原地调头180度，指向180度方向  
                    turn_left_180_with_6050_check(180.0);    
                    
                     car_stop();
                       //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            
                            
                            
                            break;
                            
                        }
                       
                    }
                    //在2号区域，准备搬运下一个五块，  因此物料索引加1  
                    grasp_order_index++;
                    count_point_a--;
                    
                    
                    
                }
                
   ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////                
//下面是有障碍物的情况    ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////                  
                
                
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
                
                else if(4<=grasp_order[grasp_order_index]&&grasp_order[grasp_order_index]<=6)
                {
                    car_point_b_pull_up_down();
                }
                
                else if(7<=grasp_order[grasp_order_index]&&grasp_order[grasp_order_index]<=9)
                {
                    car_point_c_pull_up_down();
                }
            }
	
	
	
//	sprintf(buf1,"[s10]程序写到这里，下面还没写，所以停车");
//	
//	
//	car_stop();
//	while(1);
	
	
}





void EndRun(void)
{
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	Crossblk(200);
	stop();
	while(1);
}





void CarryYellow(void)
{
	//搬运黄色块
	int i;
	
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	delay_ms(50);
OCtoOA();
	//	OCtoOB();
//	OBtoOA();
	if(yellownum == 0)
	{
		FirstOtoMarkArea();
	}
	else
	{
		NoFirstOtoMarkArea();
	}
	holdhand();
	delay_ms(50);
	switch(yellownum)
	{
		case 0:
			TurnLeftAnyPulse(5);
		putbottom();
			break;
		case 1:
			TurnLeftAnyPulse(2);
		putmid();
			break;
		case 2:
			puttop();
			break;
		default:
			stop();
			break;
	}
	for(i=0;i<13;i++)
	{
		MoveMiddleQti();
	}
	Turn180();
	AOtoStartLine();

	yellownum++;
}

void CarryWhite(void)
{
	//搬运白色块
	int i;
	
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	delay_ms(50);
	OCtoOB1();
	if(whitenum==0)
	{
		FirstOtoMarkArea();
	}
	else
	{
		NoFirstOtoMarkArea();
	}
	holdhand();
	delay_ms(50);
	switch(whitenum)
	{
		case 0:
		TurnLeftAnyPulse(4);
		putbottom();
			break;
		case 1:
			TurnLeftAnyPulse(3);
		putmid();
			break;
		case 2:
			puttop();
			break;
		default:
			stop();
			break;
	}
	for(i=0;i<13;i++)
	{
		MoveMiddleQti();
	}
	Turn180();
	BOtoStartLine();
	whitenum++;
}

void CarryRed(void)
{
	int i;
	//搬运红色块
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	delay_ms(50);
	if(rednum==0)
	{
		FirstOtoMarkArea();
	}
	else
	{
		NoFirstOtoMarkArea();
	}
	holdhand();
	delay_ms(50);
	switch(rednum)
	{
		case 0:
			TurnLeftAnyPulse(4);
			putbottom();
			break;
		case 1:
			TurnLeftAnyPulse(4);
		putmid();
			break;
		case 2:
			puttop();
			break;
		default:
			stop();
			break;
	}
	for(i=0;i<13;i++)
	{
		MoveMiddleQti();
	}
	Turn180();
	COtoStartLine();
	rednum++;
}

void CarryBlack(void)
{
	int i;
	//搬黑色块
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	delay_ms(50);
	OCtoOD();
	if(blacknum==0)
	{
		FirstOtoMarkArea();
	}
	else
	{
		NoFirstOtoMarkArea();
	}
	holdhand();
	delay_ms(50);
	switch(blacknum)
	{
		case 0:
			TurnLeftAnyPulse(5);
			putbottom();
			break;
		case 1:
			TurnLeftAnyPulse(3);
		putmid();
			break;
		case 2:
			puttop();
			break;
		default:
			stop();
			break;
	}
	for(i=0;i<13;i++)
	{
		MoveMiddleQti();
	}
	Turn180();
	DOtoStartLine();
	blacknum++;
}

void CarryBlue(void)
{
	int i;
	//搬运蓝色块
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	delay_ms(50);
	OCtoOE();
	if(bluenum==0)
	{
		FirstOtoMarkArea();
	}
	else
	{
		NoFirstOtoMarkArea();
	}
	holdhand();
	delay_ms(50);
	switch(bluenum)
	{
		case 0:
			TurnLeftAnyPulse(4);
			putbottom();
			break;
		case 1:
			TurnLeftAnyPulse(4);
		putmid();
			break;
		case 2:
			puttop();
			break;
		default:
			stop();
			break;
	}
	for(i=0;i<13;i++)
	{
		MoveMiddleQti();
	}
	Turn180();
	EOtoStartLine();
	bluenum++;
}

void ECarryYellow(void)
{
	int i;
	OEtoOD();
	ODtoOC();
	OCtoOA();
	FirstOtoMarkArea();
	holdhand();
	delay_ms(50);
	switch(yellownum)
	{
		case 0:
			TurnLeftAnyPulse(5);
		putbottom();
			break;
		case 1:
			putmid();
			break;
		case 2:
			puttop();
			break;
		default:
			stop();
			break;
	}

	for(i=0;i<13;i++)
	{
		MoveMiddleQti();
	}
	Turn180();
	AOtoStartLine();
	yellownum++;
}

void ECarryWhite(void)
{
	int i;
	OEtoOD();
	ODtoOC();
	OCtoOB();
	FirstOtoMarkArea();
	holdhand();
	delay_ms(50);
	switch(whitenum)
	{
		case 0:
			TurnLeftAnyPulse(5);
		putbottom();
			break;
		case 1:
			putmid();
			break;
		case 2:
			puttop();
			break;
		default:
			stop();
			break;
	}
	for(i=0;i<13;i++)
	{
		MoveMiddleQti();
	}

	Turn180();
	BOtoStartLine();
	whitenum++;
}

void ECarryRed(void)
{
	int i;
	OEtoOD();
	ODtoOC();
	FirstOtoMarkArea();
	holdhand();
	delay_ms(50);
	switch(rednum)
	{
		case 0:
			TurnLeftAnyPulse(8);
		putbottom();
			break;
		case 1:
			putmid();
			break;
		case 2:
			puttop();
			break;
		default:
			stop();
			break;
	}
	for(i=0;i<13;i++)
	{
		MoveMiddleQti();
	}

	Turn180();
	COtoStartLine();
	rednum++;
}

void ECarryBlack(void)
{
	int i;
	OEtoOD();
	FirstOtoMarkArea();
	holdhand();
	delay_ms(50);
	switch(blacknum)
	{
		case 0:
			TurnLeftAnyPulse(6);
			putbottom();
			break;
		case 1:
			putmid();
			break;
		case 2:
			puttop();
			break;
		default:
			stop();
			break;
	}
	for(i=0;i<13;i++)
	{
		MoveMiddleQti();
	}

	Turn180();
	DOtoStartLine();
	blacknum++;
}

void ECarryBlue(void)
{
	int i;
	stop();
	delay_ms(100);
	FirstOtoMarkArea();
	holdhand();
	delay_ms(50);
	switch(bluenum)
	{
		case 0:
			TurnLeftAnyPulse(5);
		putbottom();
			break;
		case 1:
			putmid();
			break;
		case 2:
			puttop();
			break;
		default:
			stop();
			break;
	}
	for(i=0;i<13;i++)
	{
		MoveMiddleQti();
	}

	Turn180();
	EOtoStartLine();
	bluenum++;
}

 void caEbottom()
{
	int cl;
	while(!MoveMiddleQti());
	while(!MoveEdgeQti());
	stop();
	delay_ms(20);
	OCtoOE();
	delay_ms(20);
	TurnRightAnyPulse(4);
	LCorrectDir();
	TurnLeftAnyPulse(12);
	delay_ms(20);
	cabottom();
	turnmotordown();
	delay_ms(200);
	cl=colourjudge(pcolor);
	delay_ms(100);
	turnmotorup();	
	printf("%d\n",cl);		
	switch(cl)
	{
		case yellow:
			ECarryYellow();
		break;
		case white:
			ECarryWhite();
		break;
		case red:
			ECarryRed();
		break;
		case black:
			ECarryBlack();
		break;
		case blue:
			ECarryBlue();
		break;
		default:
			break;
	}
}

void CarryEtoO(void)
{
	switch(EPnum)
	{
		case 3:
			caEtop();
			break;
		case 2:
			caEmid();
			break;
		case 1:
			caEbottom();
			break;
		default:
			func = 4;
			break;
	}
	EPnum = EPnum - 1;
}

void CarryToTargetArea(void)
{
	int i;
	int cl;
	for(i=0;i<30;i++)
	{
		MoveQti1();
		delay_ms(10);
	}
	stop();
	delay_ms(200);
	i=0;
	while(1)
	{
		MoveQti1();
		delay_ms(10);
//		PulseOut(leftservo,1500);
//		PulseOut(rightservo,1500);
//		delay_ms(20);
		dis = Get_Distance();
		if(dis<190 && dis>0)
		{
			i++; 
			if(i>=2){
				i=0;
				stop();
				break;
			}
		}
	}
	stop();
	delay_ms(100);
	while(1)
	{
		MoveSlowQti();
		delay_ms(10);
//		PulseOut(leftservo,1500);
//		PulseOut(rightservo,1500);
//		delay_ms(10);
		dis = Get_Distance();
		if(dis<164 && dis>0)
		{
			i++;
			if(i>=2)
			{
				i=0;
				stop();
				break;
			}
		}
	}


	stop();
	switch(remainsum)
	{
		case 8:
			TurnLeftAnyPulse(7);
		camid();
			break;
		case 7:
			TurnRightAnyPulse(3);
		cabottom();
			break;
		case 6:
			TurnLeftAnyPulse(2);
		catop();
			break;
		case 5:
			TurnLeftAnyPulse(2);
			camid();
			break;
		case 4:
			TurnLeftAnyPulse(2);
		cabottom();
			break;
		case 3:
			TurnLeftAnyPulse(2);
			catop();
			break;
		case 2:
			TurnLeftAnyPulse(2);
			camid();
			break;
		case 1:
			TurnLeftAnyPulse(2);
			cabottom();
			break;
		default:
			break;
	}
	remainsum = remainsum - 1;
	if(remainsum == 0)
	{
		func = 5;
	}
	turnmotordown();
	delay_ms(100);
	cl=colourjudge(pcolor);
	delay_ms(100);
	turnmotorup();
	Turn180();
	switch(cl)
	{
		case yellow:
			CarryYellow();
			break;
		case white:
			CarryWhite();
			break;
		case red:
			CarryRed();
			break;
		case black:
			CarryBlack();
			break;
		case blue:
			CarryBlue();
			break;
		default:
			break;
	}
	
}

void loop(void)
{	
	switch(func)
	{
		case 1:
			CarryAtoO();	//搬运A点色块
		break;
		case 2:
			CarryCtoO();	//搬运C点色块
		break;
		case 3:
			CarryEtoO();	//搬运E点色块
		break;
		case 4:
			CarryToTargetArea();//搬运色块到目标区域
		break;
		case 5:
			EndRun();
		break;
		default:
			break;
	}
}
























void EXTIX_Init(void)
{
 
   	EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

   GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE4,3

//	//初始化 WK_UP-->GPIOA.0	  下拉输入
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟



//   //GPIOE.3	  中断线以及中断初始化配置 下降沿触发 //KEY1
//  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
//  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

//   //GPIOE.4	  中断线以及中断初始化配置  下降沿触发	//KEY0
//  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
//  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
//  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器



   //GPIOE.5	  中断线以及中断初始化配置  下降沿触发	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	 //GPIOE.6	  中断线以及中断初始化配置  下降沿触发	//KEY0
//  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource6);
//  	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
//  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	 //GPIOE.7	  中断线以及中断初始化配置  下降沿触发	//KEY0
//  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource7);
//  	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
//  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


//   //GPIOA.0	  中断线以及中断初始化配置 上升沿触发 PA0  WK_UP
// 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); 

//  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


//  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键WK_UP所在的外部中断通道
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
//  	NVIC_Init(&NVIC_InitStructure); 

//  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键KEY1所在的外部中断通道
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1 
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
//  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

//  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键KEY0所在的外部中断通道
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
//  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
//		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY0所在的外部中断通道
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
//  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
//	
//		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY0所在的外部中断通道
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
//  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
}

////外部中断0服务程序 
//void EXTI0_IRQHandler(void)//pa0
//{
//	delay_ms(10);//消抖
//	if(WK_UP==1)	 	 //WK_UP按键
//	{				 
////		BEEP=!BEEP;	
//	}
//	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
//}
// 

////外部中断3服务程序
//void EXTI3_IRQHandler(void)//pe3
//{
//	delay_ms(10);//消抖
//	if(KEY1==0)	 //按键KEY1
//	{				 
////		LED1=!LED1;
//	}		 
//	EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
//}

//void EXTI4_IRQHandler(void)//pe4
//{
//	delay_ms(10);//消抖
//	if(KEY0==0)	 //按键KEY0
//	{
////		LED0=!LED0;
////		LED1=!LED1; 
//		tim5_interruput_flag++;
//	}		 
//	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位  
//}
// 

//void EXTI9_5_IRQHandler(void)//pe4
//{
//	delay_ms(10);//消抖
//	if(KEY0==0)	 //按键KEY0
//	{
////		LED0=!LED0;
////		LED1=!LED1; 
//	}		 
//	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位  
//}



void EXTI9_5_IRQHandler(void)
{
	if( EXTI_GetITStatus(EXTI_Line5) != RESET) 
	{
		count_tcs230++;
//		tim5_interruput_flag=0x01;
	}
	EXTI_ClearITPendingBit(EXTI_Line5);
	
}





test_check_color()
{
	while(1)
	{
		sprintf(buf1,"[s10]请放色块");
		
		
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		
		
		sprintf(buf1,"[s8]开始测量颜色");
		
		
		delay_ms(1000);
		delay_ms(1000);
		colourjudge(pcolor);
		
		delay_ms(1000);
	}

}


test_qti_1_2()
{
	while(1)
	{
		if(qti_2==0x01)	
		{
			sprintf(buf1,"高");
			
			 
		}
		if(qti_2==0x00)	
		{
			sprintf(buf1,"低");
			
			 
		}
		
		delay_ms(300);
	}
}
	



turn_left_90_continue()
{
	while(1)
	{
		//turn_left_90();
		turn_right_90();
		delay_ms(1000);
		delay_ms(1000);
	}
}
	

//void show_gy25(void)
//{
//	float tempxx=0;
//	
//	if(rx_end_flag==1)//PITCH,ROLL;
//	{
//		
//		
//		memset(buf1, 0,sizeof(buf1));
//		if(YAW>=0)
//		{
//			tempxx=YAW/100.0;
//			sprintf(buf1,"+%-5.2f",tempxx);
//			
//		}
//		else
//		{
//			tempxx=YAW/100.0;
//			sprintf(buf1,"%-5.2f",tempxx);
//		}
//		OLED_P8x16Str(32, 0, (unsigned char*)buf1, 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/
//		
//		
//		
//		rx_end_flag=0;

//	}
//}
















int main(void)
{   
    int jj;
	float tempxxa=0;

	colour_flag=0;
    delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200//vedio control
  
	uart2_init(9600);	 	//串口初始化为115200//6050,,vedio control
//	
//	
//	while((USART2->SR&0X40)==0);  
//	USART2->DR = 0xa5; 
//	
//	while((USART2->SR&0X40)==0);  
//	USART2->DR = 0x51; 
	
	
	
	
  uart3_init(9600);	 	//串口初始化为115200//voice
	cfg_Init();
	EXTIX_Init();         	//初始化外部中断输入 

	led_init();//注意大小写，因为有个大写的LED_Init（）；
	
	
	
//	OLED_Init();         //OLED显示初始化
//	

//	
//	tempxxa=YAW/100.0;
//	sprintf(buf1,"YAW=%-5.2f ",tempxxa);
//				
//	OLED_P8x16Str(0, 0, (unsigned char*)buf1, 0); //
	
//	OLED_P8x16Str(0, 0, "DIR= 0   ", 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/
//	OLED_P8x16Str(0, 2, "width=00080 CM", 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/
	
	
	
	
	TIM3_PWM_Init(20000-1,72-1);
	
	//f
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	


	 //TIM7_Int_Init(0xfff0-1,720-1);
	 
	 TIM5_Int_Init(0xfff0-1,720-1);
	 
	 
	;

	 //TIM6_Int_Init(50000-1,72-1);
	 
//	  while(1)

	delay_ms(50);

	
	//printf("program running!\r\n");
	
	
	delay_ms(50);
	//Tim1_Init(servo_1_orign_position,servo_2_orign_position,2500,1350);//直立值	
	//Tim1_Init(servo_1_orign_position,servo_2_orign_position,2500,500);//直立值	2304 is no 3 
	Tim1_Init(servo_1_orign_position,servo_2_orign_position,servo_3_orign_position,500);//直立值	2304 is no 3 
	Tim2_Init(1600,1560,270,250);
	
	delay_ms(100);
	
	
//	uart2_init(115200);	 	//串口初始化为115200//6050
	
	
//	while((USART2->SR&0X40)==0);  value
//	USART2->DR = 0xa5; 
//	
//	while((USART2->SR&0X40)==0);  
//	USART2->DR = 0x52; 


//////////////////////////////////////////////
//
//适应场地 用如下函数
////////////////////////////////////////////
//	fastforward();
	

	//test_qti_1_2();

	//turn_left_90_continue();
//	car_right_turn();
	//car_left_turn();
	jj=0;
	

	
	
	sprintf(buf1,"[s10]求援小车准备就绪");
	
	
	delay_ms(1000);
	
	//test_check_color();
//	pick_up_a_ponit_1_top();
//	
//	sprintf(buf1,"[s8]开始测量颜色");
//	
//	
//	delay_ms(1000);
//	delay_ms(1000);
//	colourjudge(pcolor);
//	while(1);
/////////////////////////////////////////
//正式开始
//
////////////////////////////////////////
  while(1)
  {
    if(rx_end_flag_1)
    {
      rx_end_flag_1=0;
      
      if(rx_buff_1[1]==0x41)
      {
        //前进
       led0pwmval=1500+180;//1550;//high mean low speed
       TIM_SetCompare3(TIM3,led0pwmval);
        
       led0pwmval=1500-200;//1450;//high mean low speed
       TIM_SetCompare4(TIM3,led0pwmval);	

       }
       else if(rx_buff_1[1]==0x42)
       {
          //后退
          led0pwmval=1500-190;//1550;//high mean low speed
         TIM_SetCompare3(TIM3,led0pwmval);
         
         led0pwmval=1500+200;//1450;//high mean low speed
         TIM_SetCompare4(TIM3,led0pwmval);	

       }

        else if(rx_buff_1[1]==0x43)
       {
          //左转10度
          turn_left_10();	

       }
       
       else if(rx_buff_1[1]==0x44)
       {
          //右转10度
          turn_right_10();	

       }
       
        else if(rx_buff_1[1]==0x45)
       {
          //右转10度
          car_stop();	

       }
       
         else if(rx_buff_1[1]==0x46)
       {
          //打开爪子
          open_claw();	

       }
       
        else if(rx_buff_1[1]==0x47)
       {
          //闭合爪子
          close_claw();	

       }
       
        else if(rx_buff_1[1]==0x48)
       {
          //初始化爪子位置
          Tim1_Init(servo_1_orign_position,servo_2_orign_position,servo_3_orign_position,500);//直立值	
          Tim2_Init(1600,1560,270,250);

       }
	   
	   
	   else if(rx_buff_1[1]==0x4c)
       {
          //肘关节向下9度
          led0pwmval=TIM_GetCapture4(TIM1);
		   
		   led0pwmval=led0pwmval+50;
		   
		   TIM_SetCompare4(TIM1,led0pwmval);
		   
		   

       }
	   
	   else if(rx_buff_1[1]==0x4d)
       {
          //肘关节抬起9度
          led0pwmval=TIM_GetCapture4(TIM1);
		   
		   led0pwmval=led0pwmval-50;
		   
		   TIM_SetCompare4(TIM1,led0pwmval);
		   
		   

       }
       
        else if(rx_buff_1[1]==0x4b)
       {
          //放下爪子
          sprintf(buf1,"[s10]放下，4号夹");
          
          
          
          for(led0pwmval=500;led0pwmval<=1400;led0pwmval=led0pwmval+10)
            {
            TIM_SetCompare4(TIM1,led0pwmval);
            delay_ms(10);//10
            }
          
//          TIM_SetCompare4(TIM1,1104);
//			 TIM_SetCompare4(TIM1,1312);
          
//          delay_ms(1000);
//          delay_ms(1000);
         
          
          sprintf(buf1,"[s10]3号夹");
          
          
          //2号就位，3号舵机从初始位置（2500），改到1316
          for(led0pwmval=servo_3_orign_position;led0pwmval>=servo_3_final_position;led0pwmval=led0pwmval-10)
            {
            TIM_SetCompare3(TIM1,led0pwmval);     //2022524
            delay_ms(10);//30
            }
          
//          TIM_SetCompare3(TIM1,1024);
//          delay_ms(1000);
//          delay_ms(1000);
          
          
          sprintf(buf1,"[s10]5号夹");
          
          
          //3号就位，5号舵机从初始位置（2100），改到1500
          
          for(led0pwmval=1300;led0pwmval>=1500;led0pwmval=led0pwmval-10)
            {
            TIM_SetCompare1(TIM2,led0pwmval);
            delay_ms(10);
            }
          
//          TIM_SetCompare1(TIM2,1500);

       }
       
       else if(rx_buff_1[1]==0x4a)
       {
          //抬起爪子
         
         
         			
			sprintf(buf1,"[s10]放下，4号夹");
          
          
          
          //for(led0pwmval=1400;led0pwmval>=100;led0pwmval=led0pwmval-10)
		  for(led0pwmval=1400;led0pwmval>=500;led0pwmval=led0pwmval-10)
            {
            TIM_SetCompare4(TIM1,led0pwmval);
            delay_ms(15);
            }
          
//          TIM_SetCompare4(TIM1,1104);
	
            
						
          
          sprintf(buf1,"[s10]3号夹");
          
          
          //2号就位，3号舵机从初始位置（2500），改到1316
          for(led0pwmval=servo_3_final_position;led0pwmval<=servo_3_orign_position;led0pwmval=led0pwmval+10)
            {
            TIM_SetCompare3(TIM1,led0pwmval);
            delay_ms(15);
            }
			
			

            
          
          
//          sprintf(buf1,"[s10]5号夹");
//          
//          
//            
//          //3号就位，5号舵机从初始位置（2100），改到1500
//          
//          for(led0pwmval=2100;led0pwmval>=1500;led0pwmval=led0pwmval-10)
//            {
//            TIM_SetCompare1(TIM2,led0pwmval);
//            delay_ms(10);
//            }
//          
//          TIM_SetCompare1(TIM2,1984);

       }
      
    }
    delay_ms(20);
  }









//    //后退
//    led0pwmval=1500-50;//1550;//high mean low speed
//     TIM_SetCompare3(TIM3,led0pwmval);
//     
//     led0pwmval=1500+50;//1450;//high mean low speed
//     TIM_SetCompare4(TIM3,led0pwmval);	
    //前进
//    led0pwmval=1500+50;//1550;//high mean low speed
//     TIM_SetCompare3(TIM3,led0pwmval);
//      
//     led0pwmval=1500-50+5;//1450;//high mean low speed
//     TIM_SetCompare4(TIM3,led0pwmval);	

    //右转
//      led0pwmval=1540;//high mean low speed
//	
//		//李晨龙把c6，c7tim3 pwm1 pwm2 烧坏了，故此改为tim3 pwm3 pwm4 2018.10.22
//	
//	 TIM_SetCompare3(TIM3,led0pwmval);
//	 
//	//tim3 2# is left servo
//	 led0pwmval=1540;//high mean low speed
//	 TIM_SetCompare4(TIM3,led0pwmval);	
//	while(new_key_flag==0x00)
//	{
//	 
//	  ;
//	}
    
    
//    while(1)
//    {
////          turn_left_45();
////        turn_right_45();
////        turn_left_90();		
////        turn_right_90();
////        turn_left_135();		
////        turn_right_135();
////         turn_left_180();		
////        mpu_6050_check(0);
//        delay_ms_xm(2000);
//    }

	new_key_flag=0x00;
//	sprintf(buf1,"[s10]开始");
//	
//	
  
	//StartRun();//从起点开始流程	
	//start_run_new();
    
    
    
    ///////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    //
    //主程序开始
    ///////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    
    
    
    
    
//    count_point_a=0x03;
//    count_point_c=0x03;
//    count_point_e=0x03;

    count_stack_yellow=0x00;
    count_stack_white=0x00;
    count_stack_red=0x00;
    count_stack_black=0x00;
    count_stack_blue=0x00;


//    count_stack_tempa=0x00;
//    count_stack_tempb=0x00;
//    count_stack_tempc=0x00;
	
	//grasp_order_index=0x00;
    
	
	car_from_star_to_2();
    car_stop();
    car_stop();
    
    while(1)
    {
        
        if(1<=grasp_order[grasp_order_index]&&grasp_order[grasp_order_index]<=3)
        {
            car_from_2_to_ae();//
            car_stop();
            mpu_6050_check(180);
            car_stop();
            
            
            //car_point_a_pull_up_down();
            
                         //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///     原地左转90度，指向270度方向,抓取物体
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
             //  
            turn_left_90_with_axis_recover_6050_check(270.0);
             car_stop();
           
            
            front_dis= GetDis_front(front_echo,front_trig);
//            if(front_dis>=90)
//            {
//                //前进
//        
//               pwm_num=0;
//                 led0pwmval=1500+50;//1550;//high mean low speed
//                 TIM_SetCompare3(TIM3,led0pwmval);
//                 
//                 led0pwmval=1500-50+15;//1450;//high mean low speed
//                 TIM_SetCompare4(TIM3,led0pwmval);	
//                
//                tempa=front_dis-88;
//                while(pwm_num<=tempa);//6v 电压 
//                
//                car_stop();
//                car_stop();
//            }
//            else if(front_dis<=86)
//            {
//                //后退
//        
//               pwm_num=0;
//                 led0pwmval=1500-50;//1550;//high mean low speed
//                 TIM_SetCompare3(TIM3,led0pwmval);
//                 
//                 led0pwmval=1500+50-15;//1450;//high mean low speed
//                 TIM_SetCompare4(TIM3,led0pwmval);	
//                 tempa=88-front_dis;
//                while(pwm_num<=tempa);//6v 电压 
//                car_stop();
//            }
            
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
             car_stop();
             //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///     判断颜色，也可以直接访问颜色序列数组，我直接访问颜色序列数组
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////     
           
            if(yellow==block_colour_order[grasp_order_index])
            {
                car_a_point_yellow();
            }
            else if(white==block_colour_order[grasp_order_index])
            {
                 car_a_point_white();
            }
            else if(red==block_colour_order[grasp_order_index])
            {
                 car_a_point_red();
            }
            else if(black==block_colour_order[grasp_order_index])
            {
                car_a_point_black();
            }
            else if(blue==block_colour_order[grasp_order_index])
            {
                car_a_point_blue();
            }
        }
        else if(4<=grasp_order[grasp_order_index]&&grasp_order[grasp_order_index]<=6)
        {
//            car_point_b_pull_up_down();
            
            car_from_2_to_ae();//
            car_stop();
            mpu_6050_check(180);
            car_stop();
            
            //car_point_a_pull_up_down();
            
                         //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///     原地修整,准备抓取物体，
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
             //  
//            turn_left_90_with_axis_recover_6050_check(270.0);
            mpu_6050_check(180);
            
             car_stop();
           
            
            front_dis= GetDis_front(front_echo,front_trig);
//            if(front_dis>=90)
//            {
//                //前进
//        
//               pwm_num=0;
//                 led0pwmval=1500+50;//1550;//high mean low speed
//                 TIM_SetCompare3(TIM3,led0pwmval);
//                 
//                 led0pwmval=1500-50+15;//1450;//high mean low speed
//                 TIM_SetCompare4(TIM3,led0pwmval);	
//                
//                tempa=front_dis-88;
//                while(pwm_num<=tempa);//6v 电压 
//                
//                car_stop();
//                car_stop();
//            }
//            else if(front_dis<=86)
//            {
//                //后退
//        
//               pwm_num=0;
//                 led0pwmval=1500-50;//1550;//high mean low speed
//                 TIM_SetCompare3(TIM3,led0pwmval);
//                 
//                 led0pwmval=1500+50-15;//1450;//high mean low speed
//                 TIM_SetCompare4(TIM3,led0pwmval);	
//                 tempa=88-front_dis;
//                while(pwm_num<=tempa);//6v 电压 
//                car_stop();
//            }
            
            if(4==grasp_order[grasp_order_index])
            {
                car_pick_up_a_ponit_1_top();
            }
            
            else if(5==grasp_order[grasp_order_index])
            {
                
                car_pick_up_a_ponit_2_middle();
            }
            
            else if(6==grasp_order[grasp_order_index])
            {
                
                car_pick_up_a_ponit_3_bottle();
            }
             car_stop();
             //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///     判断颜色，也可以直接访问颜色序列数组，我直接访问颜色序列数组
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////     
           
            if(yellow==block_colour_order[grasp_order_index])
            {
                car_c_ponit_yellow();
            } 
       
            else if(white==block_colour_order[grasp_order_index])
            {
                car_c_point_white();
            }
            
            else if(red==block_colour_order[grasp_order_index])
            {
                 car_c_point_red();
            }
            
            else if(black==block_colour_order[grasp_order_index])
            {
                car_c_point_black();
            }
            
            else if(blue==block_colour_order[grasp_order_index])
            {
                car_c_point_blue();
            }
        }
        
        else if(7<=grasp_order[grasp_order_index]&&grasp_order[grasp_order_index]<=9)
        {
//            car_point_c_pull_up_down();
            
            //car_point_a_pull_up_down();
            car_from_2_to_ae();//
            car_stop();
            mpu_6050_check(180);
            car_stop();
            
                         //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///     原地左转90度，指向270度方向,抓取物体
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
             //  
//            turn_right_90_with_axis_recover_6050_check(270.0);
            turn_right_90();
            mpu_6050_check(90);
             car_stop();
           
            
            front_dis= GetDis_front(front_echo,front_trig);
//            if(front_dis>=90)
//            {
//                //前进
//        
//               pwm_num=0;
//                 led0pwmval=1500+50;//1550;//high mean low speed
//                 TIM_SetCompare3(TIM3,led0pwmval);
//                 
//                 led0pwmval=1500-50+15;//1450;//high mean low speed
//                 TIM_SetCompare4(TIM3,led0pwmval);	
//                
//                tempa=front_dis-88;
//                while(pwm_num<=tempa);//6v 电压 
//                
//                car_stop();
//                car_stop();
//            }
//            else if(front_dis<=86)
//            {
//                //后退
//        
//               pwm_num=0;
//                 led0pwmval=1500-50;//1550;//high mean low speed
//                 TIM_SetCompare3(TIM3,led0pwmval);
//                 
//                 led0pwmval=1500+50-15;//1450;//high mean low speed
//                 TIM_SetCompare4(TIM3,led0pwmval);	
//                 tempa=88-front_dis;
//                while(pwm_num<=tempa);//6v 电压 
//                car_stop();
//            }
            
            if(7==grasp_order[grasp_order_index])
            {
                car_pick_up_a_ponit_1_top();
            }
            
            else if(8==grasp_order[grasp_order_index])
            {
                
                car_pick_up_a_ponit_2_middle();
            }
            
            else if(9==grasp_order[grasp_order_index])
            {
                
                car_pick_up_a_ponit_3_bottle();
            }
             car_stop();
             //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///     判断颜色，也可以直接访问颜色序列数组，我直接访问颜色序列数组
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////     
           
            if(yellow==block_colour_order[grasp_order_index])
            {
                car_e_point_yellow();
            }
            else if(white==block_colour_order[grasp_order_index])
            {
                car_e_point_white();
            }
            else if(red==block_colour_order[grasp_order_index])
            {
                car_e_point_red();
            }
            else if(black==block_colour_order[grasp_order_index])
            {
                car_e_point_black();
            }
            else if(blue==block_colour_order[grasp_order_index])
            {
                car_e_point_blue();
            }
         }
            
	
	
	
        else if(grasp_order[grasp_order_index]>=10)
        {
              if(grasp_order[grasp_order_index]==10)
              {
                   car_from_2_to_ae();//
                   car_stop();
              
                    turn_left_180();    
                    
                    mpu_6050_check(0);
              
                    car_stop();
                  
                   while(1)
                  {
                      car_position_test_all_white();

                      car_judge_decision();
                      car_excute_decision();
                      
                      if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                      {
                          walk_ae_state=1;
                          
                          sprintf(buf1,"[s10]头黑");
                          
                          
                           car_stop();
                          break;
                          
                      }
                      
                      first_start_state_1=0x00;
                  }
                  grasp_order_index=10;
              }
               
            
          
            
            if (count_stack_tempa>=1)
            {
                mpu_6050_check(0);
              
                if(1==count_stack_tempa)
                {
                     car_pick_up_a_ponit_3_bottle();
                }
                
                else if(2==count_stack_tempa)
                {
                    
                    car_pick_up_a_ponit_2_middle();
                }
                
                else if(3==count_stack_tempa)
                {
                    car_pick_up_a_ponit_1_top();
                   
                }
                 car_stop();
                
                
                 turn_left_180_with_6050_check(180.0);
                
                  mpu_6050_white();
        
                 car_stop();
                   //////////////////////////////////////////////////////
        //////////////////////////////////////////////////////                    
        //////////////////////////////////////////////////////
        ///              直行10毫米，避开黑区
        /////////////////////////// /////////////////////////// 
        //////////////////////////////////////////////////////
        //////////////////////////////////////////////////////                    
                 pwm_num_distance=0;  
                
                while(1)
                {
                    car_position_test_all_white();
                    
                    
                    car_judge_decision();
                    car_excute_decision();
                    //直行10毫米，避开黑区
                    if(pwm_num_distance>=11)
                    {
                        walk_ae_state=2;
                        sprintf(buf1,"[s10]2号区域");
                        
                        
                        
                        break;
                        
                    }
                   
                }
                
                car_from_2_to_ae();//
                car_stop();
                mpu_6050_check(180);
                car_stop();
                
                
                
                
                
                if(colour_tempa==yellow)
                {
                    car_o_point_yellow();
                }
                else if(colour_tempa==white)
                {
                    car_o_point_white();
                }
                else if(colour_tempa==red)
                {
                    car_o_point_red();
                }
                else if(colour_tempa==black)
                {
                    car_o_point_black();
                }
                else if(colour_tempa==blue)
                {
                    car_o_point_blue();
                }
                count_stack_tempa--;
                grasp_order_index=10;
            }
            else if(count_stack_tempb>=1)
            {
                //2#区域开始，但是区域b，区域c车头对准o，也认为是2号区域
                turn_right_90();
                mpu_6050_check(270);
              
                 if(1==count_stack_tempb)
                {
                     car_pick_up_a_ponit_3_bottle();
                }
                
                else if(2==count_stack_tempb)
                {
                    
                    car_pick_up_a_ponit_2_middle();
                }
                
                else if(3==count_stack_tempb)
                {
                    car_pick_up_a_ponit_1_top();
                   
                }
                 car_stop();
                
                turn_right_90();
                mpu_6050_check(180);
                mpu_6050_white();
                
                
                while(1)
                {
                    
                     if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                    {
                        walk_ae_state=1;
                        
                        sprintf(buf1,"[s10]3");
                        
                        
                        car_stop();
                       
                        car_stop();
                        break;
                    }
                    car_position_test();
                    
                   
                    car_judge_decision();
                    car_excute_decision();
                    
                   
                    
                    first_start_state_1=0x00;
                        
                  
                }
                
                
                
                while(1)
                {

                    car_position_test();

                    car_judge_decision();
                    car_excute_decision();
                    
                    if(1==qti_5 && 1==qti_6 && 1==qti_7)
                    {
                        walk_ae_state=2;
                        sprintf(buf1,"[s10]4");
                        
                        
                        break;
                        
                    }

                    
                }
                
                
                
                if(colour_tempb==yellow)
                {
                    car_o_point_yellow();
                }
                else if(colour_tempb==white)
                {
                    car_o_point_white();
                }
                else if(colour_tempb==red)
                {
                    car_o_point_red();
                }
                else if(colour_tempb==black)
                {
                    car_o_point_black();
                }
                else if(colour_tempb==blue)
                {
                    car_o_point_blue();
                }
                count_stack_tempb--;
                grasp_order_index=10;
            }
            else if(count_stack_tempc>=1)
            {
                
                 //2#区域开始，但是区域b，区域c车头对准o，也认为是2号区域
                 turn_left_90();
                 mpu_6050_check(90);
                
                //准备抓取物体
                if(1==count_stack_tempc)
                {
                     car_pick_up_a_ponit_3_bottle();
                }
                
                else if(2==count_stack_tempc)
                {
                    
                    car_pick_up_a_ponit_2_middle();
                }
                
                else if(3==count_stack_tempc)
                {
                    car_pick_up_a_ponit_1_top();
                   
                }
                 car_stop();
                
                 turn_left_90();
                 mpu_6050_check(180);
                 mpu_6050_white();
                
                
                while(1)
                {
                    
                     if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                    {
                        walk_ae_state=1;
                        
                        sprintf(buf1,"[s10]3");
                        
                        
                        car_stop();
                       
                        car_stop();
                        break;
                    }
                    car_position_test();
                    
                   
                    car_judge_decision();
                    car_excute_decision();
                    
                   
                    
                    first_start_state_1=0x00;
                        
                  
                }
                
               
                
                
                
                while(1)
                {

                    car_position_test();

                    car_judge_decision();
                    car_excute_decision();
                    
                    if(1==qti_5 && 1==qti_6 && 1==qti_7)
                    {
                        walk_ae_state=2;
                        sprintf(buf1,"[s10]4");
                        
                        
                        car_stop();
                        break;
                        
                    }

                    
                }
                
                
                
                
                if(colour_tempc==yellow)
                {
                    car_o_point_yellow();
                }
                else if(colour_tempc==white)
                {
                    car_o_point_white();
                }
                else if(colour_tempc==red)
                {
                    car_o_point_red();
                }
                else if(colour_tempc==black)
                {
                    car_o_point_black();
                }
                else if(colour_tempc==blue)
                {
                    car_o_point_blue();
                }
                count_stack_tempc--;
                grasp_order_index=10;
            }
            else
            {
//                car_go_home();
                
                
                 mpu_6050_check(0);   
        
                 car_stop();
                   //////////////////////////////////////////////////////
        //////////////////////////////////////////////////////                    
        //////////////////////////////////////////////////////
        ///              直行10毫米，避开黑区
        /////////////////////////// /////////////////////////// 
        //////////////////////////////////////////////////////
        //////////////////////////////////////////////////////                    
                 pwm_num_distance=0;  
                
                while(1)
                {
                    car_position_test_all_white();
                    
                    
                    car_judge_decision();
                    car_excute_decision();
                    //直行10毫米，避开黑区
                    if(pwm_num_distance>=151)
                    {
                        walk_ae_state=2;
                        sprintf(buf1,"[s10]比赛结束");
                        
                        
                        car_stop();
                      delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                      
                      delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                      
                      delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                        delay_ms_xm(60000);//停机60秒
                      
                      
                        break;
                        
                    }
                   
                }
                
            }
        }
         
         
         
         //	sprintf(buf1,"[s10]程序写到这里，下面还没写，所以停车");
//	
//	
//	car_stop();
//	while(1);
	
	
 
    
    
//        while(1)
//        {
//            
//           
//        }
//以上未屏蔽的部分是完成动作的全部函数，屏蔽部分用于测试
    }





//搬运特定颜色的物料，整个流程的框架，都在此文件中，另外还包括了直接搬运E点最底下物料块到对应区域
//以ECarry开头的函数是专门用来搬运E点bottom的，5种颜色对应5个函数
}


