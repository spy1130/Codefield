#include "IIC.h"

sbit SCL = P2^0;
sbit SDA = P2^1;

void Delay5us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 11;
	while (--i);
}


void iic_start()
{
	SDA = 1;
	SCL = 1;
	Delay5us();
	SDA = 0;
	Delay5us();
}

void iic_stop()
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
}

bit iic_waitack()
{
	bit a;
	SDA = 1;
	SCL = 1;
	Delay5us();
	a = SDA;
	SCL = 0;
	return a;
}

void iic_WriteByte(unsigned char da)
{
	unsigned char i;
	for(i=0; i<8; i++)
	{
		SCL = 0;
		SDA = da&0x80;
		da <<= 1;
		Delay5us();
		SCL = 1;
		Delay5us();
	}
	SCL = 0;
}

unsigned char iic_ReadByte()
{
	unsigned char i, temp;
	for(i=0; i<8; i++)
	{
		SCL = 1;
		Delay5us();
		temp <<= 1;
		temp = temp | SDA;
		SCL = 0;
	}
	return temp;
}

unsigned char PCF()
{
	unsigned char temp;
	iic_start();
	iic_WriteByte(0x90);
	iic_waitack();
	iic_WriteByte(0x03);
	iic_waitack();
	iic_start();	
	iic_WriteByte(0x91);
	iic_waitack();
	temp = iic_ReadByte();
	iic_sendack();
	iic_stop();
	return temp;
}

void Write_EEPROM(unsigned char da)
{
	iic_start();
	iic_WriteByte(0xA0);
	iic_waitack();
	iic_WriteByte(0x00);
	iic_waitack();
	iic_WriteByte(da);
	iic_waitack();
	iic_stop();
}

unsigned char Read_EEPROM()
{
	unsigned char temp;
	iic_start();
	iic_WriteByte(0xA0);
	iic_waitack();
	iic_WriteByte(0x00);
	iic_waitack();
	iic_start();	
	iic_WriteByte(0xA1);
	iic_waitack();
	temp = iic_ReadByte();
	iic_sendack();
	iic_stop();
	return temp;
}