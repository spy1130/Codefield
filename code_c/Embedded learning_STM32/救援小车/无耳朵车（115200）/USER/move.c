#include "action.h"
#include "move.h"
#include "stdio.h"
#include "delay.h"
#include "main.h"
#include <string.h>
#include "oled.h"

#define forwadLoop  1
#define forwadLoop1  1
#define TurnLeft_leftLoop    25//10	//50->25->
#define TurnLeft_rightLoop   13//4 //12->13

#define TurnRight_leftLoop    11//4 //10->15->
#define TurnRight_rightLoop   21//10	//50->25->

#define  right_serve GPIO_Pin_1	
#define left_serve GPIO_Pin_0
extern int turn_num;
extern int led0pwmval;
extern int pwm_num;
extern int left_pwmval;
extern int right_pwmval;

extern int speak_len,speak_i,s_i;

extern int speak_len1;//(sizeof(buf_xm)/sizeof(buf_xm[0]));

extern char buf1[30];//\[b1] 欢迎光临，请进
u8 loopCount=0;
u8 change = 0;

void forwardOneStep(void)	 // 前进一步
{
//	int i;
//	for(i=0;i<3;i++)
//	{
//		GPIO_SetBits(GPIOC, right_serve);
//		delay_us(1300);
//		GPIO_ResetBits(GPIOC,right_serve);
//		
//		GPIO_SetBits(GPIOC,left_serve);
//		delay_us(1630);
//		GPIO_ResetBits(GPIOC,left_serve);
//		delay_ms(10);	
//	}
	
	led0pwmval=1400;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1600;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	pwm_num=0x00;
	
	while(pwm_num<=0x02);
	
//	 led0pwmval=1500;//high mean low speed
//	 TIM_SetCompare1(TIM3,led0pwmval);
//	 
//	 led0pwmval=1500;//high mean low speed
//	 TIM_SetCompare2(TIM3,led0pwmval);	
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	
	
}


void adjustDirLeft(u32 speedl,u32 speedr)
{
    for(change=0;change<TurnLeft_leftLoop;change++)
		adjustDirection(speedl,speedr)	;  //偏左小步

	for(loopCount=0;loopCount<forwadLoop;loopCount++)
		forwardOneStep();

	for(change=0;change<TurnLeft_rightLoop;change++)
		adjustDirection(speedr,speedl)	;  //偏右小步

	for(loopCount=0;loopCount<forwadLoop1;loopCount++)
		forwardOneStep();
}
void adjustDirRight(u32 speedl,u32 speedr)
{
    
	for(change=0;change<TurnRight_rightLoop;change++)
		adjustDirection(speedl,speedr)	;  //偏右小步
	for(loopCount=0;loopCount<forwadLoop;loopCount++)
		forwardOneStep();
	for(change=0;change<TurnRight_leftLoop;change++)
		adjustDirection(speedr,speedl)	;  //偏左小步
	for(loopCount=0;loopCount<forwadLoop1;loopCount++)
		forwardOneStep();
}
  /**
  void turn_right_30();
void turn_left_30();
  **/
	
void adjustDirLeftBig(u32 speedl,u32 speedr)
{
    for(change=0;change<TurnLeft_leftLoop;change++)
		adjustDirection(speedl,speedr)	;  //偏左小步

	for(loopCount=0;loopCount<forwadLoop;loopCount++)
		forwardOneStep();

	for(change=0;change<TurnLeft_rightLoop;change++)
		adjustDirection(speedr,speedl)	;  //偏右小步

	for(loopCount=0;loopCount<forwadLoop1;loopCount++)
		forwardOneStep();
		

} 
void adjustLeftBig()
{
//	 Turn_small_delay(left,70,20,20);
//
//
//	for(loopCount=0;loopCount<15;loopCount++)
//		forwardOneStep1();
//
//	Turn_small_delay(right,35,32,32);
//
//	for(loopCount=0;loopCount<5;loopCount++)
//		forwardOneStep1();



		turnLeft_left_30();
		for(loopCount=0;loopCount<8;loopCount++)
		forwardOneStep();
		turnLeft_right_30();
		for(loopCount=0;loopCount<8;loopCount++)
		forwardOneStep();
}

void adjustRightBig()
{
	 
//	Turn_small_delay(right,32,32,32);
//
//	for(loopCount=0;loopCount<20;loopCount++)
//		forwardOneStep1();
//
//	Turn_small_delay(left,60,20,20);
//
//	for(loopCount=0;loopCount<6;loopCount++)
//		forwardOneStep1();
	
	
	
	turnRight_right_30();
	for(loopCount=0;loopCount<8;loopCount++)
		forwardOneStep();
	turnRight_left_30();
	for(loopCount=0;loopCount<8;loopCount++)
		forwardOneStep();
}

void adjustDirRightBig(u32 speedl,u32 speedr)
{
    
	for(change=0;change<TurnRight_rightLoop;change++)
		adjustDirection(speedl,speedr-10)	;  //偏右小步
	for(loopCount=0;loopCount<forwadLoop;loopCount++)
		forwardOneStep();
	for(change=0;change<TurnRight_leftLoop+1;change++)
		adjustDirection(0,speedl+100)	;  //偏左小步
	for(loopCount=0;loopCount<20;loopCount++)
		forwardOneStep();
}

void adjustDirection(u32 speedl, u32 speedr)	   //方向调整，用于循墙
{
	//tim3 1# right wheel ; tim3 2# is left wheel
//	right_pwmval=1500-speedr;//high mean low speed
//	 TIM_SetCompare3(TIM3,right_pwmval);
//	 
//	 left_pwmval=1500+speedl;//high mean low speed
//	 TIM_SetCompare4(TIM3,left_pwmval);	
//	
//	pwm_num=0x00;
	
	right_pwmval=speedr;//high mean low speed
	 TIM_SetCompare3(TIM3,right_pwmval);
	 
	 left_pwmval=speedl;//high mean low speed
	 TIM_SetCompare4(TIM3,left_pwmval);	
	
	pwm_num=0x00;
	
//	while(pwm_num<=0x02);
//	//tim3 1# right wheel ; tim3 2# is left wheel
//	 led0pwmval=1500;//high mean low speed
//	 TIM_SetCompare1(TIM3,led0pwmval);
//	 
//	 led0pwmval=1500;//high mean low speed
//	 TIM_SetCompare2(TIM3,led0pwmval);	
	
}



void adjustDirection_1(u32 speedl, u32 speedr)	   //方向调整，用于循墙
{
	//tim3 1# right wheel ; tim3 2# is left wheel
	right_pwmval=speedr;//high mean low speed
	 TIM_SetCompare3(TIM3,right_pwmval);
	 
	 left_pwmval=speedl;//high mean low speed
	 TIM_SetCompare4(TIM3,left_pwmval);	
	
	pwm_num=0x00;
//	while(pwm_num<=0x02);
//	//tim3 1# right wheel ; tim3 2# is left wheel
//	 led0pwmval=1500;//high mean low speed
//	 TIM_SetCompare1(TIM3,led0pwmval);
//	 
//	 led0pwmval=1500;//high mean low speed
//	 TIM_SetCompare2(TIM3,led0pwmval);	
	
}

void turnLeft_left_30()
{
   	int counter = 0;
	turn_num=0;
		for(counter=0;counter<6;counter++)
		{
		 	GPIO_SetBits(GPIOC,right_serve);
			delay_us(1300);
			GPIO_ResetBits(GPIOC,right_serve);

			GPIO_SetBits(GPIOC, left_serve);
			delay_us(1330);
			GPIO_ResetBits(GPIOC,left_serve);
			delay_ms(20);

		}

}

void turnRight_left_30()
{
	int counter = 0;
	turn_num=0;
		for(counter=0;counter<9;counter++)
		{
		 	GPIO_SetBits(GPIOC,right_serve);
			delay_us(1300);
			GPIO_ResetBits(GPIOC,right_serve);

			GPIO_SetBits(GPIOC, left_serve);
			delay_us(1425);
			GPIO_ResetBits(GPIOC,left_serve);
			delay_ms(20);

		}

}
void turn_left_90()			 //左转90度
{
	//tim3 1# is right servo
	//李晨龙把c6，c7tim3 pwm1 pwm2 烧坏了，故此改为tim3 pwm3 pwm4 2018.10.22
	led0pwmval=1460;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1460;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	pwm_num=0x00;
	
	sprintf(buf1,"左转90度");
		speak_len1=strlen(( const char *)buf1);
		speak_context((u8*)buf1,speak_len1);	
	
	//while(pwm_num<=54);//5v 电压
	while(pwm_num<=49);//6v 电压 
	
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
}

void turnLeft_right_30()
{
   int counter = 0;
	turn_num=0;
		for(counter=0;counter<9;counter++)
		{
		 	GPIO_SetBits(GPIOC, right_serve);
			delay_us(1521);
			GPIO_ResetBits(GPIOC,right_serve);

			GPIO_SetBits(GPIOC, left_serve);
			delay_us(1700);
			GPIO_ResetBits(GPIOC,left_serve);
			delay_ms(20);

		}


}

void turnRight_right_30()
{
	int counter = 0;
	turn_num=0;
		for(counter=0;counter<9;counter++)
		{
		 	GPIO_SetBits(GPIOC, right_serve);
			delay_us(1630);
			GPIO_ResetBits(GPIOC,right_serve);

			GPIO_SetBits(GPIOC, left_serve);
			delay_us(1700);
			GPIO_ResetBits(GPIOC,left_serve);
			delay_ms(20);
		}
}
void turn_right_90()		   //右转90度
{
	led0pwmval=1540;//high mean low speed
	
		//李晨龙把c6，c7tim3 pwm1 pwm2 烧坏了，故此改为tim3 pwm3 pwm4 2018.10.22
	
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1540;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	pwm_num=0x00;
	
	sprintf(buf1,"右转90度");
	speak_len1=strlen(( const char *)buf1);
	speak_context((u8*)buf1,speak_len1);	
	
	//while(pwm_num<=54);//5v 电压
	while(pwm_num<=52);//6v 电压 
	
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
}

void turn_right_30()		   //右转90度
{
	//tim3 1# is right servo
	led0pwmval=1540;//high mean low speed
	 TIM_SetCompare1(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1540;//high mean low speed
	 TIM_SetCompare2(TIM3,led0pwmval);	
	
	pwm_num=0x00;
	
	sprintf(buf1,"右转30度");
	speak_len1=strlen(( const char *)buf1);
	speak_context((u8*)buf1,speak_len1);	
	
	while(pwm_num<=18);
	
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare1(TIM3,led0pwmval);
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare2(TIM3,led0pwmval);	
}

void turn_right_90_room2()
{
	int counter = 0;
	turn_num=0;
		for(counter=0;counter<17;counter++)
		{
		 	GPIO_SetBits(GPIOC, right_serve);
			delay_us(1680);
			GPIO_ResetBits(GPIOC,right_serve);

			GPIO_SetBits(GPIOC, left_serve);
			delay_us(1700);
			GPIO_ResetBits(GPIOC,left_serve);
			delay_ms(20);

		}
}

void turn_right_180()		   //右转180度
{
	 led0pwmval=1540;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	//tim3 2# is left servo
	 led0pwmval=1540;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
	
	pwm_num=0x00;
	
	sprintf(buf1,"右转180度");
	speak_len1=strlen(( const char *)buf1);
	speak_context((u8*)buf1,speak_len1);	
	
	//while(pwm_num<=54);//5v 电压
	while(pwm_num<=103);//6v 电压 
	
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare3(TIM3,led0pwmval);
	 
	 led0pwmval=1500;//high mean low speed
	 TIM_SetCompare4(TIM3,led0pwmval);	
}


void Turn_small_delay(u8 dir,u32 cnt,u8 speedl,u8 speedr) 	  // 转弯，可调方向和角度
{		
	u32 a=0, b=0;
	turn_num=0;
	if (dir==right)
	{
	
    a=1500+speedl;
    b=1500+speedr;
	}
	else if(dir==left)
	{
	
    a=1500-speedl;
    b=1500-speedr;
	}
	while(cnt > 0)
	{
		GPIO_SetBits(GPIOC, right_serve);
		delay_us(a);        		   		 		 
		GPIO_ResetBits(GPIOC,right_serve);
					
		GPIO_SetBits(GPIOC,left_serve);
		delay_us(b);   								 
		GPIO_ResetBits(GPIOC, left_serve);
		delay_ms(10);
        cnt--;
	}	
}

void Turn(u8 dir,u32 cnt,u8 speedl,u8 speedr) 	  // 转弯，可调方向和角度
{		
	u32 a=0, b=0;
	turn_num=0;
	if (dir==right)
	{
	
    a=1500+speedl;
    b=1500+speedr;
	}
	else if(dir==left)
	{
	
    a=1500-speedl;
    b=1500-speedr;
	}
	while(cnt > 0)
	{
		GPIO_SetBits(GPIOC, right_serve);
		delay_us(a);        		   		 		 
		GPIO_ResetBits(GPIOC,right_serve);
					
		GPIO_SetBits(GPIOC,left_serve);
		delay_us(b);   								 
		GPIO_ResetBits(GPIOC, left_serve);
		delay_ms(20);
        cnt--;
	}	
}
void stop(u8 time)
{
	int i;
	for(i=0;i<time;i++)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_0);
		delay_us(1500);
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	
		GPIO_SetBits(GPIOC, GPIO_Pin_1);
		delay_us(1500);
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
		delay_ms(20);
		
	}
}
/***************************************************************************/
//		矫正函数  
//	    参数 turn_step:初始旋转步数  正为顺时针 负为逆时针
//		参数 turn_direction : right 顺指针扫描  left逆时针扫描
//		矫正车跟墙面的位置 使车跟墙面平行
//		   2017.10.27 	turn_direction is no useful
/***************************************************************************/

void CorrectDirection(int turn_step,u8 turn_direction)
{ 
	 u8 i;
     u8 deviation=3;      //校准精度 //偏差
	 u8 speedt=21;     	//校准速度
	 //int left_dis;
	 int right_dis,last_dis;
	 last_dis = 1000;
	 delay_ms(200);
	 scanValue();
	 turn_num=0;
	
	 //left_value=GetDis(GPIOA,GPIO_Pin_1,GPIO_Pin_0);	
	//front_value=GetDis(GPIOA,GPIO_Pin_3,GPIO_Pin_2);	   
	//right_value=GetDis(GPIOC,GPIO_Pin_7,GPIO_Pin_6);
		if(turn_direction==right)
		{
			
			i = 0;
			while(i < turn_step)//先旋转一定步数并测量与墙的距离，获取最小值
			{
							Turn(left, 1, speedt, speedt);
							right_dis = GetDis_right(right_echo,right_trig);
						//	printf("   right=%d\n",right_dis );	
							if(right_dis < last_dis)
							{
								last_dis = right_dis;
							}
							i++;
			}
			i = 0;
			//	printf(" 1  turn left is over******************=%d\n",last_dis );	
		while(i < turn_step) //旋转之后再回转，找到最小值，没有则回到原来位置
			{
				Turn(right, 1, speedt + 5, speedt);
				right_dis = GetDis_right(right_echo,right_trig);
			//		printf("   right=%d\n",right_dis );	
				if((last_dis - right_dis) < deviation && (right_dis - last_dis) < deviation)
				{
					break;
				}
				i++;
			}
			//	printf("  2  return right is over***********************=%d\n",last_dis );
			Turn(right, 1, speedt + 5, speedt);//继续回转并测量与墙的距离
			right_dis = GetDis_right(right_echo,right_trig);
		//		printf("   right=%d\n",right_dis );		
			if(right_dis < last_dis)
	     	{
					while(right_dis < last_dis) //如果再次测量的距离比之前的小则一直回转直到测量的结果大于上次记录的结果
					{
						last_dis = right_dis;
						Turn(right, 1, speedt + 5, speedt);
					  right_dis = GetDis_right(right_echo,right_trig);
			//		  	printf("   right=%d\n",right_dis );		
					}
			//		printf("  3  check again is over***********************=%d\n",last_dis );
			}
          }

		 if(turn_direction==left)
		{
			i = 0;
			while(i < turn_step) //先旋转一定步数并测量与墙的距离，获取最小值

			{
							Turn(right, 1, speedt + 5, speedt);
							right_dis = GetDis_right(right_echo,right_trig);
			//				printf("   right=%d\n",right_dis );	
							if((right_dis <= last_dis))
							{
								last_dis = right_dis;
							}
							i++;
			}
			i = 0;
		//		printf(" 4  turn right is over******************=%d\n",last_dis );
		while(i < turn_step) //旋转之后再回转，找到最小值，没有则回到原来位置
			{
				Turn(left, 1, speedt, speedt);
				right_dis = GetDis_right(right_echo,right_trig);
		//		printf("   right=%d\n",right_dis );
				if((last_dis - right_dis) < deviation && (right_dis - last_dis) < deviation)
				{
					break;
				}
				i++;
			}
	//		printf("  5  return left is over***********************=%d\n",last_dis );
			Turn(left, 1, speedt, speedt); //继续回转并测量与墙的距离

			right_dis = GetDis_right(right_echo,right_trig);
			if(right_dis < last_dis)
			{
				while(right_dis <= last_dis) //如果再次测量的距离比之前的小则一直回转直到测量的结果大于上次记录的结果
				{
					last_dis = right_dis;
					Turn(left, 1, speedt, speedt);
					right_dis = GetDis_right(right_echo,right_trig);
					printf("   right=%d\n",right_dis );
				}
	//		   	printf("  6  check again is over***********************=%d\n",last_dis );
			}
	   }
	   scanValue();
}

//****************************************
//********************		 *************
void CorrectDirection2(int turn_step,u8 turn_direction)
{ 
	 u8 i;
     u8 deviation=3;      //校准精度 //偏差
	 u8 speedt=21;     	//校准速度
	 //int left_dis;
	 int left_value,last_dis;
	 last_dis = 1000;
	 delay_ms(200);
	 scanValue();
	 turn_num=0;
	
	 //left_value=GetDis(GPIOA,GPIO_Pin_1,GPIO_Pin_0);	
	//front_value=GetDis(GPIOA,GPIO_Pin_3,GPIO_Pin_2);	   
	//right_value=GetDis(GPIOC,GPIO_Pin_7,GPIO_Pin_6);
		if(turn_direction==right)
		{
			
			i = 0;
			while(i < turn_step)//先旋转一定步数并测量与墙的距离，获取最小值
			{
							Turn(left, 1, speedt, speedt);
							left_value= GetDis_right(left_echo,left_trig);
					//		printf("   left=%d",left_value );
					//		printf("   turn_step=%d\n",i );		
							if(left_value < last_dis)
							{
								last_dis = left_value;
							}
							i++;
			}
			i = 0;
		//		printf(" 1  turn left is over******************=%d\n",last_dis );	
		while(i < turn_step) //旋转之后再回转，找到最小值，没有则回到原来位置
			{
				Turn(right, 1, speedt + 5, speedt);
				left_value= GetDis_right(left_echo,left_trig);
			//		printf("   left=%d",left_value );
			//		printf("   turn_step=%d\n",i );	
				if((last_dis - left_value) < deviation && (left_value - last_dis) < deviation)
				{
					break;
				}
				i++;
			}
		//		printf("  2  return right is over***********************=%d\n",last_dis );
			Turn(right, 1, speedt + 5, speedt);//继续回转并测量与墙的距离
			left_value= GetDis_right(left_echo,left_trig);
		//		printf("   right=%d\n",left_value );		
			if(left_value < last_dis)
	     		{
					while(left_value < last_dis) //如果再次测量的距离比之前的小则一直回转直到测量的结果大于上次记录的结果
					{
						last_dis = left_value;
						Turn(right, 1, speedt + 5, speedt);
					        left_value= GetDis_right(left_echo,left_trig);
	//				  	printf("   right=%d\n",left_value );		
					}
	//				printf("  3  check again is over***********************=%d\n",last_dis );
			    }
         }



		 if(turn_direction==left)
		{
			i = 0;
			while(i < turn_step) //先旋转一定步数并测量与墙的距离，获取最小值

			{
							Turn(right, 1, speedt + 5, speedt);
							left_value= GetDis_right(left_echo,left_trig);
			//				printf("   left=%d",left_value );
			//				printf("   turn_step=%d\n",i );	
							if((left_value <= last_dis))
							{
								last_dis = left_value;
							}
							i++;
			}
			i = 0;
		//		printf(" 4  turn right is over******************=%d\n",last_dis );
		        while(i < turn_step) //旋转之后再回转，找到最小值，没有则回到原来位置
			{
				Turn(left, 1, speedt, speedt);
				left_value= GetDis_right(left_echo,left_trig);
		//		printf("   left=%d",left_value );
		//		printf("   turn_step=%d\n",i );
				if((last_dis - left_value) < deviation && (left_value - last_dis) < deviation)
				{
					break;
				}
				i++;
			}
	//		printf("  5  return left is over***********************=%d\n",last_dis );
			Turn(left, 1, speedt, speedt); //继续回转并测量与墙的距离

			left_value= GetDis_right(left_echo,left_trig);
			if(left_value < last_dis)
			{
				while(left_value <= last_dis) //如果再次测量的距离比之前的小则一直回转直到测量的结果大于上次记录的结果
				{
					last_dis = left_value;
					Turn(left, 1, speedt, speedt);
					left_value= GetDis_right(left_echo,left_trig);
//					printf("   right=%d\n",left_value );
				}
//			   	printf("  6  check again is over***********************=%d\n",last_dis );
			}
			//printf("  6  check again is over***********************=%d\n",last_dis );
	   }
	  scanValue();
}

