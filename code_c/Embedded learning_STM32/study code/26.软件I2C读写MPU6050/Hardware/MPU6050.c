#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"
//建立在MYI2C的模块之上的MPU6050模块
#define MPU6050_ADDRESS		0xD0//从机地址替换为

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)//指定地址写寄存器
{	//指定地址
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);//从机地址
	MyI2C_ReceiveAck();//应答位
	MyI2C_SendByte(RegAddress);//发送寄存器地址，会存在MPU6050当前地址指针里，用于指定具体读写哪个寄存器
	MyI2C_ReceiveAck();
	
	MyI2C_SendByte(Data);//指定寄存器地址下的数据
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)//指定地址读寄存器
{
	uint8_t Data;
	//指定地址
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();//重新指定读写位
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);//变成0XD1，读写位为1
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();//返回值即为接受到的数据
	MyI2C_SendAck(1);//无需给应答
	MyI2C_Stop();
	
	return Data;
}
//对MPU6050硬件电路进行初始配置
void MPU6050_Init(void)
{	//看着手册的寄存器配
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);//选择X轴陀螺仪时钟
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);//10分频
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);//低通滤波器
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}
//显示ID号
uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)//x,y,z加速度值和陀螺仪值，*AccX在函数参数里代表AccX的地址
{
	uint8_t DataH, DataL;
	//读取数据
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;//加速度X轴16位数据，用指针返回数据
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}
