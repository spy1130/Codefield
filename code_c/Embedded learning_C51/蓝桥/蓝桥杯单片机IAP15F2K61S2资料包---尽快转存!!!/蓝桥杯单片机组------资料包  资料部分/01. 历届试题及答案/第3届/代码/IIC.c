#include "IIC.h"


void Delay5us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 5;
	while (i--)
	{
		_nop_();
	}
}

void iic_Start()
{
	SDA = 1;
	SCL = 1;
	Delay5us();
	SDA = 0;
	Delay5us();
	SCL = 0;
	Delay5us();
}

void iic_Stop()
{
	SDA = 0;
	SCL = 1;
	Delay5us();
	SDA = 1;
	Delay5us();
	SCL = 0;
	Delay5us();
}

void iic_sendack()
{
	SCL = 0;
	SDA = 0;
	Delay5us();
	SCL = 1;
	Delay5us();
	SCL = 0;
	SDA = 1;
	Delay5us();
}

bit iic_waitack()
{
	bit ackbit;
	SCL = 1;
	Delay5us();
	ackbit = SDA;
	SCL = 0;
	Delay5us();
	return ackbit;
}

void iic_Write(unsigned char da)
{
	unsigned char i;
	for (i=0; i<8; i++)
	{
		SCL = 0;
		Delay5us();
		SDA = da&0x80;
		da <<= 1;
		Delay5us();
		SCL = 1;
		Delay5us();
	}
	SCL = 0;
}

unsigned char iic_Read()
{
	unsigned char i,temp;
	for (i=0; i<8; i++)
	{
		SCL = 1;
		Delay5us();
		temp <<= 1;
		temp = temp|SDA;

		SCL = 0;
		Delay5us();
	}
	Delay5us();
	return temp;
}


void Pcf8591()
{
	unsigned char a;
	iic_Start();
	iic_Write(0x90);
	iic_waitack();
	iic_Write(0x01);
	iic_waitack();
	iic_Start();
	iic_Write(0x91);
	iic_waitack();
	a = iic_Read();
	iic_sendack();
	iic_Stop();
	a = a*5/255;
	if (a < 1.25)
	{
		P2 = 0x80;
		P0 = 0xFE;
		P2 = 0x00;
	}
	else
	{
		P2 = 0x80;
		P0 = 0xFF;
		P2 = 0x00;
	}
}