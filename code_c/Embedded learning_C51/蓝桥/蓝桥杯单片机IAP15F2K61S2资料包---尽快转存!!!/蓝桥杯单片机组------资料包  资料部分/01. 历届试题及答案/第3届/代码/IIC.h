#ifndef IIC_H
#define IIC_H

#include <STC15F2K60S2.h>
#include <intrins.h>
sbit SDA = P2^1;
sbit SCL = P2^0;

void Pcf8591();

#endif