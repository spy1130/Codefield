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
typedef struct DuLnode
{
    Data note;             // ����������
    DuLnode *prior, *next; // ����ָ����
} DuLnode, *LinkList;
bool InitList(LinkList &L) // �������⻰��LinkList &L�ȼ��� DuLnode *&L��DuLnode *&L��һ��ָ��ָ�������
{
    L = new DuLnode;    // ��������һ��ͷ��㣬������������ΪDuLnode
    L->prior = nullptr; // ˫������
    L->next = nullptr;  // �ձ�Ҳ����˵ͷ����ָ��ָ��Ϊ��
    return true;
}
void CreatListHead(LinkList &L, const int n)
{
    for (int i = 0; i < n; ++i)
    {
        DuLnode *p = new DuLnode;
        if (p == NULL)
        {
            cout << "Memory allocation failed." << endl;
            return;
        }
        p->note.name = rand_str(5);
        // cin >> "input name:">>p->note.name;
        p->note.age = rand() % 100;
        // cin >> "input age:">>p->note.age;
        p->prior = L;
        if (L->next == NULL)
        { // the list is empty
            p->next = NULL;
        }
        else
        { // the list is not empty
            p->next = L->next;
            L->next->prior = p;
        }
        L->next = p;
    }
}
void CreatListTail(LinkList &L, const int n)
{
    DuLnode *r = L; // β�ڵ㣬
    for (int i = 0; i < n; ++i)
    {
        DuLnode *p = new DuLnode;

        if (p == NULL)
            return; // ����Ƿ����ɹ�
        p->note.name = rand_str(5);
        // cin >> "input name:">>p->note.name;
        p->note.age = rand() % 100;
        // cin >> "input age:">>p->note.age;
        p->prior = r;
        p->next = r->next; // r->next�������ѭ��������һֱΪ��
        r->next = p;
        r = p; // ��r�ƶ���ĩβ
    }
}
void printlist(const LinkList &L, const int len)
{
    DuLnode *p = L->next;
    for (int i = 1; i <= len; i++)
    {
        cout << p->note.name << ':' << p->note.age << " ";
        p = p->next;
    }
    cout << endl;
}
int GetLength(const LinkList &L)
{
    DuLnode *p;
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
    DuLnode *p = L->next;
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
    DuLnode *p = L->next;
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
bool InsertList(LinkList &L, const int i, const Data &e)
{
    // �ƶ�ָ�뵽i��
    DuLnode *p = L->next;
    int j = 1;
    while ((p->next) && (j < i))
    {
        ++j;
        p = p->next;
    }
    if (j < i || j < 1) // ���i������Χ�ڣ������whileѭ������ֹ��������j<i
    {
        cerr << "out of range" << endl;
        return false;
    }
    // �ڶ�������Ҫ����Ľ��
    DuLnode *s = new DuLnode;
    if (s == NULL) // ����ڴ�����Ƿ�ɹ�
    {
        cout << "Memory allocation failed." << endl;
        return false;
    }
    s->note = e;
    // ���½������ӹ�ϵ
    if (i == 1) // �������λ��Ϊ��һ���ڵ�
    {
        s->prior = L;  // s��ǰ��Ϊͷ�ڵ�
        s->next = p;   // s�ĺ��Ϊԭ���ĵ�һ���ڵ�
        if (p != NULL) // �������Ϊ��
        {
            p->prior = s; // ԭ���ĵ�һ���ڵ��ǰ��Ϊs
        }
        L->next = s; // ͷ�ڵ�ĺ��Ϊs
    }
    else // �������λ�ò��ǵ�һ���ڵ�
    {
        s->prior = p->prior; // s��ǰ������p��ǰ��
        p->prior->next = s;  // p��ǰ������nextָ��s
        s->next = p;         // s�ĺ��ָ��p
        p->prior = s;        // p��ǰ����Ϊָ��s
    }
    // return
    return true;
}
bool deletelist(LinkList &L, const int i, Data &e)
{
    // �ƶ�ָ�뵽i��
    DuLnode *p = L->next;
    int j = 1;
    while (p->next && j < i)
    {
        ++j;
        p = p->next;
    }
    if (j < i || j < 1) // ���i������Χ�ڣ������whileѭ������ֹ��������j<i
    {
        cerr << "out of range" << endl;
        return false;
    }
    // �ı����ӹ�ϵ
    p->prior->next = p->next; // p��ǰ������next����p�ĺ��
    if ((p->next))            // ���ɾ���Ĳ������һ��Ԫ��
    {
        p->next->prior = p->prior;
    }
    e = p->note; // ����ɾ���Ľڵ�
    // �ͷ�p
    delete p;
    // ����
    return true;
}
bool clearlist(LinkList &L)
{
    DuLnode *p, *q;
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
    // CreatListTail(L, 9);
    CreatListHead(L, 9);
    InsertList(L, 3, S);
    int length = GetLength(L);
    printlist(L, length);
    GetElem(L, 5, e);
    cout << e.name << ":" << e.age << endl;
    int locate_nember = LocateElem(L, S);
    cout << locate_nember << endl;
    deletelist(L, 2, e);
    printlist(L, length - 1);
    cout << e.name << ":" << e.age << endl;
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
