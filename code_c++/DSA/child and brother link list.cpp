#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
//���������Һ������ף���˫���ѣ�
// ���ӽ��ṹ
typedef struct CTNode
{
    char child;
    struct CTNode *next;
} *ChildPtr;
// ˫�׽��ṹ
typedef struct
{
    char data;
    ChildPtr firstchild;
} CTBox;
// ���ṹ
typedef struct
{
    CTBox nodes[100]; // max=100
    int n, r;         // ������͸���λ��
} CTree;


// �����ֵ�����
typedef struct CSNode
{
    char data;
    struct CSNode *firstchild, *nextsibling;//ָ���һ�����Ӻ���һ���ֵ�
} CSNode, *CSTree;

//������
typedef struct BiTNode
{
    char data;
    struct BiTNode *lchild, *rchild; // ���Һ��ӵ�ָ��
} BiTNode, *BiTree;

// �����ֵ�����Ľ���
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
// �����ֵ�������������
void PreOrder(CSTree T)
{
    if (T)
    {
        cout << T->data;
        PreOrder(T->firstchild);
        PreOrder(T->nextsibling);
    }
}
// �����ֵ�����ĺ������
void PostOrder(CSTree T)
{
    if (T)
    {
        PostOrder(T->firstchild);
        cout << T->data;
        PostOrder(T->nextsibling);
    }
}
// �����ֵ���������
int Depth(CSTree T)
{
    int max = 0, dep;
    CSTree p;
    if (T)
    {
        p = T->firstchild;
        while (p)
        {
            dep = Depth(p);
            if (dep > max)
                max = dep;
            p = p->nextsibling;
        }
        return max + 1;
    }
    else
        return 0;
}
// �����ֵ������Ҷ�ӽ����
int LeafCount(CSTree T)
{
    int count = 0;
    CSTree p;
    if (T)
    {
        p = T->firstchild;
        if (!p)
            count++;
        while (p)
        {
            count += LeafCount(p);
            p = p->nextsibling;
        }
        return count;
    }
    else
        return 0;
}
// �����ֵ�����ĸ���
void Copy(CSTree &T, CSTree &NewT)
{
    if (T)
    {
        NewT = new CSNode;
        NewT->data = T->data;
        Copy(T->firstchild, NewT->firstchild);
        Copy(T->nextsibling, NewT->nextsibling);
    }
    else
        NewT = NULL;
}
// �����ֵ����������
void Destroy(CSTree &T)
{
    if (T)
    {
        Destroy(T->firstchild);
        Destroy(T->nextsibling);
        delete T;
    }
}
// �����ֵ��������ֵ
int Value(CSTree T)
{
    int x, y;
    if (T)
    {
        if (!T->firstchild)
            return T->data - '0';
        else
        {
            x = Value(T->firstchild);
            y = Value(T->firstchild->nextsibling);
            switch (T->data)
            {
            case '+':
                return x + y;
            case '-':
                return x - y;
            case '*':
                return x * y;
            case '/':
                return x / y;
            }
        }
    }
    else
        return 0;
}
// �����ֵ��������ֵ(��׺���ʽ)
int Value2(CSTree T)
{
    int x, y;
    if (T)
    {
        if (!T->firstchild)
            return T->data - '0';
        else
        {
            x = Value2(T->firstchild);
            y = Value2(T->firstchild->nextsibling);
            switch (T->data)
            {
            case '+':
                return x + y;
            case '-':
                return x - y;
            case '*':
                return x * y;
            case '/':
                return x / y;
            }
        }
    }
    else
        return 0;
}
// �����ֵ��������ֵ(ǰ׺���ʽ)
int Value3(CSTree T)
{
    int x, y;
    if (T)
    {
        if (!T->firstchild)
            return T->data - '0';
        else
        {
            x = Value3(T->firstchild->nextsibling);
            y = Value3(T->firstchild);
            switch (T->data)
            {
            case '+':
                return x + y;
            case '-':
                return x - y;
            case '*':
                return x * y;
            case '/':
                return x / y;
            }
        }
    }
    else
        return 0;
}
// �����ֵ��������ֵ(��׺���ʽ)
int Value4(CSTree T)
{
    int x, y;
    if (T)
    {
        if (!T->firstchild)
            return T->data - '0';
        else
        {
            x = Value4(T->firstchild);
            y = Value4(T->firstchild->nextsibling);
            switch (T->data)
            {
            case '+':
                return x + y;
            case '-':
                return x - y;
            case '*':
                return x * y;
            case '/':
                return x / y;
            }
        }
    }
    else
        return 0;
}
//�����ĺ����ֵ������ʾת��Ϊ��������ʾ
void CSTree2BiTree(CSTree T, BiTree &BT)
{
    if (T)
    {
        BT = new BiTNode;
        BT->data = T->data;
        CSTree2BiTree(T->firstchild, BT->lchild);
        CSTree2BiTree(T->nextsibling, BT->rchild);
    }
    else
        BT = NULL;
}
//��������ת��Ϊ���ĺ����ֵ�����
void BiTree2CSTree(BiTree BT, CSTree &T)
{
    if (BT)
    {
        T = new CSNode;
        T->data = BT->data;
        BiTree2CSTree(BT->lchild, T->firstchild);
        BiTree2CSTree(BT->rchild, T->nextsibling);
    }
    else
        T = NULL;
}
int main()
{
    CSTree T;
    CreateCSTree(T);
    PreOrder(T);
    cout << endl;
    PostOrder(T);
    cout << endl;
    cout << Depth(T) << endl;
    cout << LeafCount(T) << endl;
    CSTree NewT;
    Copy(T, NewT);
    PreOrder(NewT);
    cout << endl;
    cout << Value(T) << endl;
    cout << Value2(T) << endl;
    cout << Value3(T) << endl;
    cout << Value4(T) << endl;
    BiTree BT;
    CSTree2BiTree(T, BT);
    BiTree2CSTree(BT, T);
    PreOrder(T);
    cout << endl;
    Destroy(T);
    system("pause");
    return EXIT_SUCCESS;
}