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



void caAtop(void);	//��A��top


void caAmid(void);	//A���mid


void caAbottom(void);	//��A��bottom


void CarryAtoO(void); //	��A�����


void caCtop(void);//��C��top




void caCmid(void);  //��C��mid




void caCbottom(void); //��C��bottom


void CarryCtoO(void);   //��C�������




void caEtop(void);  //��E��top



void caEmid(void);	 //��E��mid



#endif
