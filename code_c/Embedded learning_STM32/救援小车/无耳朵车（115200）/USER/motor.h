#ifndef __MOTOR_H
#define __MOTOR_H

#include <stdio.h>
#include "stm32f10x.h"
#include "delay.h"
#include "cfg.h"


/*��е�����ж�������*/
/*�Գ����������������ڵ�һ��Ϊǰ��*/
/*�ӳ���󷽿���ȥ��е�����������Ϊ1�ŵ�����Ҳ�Ϊ2�ŵ������������Ϊ3��4��5�ŵ��*/
/*��ʱ��Tim1���ĸ������ֱ����1��2��3��4�ŵ����Tim2�ĵ�һ����������5�ŵ��������3��������Ч*/

void catop(void); 


void camid(void);  //ץ���м�һ��


void cabottom(void);  //ץ��������һ��

void putbottom(void);  //���µ���һ��


void putbottom1(void);  //���µ���һ��

void putmid(void);  //�����м�

void puttop(void);  //����������һ��


void turnmotordown(void);//���ĸ������ת90�ȣ�������ץ���ٺ�£��ֹ���ϵ���


void turnmotorup(void); //�պ���ץ������е�ָֻ�ֱ����̬


void holdhand(void);//����ֱ����̬


void turnmotor(void);//ת����ץ

#endif
