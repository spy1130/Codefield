#ifndef __ENCODER_H
#define __ENCODER_H

#ifdef GLOBALS
int16_t ARR_Count;
#else
extern int16_t ARR_Count;
#endif
void Encoder_Init(void);
int16_t Encoder_Get(void);

#endif
