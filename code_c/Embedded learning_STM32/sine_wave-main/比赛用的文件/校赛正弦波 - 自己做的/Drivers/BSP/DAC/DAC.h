#ifndef __DAC_H
#define __DAC_H
#include "sys.h"
#include "dma.h"
#include "Tim.h"
void DAC_Config( void);
void dac_dma_wave_enable( uint16_t psc,uint16_t arr);
#endif
