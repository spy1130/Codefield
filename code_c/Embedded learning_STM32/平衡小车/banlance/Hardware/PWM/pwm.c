#include "pwm.h"
/*****************************************************
功能：PWM输出初始化
arr：自动重装值
psc：时钟预分频数
TIM1_PWM_Init(7199,0);//PWM频率=72000/(7199+1)=10Khz
*****************************************************/

void PWM_Init_TIM1(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  // 使能TIM1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能GPIOA时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11; // TIM_CH1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);             // 初始化结构体，所有都配置了就不需要加了
    TIM_TimeBaseStructure.TIM_Period = arr;                     // 设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  // 设置用来作为TIMx时钟频率除数的预分频值  不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                // 设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);             // 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    // 输出比较配置
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // 配置为PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // 设置跳变值，当计数器计数到这个值时，电平发生跳变
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM1,ENABLE);	//高级定时器专属-MOE主输出使能（很重要）	
    
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能	 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH4预装载使能	 
    TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIM1在ARR上的预装载寄存器

    TIM_Cmd(TIM1, ENABLE);  //使能TIM1
}
