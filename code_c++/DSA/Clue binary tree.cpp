#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
// �����������������һ��ͷ���

typedef struct BiThrNode
{
    char data;
    struct BiThrNode *lchild, *rchild;
    int LTag, RTag;
} BiThrNode, *BiThrTree;
BiThrTree pre; // ȫ�ֱ�����ʼ��ָ��ոշ��ʹ��Ľڵ�
// ���򴴽�������
void CreateBiThrTree(BiThrTree &T)
{
    char ch;
    cin >> ch;
    if (ch == '#')
        T = NULL;
    else
    {
        T = new BiThrNode;
        T->data = ch;
        T->LTag = 0;
        T->RTag = 0;
        CreateBiThrTree(T->lchild);
        CreateBiThrTree(T->rchild);
    }
}
// �������������
void PreThreading(BiThrTree T)
{
    if (T)
    {
        if (!T->lchild) // û������
        {
            T->LTag = 1; // ǰ������
            T->lchild = pre;
        }
        if (!pre->rchild) // ǰһ���ڵ�û���Һ���
        {
            pre->RTag = 1; // �������
            pre->rchild = T;
        }
        pre = T; // ����preָ��T��ǰ��
        PreThreading(T->lchild);
        PreThreading(T->rchild);
    }
}
// �������������
void InThreading(BiThrTree T)
{
    if (T)
    {
        InThreading(T->lchild); // �ݹ�������������
        if (!T->lchild)         // û������
        {
            T->LTag = 1; // ǰ������
            T->lchild = pre;
        }
        if (!pre->rchild) // ǰһ���ڵ�û���Һ���
        {
            pre->RTag = 1; // �������
            pre->rchild = T;
        }
        pre = T; // ����preָ��T��ǰ��
        InThreading(T->rchild);
    }
}
// �������������
void PostThreading(BiThrTree T)
{
    if (T)
    {
        PostThreading(T->lchild);
        PostThreading(T->rchild);
        if (!T->lchild) // û������
        {
            T->LTag = 1; // ǰ������
            T->lchild = pre;
        }
        if (!pre->rchild) // ǰһ���ڵ�û���Һ���
        {
            pre->RTag = 1; // �������
            pre->rchild = T;
        }
        pre = T; // ����preָ��T��ǰ��
    }
}
// �������������
void InOrder(BiThrTree T)
{
    BiThrTree p;
    p = T->lchild; // pָ����ڵ�
    while (p != T)
    {
        while (p->LTag == 0)
            p = p->lchild;
        cout << p->data;
        while (p->RTag == 1 && p->rchild != T)
        {
            p = p->rchild;
            cout << p->data;
        }
        p = p->rchild;
    }
}
int main()
{
    BiThrTree T;
    CreateBiThrTree(T);
    pre = T;
    // PreThreading(T);
    // PostThreading(T);
    InThreading(T);
    InOrder(T);
    system("pause");
    return 0;
}
