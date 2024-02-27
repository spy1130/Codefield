#include "control.h"
#include "usart2.h"
/**************************************************************************
 ��  �� ���������
 �Ա���ַ��https://shop119207236.taobao.com
**************************************************************************/
/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
         5ms��ʱ�ж���MPU6050��INT���Ŵ���
         �ϸ�֤���������ݴ����ʱ��ͬ��	
				 ��MPU6050�Ĳ���Ƶ�������У����ó�100HZ�����ɱ�֤6050��������10ms����һ�Ρ�
				 ���߿���imv_mpu.h�ļ���26�еĺ궨������޸�(#define DEFAULT_MPU_HZ  (100))
**************************************************************************/
#define SPEED_Y 100 //����(ǰ��)����趨�ٶ�
#define SPEED_Z 80//ƫ��(����)����趨�ٶ� 

int Balance_Pwm,Velocity_Pwm,Turn_Pwm,Turn_Kp;

float Mechanical_angle=0; 
float Target_Speed=0;	//�����ٶȣ������������ڿ���С��ǰ�����˼����ٶȡ�
float Turn_Speed=0;		//�����ٶȣ�ƫ����

//��Բ�ͬ���Ͳ�������sys.h������define�ĵ������
float balance_UP_KP=BLC_KP; 	 // С��ֱ����PD����
float balance_UP_KD=BLC_KD;

float velocity_KP=SPD_KP;     // С���ٶȻ�PI����
float velocity_KI=SPD_KI;

float Turn_Kd=TURN_KD;//ת��KP��KD
float Turn_KP=TURN_KP;



void EXTI9_5_IRQHandler(void) 
{
	static u8 Voltage_Counter=0;
	if(PBin(5)==0)
	{
		EXTI->PR=1<<5;                                           //���LINE5�ϵ��жϱ�־λ   
		mpu_dmp_get_data(&pitch,&roll,&yaw);										 //�õ�ŷ���ǣ���̬�ǣ�������
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);								 //�õ�����������
		Encoder_Left=Read_Encoder(2);                            //��ȡ��������ֵ����֤�������һ��
		Encoder_Right=-Read_Encoder(3);                          //��ȡ��������ֵ
		Led_Flash(100);
		
		Voltage_Counter++;
		if(Voltage_Counter==20)									 									//100ms��ȡһ�ε�ѹ
		{
			Voltage_Counter=0;
			Voltage=Get_battery_volt();		                          //��ȡ��ص�ѹ
		}
		
		if(KEY_Press(100))																				//���������л�ģʽ������ģʽ�л���ʼ��
		{
			if(++CTRL_MODE>=101) CTRL_MODE=97;
			Mode_Change=1;
		}
		
		Get_RC();
			
		Target_Speed=Target_Speed>SPEED_Y?SPEED_Y:(Target_Speed<-SPEED_Y?(-SPEED_Y):Target_Speed);//�޷�
		Turn_Speed=Turn_Speed>SPEED_Z?SPEED_Z:(Turn_Speed<-SPEED_Z?(-SPEED_Z):Turn_Speed);//�޷�( (20*100) * 100)
			
		Balance_Pwm =balance_UP(pitch,Mechanical_angle,gyroy);   							//===ֱ����PID����	
		Velocity_Pwm=velocity(Encoder_Left,Encoder_Right,Target_Speed);       //===�ٶȻ�PID����	 
		Turn_Pwm =Turn_UP(gyroz,Turn_Speed);        													//===ת��PID����
		Moto1=Balance_Pwm-Velocity_Pwm+Turn_Pwm;  	            	//===�������ֵ������PWM
		Moto2=Balance_Pwm-Velocity_Pwm-Turn_Pwm;                  //===�������ֵ������PWM
	  Xianfu_Pwm();  																					  //===PWM�޷�
		Turn_Off(pitch,12);																 			  //===���Ƕ��Լ���ѹ�Ƿ�����
		Set_Pwm(Moto1,Moto2);                                     //===��ֵ��PWM�Ĵ���  
	}
}

/**************************************************************************
�������ܣ�ֱ��PD����
��ڲ������Ƕȡ���еƽ��Ƕȣ���е��ֵ�������ٶ�
����  ֵ��ֱ������PWM
��    �ߣ��������
**************************************************************************/
int balance_UP(float Angle,float Mechanical_balance,float Gyro)
{  
   float Bias;
	 int balance;
	 Bias=Angle-Mechanical_balance;    							 //===���ƽ��ĽǶ���ֵ�ͻ�е���
	 balance=balance_UP_KP*Bias+balance_UP_KD*Gyro;  //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance;
}

/**************************************************************************
�������ܣ��ٶ�PI����
��ڲ����������������ֵ
����  ֵ���ٶȿ���PWM
��    �ߣ��������
**************************************************************************/
int velocity(int encoder_left,int encoder_right,int Target_Speed)
{  
    static float Velocity,Encoder_Least,Encoder;
	  static float Encoder_Integral;
   //=============�ٶ�PI������=======================//	
		Encoder_Least =(Encoder_Left+Encoder_Right);//-target;                    //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶ� 
		Encoder *= 0.8;		                                                //===һ�׵�ͨ�˲���       
		Encoder += Encoder_Least*0.2;	                                    //===һ�׵�ͨ�˲���    
		Encoder_Integral +=Encoder;                                       //===���ֳ�λ�� ����ʱ�䣺10ms
		Encoder_Integral=Encoder_Integral - Target_Speed;                       //===����ң�������ݣ�����ǰ������
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===�����޷�
		if(Encoder_Integral<-10000)		Encoder_Integral=-10000;            //===�����޷�	
		Velocity=Encoder*velocity_KP+Encoder_Integral*velocity_KI;        //===�ٶȿ���	
	  if(pitch<-40||pitch>40) 			Encoder_Integral=0;     						//===����رպ��������
	  return Velocity;
}
/**************************************************************************
�������ܣ�ת��PD����
��ڲ����������������ֵ��Z����ٶ�
����  ֵ��ת�����PWM
��    �ߣ��������
**************************************************************************/

int Turn_UP(int gyro_Z, int RC)
{
	int PWM_out;
		/*ת��Լ��*/
	if(RC==0)Turn_Kd=TURN_KD;//��������ת��ָ�����ת��Լ��
	else Turn_Kd=0;//������ת��ָ����յ�����ȥ��ת��Լ��
	
	PWM_out=Turn_Kd*gyro_Z + Turn_KP*RC;
	return PWM_out;
}

void Tracking()
{
	TkSensor=0;
	TkSensor+=(C1<<3);
	TkSensor+=(C2<<2);
	TkSensor+=(C3<<1);
	TkSensor+=C4;
}
void Get_RC()
{
	static u8 SR04_Counter =0;
	static float RATE_VEL = 1;
	float RATE_TURN = 1.6;
	float LY,RX;      //PS2�ֱ����Ʊ���
	int Yuzhi=2;  		//PS2���Ʒ�����ֵ
	switch(CTRL_MODE)
	{
		case 97:
			SR04_Counter++;
			if(SR04_Counter>=20)									         //100ms��ȡһ�γ�����������
			{
				SR04_Counter=0;
				SR04_StartMeasure();												 //��ȡ��������ֵ
			}
			if(SR04_Distance<=30){
			 Target_Speed=0,Turn_Speed=40;
			}
			else{
			 Target_Speed=30,Turn_Speed=0;
			}
			break;
		case 98://����ģʽ
			if((Fore==0)&&(Back==0))Target_Speed=0;//δ���ܵ�ǰ������ָ��-->�ٶ����㣬����ԭ��
			if(Fore==1)Target_Speed--;//ǰ��1��־λ����-->��Ҫǰ��
			if(Back==1)Target_Speed++;//
			/*����*/
			if((Left==0)&&(Right==0))Turn_Speed=0;
			if(Left==1)Turn_Speed-=30;	//��ת
			if(Right==1)Turn_Speed+=30;	//��ת
			break;
		case 99://ѭ��ģʽ
			Tracking();
			switch(TkSensor)
			{
				case 15:
					Target_Speed=0;
					Turn_Speed=0;
					break;
				case 9:
					Target_Speed--;
					Turn_Speed=0;
					break;
				case 2://����ת
					Target_Speed--;
					Turn_Speed=15;
					break;
				case 4://����ת
					Target_Speed--;
					Turn_Speed=-15;
					break;
				case 8:
					Target_Speed=-10;
					Turn_Speed=-80;
					break;
				case 1:
					Target_Speed=-10;
					Turn_Speed=80;
					break;
			}
			break;
		case 100://PS2�ֱ�ң��
			if(PS2_Plugin)
			{
				LY=PS2_LY-128; //��ȡƫ��
				RX=PS2_RX-128; //��ȡƫ��
				if(LY>-Yuzhi&&LY<Yuzhi)LY=0; //����С�Ƕȵ�����
				if(RX>-Yuzhi&&RX<Yuzhi)RX=0; //����С�Ƕȵ�����
				if(Target_Speed>-LY/RATE_VEL) Target_Speed--;
				else if(Target_Speed<-LY/RATE_VEL) Target_Speed++;
				Turn_Speed=RX/RATE_TURN;
			}
			else
			{
				Target_Speed=0,Turn_Speed=0;
			}
		break;
	}
}
