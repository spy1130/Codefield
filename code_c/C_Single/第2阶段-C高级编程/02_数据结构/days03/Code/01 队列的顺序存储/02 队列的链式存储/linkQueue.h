#pragma  once
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//�ڵ�ṹ��
struct QueueNode
{
	struct QueueNode * next;

};

//����Ľṹ�� --- ����
struct LQueue
{
	struct QueueNode pHeader; //ͷ�ڵ�
	int m_Size; //���еĴ�С
	struct QueueNode * pTail; //��¼β�ڵ��ָ��
};

typedef void * LinkQueue;

//��ʼ������
LinkQueue init_LinkQueue();
//���
void push_LinkQueue(LinkQueue queue, void * data);
//����
void pop_LinkQueue(LinkQueue queue);
//���ض�ͷ
void * front_LinkQueue(LinkQueue queue);
//���ض�β
void * back_LinkQueue(LinkQueue queue);
//���ض��д�С
int size_LinkQueue(LinkQueue queue);
//�ж϶����Ƿ�Ϊ��
int isEmpty_LinkQueue(LinkQueue queue);
//���ٶ���
void destroy_LinkQueue(LinkQueue queue);