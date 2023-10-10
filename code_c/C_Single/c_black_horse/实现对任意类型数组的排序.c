#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// �ײ����
void selectSort(void *arr, int eleSize, int len, int (*myCompare)(void *, void *))
{
    char *temp = malloc(eleSize);
    for (int i = 0; i < len; i++)
    {
        // �϶�i������Сֵ �������ֵ
        int minOrmax = i;
        for (int j = i + 1; j < len; j++)
        {
            // ��ȡjԪ�ص��׵�ַ
            char *p = (char *)arr + eleSize * j;
            // ��ȡ��ǰ��Сֵ�������ֵ���׵�ַ
            char *pmami = (char *)arr + eleSize * minOrmax;
            if (myCompare(p, pmami))
            {
                minOrmax = j;
            }
        }
        if (i != minOrmax) // ����϶�����Сֵ�����ֵ���Ǽ������minOrmax
        {
            // ͨ���ڴ濽��ʵ�ֽ���
            char *pi = (char *)arr + eleSize * i;
            char *pminormax = (char *)arr + eleSize * minOrmax;

            memcpy(temp, pi, eleSize);
            memcpy(pi, pminormax, eleSize);
            memcpy(pminormax, temp, eleSize);
        }
    }
    if (temp != NULL)
    {
        free(temp);
        temp = NULL;
    }
}
// �û���
int compareInt(void *data1, void *data2)
{
    int *num1 = data1;
    int *num2 = data2;

    // ����
    if (*num1 < *num2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// �ṹ����������
struct Person
{
    char name[128];
    int age;
};
// �����併������
int comparearr(void *data1, void *data2)
{
    struct Person *p1 = data1;
    struct Person *p2 = data2;
    //     if(p1->age>p2->age){
    //        return 1;
    //    }
    //    else{
    //        return 0;
    //    }
    return p1->age > p2->age;
}
void test()
{
    struct Person arr[] = {
        {"aaa", 1323},
        {"bbb", 139},
        {"ddd", 136},
        {"ccc", 312},
        {"eee", 135},
    };
    int len = sizeof(arr) / sizeof(struct Person);
    selectSort(arr, sizeof(struct Person), len, comparearr);
    for (int i = 0; i < len; i++)
    {
        printf("%s\t%d", arr[i].name, arr[i].age);
    }
}

int main()
{
    int arr[] = {10, 20, 401, 102, 1241, 1410, 1412, 14121, 412411};
    int len = sizeof(arr) / sizeof(int);
    selectSort(arr, sizeof(int), len, compareInt);

    for (int i = 0; i < len; i++)
    {
        printf("%d\n", arr[i]);
    }
    system("pause");
    return 0;
}