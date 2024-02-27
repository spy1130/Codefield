#ifndef __ACTION_H
#define __ACTION_H

#include "reg52.h"
#include "motor.h"
#include "Ultrasonic.h"


void Circulation_wall_right(void);
void Circulation_wall_left(void);

void forward();
void car_stop();
void turn_left_90();
void turn_right_90();
void turn_right_180();
void turn_left_90_big();
void turn_right_90_small();

int kalmanfilter(int z_measure, int position);


extern unsigned int last_dis;
extern unsigned int left_dis ;
extern unsigned int right_dis ;
extern unsigned int front_dis ;


#endif