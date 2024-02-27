

#include "stm32f10x.h"
#include <stdio.h>
#include "cfg.h"
#include "delay.h"

/*本文件为超声波函数*/

#define Trig_Write_1()     GPIO_SetBits(GPIOD,GPIO_Pin_10) //写1
#define Trig_Write_0()     GPIO_ResetBits(GPIOD,GPIO_Pin_10) //写0
#define Echo_Read()				GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) //读电平
volatile u16 times=0;  //计数
u16 dis;

u16 Get_Distance(void) //超声波测距
{
	u16 distance=0;
	
//	TIM_SetCounter(TIM4,0);//清除计数
    TIM4_Configure();
	
	Trig_Write_1();//10us启动脉冲
	delay_us(10);
	Trig_Write_0();
	
	while(Echo_Read() == 0);//等待上升沿
	TIM_Cmd(TIM4, ENABLE);//开启定时器
	while(Echo_Read() == 1);//等待下降沿
	TIM_Cmd(TIM4, DISABLE);//关闭定时器
	distance = TIM_GetCounter(TIM4);//获取计数器值，单位1us
	
//	dis = distance/60.085;//转换成距离
	dis = distance/5.88;
	return dis;
}





