#pragma  once
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "dynamicArray.h"


#define  MAX 1024

typedef void * seqQueue;

//��ʼ������
seqQueue init_SeqQueue();
//���
void push_SeqQueue( seqQueue queue , void * data);
//����
void pop_SeqQueue(seqQueue queue);
//���ض�ͷԪ��
void * front_SeqQueue(seqQueue queue);

//���ض�βԪ��
void * back_SeqQueue(seqQueue queue);

//���д�С
int size_SeqQueue(seqQueue queue);

//�ж��Ƿ�Ϊ��
int isEmpty_SeqQueue(seqQueue queue);

//���ٶ���
void destroy_SeqQueue(seqQueue queue);
