#include "control.h"
#define SPEED_Y 100 //俯仰(前后)最大设定速度
#define SPEED_Z 80//偏航(左右)最大设定速度 

float Med_Angle =0;            // 机械中值
float Target_Speed=0;	//期望速度。---二次开发接口，用于控制小车前进后退及其速度。

float Vertical_Kp=-400,
	  Vertical_Kd=-0.75; // 直立环参数
float velocity_Kp=-80,
      velocity_Ki=-0.4; // 速度环参数

int Vertical_Pwm, Velocity_Pwm, Turn_Pwm;//直立环&速度环&转向环的输出变量

int Vertical(float Med, float Angle, float Gyro);//函数声明
int velocity(int Target,int encoder_left, int encoder_right);
int Turn(int gyro_Z);

void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line5) != RESET) // 判断是否产生了EXTI Line5中断
    {
        int Pwm_out;
        if (PBin(5) == 0) // 二次检查低电平是否为真
        {
            EXTI_ClearITPendingBit(EXTI_Line5); // 清除中断标志位

            // 1.采集编码器数据&MPU6050角度信息
            Encoder_Left = Read_Speed(2);             // 读取编码器的值，保证输出极性一致
            Encoder_Right = -Read_Speed(3);           // 读取编码器的值
            mpu_dmp_get_data(&pitch, &roll, &yaw);      // 得到欧拉角（姿态角）的数据
            MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);  // 得到陀螺仪数据
            MPU_Get_Accelerometer(&aacx, &aacy, &aacz); // 加速度
            // 2.将数据压入闭环控制中，计算出控制输出量
			Target_Speed=Target_Speed>SPEED_Y?SPEED_Y:(Target_Speed<-SPEED_Y?(-SPEED_Y):Target_Speed);//限幅
			
            Velocity_Pwm = velocity(Target_Speed,Encoder_Left, Encoder_Right);//速度环
			Vertical_Pwm = Vertical(Velocity_Pwm+Med_Angle, pitch, gyroy);//直立环
            
            Turn_Pwm = Turn(gyroz);//转向环

            Pwm_out = Vertical_Pwm; // 推导后的串级公式
            // 3.把控制输出量加载到电机上，完成最终的控制
            MOTO1 = Pwm_out+Turn_Pwm; // 计算左轮电机最终PWM
            MOTO2 = Pwm_out-Turn_Pwm; // 计算右轮电机最终PWM
            Limit(&MOTO1, &MOTO2);      // PWM限幅
            Load(MOTO1, MOTO2);//加载到电机上
        }
    }
}

/**************************************************************************
函数功能：直立PD控制Kp*Ek+Kd*Ek_D
入口参数：期望角度（机械中值）、真实角度、角速度差值（真实-0）
返回值：直立控制PWM
**************************************************************************/
int Vertical(float Med, float Angle, float Gyro)
{
    int PWM_out;

    PWM_out = Vertical_Kp * (Angle - Med) + Vertical_Kd * Gyro;

    return PWM_out;
}

/**************************************************************************
函数功能：速度PI控制(Kp*Ek+Ki*Ek_Sum)，目标速度默认为0
入口参数：电机编码器的值
返回值：速度控制PWM
**************************************************************************/
int velocity(int Target,int encoder_left, int encoder_right)
{
    static int  PWM_out, Encoder_Err, Encoder_S, EnC_Err_Lowout,EnC_Err_Lowout_Last;
    float a = 0.8;
	
    // 1.计算速度偏差
    Encoder_Err = ((encoder_left + encoder_right) - Target); // 由于做“（左+右）/2”处理过程中，1）除法占用资源；2）数据类型的限制使得先加后除的计算结果导致了末尾数据的丢弃增加了 单片机计算上的误差。
    // 2.对速度偏差进行低通滤波,使得波形更加平滑，滤除高频干扰。防止速度突变，过大影响直立环正常工作
    // low_out=(1-a)*本次偏差+(a*上次低通滤波的输出;
    EnC_Err_Lowout = (1 - a) * Encoder_Err + a * EnC_Err_Lowout_Last;
    EnC_Err_Lowout_Last = EnC_Err_Lowout;
    // 3.计算速度偏差积分,积分出位移
    Encoder_S += EnC_Err_Lowout;
    if (Encoder_S > 10000)
        Encoder_S = 10000; // 积分限幅
    if (Encoder_S < -10000)
        Encoder_S = -10000; // 积分限幅
    // 4.计算速度控制PWM
    PWM_out = velocity_Kp * EnC_Err_Lowout + velocity_Ki * Encoder_S;
	if(pitch<-40||pitch>40) Encoder_S=0;//电机关闭后清除积分
    return PWM_out;
}

/**************************************************************************
函数功能：转向P控制（系数*Z轴角速度）只为立住
入口参数：电机编码器的值、Z轴角速度
返回  值：转向控制PWM
**************************************************************************/
int Turn(int gyro_Z)
{
    int PWM_out;
    PWM_out = (-0.6) * gyro_Z;
    return PWM_out;
}
