#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
using namespace std;
#define MAX 20
typedef char TElemType;
typedef int Status;
typedef struct BiTNode
{
    TElemType data;
    struct BiTNode *lchild, *rchild; // ���Һ��ӵ�ָ��
} BiTNode, *BiTree;
typedef struct
{
    BiTree link;
    int flag;
} stacktype;
// ���򴴽�������
void CreateBiTree(BiTree &T)
{
    char ch;
    ch = getchar();
    if (ch == '#')
        T = NULL; // #�����ָ��
    else
    {
        T = new BiTNode; // ����ڵ�
        T->data = ch;    // ���ɸ��ڵ�
        CreateBiTree(T->lchild);
        CreateBiTree(T->rchild);
    }
}
// �������������
void PreOrder(BiTree T)
{
    if (T)
    {
        printf("%2c", T->data); // ���ʸ��ڵ㣬�˴�Ϊ������ڵ������ֵ
        PreOrder(T->lchild);    // �������������
        PreOrder(T->rchild);    // �������������
    }
}
// �������������
void InOrder(BiTree T)
{
    if (T)
    {
        InOrder(T->lchild);
        printf("%2c", T->data);
        InOrder(T->rchild);
    }
}
// �������������
void PostOrder(BiTree T)
{
    if (T)
    {
        PostOrder(T->lchild);
        PostOrder(T->rchild);
        printf("%2c", T->data);
    }
}
// ����ǵݹ�����
int NRPreOrder(BiTree T) // �ǵݹ��������������
{
    BiTree stack[MAX], p;
    int top; // ջ��
    if (T == NULL)
        return 0; // ��Ϊ���򷵻�
    top = 0;
    p = T;                           // ����һ�����ڴ洢�ƶ���ָ��ı���
    while (!(p == NULL && top == 0)) // ������㲻Ϊ����ջ��Ϊ��ʱ
    {
        while (p != NULL) // ������������
        {
            printf("%2c", p->data); // �ڴ˴��ô�ӡ��ʾ���ʸý��
            if (top <= MAX - 1)
            {
                stack[top] = p; // ��ջ
                top++;          // ջ��λ��++
            }
            else // ջ��ʱ
            {
                printf("ջ���");
                return 0;
            }
            p = p->lchild; // ���뵱ǰ������������
        }
        if (top <= 0)
            return 0;
        else
        {
            top--;          // ջ��--,
            p = stack[top]; // ����ջ��Ԫ��
            p = p->rchild;  // ָ��������
        }
    }
}
// ����ǵݹ�����
int NRInOrder(BiTree T) // �ǵݹ��������������
{
    BiTree stack[MAX], p;
    int top; // ջ��
    if (T == NULL)
        return 0; // ��Ϊ���򷵻�
    top = 0;
    p = T;                           // ����һ�����ڴ洢�ƶ���ָ��ı���
    while (!(p == NULL && top == 0)) // ������㲻Ϊ����ջ��Ϊ��ʱ
    {
        while (p != NULL) // ������������
        {
            if (top <= MAX - 1)
            {
                stack[top] = p; // ��ջ
                top++;          // ջ��λ��++
            }
            else // ջ��ʱ
            {
                printf("ջ���");
                return 0;
            }
            p = p->lchild; // ���뵱ǰ������������
        }
        if (top <= 0)
            return 0;
        else
        {
            top--;                  // ջ��--,
            p = stack[top];         // ����ջ��Ԫ��
            printf("%2c", p->data); // �ڴ˴��ô�ӡ��ʾ���ʸý��
            p = p->rchild;          // ָ��������
        }
    }
}
// ��α���������
void LevelOrder(BiTree T)
{
    BiTree Queue[MAX], b; // ��һά�����ʾ���У�front��rear��ʾ���׺Ͷ�β��ָ��
    int front, rear;
    front = rear = 0;
    if (T)
    // ����Ϊ��
    {
        Queue[rear++] = T;    // ���ڵ������
        while (front != rear) // �����зǿ�
        {
            b = Queue[front++]; // ����Ԫ�س����У�����������ڵ�
            printf("%2c", b->data);
            if (b->lchild != NULL)
                Queue[rear++] = b->lchild; // ���������ǿգ��������
            if (b->rchild != NULL)
                Queue[rear++] = b->rchild; // ���������ǿգ��������
        }
    }
}
// ���������
int depth(BiTree T)
{
    int dep1, dep2;
    if (T == NULL)
        return 0;
    else
    {
        dep1 = depth(T->lchild);
        dep2 = depth(T->rchild);
        return dep1 > dep2 ? dep1 + 1 : dep2 + 1;
    }
}
// ����ǵݹ�
void Nrpostorder(BiTree T)
{
    stacktype stack[MAX];
    BiTree p;
    int top, sign;
    if (T == NULL)
        return;
    top = -1;
    p = T;
    while (!(p == NULL && top == -1))
    {
        if (p != NULL)
        {
            top++;
            stack[top].link = p;
            stack[top].flag = 1;
            p = p->lchild;
        }
        else
        {
            p = stack[top].link;
            sign = stack[top].flag;
            top--;
            if (sign == 1)
            {
                top++;
                stack[top].link = p;
                stack[top].flag = 2;
                p = p->rchild;
            }
            else
            {
                printf("%2c", p->data);
                p = NULL;
            }
        }
    }
}

int copy(BiTree T, BiTree &NewT)
{
    if (T == NULL)
        NewT = NULL;
    else
    {
        NewT = new BiTNode;
        NewT->data = T->data;
        copy(T->lchild, NewT->lchild);
        copy(T->rchild, NewT->rchild);
    }
    return 1;
}
int NoteCount(BiTree T)
{
    if (T == NULL)
        return 0;
    else
        return NoteCount(T->lchild) + NoteCount(T->rchild) + 1; // 1Ϊ��ͬ�׶β�ͬ�ĸ��ڵ�
}
int LeafCount(BiTree T)
{
    if (T == NULL)
        return 0;
    else if (T->lchild == NULL && T->rchild == NULL)
        return 1;
    else
        return LeafCount(T->lchild) + LeafCount(T->rchild);
}
int main()
{
    BiTree T = NULL;
    BiTree NewT = NULL;
    printf("\n ����һ�ö������� \n");
    CreateBiTree(T); // ����������
    printf("\n��������Ľ��Ϊ:\n");
    PreOrder(T); // �������
    printf("\n����ǵݹ�����Ľ��Ϊ:\n");
    NRPreOrder(T);
    printf("\n��������Ľ��Ϊ:\n");
    InOrder(T); // �������
    printf("\n����ǵݹ�����Ľ��Ϊ:\n");
    NRInOrder(T); // �������
    printf("\n ��������Ľ��Ϊ�� \n");
    PostOrder(T);
    printf("\n ��������ǵݹ�Ľ��Ϊ�� \n");
    Nrpostorder(T);
    printf("\n ��α����Ľ��Ϊ: \n");
    LevelOrder(T); // ��α���
    printf("\n �������Ϊ:%d\n", depth(T));
    printf("\n ���Ľ�����Ϊ:%d\n", NoteCount(T));
    printf("\n ����Ҷ�ӽ�����Ϊ:%d\n", LeafCount(T));
    copy(T, NewT);
    printf("\n���ƺ��������������Ľ��Ϊ:\n");
    PreOrder(NewT); // �������
    printf("\n���ƺ��������������Ľ��Ϊ:\n");
    InOrder(NewT); // �������
    printf("\n���ƺ�����ĺ�������Ľ��Ϊ:\n");
    PostOrder(NewT); // �������
    printf("\n���ƺ�����Ĳ�α����Ľ��Ϊ:\n");
    LevelOrder(NewT); // ��α���
    printf("\n");
    printf("\n ���ƺ����Ľ�����Ϊ:%d\n", NoteCount(T));
    printf("\n ���ƺ�����Ҷ�ӽ�����Ϊ:%d\n", LeafCount(T));

    return 0;
}
// https://www.cnblogs.com/kangna/p/11846156.html#:~:text=void%20NRPreOrder%28BiTree%20t%29%20%2F%2F%E9%9D%9E%E9%80%92%E5%BD%92%E5%85%88%E5%BA%8F%E9%81%8D%E5%8E%86%E4%BA%8C%E5%8F%89%E6%A0%91%20%7B%20BiTree%20stack%20%5Bmaxsize%5D%2Cp%3B,printf%20%EF%BC%88%20%22%252c%22%2C%26p-%3Edata%EF%BC%89%EF%BC%9B%20%2F%2F%E5%9C%A8%E6%AD%A4%E5%A4%84%E7%94%A8%E6%89%93%E5%8D%B0%E8%A1%A8%E7%A4%BA%E8%AE%BF%E9%97%AE%E8%AF%A5%E7%BB%93%E7%82%B9%20p%3Dp-%3Erchild%3B%20%2F%2F%E6%8C%87%E5%90%91%E5%8F%B3%E5%AD%90%E6%A0%91%20%7D%20%7D
