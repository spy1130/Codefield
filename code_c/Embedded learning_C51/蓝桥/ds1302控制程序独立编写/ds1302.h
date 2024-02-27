#ifndef _DS1302_H
#define _DS1302_H
void write_ds1302(unsigned char temp);
void write_ds1302_byte(unsigned char address,unsigned char date);
unsigned char read_ds1302_byte(unsigned char address);
#endif
