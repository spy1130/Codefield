#ifndef __ATIM_H
#define __ATIM_H

#include "./SYSTEM/sys/sys.h"


void atim_timx_cplm_pwm_init(uint16_t arr, uint16_t psc);
void atim_timx_cplm_pwm_set(uint16_t ccr, uint8_t dtg);

#endif

















