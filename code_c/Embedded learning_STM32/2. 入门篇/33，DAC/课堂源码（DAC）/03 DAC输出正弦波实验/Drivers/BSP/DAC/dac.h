#ifndef __DAC_H
#define __DAC_H

#include "./SYSTEM/sys/sys.h"


void dac_dma_wave_init(void);
void dac_dma_wave_enable(uint16_t cndtr, uint16_t arr, uint16_t psc);

#endif

















