#ifndef __MOTOR_H
#define __MOTOR_H

#include <stdio.h>
#include "stm32f10x.h"
#include "delay.h"
#include "cfg.h"


/*机械臂所有动作代码*/
/*以超声波和驱动轮所在的一方为前方*/
/*从车体后方看过去机械臂最下面左侧为1号电机，右侧为2号电机，依次往上为3、4、5号电机*/
/*定时器Tim1的四个参数分别代表1、2、3、4号电机，Tim2的第一个参数代表5号电机，其余3个参数无效*/

void catop(void); 


void camid(void);  //抓起中间一个


void cabottom(void);  //抓起最下面一个

void putbottom(void);  //放下底下一个


void putbottom1(void);  //放下底下一个

void putmid(void);  //放下中间

void puttop(void);  //放下最上面一个


void turnmotordown(void);//第四个电机后转90度，并打开手抓，再合拢防止物料掉落


void turnmotorup(void); //闭合手抓，并机械手恢复直立姿态


void holdhand(void);//保持直立姿态


void turnmotor(void);//转动手抓

#endif
