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

extern char buf1[30];//\[b1] ��ӭ���٣����

extern int led0pwmval;
extern int pwm_num; 
extern int left_pwmval;
extern int right_pwmval;

extern unsigned char DataScope_OutPut_Buffer[42];	   //���ڷ��ͻ�����


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


/* ���������������ɼ�      --------------------------------------------------------*/
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
	
	
		GPIO_SetBits(GPIOA,trig);		          //trig�ø� ����10us�ĸߵ�ƽ�ź� 
		delay_us(20); 
		GPIO_ResetBits(GPIOA,trig);  
				  
		while(GPIO_ReadInputDataBit(GPIOA, echo) == 0);

		TIM_SetCounter(TIM4,0)	;
		TIM_Cmd(TIM4,ENABLE);    //����������
		count = TIM_GetCounter(TIM4);//��ȡ������ֵ
	       //������ʱ����ʼ��ʱ
		while(GPIO_ReadInputDataBit(GPIOA, echo));   //�ȴ�echo�õ�
		TIM_Cmd(TIM4,DISABLE);   //�رռ�����
	
		count = TIM_GetCounter(TIM4);//��ȡ������ֵ
		//dis = count/60.034   ;//ת��Ϊ����,��ȷ��Ϊcm		  // ��dis=count/60.034	,dis�ľ�ȷ��Ϊcm
		dis = dis+count/6.0034   ;//ת��Ϊ����,��ȷ��Ϊmm	
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
	
	
		GPIO_SetBits(GPIOD,trig);		          //trig�ø� ����10us�ĸߵ�ƽ�ź� 
		delay_us(20); 
		GPIO_ResetBits(GPIOD,trig);  
				  
		while(GPIO_ReadInputDataBit(GPIOA, echo) == 0);

		TIM_SetCounter(TIM4,0)	;
		TIM_Cmd(TIM4,ENABLE);    //����������
		count = TIM_GetCounter(TIM4);//��ȡ������ֵ
	       //������ʱ����ʼ��ʱ
		while(GPIO_ReadInputDataBit(GPIOA, echo));   //�ȴ�echo�õ�
		TIM_Cmd(TIM4,DISABLE);   //�رռ�����
	
		count = TIM_GetCounter(TIM4);//��ȡ������ֵ
		//dis = count/60.034   ;//ת��Ϊ����,��ȷ��Ϊcm		  // ��dis=count/60.034	,dis�ľ�ȷ��Ϊcm
		dis = dis+count/6.0034   ;//ת��Ϊ����,��ȷ��Ϊmm	
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
	
	
		GPIO_SetBits(GPIOA,trig);		          //trig�ø� ����10us�ĸߵ�ƽ�ź� 
		delay_us(20); 
		GPIO_ResetBits(GPIOA,trig);  
				  
		while(GPIO_ReadInputDataBit(GPIOA, echo) == 0);

		TIM_SetCounter(TIM4,0)	;
		TIM_Cmd(TIM4,ENABLE);    //����������
		count = TIM_GetCounter(TIM4);//��ȡ������ֵ
	       //������ʱ����ʼ��ʱ
		while(GPIO_ReadInputDataBit(GPIOA, echo));   //�ȴ�echo�õ�
		TIM_Cmd(TIM4,DISABLE);   //�رռ�����
	
		count = TIM_GetCounter(TIM4);//��ȡ������ֵ
		//dis = count/60.034   ;//ת��Ϊ����,��ȷ��Ϊcm		  // ��dis=count/60.034	,dis�ľ�ȷ��Ϊcm
		dis = dis+count/6.0034   ;//ת��Ϊ����,��ȷ��Ϊmm	
		delay_ms(30);	 


	}
	 	dis=dis/1;
		return dis;
}

/* �������������ɼ�      --------------------------------------------------------*/
void scanValue(void)       //�ɼ�����������������������
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

/* ѭ��ǽ      --------------------------------------------------------*/
void walkalone_left(void)
{	
	
	last_value=left_value;									 //��¼�ϴβ���ֵ
															 
	//left_value=GetDis(GPIOA,GPIO_Pin_1,GPIO_Pin_0);			 //��¼��ǰ����ֵ
	  scanValue();
	if(left_value>last_value)//������ƫ����ǽǰ��	   
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

					 //adjustDirection(60, 200);	//	   ��ƫһ��
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
					//adjustDirection(85, 200);   // ��ƫһС��
					adjustDirLeft(left_speed_adjust, 200);	//	   ��ƫһС�� 
					 		  	  	
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
					//adjustDirection(200, 200);	  //��ǽֱ�ߣ�������Ҫ����������ٶ��п��ܲ�һ�£�
					forwardOneStep();
		}
	}
	else if (left_value<last_value)		 //�����˿�����ǽǰ��
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
					//adjustDirection(200, 85);	   //		   ��ƫһС��			
					adjustDirRight(200, right_speed_adjust);	   //		   ��ƫһС��	
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
					//adjustDirection(200,65);			  //		   ��ƫһ��
					//adjustDirRight(200, right_speed_adjust-5);	   //		   ��ƫһС��		  		
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
						//adjustDirection(200, 200);	  //��ǽֱ�ߣ�������Ҫ����������ٶ��п��ܲ�һ�£�
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
						adjustDirLeft(left_speed_adjust, 200);	//	   ��ƫһС�� 						 
					
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
						adjustDirRight(200, right_speed_adjust);	   //		   ��ƫһС��				 

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
				//adjustDirection(200, 200);	  //��ǽֱ�ߣ�������Ҫ����������ٶ��п��ܲ�һ�£�
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
	if(right_value>last_value)//������ƫ����ǽǰ��	   
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
					//adjustDirection(200, 85);  //	��ƫһ��
				//adjustDirRight(200, right_speed_adjust-5);  //	��ƫһ��	
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
					 //adjustDirection(200, 65);   // ��ƫһС��
					adjustDirRight(200, right_speed_adjust);  //	��ƫһС��
					 							 
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
					 forwardOneStep();			//��ǽֱ�ߣ�������Ҫ����������ٶ��п��ܲ�һ�£�
					 turn_num--;
					 

			
		}
	}
	else if (right_value<last_value)		 //�����˿�����ǽǰ��
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
					 adjustDirLeft(left_speed_adjust, 200);	//	   ��ƫһС�� 
									   
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
					//adjustDirLeft(left_speed_adjust-5, 200);	//	// 	��ƫһ��
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
					forwardOneStep();			//��ǽֱ�ߣ�������Ҫ����������ٶ��п��ܲ�һ�£�		   
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
					 //adjustDirection(200,65);		 //		��ƫһ��	
					adjustDirRight(200, right_speed_adjust);;  //	��ƫһ��	
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
					 //adjustDirection(85,200);		 //  ��ƫһ��	
					adjustDirLeft(left_speed_adjust, 200);	//	// 	��ƫһ��
					
					 			   
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
					 forwardOneStep();		 		//��ǽֱ�ߣ�������Ҫ����������ٶ��п��ܲ�һ�£�
				
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
//						 OLED_P8x16Str(49, 0, (unsigned char*)buf1, 0); /*����6050��ʼ����Ϊ���ӣ�������ʾ���ʼ��״̬*/
//						 sprintf(buf1,"%d",front_dis);
//						 OLED_P8x16Str(49, 0, (unsigned char*)buf1, 0); /*����6050��ʼ����Ϊ���ӣ�������ʾ���ʼ��״̬*/
	

	
	
	 right_dis= GetDis_right(right_echo,right_trig);
//						 sprintf(buf1,"     ");
//						 OLED_P8x16Str(49, 4, (unsigned char*)buf1, 0); /*����6050��ʼ����Ϊ���ӣ�������ʾ���ʼ��״̬*/
//						  sprintf(buf1,"%d",right_dis);
//						 OLED_P8x16Str(49, 4, (unsigned char*)buf1, 0); /*����6050��ʼ����Ϊ���ӣ�������ʾ���ʼ��״̬*/	
	
	 if(right_dis > 320)//�ұ߲���ǽʱɨ�������ǽ�ľ���
	 {
		 left_dis= GetDis_left(left_echo,left_trig);
		  forwardOneStep();
		  forwardOneStep();
		  forwardOneStep();
		  forwardOneStep();
		  forwardOneStep();
		 
		  speed_right=1300;
		 speed_left=1700;
		 sprintf(buf1,"ֱ��");
		speak_len1=strlen(( const char *)buf1);
		speak_context((u8*)buf1,speak_len1);	
	 }
	
	
	 else if(right_dis >=170)//turn right
	 {
		 error_dis=(right_dis-170)*kp_dis;
		 
		 if(right_dis-170>50)
		 {
/////////////////////////////
//С���������ߴ���50mm��ֻ��λ�û�pid��û�з���pid
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
			 sprintf(buf1,"��ת");
			speak_len1=strlen(( const char *)buf1);
			speak_context((u8*)buf1,speak_len1);	
			 
		 }
		 else
		 {
/////////////////////////////
//С����������С��50mm������30mm����λ�û�pid���з���pid
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
					 //����������
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
					 //Զ��������
					 speed_right=1300+error_dis-error_dir;
					 if(speed_right>=1500)
					 {
						 speed_right=1450;
					 }
					 speed_left=1700;
					 adjustDirection_1(1700,(int)speed_right);
				 }
				  sprintf(buf1,"��ת");
				speak_len1=strlen(( const char *)buf1);
				speak_context((u8*)buf1,speak_len1);	
			 }
/////////////////////////////
//С����������С��30mm������10mm����λ�û�pid���з���pid
/////////////////////////	
			 else
			 {
	
				  kp_dis=kp_dis_big;
				  kp_dir=kp_dir_big;
				  error_dis=(right_dis-170)*kp_dis;
				  error_dir=(last_dis-right_dis)*kp_dir;
				 
					if((last_dis-right_dis)>0)
					 {
						 //����������
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
//						  sprintf(buf1,"��ת");
//						speak_len1=strlen(( const char *)buf1);
//						speak_context((u8*)buf1,speak_len1);
						//��ת�Ǵ󻷾���������Ŀ���ߺܽ�����Ҫ������ͷ����Ϊ��գ���ֹ���� 
						 speed_left=1620-error_dir;
						 if(speed_left<=1500)
						 {
							 speed_left=1550;
						 }
						  speed_right=1300;
						 adjustDirection_1((int)speed_left,1300);
						  sprintf(buf1,"Lת");
						speak_len1=strlen(( const char *)buf1);
						speak_context((u8*)buf1,speak_len1);
					 
					 
					 }
					 else
					 {
						 //Զ��������
						 	 
						 speed_right=1380+error_dis-error_dir;
						 if(speed_right>=1500)
						 {
							 speed_right=1450;
						 }
						  speed_left=1700;
						 adjustDirection_1(1700,(int)speed_right);
						  sprintf(buf1,"��ת");
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
//С����ǽ����С��172mm
/////////////////////////	 
	 
	 else//turn left
	 {
		 error_dis=(170-right_dis)*kp_dis;
 
		 if(170-right_dis>50)
		 {
/////////////////////////////
//С���������ߴ���50mm��ֻ��λ�û�pid��û�з���pid
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
			 sprintf(buf1,"Lת");
			speak_len1=strlen(( const char *)buf1);
			speak_context((u8*)buf1,speak_len1);
			 
		 }
		 else
		 {
/////////////////////////////
//С����������С��50mm������30mm����λ�û�pid���з���pid
/////////////////////////				
			  if(170-right_dis>30)
			 {
				   kp_dis=kp_dis_mid;
				   kp_dir=kp_dir_mid;
				  error_dis=(170-right_dis)*kp_dis;
				  error_dir=(last_dis-right_dis)*kp_dir;
				 if((last_dis-right_dis)>0)
				 {
					 //Զ��������
					 
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
					 //����������
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
				 
				  sprintf(buf1,"Lת");
				speak_len1=strlen(( const char *)buf1);
				speak_context((u8*)buf1,speak_len1);
			 }
 /////////////////////////////
//С����������С��30mm������10mm����λ�û�pid���з���pid
/////////////////////////
			 else
			 {
				 kp_dis=kp_dis_big;
				  kp_dir=kp_dir_big;
				  error_dis=(170-right_dis)*kp_dis;
				 error_dir=(last_dis-right_dis)*kp_dir;
				 

				 
				
					if((last_dis-right_dis)>0)
					 {
						 //Զ��������
						 
						 kp_dis=kp_dis_big;
						 kp_dir=kp_dir_big;
						  speed_left=1620-error_dis-error_dir;
						 if(speed_left<=1500)
						 {
							 speed_left=1550;
						 }
						  speed_right=1300;
						 adjustDirection_1((int)speed_left,1300);
						  sprintf(buf1,"Lת");
						speak_len1=strlen(( const char *)buf1);
						speak_context((u8*)buf1,speak_len1);
					 }
					 else
					 {
						 //����������
						 
						 //����������
						 //��ת�Ǵ󻷾���������Ŀ���ߺܽ�����Ҫ������ͷ����Ϊ�ҹգ���ֹ����
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
						 
						 sprintf(buf1,"��ת");
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
////						 OLED_P8x16Str(49, 0, (unsigned char*)buf1, 0); /*����6050��ʼ����Ϊ���ӣ�������ʾ���ʼ��״̬*/
////						 sprintf(buf1,"%d",front_dis);
////						 OLED_P8x16Str(49, 0, (unsigned char*)buf1, 0); /*����6050��ʼ����Ϊ���ӣ�������ʾ���ʼ��״̬*/
//	

//	
//	
//	 right_dis= GetDis_right(right_echo,right_trig);
////						 sprintf(buf1,"     ");
////						 OLED_P8x16Str(49, 4, (unsigned char*)buf1, 0); /*����6050��ʼ����Ϊ���ӣ�������ʾ���ʼ��״̬*/
////						  sprintf(buf1,"%d",right_dis);
////						 OLED_P8x16Str(49, 4, (unsigned char*)buf1, 0); /*����6050��ʼ����Ϊ���ӣ�������ʾ���ʼ��״̬*/	
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
//		sprintf(buf1,"��ת");
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
//		sprintf(buf1,"Lת");
//		speak_len1=strlen(( const char *)buf1);
//		speak_context((u8*)buf1,speak_len1);
//	 }
//	 
//	 if(right_dis > 450)//�ұ߲���ǽʱɨ�������ǽ�ľ���
//	 {
//		 right_pwmval=1400;//high mean low speed
//		 TIM_SetCompare1(TIM3,right_pwmval);
//		 
//		 left_pwmval=1600;//high mean low speed
//		 TIM_SetCompare2(TIM3,left_pwmval);	
//		
//		pwm_num=0x00;
//		 sprintf(buf1,"ֱ��");
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
	
	
	if(left_dis > 320)//�ұ߲���ǽʱɨ�������ǽ�ľ���
	 {
		 left_dis= GetDis_left(left_echo,left_trig);
		  forwardOneStep();
		  forwardOneStep();
		  forwardOneStep();
		  forwardOneStep();
		  forwardOneStep(); 
		 speed_right=1300;
		 speed_left=1700;
		 sprintf(buf1,"ֱ��");
		
		speak_len1=strlen(( const char *)buf1);
		speak_context((u8*)buf1,speak_len1);	
	 }
/////////////////////////////
//С����ǽ����С��172mm
/////////////////////////	
	  else if(left_dis <=170)//turn right
	 {
		 error_dis=(170-left_dis)*kp_dis;
/////////////////////////////
//С���������ߴ���50mm��ֻ��λ�û�pid��û�з���pid
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
			sprintf(buf1,"��ת");
			speak_len1=strlen(( const char *)buf1);
			speak_context((u8*)buf1,speak_len1);	
			 
		 }
		 else
		 {
/////////////////////////////
//С����������С��50mm������30mm����λ�û�pid���з���pid
/////////////////////////	
			 if(170-left_dis>30)
			 {
				 kp_dis=kp_dis_mid;
				 kp_dir=kp_dir_mid;
				  error_dis=(170-left_dis)*kp_dis;
				 error_dir=(last_dis-left_dis)*kp_dir;
				 if((last_dis-left_dis)>0)
				 {
					 //Զ��������
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
					 //����������
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
				 sprintf(buf1,"��ת");
				speak_len1=strlen(( const char *)buf1);
				speak_context((u8*)buf1,speak_len1);	
			 }
			 else
			 {
/////////////////////////////
//С����������С��30mm����λ�û�pid���з���pid
/////////////////////////					 
				 
					  kp_dis=kp_dis_big;
					  kp_dir=kp_dir_big;
					  error_dis=(170-left_dis)*kp_dis;
					  error_dir=(last_dis-left_dis)*kp_dir;
					 if((last_dis-left_dis)>0)
					 {
						 //Զ��������
						speed_right=1350+error_dis+error_dir;
						  if(speed_right>=1500)
						 {
							 speed_right=1450;
						 }
						  speed_left=1700;
						 adjustDirection_1(1700,(int)speed_right);
						  sprintf(buf1,"��ת");
						speak_len1=strlen(( const char *)buf1);
						speak_context((u8*)buf1,speak_len1);	
					 }
					 else
					 {
						 //����������
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
						 //ת�е�ת
						  //��ת�Ǵ󻷾���������Ŀ���ߺܽ�����Ҫ������ͷ��գ���ֹ����
						speed_left=1650+error_dir;
						  if(speed_left<=1500)
						 {
							 speed_left=1550;
						 }
						 speed_right=1300;
						 adjustDirection_1((int)speed_left,1300);
						 sprintf(buf1,"Lת");
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
//С����ǽ�������172mm
/////////////////////////	 
	 else//turn left
	 {
		 error_dis=(left_dis-170)*kp_dis;
/////////////////////////////
//С���������ߴ���50mm��ֻ��λ�û�pid��û�з���pid
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
			 
			 sprintf(buf1,"Lת");
			speak_len1=strlen(( const char *)buf1);
			speak_context((u8*)buf1,speak_len1);
			 
		 }
		 else
		 {
/////////////////////////////
//С����������С��50mm������30mm����λ�û�pid���з���pid
/////////////////////////			
			 if(left_dis-170>30)
			 {
				   kp_dis=kp_dis_mid;
				   kp_dir=kp_dir_mid;
				 error_dis=(left_dis-170)*kp_dis;
				 error_dir=(last_dis-left_dis)*kp_dir;
				 if((last_dis-left_dis)>0)
				 {
					 //����������
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
					 //Զ��������
					speed_left=1700-error_dis+error_dir;
					  if(speed_left<=1500)
					 {
						 speed_left=1550;
					 }
					  speed_right=1300;
					 adjustDirection_1((int)speed_left,1300);
				 }
				 sprintf(buf1,"Lת");
				speak_len1=strlen(( const char *)buf1);
				speak_context((u8*)buf1,speak_len1);
			 }
			 else
			 {
	
				 
				 kp_dis=kp_dis_big;
				  kp_dir=kp_dir_big;
				 error_dis=(left_dis-170)*kp_dis;
/////////////////////////////
//С����������С��30mm������10mm����λ�û�pid���з���pid
/////////////////////////
				
					error_dir=(last_dis-left_dis)*kp_dir;
					 if((last_dis-left_dis)>0)
					 {
						 //����������
						 
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
						 //��ת�Ǵ󻷾���������Ŀ���ߺܽ�����Ҫ������ͷ�ҹգ���ֹ����
						 speed_right=1350+error_dir;
						  if(speed_right>=1500)
						 {
							 speed_right=1450;
						 }
						  speed_left=1700;
						 adjustDirection_1(1700,(int)speed_right);
						 
						 sprintf(buf1,"��ת");
						speak_len1=strlen(( const char *)buf1);
						speak_context((u8*)buf1,speak_len1);

						  
					 }
					 else
					 {
						 //Զ��������
						speed_left=1650-error_dis+error_dir;
						  if(speed_left<=1500)
						 {
							 speed_left=1550;
						 }
						  speed_right=1300;
						 adjustDirection_1((int)speed_left,1300);
						 sprintf(buf1,"Lת");
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
////						 OLED_P8x16Str(49, 0, (unsigned char*)buf1, 0); /*����6050��ʼ����Ϊ���ӣ�������ʾ���ʼ��״̬*/
////						 sprintf(buf1,"%d",front_dis);
////						 OLED_P8x16Str(49, 0, (unsigned char*)buf1, 0); /*����6050��ʼ����Ϊ���ӣ�������ʾ���ʼ��״̬*/
//	

//	
//	
//	  left_dis= GetDis_left(left_echo,left_trig);
////						 sprintf(buf1,"     ");
////						 OLED_P8x16Str(49, 4, (unsigned char*)buf1, 0); /*����6050��ʼ����Ϊ���ӣ�������ʾ���ʼ��״̬*/
////						  sprintf(buf1,"%d",right_dis);
////						 OLED_P8x16Str(49, 4, (unsigned char*)buf1, 0); /*����6050��ʼ����Ϊ���ӣ�������ʾ���ʼ��״̬*/	
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
//		sprintf(buf1,"Lת");
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
//		sprintf(buf1,"��ת");
//		speak_len1=strlen(( const char *)buf1);
//		speak_context((u8*)buf1,speak_len1);	
//	 }
//	 
//	 if(left_dis > 450)//�ұ߲���ǽʱɨ�������ǽ�ľ���
//	 {
//		 right_pwmval=1400;//high mean low speed
//		 TIM_SetCompare1(TIM3,right_pwmval);
//		 
//		 left_pwmval=1600;//high mean low speed
//		 TIM_SetCompare2(TIM3,left_pwmval);	
//		
//		pwm_num=0x00;
//		 sprintf(buf1,"ֱ��");
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
/* ѭ��ǽ      --------------------------------------------------------*/

////////////////////
void find_fire_turn_right(void)
{
	pwm_num=0x00;
	do
	{
         //��i��ֵ����y
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
	while(!(LeftFire==0&&RightFire==0));//������������洫����ʶ�𵽻���ʱ//��⵽��Դ������ǰwhileѭ��
	record_pwm_num=pwm_num;
	
//	pwm_num=0x00;
//	do
//	{
//         //��i��ֵ����y
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
         //��i��ֵ����y
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
		while(!(LeftFire==0&&RightFire==0));//������������洫����ʶ�𵽻���ʱ//��⵽��Դ������ǰwhileѭ��
		car_stop();	
			
      record_pwm_num=pwm_num;
}

////////////////////
void find_fire_turn_back_right(void)
{
	pwm_num=0x00;
	do
	{
         //��i��ֵ����y
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
		while(1);//������������洫����ʶ�𵽻���ʱ//��⵽��Դ������ǰwhileѭ��
		car_stop();		
			
      record_pwm_num=pwm_num;
         
    
}
//////////////////////////////

		

void find_fire_turn_back_left()
{
	pwm_num=0x00;
	do
	{
         //��i��ֵ����y
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
		while(1);//������������洫����ʶ�𵽻���ʱ//��⵽��Դ������ǰwhileѭ��
		
		car_stop();
			
      record_pwm_num=pwm_num;
}

/* ��������      --------------------------------------------------------*/
u32 findRoom(u32 roomNumber)
{
	 u32 count;	 
		switch(roomNumber)
		{
				//ƥ������������ÿ���������������ѡ�񣬵�һ�������������ߣ�ǰ������ǽ56~66cm���ұ߽�ǽ
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
							forwardOneStep();			  //��ǽֱ�ߣ�������Ҫ����������ٶ��п��ܲ�һ�£�
						}
					}
					break;
				case 2://���ŷ�����Ҫ���ҵ�T��·���ҹգ����ҵ���ӦL���ҹ�·��
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
								forwardOneStep();			  //��ǽֱ�ߣ�������Ҫ����������ٶ��п��ܲ�һ�£�
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

//							while(left_value > 300)//����T��·�ڣ�����ֱ�е���right_dis > 300��ʾ�ұ߲���ǽ��ͬʱ�����ٴν����һ���ж�������
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
				case 3://���ŷ��䴦��L��·���ҹ�λ��
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
				case 4://�ĺŷ��䴦��T��·���ҹյ�λ��
					if(right_value > 300 && front_value > 600)//front_dis��ת������Ӱ��ϴ�ʵ��ֵ���ܱ�Ԥ��ֵС
					{
						for(count=0;count<1;count++)//ǰ��һ�ξ��룬�ҵ����ʵ����λ��
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

/* ���뷿��      --------------------------------------------------------*/
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
/* Ѱ�ұ�־     --------------------------------------------------------*/
void FindMarks(u8 mark_temp,u8 base_wall) // ������ mark_tempѰ�ҵı�־��base_wall Ϊ����߻����ұ�Ϊ��׼ right left
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
void FindMarks1(u8 mark_temp,u8 base_wall) // ������ mark_tempѰ�ҵı�־��base_wall Ϊ����߻����ұ�Ϊ��׼ right left
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

/* �жϹ�������      --------------------------------------------------------*/
u8 ID_marks(void)  //�жϹ������� 1:ֱ�� 2:L���� 3:L���� 4:���� 5:����  6:T��
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
			else if(left_value<=right_value&&left_value<300) {mark=5;}//left_disѡ300������յ�·�ڹ�ƫ���ܽ��ѵ�׼��
			else {mark=6;}
	    }
	  return mark;
		
}
//***********************
//*******************
u8 ID_marks1(void)  //�жϹ������� 1:ֱ�� 2:L���� 3:L���� 4:���� 5:����  6:T��
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
			else if(left_value<=right_value&&left_value<300) {mark=5;}//left_disѡ300������յ�·�ڹ�ƫ���ܽ��ѵ�׼��
			else {mark=6;}
	    }
	  return mark;
		
}

/* ���      --------------------------------------------------------*/
void PutOutFire(u8 turn_direction)
{
	  u16 a1=225; 
	   u16 a2=95; 
	  delay_ms(200);
	  if(turn_direction == left)
		{
			while	(--a1&&LeftFire&&RightFire)     //������תѰ�һ�Դ  �ж�a��ֵ ���aΪ0��û�ҵ���Դ ����Ϊ�ҵ���Դ
			{  
				Turn(turn_direction,1,15,15);//С����ת������ʵ������������ڴˣ�δ��⵽���������£����ת��90��
			}
		}
	  else if(turn_direction == right)
		{
			while	(--a2&&LeftFire&&RightFire)     //������תѰ�һ�Դ  �ж�a��ֵ ���aΪ0��û�ҵ���Դ ����Ϊ�ҵ���Դ
			{  
				Turn(turn_direction,1,21,21);//��ת ��ת��ֵ��Ҫ������Խϴ󣬸���������й�
			}
		}			
	  if(a1==0||a2==0)             //��a=0ʱ֤��û�е���Դ
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
		else            //�ҵ��˻�Դ
		{
					if(turn_direction==right)     //����΢ת������ת������Լת90�Ƚǣ�
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
		      		while(front_value>180)            //ʹС����ǽ���� ֱ��С������ǰ����ǽ�ھ���160mm��ͣ��
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
					 delay_ms(200);                //��ʱһ��ʱ�� ��ֹͻȻת�� ʹС���㵹
			       if(turn_direction==right)      //�ж���ת����  ����ת ʹС�����Ż�Դ
					 {
				        Turn(left,50,20,20);
		             }
				   else 
					 {
				       Turn(right,60,20,20);
		             }
			       while (!LeftFire||!RightFire)   //�ж������Ƿ���  δ������һֱ��
					 {
			 			Fan_On;	          // ��������
						stop(100);	      //��ʱ3��	  �����delay��ʱ�Ļ����ŷ��������Ϊû��1500us�������ż����һ�£����߿�����delay��һ��
				        Fan_Off;          // �رշ���
						stop(30);	      //��ʱ1��
		             }	  
					 if(turn_direction==right)      //�ж���ת����  ����ת ʹС�����Ż�Դ
					 {
				         Turn(right,70,20,20); 
		             }
					 else 
					 {
				         Turn(left,60,20,20);
		             }
					 turn_right_180();//�����ϣ���ת����
     }
}
/*�ж���ת��ת     --------------------------------------------------------*/
void checkThePath(void)
{
	/****
	my add code 
	****/

	if(front_value < 250)
		{
			if(right_value > 400)
			{
				turn_right_90();//��ת����ת90��,������ת
				scanValue();						
			}
			else if(left_value > 400)
			{
//				if(roomNumber==1)
//				{
//					
//					PutOutFire(right);	
//				}
				turn_left_90();//��ת����ת90��
				scanValue();
			}
			else
			{
				forwardOneStep();
			}
	   }
}
/* ������      --------------------------------------------------------*/
void Out(u8 out_mark)  //dr ��׼ǽ���� out_mark ���ڴ���־   //�жϹ������� 1:ֱ�� 2:L���� 3:L���� 4:���� 5:����  6:T��
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
/* ������      --------------------------------------------------------*/
void getOut()
{
	scanValue();
	switch(roomNumber)
	{
		case 1:
			Out(6); //�ҵ������T��·��  
			break;
		case 2: //����Ҫ�����������ҵ���·
			break;
		case 3://����Ҫ�����������ҵ���·
			break;
		case 4:
			Out(6); //Ѱ�ҿ��ҹ�T��·��
			break;
		default:
			break;
	}
	
}
