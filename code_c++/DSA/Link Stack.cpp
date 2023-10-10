#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

typedef struct StackNode
{
    int data;
    StackNode *next;
} * LinkStack;
void InitStack(LinkStack &S)
{
    //����ͷ���
    S = new StackNode;
    S->next = nullptr;
    S->data = 0; //��ʾջ��Ԫ�ظ���
}
//����ʦ����Ƶ��û������ͷ���
//��������һ��ͷ��㣬��ͷ�����������б���ջ��Ԫ�ظ���

void Push(LinkStack &S, const int &e)
{
    //����Ԫ��
    StackNode *temp = new StackNode;
    temp->data = e;
    temp->next = S->next;
    S->next = temp;
    ++(S->data);//Ԫ�ظ�����һ
}
void Pop(LinkStack &S, int &e)
{
    //ɾ��Ԫ��
    StackNode *p = S->next;
    e = p->data;
    S->next = p->next;
    --(S->data);
    delete p;
}
void CreatStack(LinkStack &S, const int n)
{
    int input;
    for (int i = 0; i < n;++i)
    {
        cin >> input;
        Push(S, input);
    }
}
int StackLength(LinkStack &S)
{
    return S->data;
}
bool IsEmpty(LinkStack &S)
{
    if(!(S->next))
    {
        return true;
    }
    else 
    {
        return false;
    }
}
bool ClearStack(LinkStack &S)
{
    if(!(S->next))
    {
        cerr << "empty Stack" << endl;
        return false;
    }
    StackNode *q, *p = S->next;
    while(p)
    {
        q = p;
        p = p->next;
        delete q;
    }
    S->data = 0;
    S->next = nullptr;
    return true;
}
bool DestoryStack(LinkStack &S)
{
    if(S)
    {
        cerr << "error" << endl;
        return false;
    }
    while(S)
    {
        StackNode *temp = S;
        S = S->next;
        delete temp;
    }
    return true;
}
int main(){
    LinkStack S;
    InitStack(S);
    CreatStack(S, 5);
    int e;
    Pop(S, e);
    cout << e << endl;
    cout << StackLength(S) << endl;
    ClearStack(S);
    cout << StackLength(S) << endl;
    DestoryStack(S);

    system("pause");
    return EXIT_SUCCESS;
}