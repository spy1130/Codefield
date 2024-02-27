/*
  程序说明: IIC总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台(12MHz)
  日    期: 2011-8-9
*/

#include "iic.h"
#define uint unsigned int 


//总线启动条件
void IIC_Start(void)
{
	SDA = 1;
	Delay5us();	
	SCL = 1;
	Delay5us();	
	SDA = 0;
	Delay5us();
	SCL = 0;
	Delay5us();	
}

//总线停止条件
void IIC_Stop(void)
{
	SDA = 0;
	Delay5us();
	SCL = 1;
	Delay5us();
	SDA = 1;
	Delay5us();
}


//等待应答
bit IIC_WaitAck(void)
{
	SDA = 1;
	Delay5us();
	SCL = 1;
	Delay5us();
	if(SDA)    
	{   
		SCL = 0;
		IIC_Stop();
		return 0;
	}
	else  
	{ 
		SCL = 0;
		return 1;
	}
}


//通过I2C总线发送数据
void IIC_SendByte(unsigned char byt)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		Delay5us();
		SCL = 1;
		byt <<= 1;
		Delay5us();
		SCL = 0;
	}

}

//从I2C总线上接收数据
unsigned char IIC_RecByte(void)
{
	unsigned char da;
	unsigned char i;		 
	
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		Delay5us();
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		Delay5us();
	}
	return da;
}

unsigned char iic_read(unsigned char add)
{
	unsigned char num =0;
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
	

	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	num = IIC_RecByte();
	IIC_WaitAck();
	IIC_Stop();	
	num=0.39*num;
	return num;
}

