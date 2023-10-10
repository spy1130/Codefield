#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"
//������MYI2C��ģ��֮�ϵ�MPU6050ģ��
#define MPU6050_ADDRESS		0xD0//�ӻ���ַ�滻Ϊ

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)//ָ����ַд�Ĵ���
{	//ָ����ַ
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);//�ӻ���ַ
	MyI2C_ReceiveAck();//Ӧ��λ
	MyI2C_SendByte(RegAddress);//���ͼĴ�����ַ�������MPU6050��ǰ��ַָ�������ָ�������д�ĸ��Ĵ���
	MyI2C_ReceiveAck();
	
	MyI2C_SendByte(Data);//ָ���Ĵ�����ַ�µ�����
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)//ָ����ַ���Ĵ���
{
	uint8_t Data;
	//ָ����ַ
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();//����ָ����дλ
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);//���0XD1����дλΪ1
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();//����ֵ��Ϊ���ܵ�������
	MyI2C_SendAck(1);//�����Ӧ��
	MyI2C_Stop();
	
	return Data;
}
//��MPU6050Ӳ����·���г�ʼ����
void MPU6050_Init(void)
{	//�����ֲ�ļĴ�����
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);//ѡ��X��������ʱ��
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);//10��Ƶ
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);//��ͨ�˲���
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}
//��ʾID��
uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)//x,y,z���ٶ�ֵ��������ֵ��*AccX�ں������������AccX�ĵ�ַ
{
	uint8_t DataH, DataL;
	//��ȡ����
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;//���ٶ�X��16λ���ݣ���ָ�뷵������
	
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
