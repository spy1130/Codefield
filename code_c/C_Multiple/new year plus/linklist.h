#pragma once
#include <stdio.h>
#include <stdlib.h>
// ������λ
struct node
{
    int data;
    struct node *next;
};
//�ⲿ������Ҫ�������� __declspec(dllexport)
// ����ͷ����(��Ҫһ��ͬ����ָ�����)
__declspec(dllexport) struct node *creatlist();
//�����µĽڵ�
struct node *newnode(int data);
// ��ͷ�����룬������ͷ�ڵ������
void insertbyhead(struct node *headnode, int data);
// ������ӡ����
void printlist(struct node *headnode);
// ��β�����루�����µĽڵ㣩
void insertbytail(struct node *headnode, int data);
// ����ĳ�����ݵ�ǰ��ĺ���
void insert(struct node *headnode, int data, int posdata);
// ��ͷ��ɾ���ڵ�
void deletebyhead(struct node *headnode);
// ��β��ɾ���ڵ�
void deletebytail(struct node *headnode);
// ���Ҳ�ɾ��
void deletenode(struct node *headnode, int data);
// �������ֻ������������
void clear_LinkList(struct node *headnode);
// �����б�(ɾ���������)
void destroy_LinkList(struct node *headnode);
// ����ķ�ת
void reverse_Linklist(struct node* pHeader);
// ͳ���������
int size_linklist(struct node* pHeader);