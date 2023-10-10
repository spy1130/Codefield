#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#define MAXSIZE 100
using namespace std;
struct Qnode
{
    int data; // Ԫ�ظ���
    Qnode *next;
};
struct LinkQueue // �ٶ���һ�������������ͣ�һ���Խ�������Qnodeָ��
{
    Qnode *front; // ͷָ��
    Qnode *rear;  // βָ��
};
void InitQueue(LinkQueue &Q)
{
    Q.front = Q.rear = new Qnode;
    Q.front->data = 0; // ���ڱ������ӵ�Ԫ�ظ���
    Q.rear->next = nullptr;
}
void InsertQueue(LinkQueue &Q, const int &e)
{
    // ��Ԫ�ز��������
    Qnode *temp = new Qnode;
    temp->data = e;
    temp->next = nullptr;
    Q.rear->next = temp;
    Q.rear = temp;
    ++Q.front->data; // Ԫ�ظ�����1
}
void CreatQueue(LinkQueue &Q, const int n)
{
    cout << "input msg" << endl;
    int input;
    for (int i = 0; i < n; ++i)
    {
        cin >> input;
        InsertQueue(Q, input);
    }
}
bool GetHead(LinkQueue Q, int &e)
{
    if (Q.front == Q.rear)
        return false;
    e = Q.front->next->data;
    return true;
}
bool EraseQueue(LinkQueue &Q)
{
    if (Q.front->next == nullptr)
    {
        cerr << "empty Queue" << endl;
        return false;
    }
    Qnode *p = Q.front->next;
    Q.front->next = p->next;
    if (Q.rear == p)
        Q.rear = Q.front;
    delete p;
    --Q.front->data; // Ԫ�ظ���-1
    return true;
}
int main()
{

    system("pause");
    return EXIT_SUCCESS;
}