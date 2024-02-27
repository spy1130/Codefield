#ifndef __DS1302_H
#define __DS1302_H

void Write_Ds1302_Byte(unsigned  char temp);
void Write_Ds1302( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302 ( unsigned char address );
extern void ds_init();
extern void read_time();

extern unsigned char shijian[7];
extern unsigned char ds[7];
#endif
