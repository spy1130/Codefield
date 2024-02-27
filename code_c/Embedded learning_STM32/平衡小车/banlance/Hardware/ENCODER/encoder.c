#include "encoder.h"

/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_TIM2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能PB端口时钟

    // 往内输入数据只需要配置模式和引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);             // 初始化结构体，所有都配置了就不需要加了
    TIM_TimeBaseStructure.TIM_Period = 65535;                   // 设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                    // 预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 选择时钟分频：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM向上计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); // 编码器配置函数，使用编码器模式3

    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10; // 滤波器
    TIM_ICInit(TIM2, &TIM_ICInitStructure);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 清除TIM的更新标志位，因为计数器溢出会产生更新中断，标志位会拉高
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    TIM_SetCounter(TIM2, 0);
    TIM_Cmd(TIM2, ENABLE);
}
/**************************************************************************
函数功能：把TIM3初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_TIM3_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能PA端口时钟

    // 往内输入数据只需要配置模式和引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);             // 初始化结构体，所有都配置了就不需要加了
    TIM_TimeBaseStructure.TIM_Period = 65535;                   // 设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                    // 预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 选择时钟分频：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM向上计数
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); // 编码器配置函数，使用编码器模式3

    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10; // 滤波器
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    TIM_ClearFlag(TIM3, TIM_FLAG_Update); // 清除TIM的更新标志位，因为计数器溢出会产生更新中断，标志位会拉高
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    TIM_SetCounter(TIM3, 0);
    TIM_Cmd(TIM3, ENABLE);
}
/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int Read_Speed(u8 TIMX)
{
    int Encoder_TIM;
    switch (TIMX)
    {
    case 2:
        Encoder_TIM = (short)TIM_GetCounter(TIM2);
        TIM_SetCounter(TIM2, 0);
        break; // 如果是定时器2，1.读取定时器2的计数值（x的单位时间变量）并保存。2，清零定时器2的计数值
    case 3:
        Encoder_TIM = (short)TIM_GetCounter(TIM4);
        TIM_SetCounter(TIM4, 0);
        break;
    default:
        Encoder_TIM = 0;
    }
    return Encoder_TIM;
}
// 编码器产生的是脉冲，计数器计脉冲数，脉冲数就是位移，单位时间内的脉冲数就是速度

// 这些中断可以不加
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != 0)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != 0)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}
