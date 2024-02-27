#ifndef __DS18B20_H
#define __DS18B20_H
#include <intrins.h>
 
#define  uchar unsigned char 
uchar temget();
uchar read();
void dsinit();
void write(uchar dat);
void Delay100us();
void Delay500us();
#endif
