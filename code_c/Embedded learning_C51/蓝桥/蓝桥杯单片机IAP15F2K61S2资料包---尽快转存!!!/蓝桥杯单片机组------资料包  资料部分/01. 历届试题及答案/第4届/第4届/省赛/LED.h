#ifndef LED_H
#define LED_H

#include "reg52.h"
#include <intrins.h>

#define Y6 0xC0
#define Y7 0xE0

void DS1(unsigned char yi, unsigned char er, unsigned char san, unsigned char si, unsigned char wu);
void DS2(unsigned char yi, unsigned char er, unsigned char san);



#endif