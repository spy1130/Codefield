#ifndef DS18B20_H
#define DS18B20_H

#include <STC15F2K60S2.h>
#include <intrins.h>

void write_DS18B20(unsigned char dat);
int Read_DS18B20();

#endif