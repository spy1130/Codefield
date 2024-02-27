 /**************************************************************************
 ��  �� ���������
 �Ա���ַ��https://shop119207236.taobao.com
**************************************************************************/
#include "sys.h"
/****************************ȫ�ֱ���*************************************/    
float Voltage;  															 //��ص�ѹ������صı���
float pitch,roll,yaw; 								  			 //ŷ����(��̬��)
short aacx,aacy,aacz;													 //���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;											 //������ԭʼ����
float SR04_Distance;                 //���������

int   Encoder_Left,Encoder_Right;         		 //���ұ��������������
int 	Moto1=0,Moto2=0;												 //������������ո��������PWM

u8 CTRL_MODE=98,Mode_Change=0; //97���������ϣ�98������99����ѭ����100 PS2
int Uart_Receive=0;
u8 TkSensor=0;

int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;
u8 PS2_Plugin=0;

/***********************************************************************/
int main(void)	
{ 
	LED_Init();                    //=====��ʼ���� LED ���ӵ�IO
	PlugIn_Init();										 //=====��ʼ���� USB ���ӵ�IO
	KEY_Init();                    //=====��ʼ���밴�����ӵ�IO
	delay_init();	    	           //=====��ʱ������ʼ��	
	uart1_init(115200);	          	 //=====����1��ʼ��
	delay_ms(100);
	NVIC_Configuration();					 //=====�ж����ȼ�����,���а��������е��ж����ȼ�������,��������һ�����޸ġ�
	Adc_Init();                    //=====��ʼ��ADC
	Encoder_Init_TIM2();           //=====��ʼ��������
	Encoder_Init_TIM3();
	OLED_Init();                   //=====OLED��ʼ��

	MPU_Init();					    			 //=====��ʼ��MPU6050
	mpu_dmp_init();								 //=====��ʼ��MPU6050��DMPģʽ					 
	TIM1_PWM_Init(7199,0);   			 //=====��ʼ��PWM 10KHZ,������������� 
	delay_ms(1000);								 //=====��ʱ1s ���С���ϵ�������ת������
	Motor_Init();									 //=====��ʼ���������ӵ�Ӳ��IO�ӿ� 
	MPU6050_EXTI_Init();					 //=====MPU6050 5ms��ʱ�жϳ�ʼ��
	
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
			PS2_LX=PS2_AnologData(PSS_LX);    //PS2���ݲɼ�    
			PS2_LY=PS2_AnologData(PSS_LY);
			PS2_RX=PS2_AnologData(PSS_RX);
			PS2_RY=PS2_AnologData(PSS_RY);
			PS2_KEY=PS2_DataKey();
			if(PS2_LX>=250 && PS2_LY>=250 && PS2_RX>=250 && PS2_RY>=250)//�жϱ��������δ�����δ���ӵ���ʧ��
				PS2_Plugin=0;
			else
				PS2_Plugin=1;
		}
	}
}
void Mode_Init(void)//����ģʽ��ͬ����IO��ʼ��
{
	OLED_Clear();									//OLED����
	oled_first_show();						//ֻ��Ҫ��ʾһ�ε��ַ�,�ڴ�ˢ��һ�μ��ɡ�
	switch(CTRL_MODE)
	{
		case 97:
			SR04_Configuration();
			Timer4_Init(5000,7199);	    	 //��������ʱ����ʼ��
			break;
		case 98:
			uart2_init(9600);							 //����2��ʼ����������ʼ��
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��PA�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	           //�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOA GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PA�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;	           //�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOA 
} 
