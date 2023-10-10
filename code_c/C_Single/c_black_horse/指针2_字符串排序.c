#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//%s��ʽ��ӡ�����Ԫ��
// n��������Ԫ�ظ���
void print_word(char *words[], int n);

// ð������
void sort_word(char *words[], int n);

int main()
{
    char *words[] = {"city", "student", "tree", "bee"};
    int n = sizeof(words) / sizeof(words[0]);

    // ����ǰ��city student tree bee
    print_word(words, n);

    // ð������
    sort_word(words, n);

    // �����bee city student tree
    print_word(words, n);

    return 0;
}
void print_word(char *words[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%s ", words[i]);
    }
    printf("\n");
}
void sort_word(char *words[], int n)
{
    for (int i = 0; i < n - 1; i++) // �Ƚϵ�����
    {
        for (int j = 0; j < n - 1 - i; j++) // ÿһ����Ҫ�ȽϵĴ���
        {
            if (strcmp(words[j], words[j + 1]) > 0)
            {
                char *tmp;
                tmp = words[j + 1];
                words[j + 1] = words[j];
                words[j] = tmp;
            }
        }
    }
}