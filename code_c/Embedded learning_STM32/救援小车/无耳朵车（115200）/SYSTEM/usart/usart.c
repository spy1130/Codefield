#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  


short  YAW=0,PITCH=0,ROLL=0;

u8   gy_5_flag=0;

u8 rx_num;
u8 rx_buff[20]={0};

u8 rx_end_flag=0x00;//1 is mean that serious port  recieve a good frame,0 mean that is recieving ;
u8 rx_star_flag=0x00;//1 mean the 0xa5 has already recieved,0 mean that we are waiting for the beginning of series port

u8 rx_num_1=0;
u8 rx_buff_1[20]={0};

u8 rx_end_flag_1=0x00;//1 is mean that serious port  recieve a good frame,0 mean that is recieving ;
u8 rx_star_flag_1=0x00;//1 mean the 0xa5 has already recieved,0 mean that we are waiting for the beginning of series port

u8 res;


void uart3_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
//	

//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOC,ENABLE);
//	
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     /*使能SWD 禁用JTAG*/ 
//	
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //LED0-->PA.12 端口配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		//fukong输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5

//	
//	
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);//要使用PA15、PB3、PA2、PA3的端口组合，要进行部分重映射




	     //GPIO_PartialRemap_USART3
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE);	//使能USART2，
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC ,ENABLE);//GPIOA时钟
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOC,ENABLE);
	
	//USART3_TX   GPIOC.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PD.5
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOD.5
   
  //USART3_RX	  GPIOB.11初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PD6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOD.6  
	
	//GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);//要使用进行重映射
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);//要使用PA15、PB3、PA2、PA3的端口组合，要进行部分重映射

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART3, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口1 

}


void USART3_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART3);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 

////////////////////
////////
////////////////
void uart2_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
//	

//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOC,ENABLE);
//	
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     /*使能SWD 禁用JTAG*/ 
//	
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //LED0-->PA.12 端口配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		//fukong输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5

//	
//	
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);//要使用PA15、PB3、PA2、PA3的端口组合，要进行部分重映射




	     //GPIO_PartialRemap_USART3
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);	//使能USART2，
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO ,ENABLE);//GPIOA时钟
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOC,ENABLE);
	
	//USART2_TX   GPIOD.5
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //PD.5
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD.5
   
  //USART2_RX	  GPIOD.6初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PD6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD.6  
	
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);//要使用进行重映射
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);//要使用PA15、PB3、PA2、PA3的端口组合，要进行部分重映射

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART2, ENABLE);                    //使能串口1 

}


void USART2_IRQHandler(void)                	//串口1中断服务程序
	{
	
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
		
		u8 res_1;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		 res =USART_ReceiveData(USART2);	//读取接收到的数据
            

		switch(rx_num_1)
			{
			  case 0://a5
			 {
				 if(0x24==res_1)
				 {
					 rx_buff_1[0]=res_1;
					 rx_num_1=0x01;
					 rx_end_flag_1=0x00;
					 rx_star_flag_1=0x01;
				 }
				 else
				 {
					 rx_buff_1[0]=0x00;
					 rx_buff_1[1]=0x00;
					 rx_buff_1[2]=0x00;
					 rx_buff_1[3]=0x00;
					 rx_buff_1[4]=0x00;
					 rx_buff_1[5]=0x00;
					 rx_buff_1[6]=0x00;
					 
					 rx_num_1=0x00;
					 rx_end_flag_1=0x00;
					 rx_star_flag_1=0x00;
				 }
				 
				 
				 break;
			 } 
			 case 1://a,b,c,d,e;;;;f,g,h,i,j,k
			 {
//				 if(0x5a==res_1)
//				 {
//					 rx_buff_1[1]=res_1;
//					 rx_num_1=0x02;
//					 rx_end_flag_1=0x00;
//					 rx_star_flag_1=0x01;
//				 }
//				 else
//				 {
//					 rx_buff_1[0]=0x00;
//					 rx_buff_1[1]=0x00;
//					 rx_buff_1[2]=0x00;
//					 rx_buff_1[3]=0x00;
//					 rx_buff_1[4]=0x00;
//					 rx_buff_1[5]=0x00;
//					 rx_buff_1[6]=0x00;
//					 
//					 rx_num_1=0x00;
//					 rx_end_flag_1=0x00;
//					 rx_star_flag_1=0x00;
//				 }

         rx_buff_1[1]=res_1;
				 rx_num_1=0x02;
				 rx_end_flag_1=0x00;
				 rx_star_flag_1=0x01;
				 
				 break;
			 }
			case 2://05
			 {
//				 if(0x05==res_1)
//				 {
//					 rx_buff_1[2]=res_1;
//					 rx_num_1=0x03;
//					 rx_end_flag_1=0x00;
//					 rx_star_flag_1=0x01;
//				 }
//				 else
//				 {
//					 rx_buff_1[0]=0x00;
//					 rx_buff_1[1]=0x00;
//					 rx_buff_1[2]=0x00;
//					 rx_buff_1[3]=0x00;
//					 rx_buff_1[4]=0x00;
//					 rx_buff_1[5]=0x00;
//					 rx_buff_1[6]=0x00;
//					 
//					 rx_num_1=0x00;
//					 rx_end_flag_1=0x00;
//					 rx_star_flag_1=0x00;
//				 }
         rx_buff_1[2]=res_1;
				 rx_num_1=0x03;
				 rx_end_flag_1=0x00;
				 rx_star_flag_1=0x01;
				 
				 break;
			 }


			 case 3://f1,f2,f3
			 {
				 rx_buff_1[3]=res_1;
				 rx_num_1=0x04;
				 rx_end_flag_1=0x00;
				 rx_star_flag_1=0x01;
				 break;
			 }


//			 case 4://value
//			 {
//				 rx_buff_1[4]=res_1;
//				 rx_num_1=0x05;
//				 rx_end_flag_1=0x00;
//				 rx_star_flag_1=0x01;
//				 break;
//			 }


//			 case 5://check_sum
//			 {
//				 rx_buff_1[5]=res_1;
//				 rx_num_1=0x06;
//				 rx_end_flag_1=0x00;
//				 rx_star_flag_1=0x01;
//				 break;
//			 }
			 
			
			 case 4:
			 {
				  if(0x40==res_1)
				 {
					 rx_buff_1[4]=res_1;
					 rx_num_1=0x00;
					 rx_star_flag_1=0x00;
					
					 rx_end_flag_1=0x01;

//					 if(rx_buff_1[3]==0xf1)
//					 {
//						TIM_SetCompare1(TIM1,1500+(rx_buff_1[4]*10-1500));

//						TIM_SetCompare2(TIM1,1500-(rx_buff_1[4]*10-1500));

//					 }
//					 else if(rx_buff_1[3]==0xf2)
//					 {
//						 TIM_SetCompare3(TIM1,rx_buff_1[4]*10);

//					 }

//					 else if(rx_buff_1[3]==0xf3)
//					 {
//						TIM_SetCompare4(TIM1,rx_buff_1[4]*10);
//					 }
//					 
//					 else if(rx_buff_1[3]==0xf4)
//					 {
//						TIM_SetCompare1(TIM2,rx_buff_1[4]*10+4);
//					 }
//					 
//					 else if(rx_buff_1[3]==0xf5)
//					 {
//						TIM_SetCompare1(TIM2,rx_buff_1[4]*10+6);
//					 }

 
				 }
				 else
				 {
					 rx_buff_1[0]=0x00;
					 rx_buff_1[1]=0x00;
					 rx_buff_1[2]=0x00;
					 rx_buff_1[3]=0x00;
					 rx_buff_1[4]=0x00;
					 rx_buff_1[5]=0x00;
					 rx_buff_1[6]=0x00;
					 
					 rx_num_1=0x00;
					 rx_end_flag_1=0x00;
					 rx_star_flag_1=0x00;
							 
							 
				 }
				 
				 break;
			 }

			 default:
			 {

				 rx_buff_1[0]=0x00;
				 rx_buff_1[1]=0x00;
				 rx_buff_1[2]=0x00;
				 rx_buff_1[3]=0x00;
				 rx_buff_1[4]=0x00;
				 rx_buff_1[5]=0x00;
				 rx_buff_1[6]=0x00;
				 
				 rx_num_1=0x00;
				 rx_end_flag_1=0x00;
				 rx_star_flag_1=0x00;

			 }
			 
		 }
			
			
			

			
			
		 } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 








/////////////////////
////////
/////////////////

  
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX   GPIOA.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}


//u8 rx_buff[10]={0};
//u8 rx_end_flag=0x00;//1 is mean that serious port  recieve a good frame,0 mean that is recieving ;
//u8 rx_star_flag=0x00;//1 mean the 0xa5 has already recieved,0 mean that we are waiting for the beginning of series port
//u8 rx_num=0;
u8 row_cloumn_all=0;
u8 row_num=0;
u8 cloumn_num=0;
u8 command_text=0x00;

u8 measure_distance_flag=0;
u8 direction_flag=0;
u8 measure_delay_count=0;



void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 res_1;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			res_1=USART_ReceiveData(USART1);	//读取接收到的数据
		
			
			
			
			
			switch(rx_num_1)
			{
			  case 0://a5
			 {
				 if(0x24==res_1)
				 {
					 rx_buff_1[0]=res_1;
					 rx_num_1=0x01;
					 rx_end_flag_1=0x00;
					 rx_star_flag_1=0x01;
				 }
				 else
				 {
					 rx_buff_1[0]=0x00;
					 rx_buff_1[1]=0x00;
					 rx_buff_1[2]=0x00;
					 rx_buff_1[3]=0x00;
					 rx_buff_1[4]=0x00;
					 rx_buff_1[5]=0x00;
					 rx_buff_1[6]=0x00;
					 
					 rx_num_1=0x00;
					 rx_end_flag_1=0x00;
					 rx_star_flag_1=0x00;
				 }
				 
				 
				 break;
			 } 
			 case 1://a,b,c,d,e;;;;f,g,h,i,j,k
			 {
//				 if(0x5a==res_1)
//				 {
//					 rx_buff_1[1]=res_1;
//					 rx_num_1=0x02;
//					 rx_end_flag_1=0x00;
//					 rx_star_flag_1=0x01;
//				 }
//				 else
//				 {
//					 rx_buff_1[0]=0x00;
//					 rx_buff_1[1]=0x00;
//					 rx_buff_1[2]=0x00;
//					 rx_buff_1[3]=0x00;
//					 rx_buff_1[4]=0x00;
//					 rx_buff_1[5]=0x00;
//					 rx_buff_1[6]=0x00;
//					 
//					 rx_num_1=0x00;
//					 rx_end_flag_1=0x00;
//					 rx_star_flag_1=0x00;
//				 }

         rx_buff_1[1]=res_1;
				 rx_num_1=0x02;
				 rx_end_flag_1=0x00;
				 rx_star_flag_1=0x01;
				 
				 break;
			 }
			case 2://05
			 {
//				 if(0x05==res_1)
//				 {
//					 rx_buff_1[2]=res_1;
//					 rx_num_1=0x03;
//					 rx_end_flag_1=0x00;
//					 rx_star_flag_1=0x01;
//				 }
//				 else
//				 {
//					 rx_buff_1[0]=0x00;
//					 rx_buff_1[1]=0x00;
//					 rx_buff_1[2]=0x00;
//					 rx_buff_1[3]=0x00;
//					 rx_buff_1[4]=0x00;
//					 rx_buff_1[5]=0x00;
//					 rx_buff_1[6]=0x00;
//					 
//					 rx_num_1=0x00;
//					 rx_end_flag_1=0x00;
//					 rx_star_flag_1=0x00;
//				 }
         rx_buff_1[2]=res_1;
				 rx_num_1=0x03;
				 rx_end_flag_1=0x00;
				 rx_star_flag_1=0x01;
				 
				 break;
			 }


			 case 3://f1,f2,f3
			 {
				 rx_buff_1[3]=res_1;
				 rx_num_1=0x04;
				 rx_end_flag_1=0x00;
				 rx_star_flag_1=0x01;
				 break;
			 }


//			 case 4://value
//			 {
//				 rx_buff_1[4]=res_1;
//				 rx_num_1=0x05;
//				 rx_end_flag_1=0x00;
//				 rx_star_flag_1=0x01;
//				 break;
//			 }


//			 case 5://check_sum
//			 {
//				 rx_buff_1[5]=res_1;
//				 rx_num_1=0x06;
//				 rx_end_flag_1=0x00;
//				 rx_star_flag_1=0x01;
//				 break;
//			 }
			 
			
			 case 4:
			 {
				  if(0x40==res_1)
				 {
					 rx_buff_1[4]=res_1;
					 rx_num_1=0x00;
					 rx_star_flag_1=0x00;
					
					 rx_end_flag_1=0x01;

//					 if(rx_buff_1[3]==0xf1)
//					 {
//						TIM_SetCompare1(TIM1,1500+(rx_buff_1[4]*10-1500));

//						TIM_SetCompare2(TIM1,1500-(rx_buff_1[4]*10-1500));

//					 }
//					 else if(rx_buff_1[3]==0xf2)
//					 {
//						 TIM_SetCompare3(TIM1,rx_buff_1[4]*10);

//					 }

//					 else if(rx_buff_1[3]==0xf3)
//					 {
//						TIM_SetCompare4(TIM1,rx_buff_1[4]*10);
//					 }
//					 
//					 else if(rx_buff_1[3]==0xf4)
//					 {
//						TIM_SetCompare1(TIM2,rx_buff_1[4]*10+4);
//					 }
//					 
//					 else if(rx_buff_1[3]==0xf5)
//					 {
//						TIM_SetCompare1(TIM2,rx_buff_1[4]*10+6);
//					 }

 
				 }
				 else
				 {
					 rx_buff_1[0]=0x00;
					 rx_buff_1[1]=0x00;
					 rx_buff_1[2]=0x00;
					 rx_buff_1[3]=0x00;
					 rx_buff_1[4]=0x00;
					 rx_buff_1[5]=0x00;
					 rx_buff_1[6]=0x00;
					 
					 rx_num_1=0x00;
					 rx_end_flag_1=0x00;
					 rx_star_flag_1=0x00;
							 
							 
				 }
				 
				 break;
			 }

			 default:
			 {

				 rx_buff_1[0]=0x00;
				 rx_buff_1[1]=0x00;
				 rx_buff_1[2]=0x00;
				 rx_buff_1[3]=0x00;
				 rx_buff_1[4]=0x00;
				 rx_buff_1[5]=0x00;
				 rx_buff_1[6]=0x00;
				 
				 rx_num_1=0x00;
				 rx_end_flag_1=0x00;
				 rx_star_flag_1=0x00;

			 }
			 
		 }
			
			
			

			
			
		} 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	

