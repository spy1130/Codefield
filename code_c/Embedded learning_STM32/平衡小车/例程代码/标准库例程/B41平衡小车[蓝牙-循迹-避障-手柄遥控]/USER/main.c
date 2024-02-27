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

u8 CTRL_MODE=98,Mode_Change=0; //97超声波避障；98蓝牙；99红外循迹；100 PS2
int Uart_Receive=0;
u8 TkSensor=0;

int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;
u8 PS2_Plugin=0;

/***********************************************************************/
int main(void)	
{ 
	LED_Init();                    //=====初始化与 LED 连接的IO
	PlugIn_Init();										 //=====初始化与 USB 连接的IO
	KEY_Init();                    //=====初始化与按键连接的IO
	delay_init();	    	           //=====延时函数初始化	
	uart1_init(115200);	          	 //=====串口1初始化
	delay_ms(100);
	NVIC_Configuration();					 //=====中断优先级分组,其中包含了所有的中断优先级的配置,方便管理和一次性修改。
	Adc_Init();                    //=====初始化ADC
	Encoder_Init_TIM2();           //=====初始化编码器
	Encoder_Init_TIM3();
	OLED_Init();                   //=====OLED初始化

	MPU_Init();					    			 //=====初始化MPU6050
	mpu_dmp_init();								 //=====初始化MPU6050的DMP模式					 
	TIM1_PWM_Init(7199,0);   			 //=====初始化PWM 10KHZ,用于驱动电机。 
	delay_ms(1000);								 //=====延时1s 解决小车上电轮子乱转的问题
	Motor_Init();									 //=====初始化与电机连接的硬件IO接口 
	MPU6050_EXTI_Init();					 //=====MPU6050 5ms定时中断初始化
	
	Mode_Init();
	
  while(1)
	{
		if(Mode_Change)
		{
			Mode_Change=0;
			Mode_Init();
		}
		oled_show();
		if(CTRL_MODE==100)
		{
			PS2_LX=PS2_AnologData(PSS_LX);    //PS2数据采集    
			PS2_LY=PS2_AnologData(PSS_LY);
			PS2_RX=PS2_AnologData(PSS_RX);
			PS2_RY=PS2_AnologData(PSS_RY);
			PS2_KEY=PS2_DataKey();
			if(PS2_LX>=250 && PS2_LY>=250 && PS2_RX>=250 && PS2_RY>=250)//判断避免接收器未插入或未连接导致失控
				PS2_Plugin=0;
			else
				PS2_Plugin=1;
		}
	}
}
void Mode_Init(void)//根据模式不同进行IO初始化
{
	OLED_Clear();									//OLED清屏
	oled_first_show();						//只需要显示一次的字符,在此刷新一次即可。
	switch(CTRL_MODE)
	{
		case 97:
			SR04_Configuration();
			Timer4_Init(5000,7199);	    	 //超声波定时器初始化
			break;
		case 98:
			uart2_init(9600);							 //串口2初始化即蓝牙初始化
			break;
		case 99:
			Tracking_Init();
			break;
		case 100:
			PS2_Init();
			break;
	}
}
void Tracking_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PA端口时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	           //端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOA GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PA端口时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;	           //端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOA 
} 
