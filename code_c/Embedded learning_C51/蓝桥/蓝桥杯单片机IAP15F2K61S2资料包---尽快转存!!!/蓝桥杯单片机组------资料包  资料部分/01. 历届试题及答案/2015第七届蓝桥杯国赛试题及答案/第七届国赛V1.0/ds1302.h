#ifndef __DS1302_H
#define __DS1302_H

#define uchar unsigned char
#define uint unsigned int

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;   // DS1302��λ

extern uchar shijian[7];

void Write_Ds1302_Byte(unsigned  char temp);
void Write_Ds1302( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302 ( unsigned char address );

void dsread();
void dswrite();
#endif
