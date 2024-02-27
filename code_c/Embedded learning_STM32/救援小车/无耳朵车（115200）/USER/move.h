#ifndef __MOVE_H
#define __MOVE_H


#include "stdio.h"
#define left 0
#define right 1
#define forward 2


void forward1(int counter);
void turn_left_90(void);
void turn_right_90(void);
void turn_right_180(void);
void adjustDirection(u32 speedl, u32 speedr);
     

void forwardOneStep(void);


void Turn(u8 dir,u32 cnt,u8 speedl,u8 speedr);
void Turn_small_delay(u8 dir,u32 cnt,u8 speedl,u8 speedr);
void stop(u8 time);

void CorrectDirection(int turn_step,u8 turn_direction);
void adjustDirection_1(u32 speedl, u32 speedr);
void CorrectDirection2(int turn_step,u8 turn_direction);
void adjustDirLeft(u32 speedl,u32 speedr);
void adjustDirRight(u32 speedl,u32 speedr);
void adjustDirLeftBig(u32 speedl,u32 speedr);
void adjustDirRightBig(u32 speedl,u32 speedr);
void turnLeft_left_30(void);
void turnLeft_right_30(void);
void turnRight_left_30(void);
void turnRight_right_30(void);
void adjustLeftBig(void);
void adjustRightBig(void);
void turn_right_30(void);


void turn_right_90_room2(void);

#endif
