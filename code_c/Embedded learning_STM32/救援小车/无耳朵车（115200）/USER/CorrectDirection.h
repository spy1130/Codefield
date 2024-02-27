#ifndef __CorrectDirection_H
#define __CorrectDirection_H



#include "stm32f10x.h"
#include "run.h"
#include "delay.h"
#include "cfg.h"
#include <stdio.h>
#include "stdbool.h"
#include "ult.h"
#include "motor.h"
#include "tcs.h"




void LCorrectDir(void); //转向并判断与待搬运块距离//是否开始搬运


void LCorrectDir1(void);//超声波对准，如果超声波失效，可能无法对准



void Direction(void);


void RCorrectDir(void);//先向左偏然后向右转对准

#endif


