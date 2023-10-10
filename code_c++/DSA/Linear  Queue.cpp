#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#define ElemType int
#define MAXSIZE 100
using namespace std;
// ѭ������
struct SqQueue
{
    ElemType *elem;
    int front; // ͷָ��
    int rear;  // βָ��
};
void InitQueue(SqQueue &Q)
{
    Q.elem = new ElemType[MAXSIZE];
    Q.front = Q.rear = 0;
}
bool IsEmpty(SqQueue &Q)
{
    if (Q.front == Q.rear)
        return true;
    else
        return false;
}
bool IsFull(SqQueue &Q)
{
    auto rear_next = (Q.rear + 1) % MAXSIZE; // ѭ���ж��Ƿ�����F������һ��Ԫ�ؿռ�
    if (rear_next == Q.front)
        return true;
    else
        return false;
}
bool InsertQueue(SqQueue &Q, const ElemType &e)
{
    // �����β+1���ڶ�ͷ���������Ѿ����ˣ��ö���������һ���ռ��ѭ�����У����ӺͿնӵ��ж�������һ�£�
    if (IsFull(Q))
    {
        cerr << "full of Queue" << endl;
        return false;
    }
    Q.elem[Q.rear] = e;
    Q.rear = (Q.rear + 1) % MAXSIZE; // ѭ�����ģ���maxsize->0
    return true;
}
void CreatQueue(SqQueue &Q, const int n) // ��������
{
    cout << "input msg" << endl;
    ElemType input;
    for (int i = 0; i < n; ++i)
    {
        cin >> input;
        InsertQueue(Q, input);
    }
}
bool EraseQueue(SqQueue &Q)
{
    // �����ͷ���ڶ�β����������û��Ԫ�أ���ִ�иó���
    if (IsEmpty(Q))
    {
        cerr << "no elem to erase" << endl;
        return false;
    }
    // e = Q.elem[Q.front];
    Q.front = (Q.front + 1) % MAXSIZE;
    return true;
}
int GetLength(SqQueue &Q)
{
    return (Q.rear - Q.front + MAXSIZE) % MAXSIZE; // ȥ����Ϊ�˷�ֹrear��û��ʼѭ��ʱ���������
}
void PrintQueue(SqQueue &Q)
{
    cout << "Queue:" << endl;
    for (auto i = Q.front; i != Q.rear; i = (i + 1) % MAXSIZE)
    {
        cout << Q.elem[i] << endl;
    }
}
ElemType GetHead(const SqQueue Q)
{
    if (Q.front != Q.rear) // ���в�Ϊ��
        return Q.elem[Q.front];
}
int main()
{
    SqQueue Q;
    InitQueue(Q);
    CreatQueue(Q, 5);
    PrintQueue(Q);
    EraseQueue(Q);
    PrintQueue(Q);
    cout << "length:" << GetLength(Q) << endl;
    cout << "head:" << GetHead(Q) << endl;
    
    system("pause");
    return EXIT_SUCCESS;
}