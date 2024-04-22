#ifndef __ADC_H
#define __ADC_H

#include "./SYSTEM/sys/sys.h"


void adc_temperature_init(void);
uint32_t adc_get_result(void);
short adc_get_temperature(void);

#endif 















