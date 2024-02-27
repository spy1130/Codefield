#include "motor.h"

void Motor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PB�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; // �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                     // �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                    // �ٶ�50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                                               // �����趨������ʼ��GPIOB

    GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15); // PB0��PB1��PB10��PB11�����
}
/**************************************************************************
�������ܣ�����PWM��ֵ
��ڲ�����*MotorA�����A��PWM��ֵ
          *MotorB�����B��PWM��ֵ
����ֵ����
**************************************************************************/
void Limit(int *MotorA, int *MotorB)
{ //===PWM������7200 ������7000
    if (*MotorA > 7000)
        *MotorA = 7000;
    else if (*MotorA < -7000)
        *MotorA = -7000;
    if (*MotorB > 7000)
        *MotorB = 7000;
    else if (*MotorB < -7000)
        *MotorB = -7000;
}
/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
Ŀ    �ģ�����ֱ�������ٶȻ��Լ�ת�򻷼��������PWM�п���Ϊ��ֵ
                    ��ֻ�ܸ�����ʱ��PWM�Ĵ���ֻ������ֵ������Ҫ��PWM���о���ֵ����
**************************************************************************/
int myabs(int a)
{
    int temp;
    if (a < 0)
        temp = -a;
    else
        temp = a;
    return temp;
}
/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Load(int moto1, int moto2)
{   //�����������µ����⣬��-ת��Ϊ��ת
    //�о������ţ���Ӧ����ת
    if (moto1 < 0)
        AIN2 = 1, AIN1 = 0; // ��ת
    else
        AIN2 = 0, AIN1 = 1; // ��ת
    //�о�PWMֵ��
    TIM_SetCompare1(TIM1, myabs(moto1));
    if (moto2 < 0)
        BIN1 = 0, BIN2 = 1;
    else
        BIN1 = 1, BIN2 = 0;
    TIM_SetCompare4(TIM1, myabs(moto2));
}
