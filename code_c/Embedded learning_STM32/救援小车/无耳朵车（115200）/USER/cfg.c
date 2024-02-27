#include "stm32f10x.h"

#include "timer.h"
#include "cfg.h"

#include <stdio.h>



/*本文件为所有配置函数的集合*/

GPIO_InitTypeDef GPIO_InitStructure; 
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
USART_InitTypeDef USART_InitStructure;
USART_ClockInitTypeDef USART_ClockInitStructure;
	
//


	
void RCC_Configuration(void)
{
	
//	在3.0版本前的库中，这个一般都是必须的配置的，在3.0以后的版本中出现了个stm32f10x.h的函数，
//	将时钟配置定为默认的，在main函数中不直接使用这个函数了
	
//	ErrorStatus HSEStartUpStatus;
//	/* Deinitialize the RCC registers */
//	RCC_DeInit();
//	/* Enable HSE */
//	RCC_HSEConfig(RCC_HSE_ON);
//	/* Wait till HSE is ready and if Time out is reached exit */
//	HSEStartUpStatus = RCC_WaitForHSEStartUp();
//	if(HSEStartUpStatus == SUCCESS)
//	{
//		/* Add here PLL ans system clock config */
//	    /* Enable Prefetch Buffer */
//		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
//		/* Flash 2 wait state */
//		FLASH_SetLatency(FLASH_Latency_2);
//		/* Configure HCLK such as HCLK = SYSCLK */
//		RCC_HCLKConfig(RCC_SYSCLK_Div1);
//		/* Configure PCLK1 such as PCLK1 = HCLK/2 */
//		RCC_PCLK1Config(RCC_HCLK_Div2);
//		/* Configure PCLK2 such as PCLK2 = HCLK */
//		RCC_PCLK2Config(RCC_HCLK_Div1);
//		/* Set PLL clock output to 72MHz using HSE (8MHz) as entry clock */
//		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
//		/* Enable the PLL */
//		RCC_PLLCmd(ENABLE);
//		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)	;
//		/* Select the PLL as system clock source */
//		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//		while(RCC_GetSYSCLKSource() != 0x08);	
//	}
	 /* Enable GPIOA and AFIO clocks */
 	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|
	 						RCC_APB2Periph_GPIOB|
							RCC_APB2Periph_GPIOC|
							RCC_APB2Periph_GPIOD|
							RCC_APB2Periph_GPIOE|
							RCC_APB2Periph_AFIO, ENABLE);
	 /* Enable USART1 clocks */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	 /* TIM4 clocks enable */
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}



void GPIO_USART1_Configure(void)
{
	/*USART1_TX*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*USART1_RX*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

void GPIO_PWM_Configuration(void) //左右轮IO口
{
	/*PWM_TEST1*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	 /*PWM_TEST2*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void GPIO_motor_Configure(void)//机械手IO设置
{
	/* GPIOE Configuration: Channel 1/1N, 2/2N, 3/3N and 4 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;	                   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	/* TIM1 Full remapping pins */
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	                   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* TIM2 Full remapping pins */
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM2, ENABLE);  
}

void GPIO_QTI_Configure(void)//红外传感器IO设置
{ 	
	//QTI 引脚配置*/qti 1,2,3,4,5,6,7,8,9,10,;;no 11 qit is b0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
	GPIO_Init(GPIOE,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;//qti11
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}


void led_init(void)
{
	////QTI 对应LED引脚配置*/led  1,2,3,4,5,6,，10 ；          7 led is a2;;8 led is b1;9 led is d2;11 led is a3,
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //浮空输入
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;//8 led is b1
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //浮空输入
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//9 led is d2
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //浮空输入
	GPIO_Init(GPIOD,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;//7 led is A2,11 led is A3
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //浮空输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //LED0-->PB.5 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	 GPIO_SetBits(GPIOA,GPIO_Pin_1); 						 //Pc0
}




void GPIO_ult_Configure(void)//超声波IO设置
{
	/* Trig */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //开漏输出
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* Echo */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);		
}

void GPIO_TCS230_1_Configure(void)//颜色传感器IO设置
{
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_Init(GPIOC, &GPIO_InitStructure); 	
}
 void GPIO_TCS230_2_Configure(void)	//颜色传感器IO设置
{
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	s0=1;
	s1=1;

	//设置TIM3的外部计数器 PD2  ETR	 （引脚的设置跟硬件有关）
	//重映像不影响在PD2上的TIM3_ETR
/*  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);  */
}

void GPIO_EXTI5_Configure(void)	//外部中断IO设置
{
	/* 初始化GPIO*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
}

void EXTI9_5_Configure(void) //外部中断与IO口关联设置
{
//	/* 配置外部中断源EXTI5 */
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource5);   
//	/* 连接EXTI5线到PE5管脚 */

//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿为中断请求 
//	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;   
//	EXTI_Init(&EXTI_InitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);	 
	GPIO_InitTypeDef GPIO_InitStructure;
 
	EXTI_InitTypeDef EXTI_InitStructure;
 	 NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE4,3

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
   //GPIOE.5	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void NVIC_Configuration(void)  //中断设置 （注意先设置关联再设置外部中断）
{
	#ifdef  VECT_TAB_RAM  
	  /* Set the Vector Table base location at 0x20000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
	  /* Set the Vector Table base location at 0x08000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif

	/* Configure the Priority Grouping with 0 bit */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
    /* Enable and set EXTI1 Interrupt with Preemption Priority 1 and Sub
	Priority as 1 */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	 

	/* Configure the Priority Grouping with 1 bit */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	
	/* Enable TIM2 global interrupt with Preemption Priority 1 and Sub
	Priority as 5 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void GPIO_TIM3_Configure(void)
{
	//设置TIM3的外部计数器 PD2  ETR	 （引脚的设置跟硬件有关）
	//重映像不影响在PD2上的TIM3_ETR
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void TIM3_Configure(void)  //定时器中断设置，用于颜色传感器
{
   TIM_DeInit( TIM3);//复位TIM3定时器
   TIM_TimeBaseStructure.TIM_Period = 100;
   TIM_TimeBaseStructure.TIM_Prescaler = 7199; 
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);
   /* Clear TIM2 update pending flag[清除TIM3溢出中断标志] */
   TIM_ClearFlag(TIM3, TIM_FLAG_Update);
   /* Enable TIM2 Update interrupt [TIM3溢出中断允许]*/
   TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);  
   /* TIM2 enable counter [允许tim3计数]*/
   TIM_Cmd(TIM3, DISABLE);
}



void TIM4_Configure() //定时器4设置，用于超声波传感器
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	TIM_DeInit(TIM4);
	
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =71; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	TIM_PrescalerConfig(TIM4,71, TIM_PSCReloadMode_Immediate);   
	
	TIM_ARRPreloadConfig(TIM4,DISABLE);  
	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4,DISABLE); 
	
	
	
	
	
	
	
}

void Tim1_Init(u16 CCR1_Val,u16 CCR2_Val,u16 CCR3_Val,u16 CCR4_Val)	//机械手，1、2、3、4电机对应定时器1相应PWM通道
{ 
  /* TIM1 Peripheral Configuration */
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
   
  TIM_DeInit(TIM1);

  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 71;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 19999;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;           
       
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val; 
          
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;         

  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;          
  
  TIM_OC1Init(TIM1,&TIM_OCInitStructure);
 
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
  TIM_OC2Init(TIM1,&TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
  TIM_OC3Init(TIM1,&TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
  TIM_OC4Init(TIM1,&TIM_OCInitStructure);  
  
  /* TIM1 counter enable */
   TIM_Cmd(TIM1,ENABLE);

   /* Main Output Enable */
   TIM_CtrlPWMOutputs(TIM1,ENABLE);
}  
void Tim2_Init(u16 CCR1_Val,u16 CCR2_Val,u16 CCR3_Val,u16 CCR4_Val)//机械手第五个电机，对应定时器2 PWM1通道
{ 
  /* TIM1 Peripheral Configuration */
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
   
  TIM_DeInit(TIM2);

  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 71;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 19999;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;         

       
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;      
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                
  
  TIM_OC1Init(TIM2,&TIM_OCInitStructure);
 
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
  TIM_OC2Init(TIM2,&TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
  TIM_OC3Init(TIM2,&TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
  TIM_OC4Init(TIM2,&TIM_OCInitStructure);  
  
  /* TIM1 counter enable */
   TIM_Cmd(TIM2,ENABLE);

   /* Main Output Enable */
   TIM_CtrlPWMOutputs(TIM2,ENABLE);
}

void USART1_Configuration(void)	//串口1设置
{
	USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStructure;

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USART1, &USART_ClockInitStructure);

	/* Enables the USART1 transmit interrupt */
	USART_ClearFlag(USART1, USART_FLAG_TC);  
	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);
}
u8 QTI_State(u8 pin)//获取红外值
{
	return (!(GPIO_ReadInputDataBit(GPIOE,pin)));
}

void cfg_Init(void)//初始化函数
{
//	RCC_Configuration();
	
	
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|
	 						RCC_APB2Periph_GPIOB|
							RCC_APB2Periph_GPIOC|
							RCC_APB2Periph_GPIOD|
							RCC_APB2Periph_GPIOE|
							RCC_APB2Periph_AFIO, ENABLE);
	 /* Enable USART1 clocks */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	 /* TIM4 clocks enable */
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	
	//NVIC_Configuration();
	

	

//	GPIO_PWM_Configuration();

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	 /*PWM_TEST2*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//	GPIO_QTI_Configure();
	/* Trig */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //开漏输出
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
//	GPIO_ult_Configure();
	/* Echo */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
	GPIO_Init(GPIOE,&GPIO_InitStructure);


//	GPIO_motor_Configure();

/* GPIOE Configuration: Channel 1/1N, 2/2N, 3/3N and 4 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;	                   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	/* TIM1 Full remapping pins */
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	                   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);


//	GPIO_TCS230_1_Configure();
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_Init(GPIOC, &GPIO_InitStructure); 	


//	GPIO_TCS230_2_Configure();

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
//	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	//TIM4_Configure();
	
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =71; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ARRPreloadConfig(TIM4,DISABLE);  
	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4,DISABLE); 
//	
//	//EXTI9_5_Configure();
//	//TIM3_Configure();
//	
	//TIM3_PWM_Init(20000-1,72-1);

//	

}
/*******************************************************************************
* Function Name  : fputc
* Description    : Retargets the C library printf function to the USART.
*******************************************************************************/
//int fputc(int ch, FILE *f)
//{
//	/* Place your implementation of fputc here */
//	/* e.g. write a character to the USART */
//	USART_SendData(USART1, (u8) ch);
//	/* Loop until the end of transmission */
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) ;  // waiting here
//	
//	return ch;
//}


