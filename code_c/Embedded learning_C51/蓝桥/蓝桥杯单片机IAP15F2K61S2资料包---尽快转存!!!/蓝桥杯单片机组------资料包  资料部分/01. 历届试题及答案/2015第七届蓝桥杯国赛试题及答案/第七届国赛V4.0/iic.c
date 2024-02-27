/*
  程序说明: IIC总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台 8051，12MHz
  日    期: 2011-8-9
*/

#include <STC15F2K60S2.h>
#include "intrins.h"
#include <iic.h>

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}    


#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//总线引脚定义
sbit SD = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */



//总线启动条件
void IIC_Start(void)
{
	SD = 1;
	SCL = 1;
	somenop;
	SD = 0;
	somenop;
	SCL = 0;	
}

//总线停止条件
void IIC_Stop(void)
{
	SD = 0;
	SCL = 1;
	somenop;
	SD = 1;
}

//应答位控制
void IIC_Ack(bit ackbit)
{
	if(ackbit) 
	{	
		SD = 0;
	}
	else 
	{
		SD = 1;
	}
	somenop;
	SCL = 1;
	somenop;
	SCL = 0;
	SD = 1; 
	somenop;
}

//等待应答
bit IIC_WaitAck(void)
{
	SD = 1;
	somenop;
	SCL = 1;
	somenop;
	if(SD)    
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
			SD = 1;
		}
		else 
		{
			SD = 0;
		}
		somenop;
		SCL = 1;
		byt <<= 1;
		somenop;
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
		somenop;
		da <<= 1;
		if(SD) 
		da |= 0x01;
		SCL = 0;
		somenop;
	}
	return da;
}

uchar adread(uchar add)
{
	 uchar dat;
	 IIC_Start();
	 IIC_SendByte(0x90);
	 IIC_WaitAck();
	 IIC_SendByte(add);
	 IIC_WaitAck();

	 IIC_Start();
	 IIC_SendByte(0x91);
	 IIC_WaitAck();
	 dat=IIC_RecByte();
	 IIC_Ack(0);
	 IIC_Stop();

	 return dat;
}