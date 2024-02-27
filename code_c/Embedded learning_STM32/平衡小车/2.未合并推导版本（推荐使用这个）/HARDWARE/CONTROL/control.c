#include "control.h"


float Med_Angle=-22;	//机械中值。---在这里修改你的机械中值即可。
float Target_Speed=0;	//期望速度。---二次开发接口，用于控制小车前进后退及其速度。

float 
	Vertical_Kp=-210,//直立环KP、KD
	Vertical_Kd=-0.4;
float 
	Velocity_Kp=0.3,//速度环KP、KI
	Velocity_Ki=0.0015;

int Vertical_out,Velocity_out,Turn_out;//直立环&速度环&转向环 的输出变量

int Vertical(float Med,float Angle,float gyro_Y);//函数声明
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z); 

void EXTI9_5_IRQHandler(void)
{
	int PWM_out;
	if(EXTI_GetITStatus(EXTI_Line5)!=0)//一级判定
	{
		if(PBin(5)==0)//二级判定
		{
			EXTI_ClearITPendingBit(EXTI_Line5);//清除中断标志位
			
			//1.采集编码器数据&MPU6050角度信息。
			Encoder_Left=-Read_Speed(2);//电机是相对安装，刚好相差180度，为了编码器输出极性一致，就需要对其中一个取反。
			Encoder_Right=Read_Speed(4);
			
			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);			//角度
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//陀螺仪
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//加速度
			//2.将数据压入闭环控制中，计算出控制输出量。
			
			/*
			【一】
			问：
				当我在小车前侧绑一块小铁块，这会使得机械中值后倾，=-27度，然后我将车子扶在机械中值的角度位置然后开机，但车子一开机就往前跑。为什么？
			答：
				问题分析：
								起始速度偏差=0（因为没有动轮子呀）——>外环输出=0（即内环期望角度=0）
								But：开局实际角度= -27度——>开局内环输出就等于(-27-0)*(-1) = +27  //Kp为负数，这里设为-1
								即根本是因为开局时 外环输出！=机械中值(这里机械中值可以等效说成实际角度，但不是任何时候都可以等效说成的。譬如我开机时小车不摆在机械中值位置时就不能等效说成。)
								=====>>>>所以开局时车子肯定往前跑！！！			
				解决思想：
								起始Target=Real就可以了。
				解决方法：
								测机械中值譬如 = -27度，则使得内环期望角度=外环输出 + 机械中值。	
								
			【二】
			若：外环输出==机械中值，即开局速度偏差=0时，机械中值=0则外环输出=0，即内环输入=0。
			又有内环反馈=0，所以这时候开局内环输出就等于(0-0)*(-1) = 0===>车子必然直立静止在原地。
			
			【三】
			若：外环输出！=机械中值，即开局速度偏差=0时，机械中值!=0，但我开局不把车子摆在机械中值位置。譬如举例当机械中值=-27度，我开机将车子实际角度摆在0度位置。则此时
			速度偏差=0则外环输出=0，即内环输入=0。
			又有内环反馈=0===>理论上小车直立静止，BUT！车子根本不在机械中值位置，又谈何直立静止，车子会根据重心向前倒下，直立环立刻发挥作用-->
			-->若小铁快非常非常轻，还有可能救回来；若铁块不轻，则必然车子一直不断向前跑不会停下；若小铁块过重，系统来不及响应，车子会直接向前倒下；
			这，就是结果。
			*/			
			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);	//速度环
			Vertical_out=Vertical(Velocity_out+Med_Angle,Pitch,gyroy);			//直立环
			Turn_out=Turn(gyroz);																						//转向环
			
			PWM_out=Vertical_out;//最终输出
			//3.把控制输出量加载到电机上，完成最终的的控制。
			MOTO1=PWM_out-Turn_out;//左电机
			MOTO2=PWM_out+Turn_out;//右电机
			Limit(&MOTO1,&MOTO2);	 //PWM限幅			
			Load(MOTO1,MOTO2);		 //加载到电机上。
			
//			Stop(&Med_Angle,&Pitch);
			
		}
	}
}




/*********************
直立环PD控制器：Kp*Ek+Kd*Ek_D

入口：期望角度、真实角度、真实角速度
出口：直立环输出
*********************/
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_out;
	
	PWM_out=Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
	return PWM_out;
}




/*********************
速度环PI：Kp*Ek+Ki*Ek_S
*********************/
int Velocity(int Target,int encoder_left,int encoder_right)
{
	static int Encoder_S,EnC_Err_Lowout_last,PWM_out,Encoder_Err,EnC_Err_Lowout;
	float a=0.7;
	
	//1.计算速度偏差
	Encoder_Err=((encoder_left+encoder_right)-Target);//舍去误差--我的理解：能够让速度为"0"的角度，就是机械中值。
	//2.对速度偏差进行低通滤波
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//使得波形更加平滑，滤除高频干扰，防止速度突变。
	EnC_Err_Lowout_last=EnC_Err_Lowout;//防止速度过大的影响直立环的正常工作。
	//3.对速度偏差积分，积分出位移
	Encoder_S+=EnC_Err_Lowout;
	//4.积分限幅
	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
	
	//5.速度环控制输出计算
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
	return PWM_out;
}



/*********************
转向环：系数*Z轴角速度
*********************/
int Turn(int gyro_Z)
{
	int PWM_out;
	
	PWM_out=(-0.6)*gyro_Z;
	return PWM_out;
}

