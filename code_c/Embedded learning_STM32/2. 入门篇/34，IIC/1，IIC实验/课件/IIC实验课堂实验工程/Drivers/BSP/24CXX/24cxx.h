#ifndef __24CXX_H
#define __24CXX_H

#include "./SYSTEM/sys/sys.h"

void at24c02_init(void);
void at24c02_write_one_byte(uint8_t addr, uint8_t data);
uint8_t at24c02_read_one_byte(uint8_t addr);

#endif













