#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
// ɭ��
typedef struct CSNode
{
    char data;
    struct CSNode *firstchild, *nextsibling;
} CSNode, *CSTree;
// �������ṹ
typedef struct BiTNode
{
    char data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

void CreateCSTree(CSTree &T)
{
    char ch;
    cin >> ch;
    if (ch == '#')
        T = NULL;
    else
    {
        T = new CSNode;
        T->data = ch;
        CreateCSTree(T->firstchild);
        CreateCSTree(T->nextsibling);
    }
}
// ɭ�ֵ��������
void PreOrder(CSTree T)
{
    if (T)
    {
        cout << T->data;
        PreOrder(T->firstchild);
        PreOrder(T->nextsibling);
    }
}
// ɭ�ֵĺ������
void PostOrder(CSTree T)
{
    if (T)
    {
        PostOrder(T->firstchild);
        cout << T->data;
        PostOrder(T->nextsibling);
    }
}
// ɭ�ֵĲ�α���
void LevelOrder(CSTree T)
{
    CSTree queue[100];
    int front = 0, rear = 0;
    if (T)
    {
        rear = (rear + 1) % 100;
        queue[rear] = T;
        while (front != rear)
        {
            front = (front + 1) % 100;
            T = queue[front];
            cout << T->data;
            if (T->firstchild)
            {
                rear = (rear + 1) % 100;
                queue[rear] = T->firstchild;
            }
            if (T->nextsibling)
            {
                rear = (rear + 1) % 100;
                queue[rear] = T->nextsibling;
            }
        }
    }
}
// ɭ��ת������
void ForestToBinaryTree(CSTree T, BiTree &BT)
{
    if (T)
    {
        BT = new BiTNode;
        BT->data = T->data;
        ForestToBinaryTree(T->firstchild, BT->lchild);
        ForestToBinaryTree(T->nextsibling, BT->rchild);
    }
    else
        BT = NULL;
}
// ������תɭ��
void BinaryTreeToForest(BiTree BT, CSTree &T)
{
    if (BT)
    {
        T = new CSNode;
        T->data = BT->data;
        BinaryTreeToForest(BT->lchild, T->firstchild);
        BinaryTreeToForest(BT->rchild, T->nextsibling);
    }
    else
        T = NULL;
}
// ɭ�ֵ�Ҷ�ӽ����
int LeafCount(CSTree T)
{
    if (T == NULL)
        return 0;
    else if (T->firstchild == NULL)
        return 1 + LeafCount(T->nextsibling);
    else
        return LeafCount(T->firstchild) + LeafCount(T->nextsibling);
}
// ɭ�ֵĸ߶�
int Height(CSTree T)
{
    int hc, hs;
    if (T == NULL)
        return 0;
    else
    {
        hc = Height(T->firstchild);
        hs = Height(T->nextsibling);
        return (hc + 1 > hs) ? hc + 1 : hs;
    }
}
// ɭ�ֵĿ��
int Width(CSTree T)
{
    int w, maxw = 0;
    while (T)
    {
        w = Width(T->firstchild);
        if (w > maxw)
            maxw = w;
        T = T->nextsibling;
    }
    return maxw;
}
// ɭ�ֵĽ����
int NodeCount(CSTree T)
{
    if (T == NULL)
        return 0;
    else
        return 1 + NodeCount(T->firstchild) + NodeCount(T->nextsibling);
}
// ɭ�ֵĶ�Ϊk�Ľ����
int DegreeCount(CSTree T, int k)
{
    if (T == NULL)
        return 0;
    else if (k == 0)
        return 1;
    else
        return DegreeCount(T->firstchild, k - 1) + DegreeCount(T->nextsibling, k);
}
// ɭ�ֵĶ����Ľ��
CSTree MaxDegreeNode(CSTree T)
{
    CSTree p, q;
    int max = 0;
    if (T == NULL)
        return NULL;
    else
    {
        p = T;
        while (p)
        {
            if (DegreeCount(p, 1) > max)
            {
                max = DegreeCount(p, 1);
                q = p;
            }
            p = p->nextsibling;
        }
        return q;
    }
}
// ɭ�ֵĶ���С�Ľ��
CSTree MinDegreeNode(CSTree T)
{
    CSTree p, q;
    int min = 100;
    if (T == NULL)
        return NULL;
    else
    {
        p = T;
        while (p)
        {
            if (DegreeCount(p, 1) < min)
            {
                min = DegreeCount(p, 1);
                q = p;
            }
            p = p->nextsibling;
        }
        return q;
    }
}
//ɭ�ֵ�����
void DestroyForest(CSTree &T)
{
    if (T)
    {
        DestroyForest(T->firstchild);
        DestroyForest(T->nextsibling);
        delete T;
    }
}

int main()
{
    CSTree T;
    BiTree BT;
    CreateCSTree(T);
    cout << "ɭ�ֵ��������Ϊ��";
    PreOrder(T);
    cout << endl;
    cout << "ɭ�ֵĺ������Ϊ��";
    PostOrder(T);
    cout << endl;
    cout << "ɭ�ֵĲ�α���Ϊ��";
    LevelOrder(T);
    cout << endl;
    cout << "ɭ�ֵ�Ҷ�ӽ����Ϊ��" << LeafCount(T) << endl;
    cout << "ɭ�ֵĸ߶�Ϊ��" << Height(T) << endl;
    cout << "ɭ�ֵĿ��Ϊ��" << Width(T) << endl;
    cout << "ɭ�ֵĽ����Ϊ��" << NodeCount(T) << endl;
    cout << "ɭ�ֵĶ�Ϊ2�Ľ����Ϊ:" << DegreeCount(T, 2) << endl;
    cout << "ɭ�ֵĶ����Ľ��Ϊ��" << MaxDegreeNode(T)->data << endl;
    cout << "ɭ�ֵĶ���С�Ľ��Ϊ��" << MinDegreeNode(T)->data << endl;
    ForestToBinaryTree(T, BT);
    BinaryTreeToForest(BT, T);
    cout << "������תɭ�ֵ��������Ϊ��";
    PreOrder(T);
    cout << endl;
    cout << "������תɭ�ֵĺ������Ϊ��";
    PostOrder(T);
    cout << endl;
    cout << "������תɭ�ֵĲ�α���Ϊ��";
    LevelOrder(T);
    cout << endl;
    DestroyForest(T);
    
    system("pause");
    return EXIT_SUCCESS;
}
