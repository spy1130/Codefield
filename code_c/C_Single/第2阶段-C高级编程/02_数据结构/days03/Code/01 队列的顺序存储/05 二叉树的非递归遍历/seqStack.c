#include "seqStack.h"


//��ʼ��ջ
SeqStack init_SeqStack()
{
	struct SStack *myStack = malloc(sizeof(struct SStack));

	if (myStack == NULL)
	{
		return NULL;
	}

	memset(myStack->data, 0, sizeof(void *)* MAX); //�����ÿ�

	myStack->m_Size = 0;

	return myStack;
}
//��ջ

void push_SeqStack(SeqStack stack, void * data)
{
	//���ʾ�������β��

	if (stack == NULL)
	{
		return;
	}
	if (data == NULL)
	{
		return;
	}

	struct SStack * myStack = stack;

	if (myStack->m_Size == MAX)
	{
		return;
	}

	myStack->data[myStack->m_Size] = data;

	myStack->m_Size++;

}


//��ջ
void pop_SeqStack(SeqStack stack)
{
	//��ջ���� βɾ
	if (stack == NULL)
	{
		return;
	}
	struct SStack * myStack = stack;

	//����ǿ�ջ ֱ�� return
	if (myStack->m_Size == 0)
	{
		return;
	}

	myStack->data[myStack->m_Size - 1] = NULL;
	myStack->m_Size--;
}


//����ջ��Ԫ��
void * top_SeqStack(SeqStack stack)
{
	if (stack == NULL)
	{
		return NULL;
	}
	struct SStack * myStack = stack;

	//����ǿ�ջ ����NULL
	if (myStack->m_Size == 0)
	{
		return NULL;
	}

	return myStack->data[myStack->m_Size - 1];
}


//��ȡջ��С
int size_SeqStack(SeqStack stack)
{
	if (stack == NULL)
	{
		return 0;
	}
	struct SStack * myStack = stack;

	return myStack->m_Size;

}

//�ж�ջ�Ƿ�Ϊ��
int isEmpty_SeqStack(SeqStack stack)
{

	if (stack == NULL)
	{
		return -1; //����������NULL������Ҳ�ǿ�ջ
	}
	struct SStack * myStack = stack;

	if (myStack->m_Size == 0)
	{
		return 1;
	}

	return 0;

}


//����ջ
void destroy_SeqStack(SeqStack stack)
{
	if (stack == NULL)
	{
		return;
	}

	free(stack);
	stack = NULL;
}
