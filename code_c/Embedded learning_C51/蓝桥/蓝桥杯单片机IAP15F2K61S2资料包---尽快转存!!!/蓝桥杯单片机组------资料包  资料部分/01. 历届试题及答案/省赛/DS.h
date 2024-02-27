#ifndef DS_H
#define DS_H

#include <STC15F2K60S2.h>

#define Y6 0xC0
#define Y7 0xE0

void DS1(unsigned char yi,unsigned char er);
void DS2(unsigned char yi);
void DS3(unsigned char yi,unsigned char er);
void DS4(unsigned char yi);
void DS5(unsigned char yi,unsigned char er);
#endif