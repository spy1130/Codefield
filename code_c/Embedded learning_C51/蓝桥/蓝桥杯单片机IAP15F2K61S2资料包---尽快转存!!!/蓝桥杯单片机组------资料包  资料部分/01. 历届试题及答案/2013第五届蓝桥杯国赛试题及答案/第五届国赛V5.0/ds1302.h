#ifndef __DS1302_H
#define __DS1302_H

#define uchar unsigned char

void Write_Ds1302_Byte(unsigned  char temp);
void Write_Ds1302( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302 ( unsigned char address );
void dswrite();
void dsread();
extern uchar shijian[7];

#endif
