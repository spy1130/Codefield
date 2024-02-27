#ifndef __DS1302_H
#define __DS1302_H

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;   // DS1302¸´Î»
extern unsigned char shijian[7];
extern unsigned char ds[7];
extern void Write_Ds1302_Byte(unsigned  char temp);
extern void Write_Ds1302( unsigned char address,unsigned char dat );
extern unsigned char Read_Ds1302 ( unsigned char address );
extern void dswrite();
extern void dsread();
#endif
