#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#define ElemType int
#define MAXSIZE 50
using namespace std;

typedef struct SqStack
{
    ElemType *base;//�ײ�
    ElemType *top;//��������һ�񣬷���������ӵ�ϻ״��
    int stacksize;//maxsize
}SqStack;

bool InitStack(SqStack &S)
{
    S.base = new ElemType[MAXSIZE];//ElwmTypr���͵�������Maxsize��ô��ĵ�Ԫ�ռ�
    if (!S.base)
    {
        cerr << "failed to get memory" << endl;
        return false;
    }
    S.top = S.base;//��ջ��־
    S.stacksize = MAXSIZE; 
    return true;
}
bool Push(SqStack &S, ElemType &e)
{
    //�ж�ջ�Ƿ�����
    if((S.top-S.base) == S.stacksize)//ָ�����Ϊ��ַ�����
    {
        cerr << "full of stack" << endl;
        return false;
    }
    *(S.top) = e;
    ++(S.top);
    return true;
}
bool CreatStack(SqStack &S, const int n)
{
    for (int i = 0; i < n;++i)
    {
        ElemType input;
        cin >> input;
        if(!Push(S,input))
        {
            cerr << "error happend at-" << i << endl;
            return false;
        }
    }
    return true;
}
bool IsEmpty(SqStack &S)
{
    if (S.base == S.top)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Pop(SqStack &S, ElemType &e)//��ջ
{
    if(!IsEmpty(S))
    {
        cerr << "empty stack!error" << endl;
        return false;
    }
    --(S.top);
    e = *(S.top);
    return true;
}
int StackLength(SqStack &S)
{
    return static_cast<int>(S.top - S.base);
}
bool ClearStack(SqStack &S)
{
    if(S.base)
    {
        S.top = S.base;
        return true;
    }
    else
    {
        return false;
    }
}
void DestoyStack(SqStack &S)
{
    if(S.base)
    {
        delete[] S.base;
        S.stacksize = 0;
        S.base = S.top = nullptr;
    }
}
int main(){
    SqStack S;
    InitStack(S);
    CreatStack(S, 5);
    ElemType e;
    Pop(S, e);
    cout << e << endl;
    cout << StackLength(S) << endl;
    ClearStack(S);
    cout << StackLength(S) << endl;
    DestoyStack(S);

    system("pause");
    return EXIT_SUCCESS;
}