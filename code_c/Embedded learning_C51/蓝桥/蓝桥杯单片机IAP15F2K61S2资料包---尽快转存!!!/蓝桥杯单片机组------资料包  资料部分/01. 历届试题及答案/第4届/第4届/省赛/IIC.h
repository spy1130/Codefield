#ifndef IIC_H
#define IIC_H

#include "reg52.h"
#include <intrins.h>

unsigned char PCF();
void Write_EEPROM(unsigned char da);
unsigned char Read_EEPROM();

#endif