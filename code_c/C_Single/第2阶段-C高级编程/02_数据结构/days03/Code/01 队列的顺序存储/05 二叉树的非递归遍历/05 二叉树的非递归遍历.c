#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "seqStack.h"

/*
	1 �� �����ڵ�ѹ��ջ��
	2 �� ִ��ѭ��
		2.1 ��ȡջ��Ԫ��
		2.2 ����ջ��Ԫ��
		2.3 ���ջ����־λ�� ֱ�����
		2.4 �����־λ��  ����־��Ϊ��
		2.5 �� ������  ������  �� ��ջ
*/


struct BinaryNode
{
	char ch;
	struct BinaryNode * lChild;
	struct BinaryNode * rChild;
	int flag; //��־
};


void nonRecursion(struct BinaryNode * root)
{
	//��ʼ��ջ 
	SeqStack myStack = init_SeqStack();

	push_SeqStack(myStack, root);

	while (size_SeqStack(myStack) >0)
	{
		//��ȡջ��Ԫ��
		struct BinaryNode * topNode =  top_SeqStack(myStack);

		//����ջ��Ԫ��
		pop_SeqStack(myStack);

		//���ջ����־λ�� ֱ�����
		if (topNode->flag == 1)
		{
			printf("%c ",topNode->ch);
			continue;
		}

		//�����־λ��  ����־��Ϊ��
		topNode->flag = 1;


		//�� ������  ������  �� ��ջ
		if (topNode->rChild != NULL)
		{
			push_SeqStack(myStack, topNode->rChild);
		}

		if (topNode->lChild != NULL)
		{
			push_SeqStack(myStack, topNode->lChild);
		}

		push_SeqStack(myStack, topNode);

	}

	//����ջ 
	destroy_SeqStack(myStack);
}

void test01()
{
	//�����ڵ�
	struct BinaryNode nodeA = { 'A', NULL, NULL, 0 };
	struct BinaryNode nodeB = { 'B', NULL, NULL, 0 };
	struct BinaryNode nodeC = { 'C', NULL, NULL, 0 };
	struct BinaryNode nodeD = { 'D', NULL, NULL, 0 };
	struct BinaryNode nodeE = { 'E', NULL, NULL, 0 };
	struct BinaryNode nodeF = { 'F', NULL, NULL, 0 };
	struct BinaryNode nodeG = { 'G', NULL, NULL, 0 };
	struct BinaryNode nodeH = { 'H', NULL, NULL, 0 };

	//������ϵ
	nodeA.lChild = &nodeB;
	nodeA.rChild = &nodeF;

	nodeB.rChild = &nodeC;

	nodeC.lChild = &nodeD;
	nodeC.rChild = &nodeE;

	nodeF.rChild = &nodeG;

	nodeG.lChild = &nodeH;


	//�ǵݹ����
	nonRecursion(&nodeA);
}

int main(){

	test01();

	system("pause");
	return EXIT_SUCCESS;
}