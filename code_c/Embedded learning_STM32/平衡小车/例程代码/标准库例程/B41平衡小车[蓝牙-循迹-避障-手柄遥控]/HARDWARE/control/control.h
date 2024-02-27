#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
/**************************************************************************
 作  者 ：大鱼电子
 淘宝地址：https://shop119207236.taobao.com
 微信公众号【大鱼机器人】
 后台回复【平衡小车】：获取平衡小车全套DIY资料
 后台回复【电子开发工具】：获取电子工程师必备开发工具
 后台回复【电子设计资料】：获取电子设计资料包
 知乎：张巧龙 
**************************************************************************/
#define C1 PBin(4)
#define C2 PBin(3)
#define C3 PAin(5)
#define C4 PAin(4)

#define PI 3.14159265

void EXTI9_5_IRQHandler(void);
int balance_UP(float Angle,float Mechanical_balance,float Gyro);
int velocity(int encoder_left,int encoder_right,int target);
int Turn_UP(int gyro_Z, int RC);
void Tracking(void);
void Get_RC(void);
#endif
