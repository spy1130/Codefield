#ifndef _key_H_
#define _key_H_

#include <STC15F2K60S2.h>
#include <delay.h>

#define uchar unsigned char 
void keyscan();

extern uchar s4,s5,s6,s7,ss;
extern uchar normal,time,kongzai,guozai;
extern uchar noise_flag,he_flag;
 void fengming(uchar xiang);
 void jidian(uchar he);


#endif