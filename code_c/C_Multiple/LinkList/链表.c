#include <stdio.h>
#include <stdlib.h>
// ������λ
struct node
{
    int data;
    struct node *next;
};
// ����ͷ����(��Ҫһ��ͬ����ָ�����)
struct node *creatlist()
{
    struct node *headnode = (struct node *)malloc(sizeof(struct node));
    headnode->next = NULL;
    return headnode;
}
// �����µĽڵ�
struct node *newnode(int data)
{
    struct node *newnode = (struct node *)malloc(sizeof(struct node));
    newnode->data = data;
    newnode->next = NULL;
    return newnode;
}
// ��ͷ�����룬������ͷ�ڵ������
void insertbyhead(struct node *headnode, int data)
{
    struct node *newNode = newnode(data);
    newNode->next = headnode->next;
    headnode->next = newNode;
}
// ������ӡ����
void printlist(struct node *headnode)
{
    struct node *pmove = headnode->next; // �б�ͷ����Ҫ�ӵڶ�����ʼ��������ͷ�ڵ������+1
    if (pmove == NULL)
        printf("������\n");
    else
    {
        while (pmove != NULL)
        {
            printf("%d->", pmove->data);
            pmove = pmove->next;
        }
        printf("\n");
    }
}
// ��β�����루�����µĽڵ㣩
void insertbytail(struct node *headnode, int data)
{
    // ����Ľڵ�
    struct node *newNode = newnode(data);
    // ��ʱβָ��
    struct node *tailnode = headnode;
    while (tailnode->next != NULL) // ����βָ��
    {
        tailnode = tailnode->next;
    }
    tailnode->next = newNode; // ��������
}
// ����ĳ�����ݵ�ǰ��ĺ���
void insert(struct node *headnode, int data, int posdata)
{
    // ����������ʱָ��ʵ�ֽڵ����
    struct node *posfrt = headnode;
    struct node *pos = headnode->next;
    // ����λ��
    while (pos != NULL && pos->data != posdata)
    {
        posfrt = pos;
        pos = pos->next;
    }
    if (pos == NULL)
        printf("�Ҳ�������λ��\n");
    else
    {
        struct node *newNode = newnode(data); // �Ҳ������ʵ�λ��ʱ����ȱ������ڴ��ʡ�ռ�
        newNode->next = pos;
        posfrt->next = newNode;
    } // ����
}
// ��ͷ��ɾ���ڵ�
void deletebyhead(struct node *headnode)
{
    if (headnode->next == NULL)
        printf("���Ǹ�������\n"); // �������޷�ɾ��
    else
    {
        struct node *nextnode = headnode->next;
        headnode->next = headnode->next->next;
        free(nextnode);
    }
}
// ��β��ɾ���ڵ�
void deletebytail(struct node *headnode)
{
    if (headnode->next == NULL)
        printf("���Ǹ�������\n"); // ��ֹbug����ѯһ���ǲ��ǿ�����
    else
    {
        // ��ʱ
        struct node *tailnode = headnode->next;
        struct node *tailfro = headnode;
        // ����β
        while (tailnode->next != NULL)
        {
            tailfro = tailnode;
            tailnode = tailnode->next;
        }
        // ɾ��
        tailfro->next = NULL;
        free(tailnode);
    }
}
// ���Ҳ�ɾ��
void deletenode(struct node *headnode, int data)
{
    // ��ʱ
    struct node *deletenodefro = headnode;
    struct node *deletenode = headnode->next;
    // ����
    while (deletenode != NULL && deletenode->data != data)
    {
        deletenodefro = deletenode;
        deletenode = deletenode->next;
    }
    if (deletenode == NULL)
        printf("�Ҳ�����Ӧ�ڵ�\n");
    else
    {
        // ɾ��
        deletenodefro->next = deletenode->next;
        free(deletenode);
    }
}
// �������ֻ������������
void clear_LinkList(struct node *headnode)
{
    if (headnode == NULL)
        return;
    else
    {
        // ��ʱ
        struct node *pCurrent = headnode->next;
        while (pCurrent != NULL)
        { // �ȼ�¼��һ���ڵ��λ��
            struct node *nextNode = pCurrent->next;
            // �ͷŵ�ǰ�ڵ�
            free(pCurrent);
            // �ڵ�����ƶ�
            pCurrent = nextNode;
        }
        // ͷ�ڵ�next�ÿ�
        headnode->next = NULL;
    }
}
// �����б�(ɾ���������)
void destroy_LinkList(struct node *headnode)
{
    if (headnode == NULL)
        return;
    else
    {
        //������б�
        clear_LinkList(headnode);
        free(headnode);
        headnode =NULL;
    }
}
