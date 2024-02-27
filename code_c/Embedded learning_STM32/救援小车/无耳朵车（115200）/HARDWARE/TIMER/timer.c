#include "timer.h"
#include "led.h"
#include "usart.h"
#include "stdio.h"
#include <string.h>
#include "main.h"
#include "cfg.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTE精英STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,增加TIM3_PWM_Init函数。
//2,增加LED0_PWM_VAL宏定义，控制TIM3_CH2脉宽									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
volatile int counter_t2=0;//1#
volatile int counter_t3=0;//2#
volatile int counter_t4=0;//3#
volatile int counter_t1=0;//4#
volatile int counter_t8=0;//5#

volatile int over_flow_t2=0;
volatile int over_flow_t3=0;
volatile int over_flow_t4=0;
volatile int over_flow_t1=0;
volatile int over_flow_t8=0;

volatile int black_frequency_1=0;
volatile int black_frequency_2=0;
volatile int black_frequency_3=0;
volatile int black_frequency_4=0;
volatile int black_frequency_5=0;

volatile int display_flag=0x00;




extern int tim5_interruput_flag;
extern int tim7_interruput_flag;

extern  short YAW,PITCH,ROLL;
extern  u8 rx_buff[20];
extern  u8 rx_end_flag;//1 is mean that serious port  recieve a good frame,0 mean that is recieving ;
extern  u8 rx_star_flag;//1 mean the 0xa5 has already recieved,0 mean that we are waiting for the beginning of series port

extern  int colour_flag;

int motor_in1=0x00;
int motor_in2=0x00;
int motor_in3=0x00;
int pwm_num=0x00;
int room_2_pwm_num=0x00;

int pwm_num_distance=0x00;

int motor_in_old=0x01;

int motor_count=0x00;
int key_count=0x00;
int key_value=0x00;
int key_last_value=0x00;

int counter_50ms=0x00;
int time_1s_flag=0x01;

char new_key_flag=0x00;

char temp_1=0x00;
char temp_2=0x00;
char temp_3=0x00;
char temp_4=0x00;
char temp_5=0x00;
char temp_6=0x00;
char temp_7=0x00;
char temp_8=0x00;
char temp_9=0x00;
char temp_10=0x00;
char temp_11=0x00;


int tim5_over_count=0x00;
int tim7_over_count=0x00;

float yaw_0=0;










void speak_context_2(u8 buf_xm[],int speak_len )
{
	//speak_len=(sizeof(buf_xm)/sizeof(buf_xm[0]));
	int Send_Count,speak_i;
	char DataScope_OutPut_Buffer[20];
	Send_Count = speak_len+5;

	DataScope_OutPut_Buffer[0]=0xfd;
	DataScope_OutPut_Buffer[1]=0x00;
	DataScope_OutPut_Buffer[2]=speak_len+2;
	
	DataScope_OutPut_Buffer[3]=0x01;
	DataScope_OutPut_Buffer[4]=0x00;//01 gbk //00 gb2312
	
	
	//说话内容
		
	for(speak_i=0;speak_i<speak_len;speak_i++)
	{
		DataScope_OutPut_Buffer[speak_i+5]=buf_xm[speak_i];
	}
	
	

	for( speak_i = 0 ; speak_i < Send_Count; speak_i++) 
	{
		while((USART2->SR&0X40)==0);  
		USART2->DR = DataScope_OutPut_Buffer[speak_i]; 
	}
}



//定时器2负责1#通道计数 
void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	
	TIM_Cmd(TIM2, DISABLE);  //使能TIM4 DISABLE
	
	TIM_SetCounter(TIM2, 0);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
	
	 
}
/////////////////////




////////////////
//定时器2溢出计数处理
void TIM2_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
				//LED0=!LED0;
				key_scan();
				TIM_SetCounter(TIM2, 0);
				TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
			
				TIM_Cmd(TIM2, ENABLE);
			  
			
		}
}







////定时器3负责2#通道计数
void TIM3_Int_Init(u16 arr,u16 psc)
{
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

   //??TIMER3 配置为计数器
//   TIM_DeInit(TIM3);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位


	TIM_ETRClockMode2Config(TIM3, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
	TIM_SetCounter(TIM3, 0);
	

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	
	TIM_Cmd(TIM3, ENABLE);
							 
}
//
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer3重映射  TIM3_CH2->PC7   TIM3_CH1->PC6    
 
	//李晨龙把c6，c7 tim3 pwm1 pwm2 烧坏了，故此改为tim3 pwm3 pwm4 2018.10.22
	//李晨龙把c6，c7 tim3 pwm1 pwm2 烧坏了，故此改为tim3 pwm3 pwm4 2018.10.22
	//李晨龙把c6，c7 tim3 pwm1 pwm2 烧坏了，故此改为tim3 pwm3 pwm4 2018.10.22
	
   //设置该引脚为复用输出功能,输出TIM3 CH1的PWM脉冲波形	GPIOC.8->ch1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
	
	//设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOC.9->ch2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
	
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	
	
	
	//初始化TIM3 Channel 3 PWM模式
	//李晨龙把c6，c7tim3 pwm1 pwm2 烧坏了，故此改为tim3 pwm3 pwm4 2018.10.22	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse=900;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);  //使能TIM3在CCR2上的预装载寄存器
	
	//初始化TIM3 Channel 4 PWM模式	 
		//李晨龙把c6，c7tim3 pwm1 pwm2 烧坏了，故此改为tim3 pwm3 pwm4 2018.10.22
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//	TIM_OCInitStructure.TIM_Pulse=900;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);  //使能TIM3在CCR2上的预装载寄存器,写入ccr的值立刻生效，如果enable，则发生更新时间后，ccr值被更新，既不能立刻生效

	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	

}
//定时器3计数溢出处理
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
				TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
				
				TIM_SetCounter(TIM3, 0);
				TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
			
				TIM_Cmd(TIM3, ENABLE);
			
				pwm_num++;
				
				pwm_num_distance++;
				
				room_2_pwm_num++;
			 
			
		}
}






////定时器4负责3#通道计数
void Tim4_Init(void)
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

   //??TIMER3 配置为计数器
//   TIM_DeInit(TIM4);
	
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =71; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ARRPreloadConfig(TIM4,DISABLE);  
	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4,DISABLE); 
							 
}
//定时4计数溢出处理

////////////
//
////////////////
void TIM4_Cap_Init(unsigned int arr,unsigned int psc)
{ 
        //这些都是一些结构体的声明，去下载一本库函数手册一看就知道 很简单的 
		  GPIO_InitTypeDef GPIO_InitStructure;
		  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		  TIM_ICInitTypeDef  TIM4_ICInitStructure;
		  NVIC_InitTypeDef NVIC_InitStructure;


        //这个就是时钟使能了 我使能TIM4 和GPIOB
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
			GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //Timer3重映射  TIM3_CH2->PC7   TIM3_CH1->PC6    

        //输入捕捉，当然端口要设置为输入模式了
			GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
			GPIO_Init(GPIOD, &GPIO_InitStructure);
//			GPIO_ResetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);

			TIM_TimeBaseStructure.TIM_Period = arr;  //这个就是设置计数值的最大值，就是这个寄存器能计多少个数，一般设置成0XFFFF
			TIM_TimeBaseStructure.TIM_Prescaler =psc;  //上面这个计数寄存器的频率的分频系数 分频的值=psc+1 所以你要2分频的话，psc= 1，以此类推
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分割 这在有一些情况用，这里我们不受影响 
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //计数向上计数 
			TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);  //初始化
			  
			TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1;             //选择通道1
			TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //上升沿触发还是下降沿触发的设置
			TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TT1上
			TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;        //配置输入分频 不分频
			TIM4_ICInitStructure.TIM_ICFilter = 0x01;                        //滤波的问题 先不管
			TIM_ICInit(TIM4, &TIM4_ICInitStructure);

			TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2;               
			TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 
			TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
			TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;       
			TIM4_ICInitStructure.TIM_ICFilter = 0x01; 
			TIM_ICInit(TIM4, &TIM4_ICInitStructure);

			TIM4_ICInitStructure.TIM_Channel = TIM_Channel_3;                
			TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;  
			TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
			TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;       
			TIM4_ICInitStructure.TIM_ICFilter = 0x01;    
			TIM_ICInit(TIM4, &TIM4_ICInitStructure);



			//中断设置
			NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4发生中断 其实就是上面发生你设置上升沿和下降沿触发事件
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占中断等级
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //从中断等级
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能中断通道ü
			NVIC_Init(&NVIC_InitStructure);  //初始化 

			TIM_ITConfig(TIM4,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_Update,ENABLE);//使能各个中断

					TIM_Cmd(TIM4,ENABLE ); //使能定时器TIM4
			}


////////////
void TIM4_IRQHandler(void)   //TIM3中断
{
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
				//LED0=!LED0;
				TIM_SetCounter(TIM4, 0);
				TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
			
				TIM_Cmd(TIM4, ENABLE);
				over_flow_t4++;
			
		}
		
		if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) //CH1通道（PB6）发生触发事件
	   { 
		 /* 处理代码 **/
		}

	  if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET) //CH2通道（PB7）发生触发事件
	   { 
		 /* 处理代码 **/
		}


	   if(TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET) //CH3通道（PB8）发生触发事件
	   {  
		   /* 处理代码 **/
		}

}







//定时器5负责固定计数时间周期
void TIM5_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	
	TIM_Cmd(TIM5, DISABLE);  //使能TIM4 DISABLE

	TIM_SetCounter(TIM5, 0);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	TIM_Cmd(TIM5, ENABLE);
	
	
	
}
//定时器5中断服务程序 0.1s period
void TIM5_IRQHandler(void)   //TIM5中断
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
			
			TIM_Cmd(TIM5, DISABLE);  //使能TIM5 DISABLE		
				
//			counter_t2=TIM_GetCounter(TIM2);
//			black_frequency_1=20*(counter_t2+over_flow_t2*65539);
			
			
			led_xm=!led_xm;
				
			tim5_over_count++; 
			
			tim5_interruput_flag=0x01;
			
			TIM_SetCounter(TIM5, 0);
			TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
			TIM_Cmd(TIM5, ENABLE);
			
		
		}
}






void TIM6_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	
	TIM_Cmd(TIM6, DISABLE);  //使能TIM4 DISABLE
	
	
	
	
	
	
	TIM_SetCounter(TIM6, 0);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	TIM_Cmd(TIM6, ENABLE);
							 
}


void  TIM7_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //时钟使能
	
	TIM_DeInit( TIM7);//复位TIM3定时器

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	//TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_ClearFlag(TIM7,TIM_FLAG_Update);
//	TIM_Cmd(TIM7, DISABLE);  //使能TIM4 DISABLE
	
	
	
	
	
	
	TIM_SetCounter(TIM7, 0);
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	TIM_Cmd(TIM7, ENABLE);
							 
}









void display_led(void)
{
//	char temp_1_a=0x00;
//	char temp_2_a=0x00;
//	char temp_3_a=0x00;
//	char temp_4_a=0x00;
//	char temp_5_a=0x00;
//	char temp_6_a=0x00;
//	char temp_7_a=0x00;
//	char temp_8_a=0x00;
//	char temp_9_a=0x00;

	temp_1=qti_1;
	temp_2=qti_2;
	temp_3=qti_3;
	temp_4=qti_4;
	temp_5=qti_5;
	temp_6=qti_6;
	temp_7=qti_7;
	temp_8=qti_8;
	temp_9=qti_9;
	temp_10=qti_10;
	temp_11=qti_11;
	
//	if(temp_1==0x01)
//	{
//		temp_1_a=0x00;
//	}
//	if(temp_1==0x00)
//	{
//		temp_1_a=0x01;
//	}
//	
//	if(temp_2==0x01)
//	{
//		temp_2_a=0x00;
//	}
//	if(temp_2==0x00)
//	{
//		temp_2_a=0x01;
//	}
//	
//	if(temp_3==0x01)
//	{
//		temp_3_a=0x00;
//	}
//	if(temp_3==0x00)
//	{
//		temp_3_a=0x01;
//	}
//	
//	if(temp_4==0x01)
//	{
//		temp_4_a=0x00;
//	}
//	if(temp_4==0x00)
//	{
//		temp_4_a=0x01;
//	}
//	
//	if(temp_5==0x01)
//	{
//		temp_5_a=0x00;
//	}
//	if(temp_5==0x00)
//	{
//		temp_5_a=0x01;
//	}
//	
//	if(temp_6==0x01)
//	{
//		temp_6_a=0x00;
//	}
//	if(temp_6==0x00)
//	{
//		temp_6_a=0x01;
//	}
//	
//	if(temp_7==0x01)
//	{
//		temp_7_a=0x00;
//	}
//	if(temp_7==0x00)
//	{
//		temp_7_a=0x01;
//	}
//	
//	
//	if(temp_8==0x01)
//	{
//		temp_8_a=0x00;
//	}
//	if(temp_8==0x00)
//	{
//		temp_8_a=0x01;
//	}
//	
//	
//	if(temp_9==0x01)
//	{
//		temp_9_a=0x00;
//	}
//	if(temp_9==0x00)
//	{
//		temp_9_a=0x01;
//	}
	
	
	
//	led_1=temp_1_a;
//	led_2=temp_2_a;
//	led_3=temp_3_a;
//	led_4=temp_4_a;
//	led_5=temp_5_a;
//	led_6=temp_6_a;
//	led_7=temp_7_a;
//	led_8=temp_8_a;
//	led_9=temp_9_a;
	
	led_1=temp_1;
	led_2=temp_2;
	led_3=temp_3;
	led_4=temp_4;
	led_5=temp_5;
	led_6=temp_6;
	led_7=temp_7;
	led_8=temp_8;
	led_9=temp_9;
	led_10=temp_10;
	led_11=temp_11;


}
///////////////////////////////////////////////////
//
//////////////////////////////////////////////
void show_gy25(void)
{
	float tempxx=0;
	char buf1_xm[10];
	
	if(rx_end_flag==1)//PITCH,ROLL;
	{
		
		
		memset(buf1_xm, 0,sizeof(buf1_xm));
		if(YAW>=0)
		{
			tempxx=YAW/100.0+180;
			sprintf(buf1_xm,"+%-5.2f",tempxx);
			
		}
		else
		{
			tempxx=YAW/100.0+180;
			sprintf(buf1_xm,"%-5.2f",tempxx);
		}		
		
		
		rx_end_flag=0;

	}
}


/////////////////////////////////////////
//
/////////////////////////////////////////
	
//定时器6中断服务程序 0.1s period
void TIM6_IRQHandler(void)   //TIM5中断
{
	
	
	
	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
//			LED3=!LED3;
			TIM_Cmd(TIM6, DISABLE);  //使能TIM5 DISABLE		
			

			if(colour_flag==0)
			{
				display_led();
			
				 key_scan();
				
				show_gy25();
				
				
				while((USART2->SR&0X40)==0);  
				USART2->DR = 0xa5; 
				
				while((USART2->SR&0X40)==0);  
				USART2->DR = 0x51; 
			}
			
	
			
			TIM_SetCounter(TIM6, 0);
			TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
			TIM_Cmd(TIM6, ENABLE);
			
			
		
		}
}


/////////////////////////////////////////
//
/////////////////////////////////////////
	
//定时器6中断服务程序 0.1s period
void TIM7_IRQHandler(void)   //TIM5中断
{
	
	
	
	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
//			LED3=!LED3;
			TIM_Cmd(TIM7, DISABLE);  //使能TIM5 DISABLE		
			

			
			led_xm=!led_xm;
				
			tim7_over_count++; 
			
			
			tim7_interruput_flag=0x01;
			
	
			
			TIM_SetCounter(TIM7, 0);
			TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
			TIM_Cmd(TIM7, ENABLE);
			
			
		
		}
}
////////////////////////////////
//
////////////////////////////////
//
///////////////////////////////////
void key_scan(void)
{
	char buf1[30]={"自检 "};//\[b1] 欢迎光临，请进

//////////////
//foward photo or backward photo
//////////////////////
	key_value=key_in;
   switch (key_count)
   {
   	case 0:
		
		if(key_value==0x01)
		{
			key_count=0x00;
		}
		else
		{
			key_count=0x01;
		}
		
   		break;
   	case 1:
		if(key_value==0x01)
		{
			key_count=0x00;
		}
		else
		{
			key_count=0x02;
		}
   		break;
	case 2:
		if(key_value==0x01)
		{
			key_count=0x00;
		}
		else
		{
			new_key_flag=0x01;
			sprintf(buf1,"开始");
			key_count=0x03;
			
			
			yaw_0=YAW/100.0+180;
		}
		
		
   		break;
		
	case 3:
		if(key_value==0x01)
		{
			key_count=0x00;
		}
		
		
		
   		break;
   	default:
		key_count=0x00;
		
   		break;
   }
	
	
}





////高级定时器1负责4#通道计数
void TIM1_Int_Init(u16 arr,u16 psc)
{
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能


//   TIM_DeInit(TIM1);

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseStructure.TIM_RepetitionCounter=0; 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位


	TIM_ETRClockMode2Config(TIM1, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
	TIM_SetCounter(TIM1, 0);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //使能指定的TIM1中断,允许更新中断
	
	TIM_Cmd(TIM1, ENABLE);
							 
}
//定时器4计数溢出处理
void TIM1_UP_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
				TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
				//LED0=!LED0;
				TIM_SetCounter(TIM1, 0);
				TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
			
				TIM_Cmd(TIM1, ENABLE);
			  over_flow_t1++;
			
		}
}


////高级定时器8负责5#通道计数
void TIM8_Int_Init(u16 arr,u16 psc)
{
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //时钟使能

//  TIM_DeInit(TIM8);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位


	TIM_ETRClockMode2Config(TIM8, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
	TIM_SetCounter(TIM8, 0);


	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	
	TIM_Cmd(TIM8, ENABLE);
							 
}
//定时器8计数溢出处理
void TIM8_UP_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
				TIM_ClearITPendingBit(TIM8, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
				//LED0=!LED0;
				TIM_SetCounter(TIM8, 0);
				TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
			
				TIM_Cmd(TIM8, ENABLE);
			  over_flow_t8++;
			
		}
}
































