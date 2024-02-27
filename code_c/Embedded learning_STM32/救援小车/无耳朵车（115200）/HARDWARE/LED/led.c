//#include "led.h"

////////////////////////////////////////////////////////////////////////////////////	 
////本程序只供学习使用，未经作者许可，不得用于其它任何用途
////ALIENTEK 精英STM32开发板
////LED驱动代码	   
////正点原子@ALIENTEK
////技术论坛:www.openedv.com
////修改日期:2012/9/2
////版本：V1.0
////版权所有，盗版必究。
////Copyright(C) 广州市星翼电子科技有限公司 2009-2019
////All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////// 	   

#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK 精英STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   
/////////////////////////////////////////
void Delay_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PORTG口时钟 
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//PORTG.11 推挽输出
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOD,GPIO_Pin_8);    //输出1
}











///////////////////////
//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
//	 //led pd7	
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //LED0-->PB.5 端口配置
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//	 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
//	 GPIO_SetBits(GPIOD,GPIO_Pin_7);						 //PB.5 输出高
//	
//	//trig pa4 pa5
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;				 //LED0-->PB.5 端口配置
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
//	 
//	//trig pd2
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //LED0-->PB.5 端口配置
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//	 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
//	 
//	 //echo pa1  pa2  pa3
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;				 //LED0-->PB.5 端口配置
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //推挽输出
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
//	 
//	 //fire sense pb0,pb1
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;				 //LED0-->PB.5 端口配置
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //推挽输出
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//	 GPIO_Init(GPIOD, &GPIO_InitStructure);		

//	//motor pc5
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//	 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	 
	  //start key pc0
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PB.5 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	 GPIO_SetBits(GPIOA,GPIO_Pin_0); 						 //Pc0 上拉，pc0 至位1 上拉， 置位0 下拉，这里需要上拉，所以置位1；
	 
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
//	 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
//	 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 输出高 

}


void check_io_ini(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB,PE端口时钟
	
	//GPIOC  4,5,7初始化设置
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //LED0-->PA.11 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //fukong输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //LED0-->PA.11 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //fukong输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //光电检测2#3#
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		//fukong输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	
	
	
	//GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10设置高，灯灭

}

void tlr230_s0_s1_ini(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
		
		
		
		//光圈100倍控制端口a6,a7
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2;				 //LED0-->PB.5 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	
	 //default s1,s0=0,1,mean *1,11 means *100;
		s0_1=1;
		s1_1=0;
		
		s0_2=1;
		s1_2=0;
		
		s0_3=1;
		s1_3=0;
		
		s0_4=1;
		s1_4=0;
		
		s0_5=1;
		s1_5=0;
		
		
		
		
	 
	 
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//tim4 etr pe0
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	 GPIO_Init(GPIOE, &GPIO_InitStructure); 
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//tim3 etr pd2 
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	 GPIO_Init(GPIOD, &GPIO_InitStructure); 
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//tim1 etr pa12
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	 GPIO_Init(GPIOA, &GPIO_InitStructure); 
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //tim8 etr pa0
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}



void step_motor_ini(void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
 	
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PB,PE端口时钟
		
		
		
		//GPIOD  3,
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//tim3 etr pd2 
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	 GPIO_Init(GPIOD, &GPIO_InitStructure); 
     GPIO_ResetBits(GPIOD,GPIO_Pin_3);//resetBits means pull down ;setBits means pull up
	//GPIOD  4,
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
	
	
	//GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10设置高，灯灭
	
}
















