#ifndef DS1302_H
#define DS1302_H

#include "reg52.h"
#include <intrins.h>

extern unsigned char Display_time[3];
void Init_DS1302();
void Read_DS1302();

#endif