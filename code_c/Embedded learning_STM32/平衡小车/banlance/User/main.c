#include "stm32f10x.h"// Device header
#include "sys.h"

int PWM_MAX=7000,PWM_MIN=-7000;//PWM限幅变量
float pitch, roll, yaw;    // 欧拉角（姿态角）的数据
short gyrox, gyroy, gyroz; // 陀螺仪数据
short aacx, aacy, aacz;    // 加速度
int Encoder_Left, Encoder_Right;//编码器数据（速度）
int MOTO1,MOTO2;       //电机装载变量

int main(void)
{	delay_init();	    	         //延时函数初始化
	uart1_init(115200);	          	 //串口1初始化
	delay_ms(100);
	NVIC_Configuration();			//中断优先级分组,其中包含了所有的中断优先级的配置,方便管理和一次性修改。
	
	Encoder_TIM2_Init();
	Encoder_TIM3_Init();
	
	OLED_Init();
	OLED_Clear();
	
	MPU_Init();	
	mpu_dmp_init();			//初始化MPU6050的DMP模式	
	
	PWM_Init_TIM1(7199,0);
	delay_ms(1000);					//延时1s 解决小车上电轮子乱转的问题
	Motor_Init();
	MPU6050_EXTI_Init();

	
	
	
	while (1)
	{
		OLED_Float(0,0,pitch,3);
	}
}
