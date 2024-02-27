#include "stm32f10x.h"// Device header
#include "sys.h"

int PWM_MAX=7000,PWM_MIN=-7000;//PWM�޷�����
float pitch, roll, yaw;    // ŷ���ǣ���̬�ǣ�������
short gyrox, gyroy, gyroz; // ����������
short aacx, aacy, aacz;    // ���ٶ�
int Encoder_Left, Encoder_Right;//���������ݣ��ٶȣ�
int MOTO1,MOTO2;       //���װ�ر���

int main(void)
{	delay_init();	    	         //��ʱ������ʼ��
	uart1_init(115200);	          	 //����1��ʼ��
	delay_ms(100);
	NVIC_Configuration();			//�ж����ȼ�����,���а��������е��ж����ȼ�������,��������һ�����޸ġ�
	
	Encoder_TIM2_Init();
	Encoder_TIM3_Init();
	
	OLED_Init();
	OLED_Clear();
	
	MPU_Init();	
	mpu_dmp_init();			//��ʼ��MPU6050��DMPģʽ	
	
	PWM_Init_TIM1(7199,0);
	delay_ms(1000);					//��ʱ1s ���С���ϵ�������ת������
	Motor_Init();
	MPU6050_EXTI_Init();

	
	
	
	while (1)
	{
		OLED_Float(0,0,pitch,3);
	}
}
