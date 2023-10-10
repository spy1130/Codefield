#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;
/*����������Ľṹ*/
typedef struct Node
{
    char data;                    /*������*/
    struct Node *lchild, *rchild; /*��������������*/
} * BiTree, BiNode;
/*�������ͽ������*/

/*���贴��������*/
void CreateBiTree(BiTree &T)
{
    char ch;
    cin >> ch;
    if (ch == '#')
        T = NULL;
    else
    {
        T = new BiNode; /*����һ���½ڵ�*/
        T->data = ch;
        CreateBiTree(T->lchild);
        CreateBiTree(T->rchild);
    }
    /*�ݹ鴴��*/
}
void InOrderTraverse(BiTree T)
{
    /*�������*/
    if (T)
    {
        InOrderTraverse(T->lchild);
        cout << T->data;
        InOrderTraverse(T->rchild);
    }
}
void PreOrderTraverse(BiTree T)
{
    /*�������*/
    if (T)
    {
        cout << T->data;
        PreOrderTraverse(T->lchild);
        PreOrderTraverse(T->rchild);
    }
}
void PostOrderTraverse(BiTree T)
{
    /*�������*/
    if (T)
    {
        PostOrderTraverse(T->lchild);
        PostOrderTraverse(T->rchild);
        cout << T->data;
    }
}
/*ͳ�ƶ������н��ĸ���*/
int NodeCount(BiTree T)
{
    if (T == NULL)
        return 0;
    else
        return NodeCount(T->lchild) + NodeCount(T->rchild) + 1;
}
/*���������*/
int Depth(BiTree T)
{
    if (T == NULL)
        return 0;
    else
    {
        int i = Depth(T->lchild);
        int j = Depth(T->rchild);
        return i > j ? i + 1 : j + 1;
    }
}
/*���ƶ�����*/
void Copy(BiTree T, BiTree &NewT)
{
    if (T == NULL)
    {
        NewT = NULL;
        return;
    }
    else
    {
        NewT = new BiNode;
        NewT->data = T->data;
        Copy(T->lchild, NewT->lchild);
        Copy(T->rchild, NewT->rchild);
    }
}
/*ͳ�ƶ�������Ҷ�ӽ��ĸ���*/
int LeafCount(BiTree T)
{
    if (!T)
        return 0;
    if (!T->lchild && !T->rchild)
        return 1;
    /*�������������������������Ϊ�գ�˵���ö��������ڵ�ΪҶ�ӽ�㣬���Ϊ1*/
    else
        return LeafCount(T->lchild) + LeafCount(T->rchild);
}
/*�������д�ÿ��Ҷ�ӽ�㵽������·��*/
void PrintAllPath(BiTree T, char path[], int pathlen)
{
    int i;
    if (T != NULL)
    {
        path[pathlen] = T->data; /*����ǰ������·����*/
        if (T->lchild == NULL && T->rchild == NULL)
        {
            /*������ڵ���Ҷ�ӽ��*/
            for (i = pathlen; i >= 0; i--)
                cout << path[i] << " ";
            cout << "\n";
        }
        else
        {
            PrintAllPath(T->lchild, path, pathlen + 1);
            PrintAllPath(T->rchild, path, pathlen + 1);
        }
    }
}
/*�ж϶������Ƿ�Ϊ��*/
int BiTree_empty(BiTree T)
{
    if (T)
        return 1;
    else
        return 0;
}
int main()
{
    BiTree T;
    //��������AB#CD##E##F#GH###
    cout << "�����������(��#����):";
    CreateBiTree(T);
    cout << "������������";
    InOrderTraverse(T);
    cout << endl
         << "����������:";
    PreOrderTraverse(T);
    cout << "\n"
         << "����������:";
    PostOrderTraverse(T);
    cout << endl
         << "�������:" << Depth(T);
    cout << endl
         << "���ĸ���:" << NodeCount(T);
    cout << endl
         << "�������д�ÿ��Ҷ�ӽ�㵽����������·����" << endl;
    char path[256];
    int pathlen = 0;
    PrintAllPath(T, path, pathlen);
    return 0;
}
