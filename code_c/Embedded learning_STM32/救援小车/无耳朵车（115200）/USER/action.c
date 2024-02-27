#include "stdio.h"
#include "action.h"
#include "stm32f10x.h"
#include "timer.h"
#include "delay.h"
#include "move.h"
#include "main.h"
#include <string.h>
#include "oled.h"
#include "led.h"
#include <math.h>
#include "DataScope_DP.h"



extern char forward_dirrection;
extern int turn_num;

extern int speak_len,speak_i,s_i;

extern int speak_len1;//(sizeof(buf_xm)/sizeof(buf_xm[0]));

extern char buf1[30];//\[b1] 欢迎光临，请进

extern int led0pwmval;
extern int pwm_num; 
extern int left_pwmval;
extern int right_pwmval;

extern unsigned char DataScope_OutPut_Buffer[42];	   //串口发送缓冲区


int front_dis=0x00;
int left_dis=0x00;
int right_dis=0x00;
int last_dis=0x00;
float error_dis=0x00;
float error_dir=0x00;
float kp_dis=5.0;
float kp_dir=5.0;
float kp=2.0;
float ki=20.0;
float sum_error=0x00;
float speed_left=0x00;
float speed_right=0x00;
int record_pwm_num=0x00;

float kp_dis_big=5.0;
float kp_dir_big=5.0;

float kp_dis_mid=3.0;
float kp_dir_mid=3.0;

float kp_dis_samll=1.8;
float kp_dir_small=1.8;

int Send_Count_tempa=0x00;


/* 超声波传感器测距采集      --------------------------------------------------------*/
int GetDis_left(int echo,int trig)
{
    int dis=0;
	int count;
	char time_i=0;
	//TIM_Cmd(TIM4,DISABLE); 
	Tim4_Init();
	GPIO_ResetBits(GPIOA,echo);
	for(time_i=1;time_i<2;time_i++)
	{
	
	
		GPIO_SetBits(GPIOA,trig);		          //trig置高 发出10us的高电平信号 
		delay_us(20); 
		GPIO_ResetBits(GPIOA,trig);  
				  
		while(GPIO_ReadInputDataBit(GPIOA, echo) == 0);

		TIM_SetCounter(TIM4,0)	;
		TIM_Cmd(TIM4,ENABLE);    //开启计数器
		count = TIM_GetCounter(TIM4);//获取计数器值
	       //开启定时器开始计时
		while(GPIO_ReadInputDataBit(GPIOA, echo));   //等待echo置低
		TIM_Cmd(TIM4,DISABLE);   //关闭计数器
	
		count = TIM_GetCounter(TIM4);//获取计数器值
		//dis = count/60.034   ;//转换为距离,精确度为cm		  // 若dis=count/60.034	,dis的精确度为cm
		dis = dis+count/6.0034   ;//转换为距离,精确度为mm	
		delay_ms(30);	 


	}
	 	dis=dis/1;
		return dis;
}

int GetDis_right(int echo,int trig)
{
    int dis=0;
	int count;
	char time_i=0;
	//TIM_Cmd(TIM4,DISABLE); 
	Tim4_Init();
	GPIO_ResetBits(GPIOA,echo);
	for(time_i=1;time_i<2;time_i++)
	{
	
	
		GPIO_SetBits(GPIOD,trig);		          //trig置高 发出10us的高电平信号 
		delay_us(20); 
		GPIO_ResetBits(GPIOD,trig);  
				  
		while(GPIO_ReadInputDataBit(GPIOA, echo) == 0);

		TIM_SetCounter(TIM4,0)	;
		TIM_Cmd(TIM4,ENABLE);    //开启计数器
		count = TIM_GetCounter(TIM4);//获取计数器值
	       //开启定时器开始计时
		while(GPIO_ReadInputDataBit(GPIOA, echo));   //等待echo置低
		TIM_Cmd(TIM4,DISABLE);   //关闭计数器
	
		count = TIM_GetCounter(TIM4);//获取计数器值
		//dis = count/60.034   ;//转换为距离,精确度为cm		  // 若dis=count/60.034	,dis的精确度为cm
		dis = dis+count/6.0034   ;//转换为距离,精确度为mm	
		delay_ms(30);	 


	}
	 	dis=dis/1;
		return dis;
}

int GetDis_front(int echo,int trig)
{
    int dis=0;
	int count;
	char time_i=0;
	//TIM_Cmd(TIM4,DISABLE); 
	Tim4_Init();
	GPIO_ResetBits(GPIOA,echo);
	for(time_i=1;time_i<2;time_i++)
	{
	
	
		GPIO_SetBits(GPIOA,trig);		          //trig置高 发出10us的高电平信号 
		delay_us(20); 
		GPIO_ResetBits(GPIOA,trig);  
				  
		while(GPIO_ReadInputDataBit(GPIOA, echo) == 0);

		TIM_SetCounter(TIM4,0)	;
		TIM_Cmd(TIM4,ENABLE);    //开启计数器
		count = TIM_GetCounter(TIM4);//获取计数器值
	       //开启定时器开始计时
		while(GPIO_ReadInputDataBit(GPIOA, echo));   //等待echo置低
		TIM_Cmd(TIM4,DISABLE);   //关闭计数器
	
		count = TIM_GetCounter(TIM4);//获取计数器值
		//dis = count/60.034   ;//转换为距离,精确度为cm		  // 若dis=count/60.034	,dis的精确度为cm
		dis = dis+count/6.0034   ;//转换为距离,精确度为mm	
		delay_ms(30);	 


	}
	 	dis=dis/1;
		return dis;
}

/* 三个超声波测距采集      --------------------------------------------------------*/
void scanValue(void)       //采集三个超声波传感器的数据
{   
//	left_value=GetDis(GPIOA,GPIO_Pin_1,GPIO_Pin_0);	
//	front_value=GetDis(GPIOA,GPIO_Pin_3,GPIO_Pin_2);	   
//	right_value=GetDis(GPIOC,GPIO_Pin_7,GPIO_Pin_6);

	 	left_value=GetDis_left(left_echo,left_trig);
	//	printf("   left=%d",right_value )	;
		//delay_ms(200);


		front_value=GetDis_right(front_echo,front_trig);
//	 	printf("    front=  %d",front_value )	;
		//delay_ms(200);

		right_value=GetDis_left(right_echo,right_trig);


	//	printf("    right=  %d\n",right_value )	;
		 
}

/* 循左墙      --------------------------------------------------------*/
void walkalone_left(void)
{	
	
	last_value=left_value;									 //记录上次测量值
															 
	//left_value=GetDis(GPIOA,GPIO_Pin_1,GPIO_Pin_0);			 //记录当前测量值
	  scanValue();
	if(left_value>last_value)//机器人偏离左墙前进	   
	{
		if(left_value>190)
		{
			if(last_value>190)
				{
					 
					 LED_L(ON);
					 LED_R(OFF);
					 LED_F(OFF);

					 if(forward_dirrection==left)
					 {
					 	turn_num++;
					 }
					 else
					 {
					 	 forward_dirrection=left;
						 turn_num=0;
					 }

					 //adjustDirection(60, 200);	//	   左偏一大步
					 //adjustDirLeft(left_speed_adjust-5, 200);	//	 
					 //adjustDirLeftBig(left_speed_adjust-5, 200);
					 adjustLeftBig();
					    					 
				}
			else
				{
					 LED_L(ON);
					 LED_R(OFF);
					 LED_F(OFF);
					  if(forward_dirrection==left)
					 {
					 	turn_num++;
					 }
					 else
					 {
					 	 forward_dirrection=left;
						 turn_num=0;
					 }
					//adjustDirection(85, 200);   // 左偏一小步
					adjustDirLeft(left_speed_adjust, 200);	//	   左偏一小步 
					 		  	  	
				}
		}
		else
		{
					 LED_L(OFF);
					 LED_R(OFF);
					 LED_F(ON);
					  if(forward_dirrection==forward)
					 {
					 	turn_num=0;
					 }
					 else
					 {
					 	 forward_dirrection=forward;
						 turn_num=0;
					 }
					//adjustDirection(200, 200);	  //沿墙直走（参数需要考量，电机速度有可能不一致）
					forwardOneStep();
		}
	}
	else if (left_value<last_value)		 //机器人靠近左墙前进
	{
		if(left_value<190)
		{
			if(last_value>190)
			{
					 LED_L(OFF);
					 LED_R(ON);
					 LED_F(OFF);
					 if(forward_dirrection==right)
					 {
					 	turn_num++;
					 }
					 else
					 {
					 	 forward_dirrection=right;
						 turn_num=0;
					 }
					//adjustDirection(200, 85);	   //		   右偏一小步			
					adjustDirRight(200, right_speed_adjust);	   //		   右偏一小步	
			}
			else  
			{		LED_L(OFF);
					 LED_R(ON);
					 LED_F(OFF);
					 if(forward_dirrection==right)
					 {
					 	turn_num++;
					 }
					 else
					 {
					 	 forward_dirrection=right;
						 turn_num=0;
					 }
					//adjustDirection(200,65);			  //		   右偏一大步
					//adjustDirRight(200, right_speed_adjust-5);	   //		   右偏一小步		  		
					//adjustDirRightBig(200, right_speed_adjust-5);
					 adjustRightBig();
			}
		}
		else
		{
					LED_L(OFF);
					 LED_R(OFF);
					 LED_F(ON);
					 if(forward_dirrection==forward)
					 {
					 	turn_num=0;
					 }
					 else
					 {
					 	 forward_dirrection=forward;
						 turn_num=0;
					 }
						//adjustDirection(200, 200);	  //沿墙直走（参数需要考量，电机速度有可能不一致）
					forwardOneStep();			
		}	
	}
	else
	{
		 if(last_value > 190)
		 {				LED_L(ON);
					 	LED_R(OFF);
					 	LED_F(OFF);
						if(forward_dirrection==left)
						 {
						 	turn_num++;
						 }
						 else
						 {
						 	 forward_dirrection=left;
							 turn_num=0;
						 }
						adjustDirLeft(left_speed_adjust, 200);	//	   左偏一小步 						 
					
		 }
		 else if(last_value < 190)
		 {
						LED_L(OFF);
						LED_R(ON);
					 	LED_F(OFF);
						if(forward_dirrection==right)
						 {
						 	turn_num++;
						 }
						 else
						 {
						 	 forward_dirrection=right;
							 turn_num=0;
						 }
						adjustDirRight(200, right_speed_adjust);	   //		   右偏一小步				 

		 }
		 else
		 {
				     LED_L(OFF);
					 LED_R(OFF);
					 LED_F(ON);
					 if(forward_dirrection==forward)
					 {
					 	turn_num=0;
					 }
					 else
					 {
					 	 forward_dirrection=forward;
						 turn_num=0;
					 }
				//adjustDirection(200, 200);	  //沿墙直走（参数需要考量，电机速度有可能不一致）
					forwardOneStep();	
		 }			
	}
}




/////////////
//
/////////////////
void walkalone_right(void)
{	
	last_value=right_value;
	//right_value=GetDis(GPIOC,GPIO_Pin_7,GPIO_Pin_6);
	scanValue();
	if(right_value>last_value)//机器人偏离右墙前进	   
	{
		//forward_dirrection=0;
		if(right_value>190)
		{
			if(last_value>190)
				{
					 LED_L(OFF);
					 LED_R(ON);
					 LED_F(OFF);
					 if(forward_dirrection==right)
					 {
					 	turn_num++;
					 }
					 else
					 {
					 	 forward_dirrection=right;
						 turn_num=0;
					 }
					//adjustDirection(200, 85);  //	右偏一大步
				//adjustDirRight(200, right_speed_adjust-5);  //	右偏一大步	
				//adjustDirRightBig(200, right_speed_adjust-5); 							 

				adjustRightBig();
				}
			else
				{
					LED_L(OFF);
					 LED_R(ON);
					 LED_F(OFF);
					 if(forward_dirrection==right)
					 {
					 	turn_num++;
					 }
					 else
					 {
					 	 forward_dirrection=right;
						 turn_num=0;
					 }
					 //adjustDirection(200, 65);   // 右偏一小步
					adjustDirRight(200, right_speed_adjust);  //	右偏一小步
					 							 
				}
		}
		else
		{
					
       				LED_L(OFF);
					 LED_R(OFF);
					 LED_F(ON);
					 if(forward_dirrection==forward)
					 {
					 	turn_num=0;
					 }
					 else
					 {
					 	 forward_dirrection=forward;
						 turn_num=0;
					 }
					 forwardOneStep();			//沿墙直走（参数需要考量，电机速度有可能不一致）
					 turn_num--;
					 

			
		}
	}
	else if (right_value<last_value)		 //机器人靠近右墙前进
	{
	  //forward_dirrection=1;
		if(right_value<190)
		{
			if(last_value>190)
			{
					LED_L(ON);
					 LED_R(OFF);
					 LED_F(OFF);
					 	if(forward_dirrection==left)
						 {
						 	turn_num++;
						 }
						 else
						 {
						 	 forward_dirrection=left;
							 turn_num=0;
						 }
					 adjustDirLeft(left_speed_adjust, 200);	//	   左偏一小步 
									   
			}
			else  
			{		LED_L(ON);
					 LED_R(OFF);
					 LED_F(OFF);
					 	if(forward_dirrection==left)
						 {
						 	turn_num++;
						 }
						 else
						 {
						 	 forward_dirrection=left;
							 turn_num=0;
						 }										   
					//adjustDirLeft(left_speed_adjust-5, 200);	//	// 	左偏一大步
					//adjustDirLeftBig(left_speed_adjust-5, 200);
					adjustLeftBig();
			}
		}
		else
		{			
					LED_L(OFF);
					 LED_R(OFF);
					 LED_F(ON);
					if(forward_dirrection==forward)
					 {
					 	turn_num=0;
					 }
					 else
					 {
					 	 forward_dirrection=forward;
						 turn_num=0;
					 }
					forwardOneStep();			//沿墙直走（参数需要考量，电机速度有可能不一致）		   
					turn_num++;
					
		}	
	}
	else
	{
		 if(last_value > 190)
		 {
					LED_L(OFF);
					 LED_R(ON);
					 LED_F(OFF);
					 if(forward_dirrection==right)
					 {
					 	turn_num++;
					 }
					 else
					 {
					 	 forward_dirrection=right;
						 turn_num=0;
					 }
					 //adjustDirection(200,65);		 //		右偏一大步	
					adjustDirRight(200, right_speed_adjust);;  //	右偏一大步	
					turn_num++;	
							   
		 }
		 else if(last_value < 190)
		 {
					LED_L(ON);
					 LED_R(OFF);
					 LED_F(OFF);
					 if(forward_dirrection==left)
						 {
						 	turn_num++;
						 }
						 else
						 {
						 	 forward_dirrection=left;
							 turn_num=0;
						 }
					 //adjustDirection(85,200);		 //  左偏一大步	
					adjustDirLeft(left_speed_adjust, 200);	//	// 	左偏一大步
					
					 			   
		 }
		 else
		 {
						LED_L(OFF);
					 LED_R(OFF);
					 LED_F(ON);
					 if(forward_dirrection==forward)
					 {
					 	turn_num=0;
					 }
					 else
					 {
					 	 forward_dirrection=forward;
						 turn_num=0;
					 }
					 forwardOneStep();		 		//沿墙直走（参数需要考量，电机速度有可能不一致）
				
		 }			
	}
}






//////////////////
//
//
///////////////////////////
void walkalone_right_1(void)
{	
	
	 last_dis=right_dis;
	 front_dis= GetDis_front(front_echo,front_trig)-15;
//						 sprintf(buf1,"     ");
//						 OLED_P8x16Str(49, 0, (unsigned char*)buf1, 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/
//						 sprintf(buf1,"%d",front_dis);
//						 OLED_P8x16Str(49, 0, (unsigned char*)buf1, 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/
	

	
	
	 right_dis= GetDis_right(right_echo,right_trig);
//						 sprintf(buf1,"     ");
//						 OLED_P8x16Str(49, 4, (unsigned char*)buf1, 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/
//						  sprintf(buf1,"%d",right_dis);
//						 OLED_P8x16Str(49, 4, (unsigned char*)buf1, 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/	
	
	 if(right_dis > 320)//右边不靠墙时扫描左边与墙的距离
	 {
		 left_dis= GetDis_left(left_echo,left_trig);
		  forwardOneStep();
		  forwardOneStep();
		  forwardOneStep();
		  forwardOneStep();
		  forwardOneStep();
		 
		  speed_right=1300;
		 speed_left=1700;
		 sprintf(buf1,"直行");
		speak_len1=strlen(( const char *)buf1);
		speak_context((u8*)buf1,speak_len1);	
	 }
	
	
	 else if(right_dis >=170)//turn right
	 {
		 error_dis=(right_dis-170)*kp_dis;
		 
		 if(right_dis-170>50)
		 {
/////////////////////////////
//小车离中心线大于50mm，只有位置环pid，没有方向环pid
/////////////////////////			 
			 kp_dis=kp_dis_samll;
			 kp_dir=kp_dir_small;
			 error_dir=0.0;
			 speed_right=1300+error_dis;
			  if(speed_right>=1500)
			 {
				 speed_right=1450;
			 }
			 speed_left=1700;
			 adjustDirection_1(1700,(int)speed_right);
			 sprintf(buf1,"右转");
			speak_len1=strlen(( const char *)buf1);
			speak_context((u8*)buf1,speak_len1);	
			 
		 }
		 else
		 {
/////////////////////////////
//小车离中心线小于50mm，大于30mm，有位置环pid，有方向环pid
/////////////////////////			 
			  error_dis=(right_dis-170)*kp_dis;
			 
			 if(right_dis-170>30)
			 {
				   kp_dis=kp_dis_mid;
				   kp_dir=kp_dir_mid;
				  
				  error_dis=(right_dis-170)*kp_dis;
				  error_dir=(last_dis-right_dis)*kp_dir;
				 if((last_dis-right_dis)>0)
				 {
					 //靠近中心线
					 if(error_dir>=0.9*error_dis)
					 {
						 error_dir=0.9*error_dis;
					 }
					 
					 
					 
					 
					  speed_right=1300+error_dis-error_dir;
					  if(speed_right>=1500)
					 {
						 speed_right=1450;
					 }
					 speed_left=1700;
					 adjustDirection_1(1700,(int)speed_right);
				 }
				 else
				 {
					 //远离中心线
					 speed_right=1300+error_dis-error_dir;
					 if(speed_right>=1500)
					 {
						 speed_right=1450;
					 }
					 speed_left=1700;
					 adjustDirection_1(1700,(int)speed_right);
				 }
				  sprintf(buf1,"右转");
				speak_len1=strlen(( const char *)buf1);
				speak_context((u8*)buf1,speak_len1);	
			 }
/////////////////////////////
//小车离中心线小于30mm，大于10mm，有位置环pid，有方向环pid
/////////////////////////	
			 else
			 {
	
				  kp_dis=kp_dis_big;
				  kp_dir=kp_dir_big;
				  error_dis=(right_dis-170)*kp_dis;
				  error_dir=(last_dis-right_dis)*kp_dir;
				 
					if((last_dis-right_dis)>0)
					 {
						 //靠近中心线
//						 if(error_dir>=0.9*error_dis)
//						 {
//							 error_dir=0.9*error_dis;
//						 }
//						 
//						 
//						 
//						 
//						  speed_right=1300+error_dis-error_dir;
//						  if(speed_right>=1500)
//						 {
//							 speed_right=1450;
//						 }
//						 adjustDirection_1(1700,(int)speed_right);
//						  sprintf(buf1,"右转");
//						speak_len1=strlen(( const char *)buf1);
//						speak_context((u8*)buf1,speak_len1);
						//右转是大环境，这里离目标线很近，需要调整车头方向为左拐，防止超调 
						 speed_left=1620-error_dir;
						 if(speed_left<=1500)
						 {
							 speed_left=1550;
						 }
						  speed_right=1300;
						 adjustDirection_1((int)speed_left,1300);
						  sprintf(buf1,"L转");
						speak_len1=strlen(( const char *)buf1);
						speak_context((u8*)buf1,speak_len1);
					 
					 
					 }
					 else
					 {
						 //远离中心线
						 	 
						 speed_right=1380+error_dis-error_dir;
						 if(speed_right>=1500)
						 {
							 speed_right=1450;
						 }
						  speed_left=1700;
						 adjustDirection_1(1700,(int)speed_right);
						  sprintf(buf1,"右转");
						speak_len1=strlen(( const char *)buf1);
						speak_context((u8*)buf1,speak_len1);
					
					 }


			 }
 
		 }
		 
//		 sprintf(buf1,"F=%d,R=%d,s_l=1700,s_r=%d\n",front_dis,right_dis,(int)speed_right);
//		speak_len1=strlen(( const char *)buf1);
//		for( s_i = 0 ; s_i < speak_len1; s_i++) 
//		{
//	//		while((USART3->SR&0X40)==0);  
//	//		USART3->DR = DataScope_OutPut_Buffer2[s_i]; 
//			while((USART1->SR&0X40)==0);  
//			USART1->DR = buf1[s_i];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
//		}
		 
						
		
			
	 }
/////////////////////////////
//小车离墙距离小于172mm
/////////////////////////	 
	 
	 else//turn left
	 {
		 error_dis=(170-right_dis)*kp_dis;
 
		 if(170-right_dis>50)
		 {
/////////////////////////////
//小车离中心线大于50mm，只有位置环pid，没有方向环pid
/////////////////////////		
			 kp_dis=kp_dis_samll;
			 kp_dir=kp_dir_small;
			  error_dis=(170-right_dis)*kp_dis;
			 error_dir=0.0;
			 speed_left=1700-error_dis;
			  if(speed_left<=1500)
			 {
				 speed_left=1550;
			 }
			 
			  speed_right=1300;
					
			 adjustDirection_1((int)speed_left,1300);
			 sprintf(buf1,"L转");
			speak_len1=strlen(( const char *)buf1);
			speak_context((u8*)buf1,speak_len1);
			 
		 }
		 else
		 {
/////////////////////////////
//小车离中心线小于50mm，大于30mm，有位置环pid，有方向环pid
/////////////////////////				
			  if(170-right_dis>30)
			 {
				   kp_dis=kp_dis_mid;
				   kp_dir=kp_dir_mid;
				  error_dis=(170-right_dis)*kp_dis;
				  error_dir=(last_dis-right_dis)*kp_dir;
				 if((last_dis-right_dis)>0)
				 {
					 //远近中心线
					 
					 kp_dis=kp_dis_big;
					 kp_dir=kp_dir_big;
					  speed_left=1700-error_dis-error_dir;
					 if(speed_left<=1500)
					 {
						 speed_left=1550;
					 }
					  speed_right=1300;
					 adjustDirection_1((int)speed_left,1300);
				 }
				 else
				 {
					 //靠近中心线
					 if(-(error_dir)>=0.9*error_dis)
					 {
						 error_dir=-0.9*error_dis;
					 }
					 speed_left=1700-error_dis-error_dir;
					 if(speed_left<=1500)
					 {
						 speed_left=1550;
					 }
					  speed_right=1300;
					 adjustDirection_1((int)speed_left,1300);
				 }
				 
				  sprintf(buf1,"L转");
				speak_len1=strlen(( const char *)buf1);
				speak_context((u8*)buf1,speak_len1);
			 }
 /////////////////////////////
//小车离中心线小于30mm，大于10mm，有位置环pid，有方向环pid
/////////////////////////
			 else
			 {
				 kp_dis=kp_dis_big;
				  kp_dir=kp_dir_big;
				  error_dis=(170-right_dis)*kp_dis;
				 error_dir=(last_dis-right_dis)*kp_dir;
				 

				 
				
					if((last_dis-right_dis)>0)
					 {
						 //远近中心线
						 
						 kp_dis=kp_dis_big;
						 kp_dir=kp_dir_big;
						  speed_left=1620-error_dis-error_dir;
						 if(speed_left<=1500)
						 {
							 speed_left=1550;
						 }
						  speed_right=1300;
						 adjustDirection_1((int)speed_left,1300);
						  sprintf(buf1,"L转");
						speak_len1=strlen(( const char *)buf1);
						speak_context((u8*)buf1,speak_len1);
					 }
					 else
					 {
						 //靠近中心线
						 
						 //靠近中心线
						 //左转是大环境，这里离目标线很近，需要调整车头方向为右拐，防止超调
//						 if(-(error_dir)>=0.9*error_dis)
//						 {
//							 error_dir=-0.9*error_dis;
//						 }
//						 speed_left=1700-error_dis-error_dir;
//						 if(speed_left<=1500)
//						 {
//							 speed_left=1550;
//						 }
//						 adjustDirection_1((int)speed_left,1300);
						  speed_right=1380-error_dir;
						  if(speed_right>=1500)
						 {
							 speed_right=1450;
						 }
						  speed_right=1300;
						 adjustDirection_1(1700,(int)speed_right);
						 
						 sprintf(buf1,"右转");
						speak_len1=strlen(( const char *)buf1);
						speak_context((u8*)buf1,speak_len1);
					 }
					 
					 
				
			 }
			 
			 
			 
			
				 
		 }
//		  sprintf(buf1,"F=%d,R=%d,s_l=%d,s_r=1300,\n",front_dis,right_dis,(int)speed_left);
//		speak_len1=strlen(( const char *)buf1);
//		for( s_i = 0 ; s_i < speak_len1; s_i++) 
//		{
//	//		while((USART3->SR&0X40)==0);  
//	//		USART3->DR = DataScope_OutPut_Buffer2[s_i]; 
//			while((USART1->SR&0X40)==0);  
//			USART1->DR = buf1[s_i];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
//		}
						
		
	 }
	 
//	target=170.0;
//	DataScope_Get_Channel_Data((float)target, 1 );
//	DataScope_Get_Channel_Data((float)right_dis, 2 );
//	DataScope_Get_Channel_Data((float)speed_left/10.0, 3 );
//	DataScope_Get_Channel_Data((float)speed_right/10.0, 4 );
//	DataScope_Get_Channel_Data((float)error_dis/10.0, 5 );
//	DataScope_Get_Channel_Data((float)error_dir/10.0, 6 );
	 
//	 	 target=170.0;
	 
//	DataScope_Get_Channel_Data((float)target, 1 );
	DataScope_Get_Channel_Data((float)right_dis, 1 );
	DataScope_Get_Channel_Data((float)speed_left/10.0, 2 );
	DataScope_Get_Channel_Data((float)speed_right/10.0, 3 );
	DataScope_Get_Channel_Data((float)error_dis, 4 );
	DataScope_Get_Channel_Data((float)error_dir, 5 );


	Send_Count_tempa = DataScope_Data_Generate(4);
	for( s_i = 0 ; s_i < Send_Count_tempa; s_i++) 
	{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer[s_i]; 
	}
	 
	 
}

///
//////////////////
//
//
///////////////////////////
//void walkalone_right_2(void)
//{	
//	
//	
//	 front_dis= GetDis_front(front_echo,front_trig)-30;
////						 sprintf(buf1,"     ");
////						 OLED_P8x16Str(49, 0, (unsigned char*)buf1, 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/
////						 sprintf(buf1,"%d",front_dis);
////						 OLED_P8x16Str(49, 0, (unsigned char*)buf1, 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/
//	

//	
//	
//	 right_dis= GetDis_right(right_echo,right_trig);
////						 sprintf(buf1,"     ");
////						 OLED_P8x16Str(49, 4, (unsigned char*)buf1, 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/
////						  sprintf(buf1,"%d",right_dis);
////						 OLED_P8x16Str(49, 4, (unsigned char*)buf1, 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/	
//	
//	sprintf(buf1,"F=%d,R=%d\n",front_dis,right_dis);
//	speak_len1=strlen(( const char *)buf1);
//	for( s_i = 0 ; s_i < speak_len1; s_i++) 
//	{
////		while((USART3->SR&0X40)==0);  
////		USART3->DR = DataScope_OutPut_Buffer2[s_i]; 
//		while((USART1->SR&0X40)==0);  
//		USART1->DR = buf1[s_i]; 
//	}
//	
//	
//	 if(right_dis >=172&&right_dis < 450)//turn right
//	 {
//		 error=right_dis-172;
//		 speed_right=200-(error*kp*0.8);
//		 if(speed_right<0)
//		 {
//			 speed_right=0x00;
//		 }
//			 
//		 if(speed_right>1500)
//		 {
//			 speed_right=1500;
//		 }
//		 
//		 sum_error=sum_error+error;
//		 
//		 adjustDirection(200,(int)speed_right);
//				
//		sprintf(buf1,"右转");
//		speak_len1=strlen(( const char *)buf1);
//		speak_context((u8*)buf1,speak_len1);	
//			
//	 }
//	 
//	 else//turn left
//	 {
//		 error=172-right_dis;
//		 speed_left=200-(error*kp*0.8);
//		 if(speed_left<0)
//		 {
//			 speed_left=0x00;
//		 }
//			 
//		  if(speed_left>1500)
//		 {
//			 speed_left=1500;
//		 }
//		 
//		 sum_error=sum_error+error;
//		 
//		 adjustDirection((int)speed_left,200);
//				
//		sprintf(buf1,"L转");
//		speak_len1=strlen(( const char *)buf1);
//		speak_context((u8*)buf1,speak_len1);
//	 }
//	 
//	 if(right_dis > 450)//右边不靠墙时扫描左边与墙的距离
//	 {
//		 right_pwmval=1400;//high mean low speed
//		 TIM_SetCompare1(TIM3,right_pwmval);
//		 
//		 left_pwmval=1600;//high mean low speed
//		 TIM_SetCompare2(TIM3,left_pwmval);	
//		
//		pwm_num=0x00;
//		 sprintf(buf1,"直行");
//		speak_len1=strlen(( const char *)buf1);
//		speak_context((u8*)buf1,speak_len1);	
//	 }
//	 
//	 
//}


//////////////////////
//////////
void walkalone_left_1(void)
{	
	 last_dis=left_dis;
	 front_dis= GetDis_front(front_echo,front_trig)-30;
	
	 left_dis= GetDis_left(left_echo,left_trig);
	
	
	if(left_dis > 320)//右边不靠墙时扫描左边与墙的距离
	 {
		 left_dis= GetDis_left(left_echo,left_trig);
		  forwardOneStep();
		  forwardOneStep();
		  forwardOneStep();
		  forwardOneStep();
		  forwardOneStep(); 
		 speed_right=1300;
		 speed_left=1700;
		 sprintf(buf1,"直行");
		
		speak_len1=strlen(( const char *)buf1);
		speak_context((u8*)buf1,speak_len1);	
	 }
/////////////////////////////
//小车离墙距离小于172mm
/////////////////////////	
	  else if(left_dis <=170)//turn right
	 {
		 error_dis=(170-left_dis)*kp_dis;
/////////////////////////////
//小车离中心线大于50mm，只有位置环pid，没有方向环pid
/////////////////////////			 
		 if(170-left_dis>50)
		 {
			 kp_dis=kp_dis_samll;
			 kp_dir=kp_dir_small;
			 error_dir=0.0;
			  error_dis=(170-left_dis)*kp_dis;
			 speed_right=1300+error_dis;
			  if(speed_right>=1500)
				 {
					 speed_right=1450;
					
				 }
		    speed_left=1700;
			adjustDirection_1(1700,(int)speed_right);
			sprintf(buf1,"右转");
			speak_len1=strlen(( const char *)buf1);
			speak_context((u8*)buf1,speak_len1);	
			 
		 }
		 else
		 {
/////////////////////////////
//小车离中心线小于50mm，大于30mm，有位置环pid，有方向环pid
/////////////////////////	
			 if(170-left_dis>30)
			 {
				 kp_dis=kp_dis_mid;
				 kp_dir=kp_dir_mid;
				  error_dis=(170-left_dis)*kp_dis;
				 error_dir=(last_dis-left_dis)*kp_dir;
				 if((last_dis-left_dis)>0)
				 {
					 //远离中心线
					speed_right=1300+error_dis+error_dir;
					  if(speed_right>=1500)
					 {
						 speed_right=1450;
						  
					 }
					 speed_left=1700;
					 adjustDirection_1(1700,(int)speed_right);
				 }
				 else
				 {
					 //靠近中心线
					  if(-(error_dir)>=0.9*error_dis)
					 {
						 error_dir=-0.9*error_dis;
					 }

					  speed_right=1300+error_dis+error_dir;
					  if(speed_right>=1500)
					 {
						 speed_right=1450;
						
					 }
					   speed_left=1700;
					 adjustDirection_1(speed_left,(int)speed_right);
				 }
				 sprintf(buf1,"右转");
				speak_len1=strlen(( const char *)buf1);
				speak_context((u8*)buf1,speak_len1);	
			 }
			 else
			 {
/////////////////////////////
//小车离中心线小于30mm，有位置环pid，有方向环pid
/////////////////////////					 
				 
					  kp_dis=kp_dis_big;
					  kp_dir=kp_dir_big;
					  error_dis=(170-left_dis)*kp_dis;
					  error_dir=(last_dis-left_dis)*kp_dir;
					 if((last_dis-left_dis)>0)
					 {
						 //远离中心线
						speed_right=1350+error_dis+error_dir;
						  if(speed_right>=1500)
						 {
							 speed_right=1450;
						 }
						  speed_left=1700;
						 adjustDirection_1(1700,(int)speed_right);
						  sprintf(buf1,"右转");
						speak_len1=strlen(( const char *)buf1);
						speak_context((u8*)buf1,speak_len1);	
					 }
					 else
					 {
						 //靠近中心线
	//					  if(-(error_dir)>=0.9*error_dis)
	//					 {
	//						 error_dir=-0.9*error_dis;
	//					 }
	//					  speed_right=1300+error_dis+error_dir;
	//					  if(speed_right>=1500)
	//					 {
	//						 speed_right=1450;
	//					 }
	//					 adjustDirection_1(1700,(int)speed_right);
						 //转中的转
						  //右转是大环境，这里离目标线很近，需要调整车头左拐，防止超调
						speed_left=1650+error_dir;
						  if(speed_left<=1500)
						 {
							 speed_left=1550;
						 }
						 speed_right=1300;
						 adjustDirection_1((int)speed_left,1300);
						 sprintf(buf1,"L转");
						speak_len1=strlen(( const char *)buf1);
						speak_context((u8*)buf1,speak_len1);	
						 
						 
					 }
				 
				 
				
			 }
			
				 
		 }
		 
//		 sprintf(buf1,"F=%d,L=%d,s_l=1700,s_r=%d\n",front_dis,left_dis,(int)speed_right);
//		speak_len1=strlen(( const char *)buf1);
//		for( s_i = 0 ; s_i < speak_len1; s_i++) 
//		{
//	//		while((USART3->SR&0X40)==0);  
//	//		USART3->DR = DataScope_OutPut_Buffer2[s_i]; 
//			while((USART1->SR&0X40)==0);  
//			USART1->DR = buf1[s_i];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
//		}

			
	 }
/////////////////////////////
//小车离墙距离大于172mm
/////////////////////////	 
	 else//turn left
	 {
		 error_dis=(left_dis-170)*kp_dis;
/////////////////////////////
//小车离中心线大于50mm，只有位置环pid，没有方向环pid
/////////////////////////		 
		 if(left_dis-170>50)
		 {
			 kp_dis=kp_dis_samll;
			 kp_dir=kp_dir_small;
			 error_dis=(left_dis-170)*kp_dis;
			 error_dir=0.0;
			 speed_left=1700-error_dis;
			 if(speed_left<=1500)
			 {
				 speed_left=1550;
			 }
			  speed_right=1300;
			 adjustDirection_1((int)speed_left,1300);
			 
			 sprintf(buf1,"L转");
			speak_len1=strlen(( const char *)buf1);
			speak_context((u8*)buf1,speak_len1);
			 
		 }
		 else
		 {
/////////////////////////////
//小车离中心线小于50mm，大于30mm，有位置环pid，有方向环pid
/////////////////////////			
			 if(left_dis-170>30)
			 {
				   kp_dis=kp_dis_mid;
				   kp_dir=kp_dir_mid;
				 error_dis=(left_dis-170)*kp_dis;
				 error_dir=(last_dis-left_dis)*kp_dir;
				 if((last_dis-left_dis)>0)
				 {
					 //靠近中心线
					  if(error_dir>=0.9*error_dis)
					 {
						 error_dir=0.9*error_dis;
					 }
					 speed_left=1700-error_dis+error_dir;
					  if(speed_left<=1500)
					 {
						 speed_left=1550;
					 }
					  speed_right=1300;
					 adjustDirection_1((int)speed_left,1300);

					  
				 }
				 else
				 {
					 //远近中心线
					speed_left=1700-error_dis+error_dir;
					  if(speed_left<=1500)
					 {
						 speed_left=1550;
					 }
					  speed_right=1300;
					 adjustDirection_1((int)speed_left,1300);
				 }
				 sprintf(buf1,"L转");
				speak_len1=strlen(( const char *)buf1);
				speak_context((u8*)buf1,speak_len1);
			 }
			 else
			 {
	
				 
				 kp_dis=kp_dis_big;
				  kp_dir=kp_dir_big;
				 error_dis=(left_dis-170)*kp_dis;
/////////////////////////////
//小车离中心线小于30mm，大于10mm，有位置环pid，有方向环pid
/////////////////////////
				
					error_dir=(last_dis-left_dis)*kp_dir;
					 if((last_dis-left_dis)>0)
					 {
						 //靠近中心线
						 
	//					  if(error_dir>=0.9*error_dis)
	//					 {
	//						 error_dir=0.9*error_dis;
	//					 }
	//					 speed_left=1700-error_dis+error_dir;
	//					  if(speed_left<=1500)
	//					 {
	//						 speed_left=1550;
	//					 }
	//					 adjustDirection_1((int)speed_left,1300);
						 //左转是大环境，这里离目标线很近，需要调整车头右拐，防止超调
						 speed_right=1350+error_dir;
						  if(speed_right>=1500)
						 {
							 speed_right=1450;
						 }
						  speed_left=1700;
						 adjustDirection_1(1700,(int)speed_right);
						 
						 sprintf(buf1,"右转");
						speak_len1=strlen(( const char *)buf1);
						speak_context((u8*)buf1,speak_len1);

						  
					 }
					 else
					 {
						 //远近中心线
						speed_left=1650-error_dis+error_dir;
						  if(speed_left<=1500)
						 {
							 speed_left=1550;
						 }
						  speed_right=1300;
						 adjustDirection_1((int)speed_left,1300);
						 sprintf(buf1,"L转");
						speak_len1=strlen(( const char *)buf1);
						speak_context((u8*)buf1,speak_len1);
					 }
				
				 
			 }	
			 
			 
			 
			
				 
		 }
//		  sprintf(buf1,"F=%d,L=%d,s_l=%d,s_r=1300,\n",front_dis,left_dis,(int)speed_left);
//		speak_len1=strlen(( const char *)buf1);
//		for( s_i = 0 ; s_i < speak_len1; s_i++) 
//		{
//	//		while((USART3->SR&0X40)==0);  
//	//		USART3->DR = DataScope_OutPut_Buffer2[s_i]; 
//			while((USART1->SR&0X40)==0);  
//			USART1->DR = buf1[s_i];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
//		}
		
						
		
	 }
//	 target=170.0;
//	DataScope_Get_Channel_Data((float)target, 1 );
//	DataScope_Get_Channel_Data((float)right_dis, 2 );
//	DataScope_Get_Channel_Data((float)speed_left/10.0, 3 );
//	DataScope_Get_Channel_Data((float)speed_right/10.0, 4 );
//	DataScope_Get_Channel_Data((float)error_dis/10.0, 5 );
//	DataScope_Get_Channel_Data((float)error_dir/10.0, 6 );
	 
//	 	 target=170.0;
//	DataScope_Get_Channel_Data((float)target, 1 );
	DataScope_Get_Channel_Data((float)left_dis, 1 );
	DataScope_Get_Channel_Data((float)speed_left/10.0,2 );
	DataScope_Get_Channel_Data((float)speed_right/10.0,3 );
	DataScope_Get_Channel_Data((float)error_dis/10.0,4 );
	DataScope_Get_Channel_Data((float)error_dir,5 );


		Send_Count_tempa = DataScope_Data_Generate(4);
		for( s_i = 0 ; s_i < Send_Count_tempa; s_i++) 
		{
			while((USART1->SR&0X40)==0);  
			USART1->DR = DataScope_OutPut_Buffer[s_i]; 
		}
	 
		

	 

}


////////////////
/////
////////////////////
////
////
/////////////////////////////
//void walkalone_left_2(void)
//{	
//	
//	
//	 front_dis= GetDis_front(front_echo,front_trig)-30;
////						 sprintf(buf1,"     ");
////						 OLED_P8x16Str(49, 0, (unsigned char*)buf1, 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/
////						 sprintf(buf1,"%d",front_dis);
////						 OLED_P8x16Str(49, 0, (unsigned char*)buf1, 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/
//	

//	
//	
//	  left_dis= GetDis_left(left_echo,left_trig);
////						 sprintf(buf1,"     ");
////						 OLED_P8x16Str(49, 4, (unsigned char*)buf1, 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/
////						  sprintf(buf1,"%d",right_dis);
////						 OLED_P8x16Str(49, 4, (unsigned char*)buf1, 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/	
//	
//	sprintf(buf1,"F=%d,L=%d\n",front_dis,left_dis);
//	speak_len1=strlen(( const char *)buf1);
//	for( s_i = 0 ; s_i < speak_len1; s_i++) 
//	{
////		while((USART3->SR&0X40)==0);  
////		USART3->DR = DataScope_OutPut_Buffer2[s_i]; 
//		while((USART1->SR&0X40)==0);  
//		USART1->DR = buf1[s_i]; 
//	}
//	
//	
//	 if(left_dis >=172&&left_dis < 450)//turn right
//	 {
//		  error=left_dis-172;
//		 speed_left=200-(error*kp);
//		 if(speed_left<0)
//		 {
//			 speed_left=0x00;
//		 }
//			 
//		  if(speed_left>200)
//		 {
//			 speed_left=200;
//		 }
//		 
//		 sum_error=sum_error+error;
//		 
//		 adjustDirection((int)speed_left,200);
//				
//		sprintf(buf1,"L转");
//		speak_len1=strlen(( const char *)buf1);
//		speak_context((u8*)buf1,speak_len1);
//			
//	 }
//	 
//	 else//turn left
//	 {
//		  error=172-left_dis;
//		 speed_right=200-(error*kp);
//		 if(speed_right<0)
//		 {
//			 speed_right=0x00;
//		 }
//			 
//		 if(speed_right>200)
//		 {
//			 speed_right=200;
//		 }
//		 
//		 sum_error=sum_error+error;
//		 
//		 adjustDirection(200,(int)speed_right);
//				
//		sprintf(buf1,"右转");
//		speak_len1=strlen(( const char *)buf1);
//		speak_context((u8*)buf1,speak_len1);	
//	 }
//	 
//	 if(left_dis > 450)//右边不靠墙时扫描左边与墙的距离
//	 {
//		 right_pwmval=1400;//high mean low speed
//		 TIM_SetCompare1(TIM3,right_pwmval);
//		 
//		 left_pwmval=1600;//high mean low speed
//		 TIM_SetCompare2(TIM3,left_pwmval);	
//		
//		pwm_num=0x00;
//		 sprintf(buf1,"直行");
//		speak_len1=strlen(( const char *)buf1);
//		speak_context((u8*)buf1,speak_len1);	
//	 }
//	 
//	 
//}


////////////////////

void car_stop(void)
{
	//tim3 1# is right servo
	led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
}



//////////////////////////////////////////
/* 循右墙      --------------------------------------------------------*/

////////////////////
void find_fire_turn_right(void)
{
	pwm_num=0x00;
	do
	{
         //将i的值赋给y
		 led0pwmval=1530;//high mean low speed
		 TIM_SetCompare1(TIM3,led0pwmval);
		 
		//tim3 2# is left servo
		 led0pwmval=1530;//high mean low speed
		 TIM_SetCompare2(TIM3,led0pwmval);	
		
		
		
		if(pwm_num>=33)
		{
			record_pwm_num=pwm_num;
			break;
		}
	}
	while(!(LeftFire==0&&RightFire==0));//当左右两侧火焰传感器识别到火焰时//检测到火源跳出当前while循环
	record_pwm_num=pwm_num;
	
//	pwm_num=0x00;
//	do
//	{
//         //将i的值赋给y
//		 led0pwmval=1600;//high mean low speed
//		 TIM_SetCompare1(TIM3,led0pwmval);
//		 
//		//tim3 2# is left servo
//		 led0pwmval=1600;//high mean low speed
//		 TIM_SetCompare2(TIM3,led0pwmval);	
//	}while(pwm_num<=7);//
	
	car_stop();	
	record_pwm_num=record_pwm_num+0;	
     
         
    
}
//////////////////////////////

		

void find_fire_turn_left()
{
	pwm_num=0x00;
	do
	{
         //将i的值赋给y
		 led0pwmval=1470;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		//tim3 2# is left servo
		 led0pwmval=1470;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
		
		
		
		if(pwm_num>=23)
		{
			record_pwm_num=pwm_num;
			break;
		}
	}
		while(!(LeftFire==0&&RightFire==0));//当左右两侧火焰传感器识别到火焰时//检测到火源跳出当前while循环
		car_stop();	
			
      record_pwm_num=pwm_num;
}

////////////////////
void find_fire_turn_back_right(void)
{
	pwm_num=0x00;
	do
	{
         //将i的值赋给y
		 led0pwmval=1530;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		//tim3 2# is left servo
		 led0pwmval=1530;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
		
		
		
		if(pwm_num>=record_pwm_num)
		{
			record_pwm_num=pwm_num;
			break;
		}
	}
		while(1);//当左右两侧火焰传感器识别到火焰时//检测到火源跳出当前while循环
		car_stop();		
			
      record_pwm_num=pwm_num;
         
    
}
//////////////////////////////

		

void find_fire_turn_back_left()
{
	pwm_num=0x00;
	do
	{
         //将i的值赋给y
		 led0pwmval=1470;//high mean low speed
		 TIM_SetCompare3(TIM3,led0pwmval);
		 
		//tim3 2# is left servo
		 led0pwmval=1470;//high mean low speed
		 TIM_SetCompare4(TIM3,led0pwmval);	
		
		
		
		if(pwm_num>=record_pwm_num)
		{
			record_pwm_num=pwm_num;
			break;
		}
	}
		while(1);//当左右两侧火焰传感器识别到火焰时//检测到火源跳出当前while循环
		
		car_stop();
			
      record_pwm_num=pwm_num;
}

/* 搜索房间      --------------------------------------------------------*/
u32 findRoom(u32 roomNumber)
{
	 u32 count;	 
		switch(roomNumber)
		{
				//匹配条件：根据每个房间入口特性来选择，第一个房间入口在左边，前方需离墙56~66cm，右边近墙
				case 1:
//					if(front_value > 600 && front_value < 630)
						if(front_value > 630 && front_value < 730)
					{
						//left_value=GetDis(GPIOA,GPIO_Pin_1,GPIO_Pin_0);
						scanValue();
						if(left_value > 300)
						{
							return Turn_Left;
						}
						else
						{
							forwardOneStep();			  //沿墙直走（参数需要考量，电机速度有可能不一致）
						}
					}
					break;
				case 2://二号房间需要先找到T型路口右拐，再找到对应L型右拐路口
					//if(right_value > 300 && front_value > 600)
						if(front_value < 280)					
					{
					      //right_value=GetDis(GPIOC,GPIO_Pin_7,GPIO_Pin_6);
						  scanValue();
						  if(right_value > 400)
							{
								return Turn_Right;
							}
						  else
							{
								forwardOneStep();			  //沿墙直走（参数需要考量，电机速度有可能不一致）
							}
					}
					if(right_value > 300 && front_value >500)
					{
					


							for(count=0;count<blind_time1;count++)
							{
								forwardOneStep();
							} 
							CorrectDirection2(40,left);
							scanValue();
							turn_right_90_room2();
							//turn_right_90();

//							while(left_value > 300)//处于T型路口，走入直行道（right_dis > 300表示右边不靠墙，同时避免再次进入第一个判断条件）
//							{
//								forwardOneStep();
//								left_value=GetDis(GPIOA,GPIO_Pin_1,GPIO_Pin_0);	
//								delay_ms(30);
//							}

								for(count=0;count<blind_time2;count++)
								{
									walkalone_left();							
								}				
					}
				
					break;
				case 3://三号房间处于L型路口右拐位置
					if(left_value < 360 && front_value < 200)
					{
						  scanValue();//right_value=GetDis(GPIOC,GPIO_Pin_7,GPIO_Pin_6);
						  if(right_value > 300)
							{
									return Turn_Right;
							}
						  else
							{
								forwardOneStep();	
							}
					}
					break;
				case 4://四号房间处于T型路口右拐的位置
					if(right_value > 300 && front_value > 600)//front_dis受转弯的情况影响较大，实际值可能比预期值小
					{
						for(count=0;count<1;count++)//前行一段距离，找到合适的入口位置
						{
							walkalone_left();
						}
					CorrectDirection2(30,left);

						if(front_value > 600)
						{
							return Turn_Right;
						}
					}
					break;
				default:
				  break;
		}
		return Not_Found;
}

/* 进入房间      --------------------------------------------------------*/
void EnterRoom(u8 room)
{
   switch(room)
	 {
     case 1:
		   FindMarks(4,right);
		   PutOutFire(left);
			 break;
     case 2:
		   FindMarks(5,left); 
       	   PutOutFire(right);
		   break;
	 case 3:
		   FindMarks1(5,left);
		   turn_right_90();
		   scanValue();
		   FindMarks1(5,left);
		   PutOutFire(right);
		   break;
	 case 4:
		   FindMarks(5,left); 
		   PutOutFire(right);	
		   break;
   }
}
/* 寻找标志     --------------------------------------------------------*/
void FindMarks(u8 mark_temp,u8 base_wall) // 参数： mark_temp寻找的标志，base_wall 为以左边还是右边为基准 right left
{  
	if(base_wall == right)
	{
		while(1)
		{
//			left_value=GetDis(GPIOA,GPIO_Pin_1,GPIO_Pin_0);	
//			front_value=GetDis(GPIOA,GPIO_Pin_3,GPIO_Pin_2);
			scanValue();	   		
			walkalone_right();
			if(ID_marks() == mark_temp)
			{
				break;
			}
		}
	}
	else
	{
		while(1)
		{

			//front_value=GetDis(GPIOA,GPIO_Pin_3,GPIO_Pin_2);	   
			//right_value=GetDis(GPIOC,GPIO_Pin_7,GPIO_Pin_6);	
			scanValue();
			walkalone_left();
			mark=ID_marks();
			if( mark== mark_temp)
			{
				break;
			}
		}
	}
}
//******************
//****************
void FindMarks1(u8 mark_temp,u8 base_wall) // 参数： mark_temp寻找的标志，base_wall 为以左边还是右边为基准 right left
{  
	if(base_wall == right)
	{
		while(1)
		{
//			left_value=GetDis(GPIOA,GPIO_Pin_1,GPIO_Pin_0);	
//			front_value=GetDis(GPIOA,GPIO_Pin_3,GPIO_Pin_2);
			scanValue();	   		
			walkalone_right();
			if(ID_marks1() == mark_temp)
			{
				break;
			}
		}
	}
	else
	{
		while(1)
		{

//			front_value=GetDis(GPIOA,GPIO_Pin_3,GPIO_Pin_2);	   
//			right_value=GetDis(GPIOC,GPIO_Pin_7,GPIO_Pin_6);
			scanValue();	
			walkalone_left();
			mark=ID_marks1();
			if( mark== mark_temp)
			{
				break;
			}
		}
	}
}

/* 判断拐弯类型      --------------------------------------------------------*/
u8 ID_marks(void)  //判断拐弯类型 1:直线 2:L型左 3:L型右 4:左弯 5:右弯  6:T型
{		  
    u8 mark=0;

	  if(left_value+right_value<400&&front_value>170)
		{
      	 mark=1;
	    }
	  else if(left_value+right_value>=400&&front_value>270)
		{
	     if(left_value>right_value) {	mark=2;}
			 else if(left_value<right_value) { mark=3;}
	    }
	  else if(left_value+right_value>=350&&front_value<=270)
		{
	  	 if(left_value>=right_value&&right_value<300){mark=4;}
			else if(left_value<=right_value&&left_value<300) {mark=5;}//left_dis选300（如果刚到路口过偏可能较难调准）
			else {mark=6;}
	    }
	  return mark;
		
}
//***********************
//*******************
u8 ID_marks1(void)  //判断拐弯类型 1:直线 2:L型左 3:L型右 4:左弯 5:右弯  6:T型
{		  
    u8 mark=0;

	  if(left_value+right_value<400&&front_value>170)
		{
      	 mark=1;
	    }
	  else if(left_value+right_value>=400&&front_value>280)
		{
	     if(left_value>right_value) {	mark=2;}
			 else if(left_value<right_value) { mark=3;}
	    }
	  else if(left_value+right_value>=350&&front_value<=280)
		{
	  	 if(left_value>=right_value&&right_value<300){mark=4;}
			else if(left_value<=right_value&&left_value<300) {mark=5;}//left_dis选300（如果刚到路口过偏可能较难调准）
			else {mark=6;}
	    }
	  return mark;
		
}

/* 灭火      --------------------------------------------------------*/
void PutOutFire(u8 turn_direction)
{
	  u16 a1=225; 
	   u16 a2=95; 
	  delay_ms(200);
	  if(turn_direction == left)
		{
			while	(--a1&&LeftFire&&RightFire)     //缓慢旋转寻找火源  判断a的值 如果a为0则没找到火源 否则为找到火源
			{  
				Turn(turn_direction,1,15,15);//小步左转，根据实际情况而定，在此（未检测到火焰的情况下）大概转角90度
			}
		}
	  else if(turn_direction == right)
		{
			while	(--a2&&LeftFire&&RightFire)     //缓慢旋转寻找火源  判断a的值 如果a为0则没找到火源 否则为找到火源
			{  
				Turn(turn_direction,1,21,21);//右转 右转的值需要设置相对较大，跟电机特性有关
			}
		}			
	  if(a1==0||a2==0)             //当a=0时证明没有到火源
		{
				 if(turn_direction == right)
				 {
					 turn_right_90();
				 }
				 else
				 { 
					 turn_left_90();
				 }
        }
		else            //找到了火源
		{
					if(turn_direction==right)     //继续微转（整个转动过程约转90度角）
					{
					     while(a2-- > 0)
						 {
								 Turn(turn_direction,1,21,21);
					     }
					}
					else
					{
						 while(a1-- > 0)
						 {
								Turn(turn_direction,1,15,15);
						 }
		
					}
					scanValue();
					delay_ms(2);
		      		while(front_value>180)            //使小车沿墙行走 直到小车距离前方的墙壁距离160mm处停下
				   	{
						scanValue();//front_value=GetDis(GPIOA,GPIO_Pin_3,GPIO_Pin_2);
			            if(left_value>right_value)
							{
			                    walkalone_right();
//								left_value=GetDis(GPIOA,GPIO_Pin_1,GPIO_Pin_0);	
//								right_value=GetDis(GPIOC,GPIO_Pin_7,GPIO_Pin_6);
						scanValue();
			                }
					    else 
							{
								walkalone_left();	   
//								right_value=GetDis(GPIOC,GPIO_Pin_7,GPIO_Pin_6);
//								left_value=GetDis(GPIOA,GPIO_Pin_1,GPIO_Pin_0);	
								scanValue();
				 			}
			        } 
					 delay_ms(200);                //延时一段时间 防止突然转向 使小车倾倒
			       if(turn_direction==right)      //判断旋转反向  并旋转 使小车对着火源
					 {
				        Turn(left,50,20,20);
		             }
				   else 
					 {
				       Turn(right,60,20,20);
		             }
			       while (!LeftFire||!RightFire)   //判断蜡烛是否吹灭  未吹灭则一直吹
					 {
			 			Fan_On;	          // 启动风扇
						stop(100);	      //延时3秒	  如果用delay延时的话，伺服电机会因为没有1500us的脉冲而偶尔动一下，读者可以用delay试一试
				        Fan_Off;          // 关闭风扇
						stop(30);	      //延时1秒
		             }	  
					 if(turn_direction==right)      //判断旋转反向  并旋转 使小车对着火源
					 {
				         Turn(right,70,20,20); 
		             }
					 else 
					 {
				         Turn(left,60,20,20);
		             }
					 turn_right_180();//灭火完毕，调转方向
     }
}
/*判断左转右转     --------------------------------------------------------*/
void checkThePath(void)
{
	/****
	my add code 
	****/

	if(front_value < 250)
		{
			if(right_value > 400)
			{
				turn_right_90();//右转，速转90度,优先右转
				scanValue();						
			}
			else if(left_value > 400)
			{
//				if(roomNumber==1)
//				{
//					
//					PutOutFire(right);	
//				}
				turn_left_90();//左转，速转90度
				scanValue();
			}
			else
			{
				forwardOneStep();
			}
	   }
}
/* 出房间      --------------------------------------------------------*/
void Out(u8 out_mark)  //dr 基准墙方向 out_mark 出口处标志   //判断拐弯类型 1:直线 2:L型左 3:L型右 4:左弯 5:右弯  6:T型
{ 
	scanValue();
	mark=ID_marks();
	while(mark!=out_mark)
	{
		mark=ID_marks();
		if(mark==4){ turn_left_90();scanValue(); }
		else if(mark==5){turn_right_90();scanValue();} 
		else if(right_value < 400)
		{
			walkalone_right();
//			left_value=GetDis(GPIOA,GPIO_Pin_1,GPIO_Pin_0);	
//			front_value=GetDis(GPIOA,GPIO_Pin_3,GPIO_Pin_2);
			scanValue();	   
		}
		else if(left_value < 400)
		{
			walkalone_left();

//			front_value=GetDis(GPIOA,GPIO_Pin_3,GPIO_Pin_2);	   
//			right_value=GetDis(GPIOC,GPIO_Pin_7,GPIO_Pin_6);
			scanValue();
		}
		else
		{
		    forwardOneStep();
		
			scanValue();
		}
	}
}
/* 出房间      --------------------------------------------------------*/
void getOut()
{
	scanValue();
	switch(roomNumber)
	{
		case 1:
			Out(6); //找到可左拐T型路口  
			break;
		case 2: //不需要处理，能自行找到出路
			break;
		case 3://不需要处理，能自行找到出路
			break;
		case 4:
			Out(6); //寻找可右拐T型路口
			break;
		default:
			break;
	}
	
}
