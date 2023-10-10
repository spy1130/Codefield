#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <ctime>
using namespace std;

string rand_str(const int len);
typedef struct Data
{
    string name;
    int age;
} Data;
typedef struct Lnode
{
    Data note;   // ����������
    Lnode *next; // ����ָ����
} Lnode, *LinkList;
bool InitList(LinkList &L) // �������⻰��LinkList &L�ȼ��� Lnode *&L��Lnode *&L��һ��ָ��ָ�������
{
    L = new Lnode;     // ��������һ��ͷ��㣬������������ΪLnode
    L->next = nullptr; // �ձ�Ҳ����˵ͷ����ָ��ָ��Ϊ��
    return true;
}
void CreatListHead(LinkList &L, const int n)
{
    for (int i = 0; i < n; ++i)
    {
        Lnode *p = new Lnode;
        p->note.name = rand_str(5);
        // cin >> "input name:">>p->note.name;
        p->note.age = rand() % 100;
        // cin >> "input age:">>p->note.age;
        p->next = L->next;
        L->next = p;
    }
}
void CreatListTail(LinkList &L, const int n)
{
    Lnode *r = L;
    for (int i = 0; i < n; ++i)
    {
        Lnode *p = new Lnode;
        p->note.name = rand_str(5);
        // cin >> "input name:">>p->note.name;
        p->note.age = rand() % 100;
        // cin >> "input age:">>p->note.age;
        p->next = nullptr;
        r->next = p;
        r = p;
    }
}
void printlist(const LinkList &L, const int len)
{
    Lnode *p = L->next;
    for (int i = 1; i <= len; i++)
    {
        cout << p->note.name << ':'<<p->note.age<<" ";
        p=p->next;
    }
    cout<<endl;
}
int GetLength(const LinkList &L)
{
    Lnode *p;
    int i = 0;
    p = L->next;
    while (p != nullptr)
    {
        i++;
        p = p->next;
    }
    return i;
}
bool GetElem(const LinkList &L, const int &i, Data &e)
{
    if (i < 0)
    {
        cerr << "out of range" << endl;
        return false;
    }
    Lnode *p = L->next;
    for (int j = 1; j < i; ++j)
    {
        p = p->next;
        if (!p)
        {
            cerr << "out of range" << endl;
            return false; // �����ʱpΪ�գ���ζ���Ѿ���������β�ˣ�ֹͣѭ��
        }
    }
    e = p->note;
    return true;
}
int LocateElem(LinkList &L, Data &e)
{
    Lnode *p = L->next;
    int cnt = 1;
    while (p)
    {
        if (!(p->note.name.compare(e.name)))
        {
            return cnt;
        }
        ++cnt;
        p = p->next;
    }
    cerr << "not found" << endl;
    return cnt;
}
bool InsertList(LinkList &L, const int &i, const Data &e)
{
    Lnode *p = L;
    int j = 0;                     // ��������ж��ڵ�һ���ڵ�ǰ����ʱ�����
    for (j; (j < i - 1) && p; j++) // ��Χ1��n+1
    {                              // p��=NULL
        p = p->next;
    }
    if (!p || j > i - 1)
        return false;
    LinkList s = new Lnode;
    s->note = e;
    s->next = p->next; // sָ��p����һ��
    p->next = s;       // pָ��s
    return true;
}
bool deletelist(LinkList &L, const int &i, Data &e)
{
    Lnode *p = L, *s;
    int j = 0;
    for (j ; (j < i - 1) && p->next; j++) // ��Χ1��n
    {
        p = p->next;
    }
    if (!(p->next) || j > i - 1)
        return false;
    s = p->next;
    p->next = p->next->next;
    e = s->note; // ����ɾ���Ľڵ�
    delete s;
    return true;
}
bool clearlist(LinkList &L)
{
    Lnode *p, *q;
    p = L->next;
    while (p != nullptr)
    {
        q = p->next;
        delete p;
        p = q;
    }
    L->next = nullptr;
    return true;
}
bool IsEmpty(const LinkList &L)
{
    if (L->next) // �ǿ�
    {
        return false;
    }
    else
    {
        return true;
    }
}
bool DestroyList(LinkList &L)
{
    // �ж������Ƿ�Ϊ��
    if (IsEmpty(L))
    {
        cerr << "empty List!" << endl;
        return false;
    }
    while (L) // ����δ����β��
    {
        auto temp = L->next; // ��ͷָ��ָ����һ�����
        delete L;
        L = temp;
    }
    return true;
}
int main()
{
    srand(unsigned(time(nullptr)));
    LinkList L;
    Data S, e;
    S.name = "spric";
    S.age = 18;
    InitList(L);
    CreatListTail(L, 4);
    CreatListHead(L, 5);
    InsertList(L, 3, S);
    int length = GetLength(L);
    printlist(L, length);
    GetElem(L, 5, e);
    cout << e.name <<":"<< e.age << endl;
    int locate_nember = LocateElem(L, S);
    cout << locate_nember << endl;
    deletelist(L, 2, e);
    printlist(L, length - 1);
    cout << e.name << ":"<< e.age << endl;
    clearlist(L);
    IsEmpty(L);
    DestroyList(L);

    system("pause");
    return EXIT_SUCCESS;
}
string rand_str(const int len) /*����Ϊ�ַ����ĳ���*/
{
    /*��ʼ��*/
    string str; /*����������������ַ�����str*/
    char c;     /*�����ַ�c����������������ɵ��ַ�*/
    int idx;    /*����ѭ���ı���*/
    /*ѭ�����ַ��������������ɵ��ַ�*/
    for (idx = 0; idx < len; idx++)
    {
        c = 'a' + rand() % 26;
        str.push_back(c); /*push_back()��string��β�庯���������������ַ�c*/
    }
    return str; /*�������ɵ�����ַ���*/
}
