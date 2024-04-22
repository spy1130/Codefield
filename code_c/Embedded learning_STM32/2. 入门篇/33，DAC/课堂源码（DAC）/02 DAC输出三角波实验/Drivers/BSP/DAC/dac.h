#ifndef __DAC_H
#define __DAC_H

#include "./SYSTEM/sys/sys.h"


void dac_init(void);
void dac_triangular_wave(uint16_t maxval, uint16_t dt, uint16_t samples, uint16_t n);

#endif

















