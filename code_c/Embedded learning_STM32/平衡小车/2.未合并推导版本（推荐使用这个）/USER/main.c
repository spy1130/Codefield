/*
B站：天下行走?_?（B站起的名字在MDK里显示不全，若要了解，在大文件的word文档的最后可以找到）		
知乎：天下行走							
CSDN：技术创造无限可能				
闲鱼：抉择之刃银

新创建QQ群：822263013
有兴趣的同学请加群交流。
*/

/*
【未推导的串级PID控制器】版本的平衡小车控制。
现在只需要在"control.c"里面修改机械中值角度就可以了。
*/




#include "stm32f10x.h"
#include "sys.h" 

float Pitch,Roll,Yaw;						//角度
short gyrox,gyroy,gyroz;				//陀螺仪--角速度
short aacx,aacy,aacz;						//加速度
int Encoder_Left,Encoder_Right;	//编码器数据（速度）

int PWM_MAX=7200,PWM_MIN=-7200;	//PWM限幅变量
int MOTO1,MOTO2;								//电机装载变量

extern int Vertical_out,Velocity_out,Turn_out;
int main(void)	
{
	delay_init();
	NVIC_Config();
	uart1_init(115200);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); 
	
	OLED_Init();
	OLED_Clear();
	
	MPU_Init();
	mpu_dmp_init();
	MPU6050_EXTI_Init();
	
	Encoder_TIM2_Init();
	Encoder_TIM4_Init();
	Motor_Init();
	PWM_Init_TIM1(0,7199);
  while(1)	
	{
		OLED_Float(0,0,Pitch,1);
		OLED_Float(50,50,Velocity_out,1);
//		OLED_ShowNumber(30,0,Velocity_out,3,16);//OK
	} 	
}



