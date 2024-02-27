 /**************************************************************************
 作  者 ：大鱼电子
 淘宝地址：https://shop119207236.taobao.com
**************************************************************************/
#include "sys.h"
/****************************全局变量*************************************/    
float Voltage;  															 //电池电压采样相关的变量
float pitch,roll,yaw; 								  			 //欧拉角(姿态角)
short aacx,aacy,aacz;													 //加速度传感器原始数据
short gyrox,gyroy,gyroz;											 //陀螺仪原始数据
float SR04_Distance;                 //超声波测距

int   Encoder_Left,Encoder_Right;         		 //左右编码器的脉冲计数
int 	Moto1=0,Moto2=0;												 //计算出来的最终赋给电机的PWM


/***********************************************************************/
int main(void)	
{ 
	LED_Init();                    //=====初始化与 LED 连接的IO

	delay_init();	    	           //=====延时函数初始化	
	uart1_init(115200);	          	 //=====串口1初始化
	delay_ms(100);
	NVIC_Configuration();					 //=====中断优先级分组,其中包含了所有的中断优先级的配置,方便管理和一次性修改。

	Encoder_Init_TIM2();           //=====初始化编码器
	Encoder_Init_TIM3();
	OLED_Init();                   //=====OLED初始化

	MPU_Init();					    			 //=====初始化MPU6050
	mpu_dmp_init();								 //=====初始化MPU6050的DMP模式					 
	TIM1_PWM_Init(7199,0);   			 //=====初始化PWM 10KHZ,用于驱动电机。 
	delay_ms(1000);								 //=====延时1s 解决小车上电轮子乱转的问题
	Motor_Init();									 //=====初始化与电机连接的硬件IO接口 
	MPU6050_EXTI_Init();					 //=====MPU6050 5ms定时中断初始化
	

	
  while(1)
	{
		
		oled_show();}
		
}
