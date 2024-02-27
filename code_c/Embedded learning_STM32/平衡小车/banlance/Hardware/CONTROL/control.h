#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "sys.h"

#define C1 PBin(4)
#define C2 PBin(3)
#define C3 PAin(5)
#define C4 PAin(4)

#define PI 3.14159265

void EXTI9_5_IRQHandler(void);
int Vertical(float Med, float Angle, float Gyro);
int velocity(int Target,int encoder_left, int encoder_right);
int Turn(int gyro_Z);

#endif



