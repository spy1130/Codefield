

#include "stm32f10x.h"
#include "delay.h"
#include <stdio.h>
#include "cfg.h"
#include "tcs.h"
#include "stm32f10x_exti.h"
#include <stdio.h>
#include "main.h"
#include "timer.h"
#include <string.h>

/* -------------------------颜色传感器引脚电平控制定义--------------------------------------*/
#define S0_Write_1()     GPIO_SetBits(GPIOE,GPIO_Pin_15) //写1
#define S0_Write_0()     GPIO_ResetBits(GPIOE,GPIO_Pin_15) //写0
#define S1_Write_1()     GPIO_SetBits(GPIOC,GPIO_Pin_10) 
#define S1_Write_0()     GPIO_ResetBits(GPIOC,GPIO_Pin_10)
#define S2_Write_1()     GPIO_SetBits(GPIOC,GPIO_Pin_11)
#define S2_Write_0()     GPIO_ResetBits(GPIOC,GPIO_Pin_11)
#define S3_Write_1()     GPIO_SetBits(GPIOC,GPIO_Pin_12)
#define S3_Write_0()     GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define LED_Write_1()    GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define LED_Write_0()    GPIO_ResetBits(GPIOC,GPIO_Pin_13)

/*本文件为颜色传感器的函数*/


#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键3(WK_UP) 

 

#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	  2	//KEY1按下
#define WKUP_PRES   3	//KEY_UP按下(即WK_UP/KEY_UP)



volatile u16 time5_conut=0;  //计数
volatile u16 count_tcs230=0;

extern char buf1[30];//\[b1] 欢迎光临，请进




u16 RGB[3]={0};
int colour;
int red_time=0x00;
int blue_time=0x00;
int green_time=0x00;
int tim5_interruput_flag=0x00;
int tim7_interruput_flag=0x00;

int red_num=0x00;
int blue_num=0x00;
int green_num=0x00; 

int colour_flag=0;


/************************************************
  * @brief  TCS230_WhiteBalance Function.
  * @param  None
  * @retval None
  ***********************************************/

	void TCS230_WhiteBalance(u16 pColor[3])
{
//	S0_Write_0();	
//	
//	S1_Write_1();//输出频率为(1/50)*500KHz=10KHz
//	
//	LED_Write_1();//打开LED
//	
//	S2_Write_0();	S3_Write_0();//选择红色
	////////////////////////////////////
	//balance red
	/////////////////////////////
	colour_flag=1;
	
	
	s0=1;
	s1=1;//不分频
	
	led=1;
	
	s2=0;//红色滤镜
	s3=0;

	time5_conut=0;
	
	
	//TIM5_Int_Init(0xfff0,72-1);
	
	count_tcs230=0;//count tcs230 plus 
	
	
//	TIM_Cmd(TIM7, DISABLE);
//	TIM_SetCounter(TIM7, 0);
//	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
//	TIM_Cmd(TIM7, ENABLE);
//			
//	EXTI->IMR |= 1<<5;//enable exit5 interrupt
//	
//	while(count_tcs230<255);
//	
////	
////	TIM_Cmd(TIM5, DISABLE);

////	EXTI->IMR &= 0<<5;//disable exit5 interrupt
//	
//	time5_conut=TIM_GetCounter(TIM7);
	
	
	
	
	
	TIM_Cmd(TIM5, DISABLE);
	TIM_SetCounter(TIM5, 0);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	TIM_Cmd(TIM5, ENABLE);
			
	EXTI->IMR |= 1<<5;//enable exit5 interrupt
	
	while(count_tcs230<255);
	
//	
//	TIM_Cmd(TIM5, DISABLE);

//	EXTI->IMR &= 0<<5;//disable exit5 interrupt
	
	time5_conut=TIM_GetCounter(TIM5);
	
	
	
	

	
	pColor[0] = time5_conut;
	red_time=time5_conut;
	////////////////////////////////////
	//balance blue
	/////////////////////////////
	s2=0;
	s3=1;//选择蓝色

	time5_conut=0;
	
	count_tcs230=0;//count tcs230 plus 
	
	TIM_Cmd(TIM5, DISABLE);
	
	TIM_SetCounter(TIM5, 0);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	TIM_Cmd(TIM5, ENABLE);
			
	EXTI->IMR |= 1<<5;//enable exit5 interrupt
	
	while(count_tcs230<255);
	
//	TIM_Cmd(TIM5, DISABLE);

//	EXTI->IMR &= 0<<5;//disable exit5 interrupt
	
	time5_conut=TIM_GetCounter(TIM5);
	

	
	pColor[1] = time5_conut;
	blue_time=time5_conut;
	
	////////////////////////////////////
	//balance green
	/////////////////////////////
	s2=1;
	s3=1;//选择绿色
	//

	time5_conut=0;
	
	count_tcs230=0;//count tcs230 plus 
	
	TIM_SetCounter(TIM5, 0);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	TIM_Cmd(TIM5, ENABLE);
			
	EXTI->IMR |= 1<<5;//enable exit5 interrupt
	
	while(count_tcs230<255);
	
//	TIM_Cmd(TIM5, DISABLE);

//	EXTI->IMR &= 0<<5;//disable exit5 interrupt
	
	time5_conut=TIM_GetCounter(TIM5);
	
	pColor[2] = time5_conut;
	green_time=time5_conut;
	
	//LED_Write_0();//关闭LED	
	
	printf("Red: %d  Blue: %d  Green: %d\n",pColor[0],pColor[1],pColor[2]);
	
	colour_flag=0;
	
}

u16 colourjudge(u16 pRGB[3]) //测试颜色，并判断识别颜色
{	
//	int colour;

//	S0_Write_0();	
//	S1_Write_1();//输出频率为(1/50)*500KHz=10KHz	
//	LED_Write_1();//打开LED
	
	colour_flag=1;
	
	
	s0=1;
	s1=1;//1:1
	///////////////////////////////////////
	//测试红颜色，
	//////////////////////////////////////
	s2=0;
	s3=0;//选择红色
	
//	S2_Write_0();	
//	
//	S3_Write_0();
	
	time5_conut=0;
	
	count_tcs230=0;//count tcs230 plus 
	tim5_interruput_flag=0x00;
	
	TIM_SetAutoreload(TIM5,red_time);
	TIM_SetCounter(TIM5, 0);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	TIM_Cmd(TIM5, ENABLE);
	
	EXTI->IMR |= 1<<5;
	
	while(tim5_interruput_flag==0x00);
	
	tim5_interruput_flag=0x00;
	
	
	TIM_Cmd(TIM5, DISABLE);
	
	EXTI->IMR &= 0<<5;
	red_num=count_tcs230;
	///////////////////////////////////////
	//测试蓝颜色，
	//////////////////////////////////////
	
	s2=0;
	s3=1;//选择蓝色
	
//	S2_Write_0();	
//	
//	S3_Write_0();
	
	time5_conut=0;
	
	count_tcs230=0;//count tcs230 plus 
	tim5_interruput_flag=0x00;
	
	TIM_SetAutoreload(TIM5,blue_time);
	TIM_SetCounter(TIM5, 0);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	TIM_Cmd(TIM5, ENABLE);
	
	EXTI->IMR |= 1<<5;
	
	while(tim5_interruput_flag==0x00);
	tim5_interruput_flag=0x00;
	
	
	TIM_Cmd(TIM5, DISABLE);
	
	EXTI->IMR &= 0<<5;
	blue_num=count_tcs230;
	///////////////////////////////////////
	//测试绿颜色，
	//////////////////////////////////////
	
	s2=1;
	s3=1;//选择绿色
	
//	S2_Write_0();	
//	
//	S3_Write_0();
	
	time5_conut=0;
	
	count_tcs230=0;//count tcs230 plus 
	tim5_interruput_flag=0x00;
	
	TIM_SetAutoreload(TIM5,green_time);
	TIM_SetCounter(TIM5, 0);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	TIM_Cmd(TIM5, ENABLE);
	
	EXTI->IMR |= 1<<5;
	
	while(tim5_interruput_flag==0x00);
	tim5_interruput_flag=0x00;
	
	
	TIM_Cmd(TIM5, DISABLE);
	
	EXTI->IMR &= 0<<5;
	green_num=count_tcs230;
	/////////////////////////////
	//测量颜色结束
	////////////////////////////
//	sprintf(buf1,"测量结束，结果如下");
//	
//	
//	delay_ms(1000);
//	delay_ms(1000);
//	
//	sprintf(buf1,"红色 %d ",red_num);
//	
//	
//	delay_ms(1000);
//	delay_ms(1000);
//	
//	
//	
//	sprintf(buf1,"蓝色 %d",blue_num);
//	
//	
//	delay_ms(1000);
//	delay_ms(1000);
//	
//	sprintf(buf1,"绿色 %d",green_num);
//	
//	
//	delay_ms(1000);
//	delay_ms(1000);
		
	
		
		/////////////////////////////
		//测量颜色结束
		////////////////////////////
		
		LED_Write_0();//关闭LED		
		//printf("Red: %d  Blue: %d  Green: %d\n",RGB[0],RGB[1],RGB[2]);//red_num
		printf("Red: %d  Blue: %d  Green: %d\n",red_num,blue_num,green_num);
		
		
//		sprintf(buf1,"开始判断颜色");
//		
//		
//		delay_ms(1000);
	
	///////////////////////////////
	//判断颜色
	///////////////////////////////////////
	if(red_num>200&&blue_num>200&&green_num>200)//white
	{
		colour=white;
		printf("colour is white\r\n");
		
		sprintf(buf1,"白色");
		
			
	}
	else if(red_num<120&&blue_num<30&&green_num<68)//black
	{ 	
		colour=black;
		printf("colour is black\r\n");
		
		sprintf(buf1,"黑色");
		
			
	}
	 else if(185<red_num&&red_num<350&&75<blue_num&&blue_num<160&&165<green_num&&green_num<270)//yellow
	{
		colour=yellow;
		printf("colour is yellow\r\n");
		
		sprintf(buf1,"黄色");
		
			
	
	}
	 else if(120<red_num&&red_num<270&&30<blue_num&&blue_num<100&&25<green_num&&green_num<120)// red
	{
		colour=red;
		printf("colour is red\r\n");
		sprintf(buf1,"红色");
		
			
	
	}
	  else if(10<red_num&&red_num<50&&32<blue_num&&blue_num<100&&25<green_num&&green_num<130)// blue
	{
		colour=blue;
		printf("colour is blue\r\n");
		sprintf(buf1,"蓝色");
		
			
	
	}
	else
	{
		colour=other;	
		printf("NULL\r\n");
		sprintf(buf1,"其他色");
		
			
	
	}
	delay_ms(300);
	
	
	while(colour==other) //重新测试颜色
	{
		s0=1;
		s1=1;//1:1
		///////////////////////////////////////
		//测试红颜色，
		//////////////////////////////////////
		s2=0;
		s3=0;//选择红色
		
	//	S2_Write_0();	
	//	
	//	S3_Write_0();
		
		time5_conut=0;
		
		count_tcs230=0;//count tcs230 plus 
		tim5_interruput_flag=0x00;
		
		TIM_SetAutoreload(TIM5,red_time);
		TIM_SetCounter(TIM5, 0);
		TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
		TIM_Cmd(TIM5, ENABLE);
		
		EXTI->IMR |= 1<<5;
		
		while(tim5_interruput_flag==0x00);
		
		tim5_interruput_flag=0x00;
		
		
		TIM_Cmd(TIM5, DISABLE);
		
		EXTI->IMR &= 0<<5;
		red_num=count_tcs230;
		///////////////////////////////////////
		//测试蓝颜色，
		//////////////////////////////////////
		
		s2=0;
		s3=1;//选择蓝色
		
	//	S2_Write_0();	
	//	
	//	S3_Write_0();
		
		time5_conut=0;
		
		count_tcs230=0;//count tcs230 plus 
		tim5_interruput_flag=0x00;
		
		TIM_SetAutoreload(TIM5,blue_time);
		TIM_SetCounter(TIM5, 0);
		TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
		TIM_Cmd(TIM5, ENABLE);
		
		EXTI->IMR |= 1<<5;
		
		while(tim5_interruput_flag==0x00);
		tim5_interruput_flag=0x00;
		
		
		TIM_Cmd(TIM5, DISABLE);
		
		EXTI->IMR &= 0<<5;
		blue_num=count_tcs230;
		///////////////////////////////////////
		//测试绿颜色，
		//////////////////////////////////////
		
		s2=1;
		s3=1;//选择绿色
		
		time5_conut=0;
		
		count_tcs230=0;//count tcs230 plus 
		tim5_interruput_flag=0x00;
		
		TIM_SetAutoreload(TIM5,green_time);
		TIM_SetCounter(TIM5, 0);
		TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
		TIM_Cmd(TIM5, ENABLE);
		
		EXTI->IMR |= 1<<5;
		
		while(tim5_interruput_flag==0x00);
		tim5_interruput_flag=0x00;
		
		
		TIM_Cmd(TIM5, DISABLE);
		
		EXTI->IMR &= 0<<5;
		green_num=count_tcs230;
		
		///////////////////////////////////////
//		sprintf(buf1,"测量结束，结果如下");
//		
//		
//		delay_ms(1000);
//		delay_ms(1000);
//		
//		sprintf(buf1,"红色 %d ",red_num);
//		
//		
//		delay_ms(1000);
//		delay_ms(1000);
//		
//		
//		
//		sprintf(buf1,"蓝色 %d",blue_num);
//		
//		
//		delay_ms(1000);
//		delay_ms(1000);
//		
//		sprintf(buf1,"绿色 %d",green_num);
//		
//		
//		delay_ms(1000);
//		delay_ms(1000);
			
		
	
		
		/////////////////////////////
		//测量颜色结束
		////////////////////////////
		
		LED_Write_0();//关闭LED		
		//printf("Red: %d  Blue: %d  Green: %d\n",RGB[0],RGB[1],RGB[2]);//red_num
		printf("Red: %d  Blue: %d  Green: %d\n",red_num,blue_num,green_num);
		
		
//		sprintf(buf1,"开始判断颜色");
//		
//		
//		delay_ms(1000);
		
			///////////////////////////////
		//判断颜色
		///////////////////////////////////////
		if(red_num>200&&blue_num>200&&green_num>200)//white
		{
			colour=white;
			printf("colour is white\r\n");
			
			sprintf(buf1,"白色");
			
				
		}
		else if(red_num<120&&blue_num<30&&green_num<68)//black
		{ 	
			colour=black;
			printf("colour is black\r\n");
			
			sprintf(buf1,"黑色");
			
				
		}
		 else if(185<red_num&&red_num<300&&60<blue_num&&blue_num<160&&130<green_num&&green_num<270)//yellow
		{
			colour=yellow;
			printf("colour is yellow\r\n");
			
			sprintf(buf1,"黄色");
			
				
		
		}
		else if(120<red_num&&red_num<270&&30<blue_num&&blue_num<100&&25<green_num&&green_num<120)// red
		{
			colour=red;
			printf("colour is red\r\n");
			sprintf(buf1,"红色");
			
				
		
		}
		 else if(10<red_num&&red_num<50&&32<blue_num&&blue_num<100&&25<green_num&&green_num<130)// blue
		{
			colour=blue;
			printf("colour is blue\r\n");
			sprintf(buf1,"蓝色");
			
				
		
		}
		else
		{
			colour=other;	
			printf("NULL\r\n");
			sprintf(buf1,"其他色");
			
				
		
		}
		delay_ms(300);
		
		
		
	}
	
	colour_flag=0;
		
	return colour;

}





//void EXTI9_5_IRQHandler(void)
//{
//	if( EXTI_GetITStatus(EXTI_Line5) != RESET) 
//	{
//		count_tcs230++;
//		tim5_interruput_flag=0x01;
//	}
//	EXTI_ClearITPendingBit(EXTI_Line5);
//	
//}

