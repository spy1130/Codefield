#include "control.h"
#define SPEED_Y 100 //����(ǰ��)����趨�ٶ�
#define SPEED_Z 80//ƫ��(����)����趨�ٶ� 

float Med_Angle =0;            // ��е��ֵ
float Target_Speed=0;	//�����ٶȡ�---���ο����ӿڣ����ڿ���С��ǰ�����˼����ٶȡ�

float Vertical_Kp=-400,
	  Vertical_Kd=-0.75; // ֱ��������
float velocity_Kp=-80,
      velocity_Ki=-0.4; // �ٶȻ�����

int Vertical_Pwm, Velocity_Pwm, Turn_Pwm;//ֱ����&�ٶȻ�&ת�򻷵��������

int Vertical(float Med, float Angle, float Gyro);//��������
int velocity(int Target,int encoder_left, int encoder_right);
int Turn(int gyro_Z);

void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line5) != RESET) // �ж��Ƿ������EXTI Line5�ж�
    {
        int Pwm_out;
        if (PBin(5) == 0) // ���μ��͵�ƽ�Ƿ�Ϊ��
        {
            EXTI_ClearITPendingBit(EXTI_Line5); // ����жϱ�־λ

            // 1.�ɼ�����������&MPU6050�Ƕ���Ϣ
            Encoder_Left = Read_Speed(2);             // ��ȡ��������ֵ����֤�������һ��
            Encoder_Right = -Read_Speed(3);           // ��ȡ��������ֵ
            mpu_dmp_get_data(&pitch, &roll, &yaw);      // �õ�ŷ���ǣ���̬�ǣ�������
            MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);  // �õ�����������
            MPU_Get_Accelerometer(&aacx, &aacy, &aacz); // ���ٶ�
            // 2.������ѹ��ջ������У���������������
			Target_Speed=Target_Speed>SPEED_Y?SPEED_Y:(Target_Speed<-SPEED_Y?(-SPEED_Y):Target_Speed);//�޷�
			
            Velocity_Pwm = velocity(Target_Speed,Encoder_Left, Encoder_Right);//�ٶȻ�
			Vertical_Pwm = Vertical(Velocity_Pwm+Med_Angle, pitch, gyroy);//ֱ����
            
            Turn_Pwm = Turn(gyroz);//ת��

            Pwm_out = Vertical_Pwm; // �Ƶ���Ĵ�����ʽ
            // 3.�ѿ�����������ص�����ϣ�������յĿ���
            MOTO1 = Pwm_out+Turn_Pwm; // �������ֵ������PWM
            MOTO2 = Pwm_out-Turn_Pwm; // �������ֵ������PWM
            Limit(&MOTO1, &MOTO2);      // PWM�޷�
            Load(MOTO1, MOTO2);//���ص������
        }
    }
}

/**************************************************************************
�������ܣ�ֱ��PD����Kp*Ek+Kd*Ek_D
��ڲ����������Ƕȣ���е��ֵ������ʵ�Ƕȡ����ٶȲ�ֵ����ʵ-0��
����ֵ��ֱ������PWM
**************************************************************************/
int Vertical(float Med, float Angle, float Gyro)
{
    int PWM_out;

    PWM_out = Vertical_Kp * (Angle - Med) + Vertical_Kd * Gyro;

    return PWM_out;
}

/**************************************************************************
�������ܣ��ٶ�PI����(Kp*Ek+Ki*Ek_Sum)��Ŀ���ٶ�Ĭ��Ϊ0
��ڲ����������������ֵ
����ֵ���ٶȿ���PWM
**************************************************************************/
int velocity(int Target,int encoder_left, int encoder_right)
{
    static int  PWM_out, Encoder_Err, Encoder_S, EnC_Err_Lowout,EnC_Err_Lowout_Last;
    float a = 0.8;
	
    // 1.�����ٶ�ƫ��
    Encoder_Err = ((encoder_left + encoder_right) - Target); // ������������+�ң�/2����������У�1������ռ����Դ��2���������͵�����ʹ���ȼӺ���ļ�����������ĩβ���ݵĶ��������� ��Ƭ�������ϵ���
    // 2.���ٶ�ƫ����е�ͨ�˲�,ʹ�ò��θ���ƽ�����˳���Ƶ���š���ֹ�ٶ�ͻ�䣬����Ӱ��ֱ������������
    // low_out=(1-a)*����ƫ��+(a*�ϴε�ͨ�˲������;
    EnC_Err_Lowout = (1 - a) * Encoder_Err + a * EnC_Err_Lowout_Last;
    EnC_Err_Lowout_Last = EnC_Err_Lowout;
    // 3.�����ٶ�ƫ�����,���ֳ�λ��
    Encoder_S += EnC_Err_Lowout;
    if (Encoder_S > 10000)
        Encoder_S = 10000; // �����޷�
    if (Encoder_S < -10000)
        Encoder_S = -10000; // �����޷�
    // 4.�����ٶȿ���PWM
    PWM_out = velocity_Kp * EnC_Err_Lowout + velocity_Ki * Encoder_S;
	if(pitch<-40||pitch>40) Encoder_S=0;//����رպ��������
    return PWM_out;
}

/**************************************************************************
�������ܣ�ת��P���ƣ�ϵ��*Z����ٶȣ�ֻΪ��ס
��ڲ����������������ֵ��Z����ٶ�
����  ֵ��ת�����PWM
**************************************************************************/
int Turn(int gyro_Z)
{
    int PWM_out;
    PWM_out = (-0.6) * gyro_Z;
    return PWM_out;
}
