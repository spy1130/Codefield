#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// 1 �����׵�ַ 2 ÿ��Ԫ�ص�ռ�ÿռ� 3 ���鳤��
void printAllArray(void *arr, int len, int eleSzie, void(*myPrint(void *)))
{
    char *p = arr; // ����pָ����������׵�ַ
    for (int i = 0; i < len; i++)
    {
        // ��ȡÿ��Ԫ�ص��׵�ַ
        char *pAddr = p + eleSzie * i;
        printf("%d\n", *(int *)pAddr);
    }
}
void printInt(void *data)
{
    int *num = data;
    printf("%d\n", *num);
}
int main()
{
    int arr[] = {1, 2, 3, 4, 5};
    int len = sizeof(arr) / sizeof(int);
    printAllArray(arr, len, sizeof(int), printInt);
    system("pause");
    return 0;
}