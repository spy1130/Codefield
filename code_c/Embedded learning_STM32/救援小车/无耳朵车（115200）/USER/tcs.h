#ifndef __TCS_H
#define __TCS_H


#include "sys.h"
#include "stm32f10x.h"
#include "delay.h"
#include <stdio.h>
#include "cfg.h"


#define white  	 1    //��
#define black  	 2    //��
#define yellow   3    //��
#define red    	 4    //��
#define blue     5    //��
#define other    6	  //�������

void TCS230_WhiteBalance(u16 pColor[3]);


u16 colourjudge(u16 pRGB[3]); //������ɫ�����ж�ʶ����ɫ


#endif
