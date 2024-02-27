#ifndef _MOTOR_H_
#define _MOTOR_H_


#include "sys.h"
//位带操作，定义后能直接赋值给对应引脚进行输出0或1
#define AIN1   PBout(14)//PB14
#define AIN2   PBout(15)//PB15
#define BIN1   PBout(13)//PB13
#define BIN2   PBout(12)//PB12
void Motor_Init(void);
void Limit(int *MotorA, int *MotorB);
void Load(int moto1, int moto2);
#endif




