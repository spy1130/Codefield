#ifndef __TCS_H
#define __TCS_H


#include "sys.h"
#include "stm32f10x.h"
#include "delay.h"
#include <stdio.h>
#include "cfg.h"


#define white  	 1    //白
#define black  	 2    //黑
#define yellow   3    //黄
#define red    	 4    //红
#define blue     5    //蓝
#define other    6	  //其他情况

void TCS230_WhiteBalance(u16 pColor[3]);


u16 colourjudge(u16 pRGB[3]); //测试颜色，并判断识别颜色


#endif
