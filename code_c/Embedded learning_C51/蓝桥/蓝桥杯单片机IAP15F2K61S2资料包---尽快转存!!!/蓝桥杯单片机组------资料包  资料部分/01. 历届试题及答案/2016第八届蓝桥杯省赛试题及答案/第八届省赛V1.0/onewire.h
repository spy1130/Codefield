#ifndef _ONEWIRE_H
#define _ONEWIRE_H

#include <STC15F2K60S2.h>
#include <delay.h>

#define OW_SKIP_ROM 0xcc
#define DS18B20_CONVERT 0x44
#define DS18B20_READ 0xbe
#define uchar unsigned char 


//IC���Ŷ���
sbit DQ = P1^4;

//��������
void Delay_OneWire(unsigned int t);
void Write_DS18B20(unsigned char dat);
bit Init_DS18B20(void);
unsigned char Read_DS18B20(void);
unsigned char wendu();

#endif