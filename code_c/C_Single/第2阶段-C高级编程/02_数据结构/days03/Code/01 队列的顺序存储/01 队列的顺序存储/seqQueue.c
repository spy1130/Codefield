#include "seqQueue.h"

//��ʼ������
seqQueue init_SeqQueue()
{
	struct dynamicArray * array = init_dynamicArray(MAX);

	return array;
}
//���
void push_SeqQueue(seqQueue queue, void * data)
{
	//�ȼ��� β��
	if (queue == NULL)
	{
		return;
	}
	if (data == NULL)
	{
		return;
	}

	struct dynamicArray * array = queue;

	if (array->m_Size >= MAX)
	{
		return;
	}

	insert_dynamicArray(array, array->m_Size, data);
}
//����
void pop_SeqQueue(seqQueue queue)
{
	//�ȼ���  ͷɾ��
	if (queue == NULL)
	{
		return;
	}

	struct dynamicArray * array = queue;

	if (array->m_Size <= 0)
	{
		return;
	}
	removeByPos_dynamicArray(array, 0);
}
//���ض�ͷԪ��
void * front_SeqQueue(seqQueue queue)
{
	if (queue == NULL)
	{
		return NULL;
	}

	struct dynamicArray * array = queue;

	return array->pAddr[0];

}

//���ض�βԪ��
void * back_SeqQueue(seqQueue queue)
{
	if (queue == NULL)
	{
		return NULL;
	}

	struct dynamicArray * array = queue;
	
	return array->pAddr[array->m_Size - 1];

}

//���д�С
int size_SeqQueue(seqQueue queue)
{
	if (queue == NULL)
	{
		return -1;
	}

	struct dynamicArray * array = queue;

	return array->m_Size;

}

//�ж��Ƿ�Ϊ��
int isEmpty_SeqQueue(seqQueue queue)
{
	if (queue == NULL)
	{
		return -1;
	}

	struct dynamicArray * array = queue;
	if (array->m_Size == 0)
	{
		return 1;
	}

	return 0;
}

//���ٶ���
void destroy_SeqQueue(seqQueue queue)
{

	if (queue == NULL)
	{
		return ;
	}

	destroy_dynamicArray(queue);
	queue = NULL;
}