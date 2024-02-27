#ifndef __BANYUN1_H
#define __BANYUN1_H



#include "stm32f10x.h"
#include "run.h"
#include "delay.h"
#include "cfg.h"
#include <stdio.h>
#include "stdbool.h"
#include "ult.h"
#include "motor.h"
#include "tcs.h"
#include "CorrectDirection.h"



void caAtop(void);	//搬A点top


void caAmid(void);	//A搬点mid


void caAbottom(void);	//搬A点bottom


void CarryAtoO(void); //	搬A到起点


void caCtop(void);//搬C点top




void caCmid(void);  //搬C点mid




void caCbottom(void); //搬C点bottom


void CarryCtoO(void);   //搬C到起点线




void caEtop(void);  //搬E点top



void caEmid(void);	 //搬E点mid



#endif
