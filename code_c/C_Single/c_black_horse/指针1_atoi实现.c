#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int my_atoi(char *str);
int main()
{
    char a[] = "      -1233344";
    printf("%d", my_atoi(a));
    return 0;
}
int my_atoi(char *str)
{
    char *t = str; // ����һ��
    int flag = 0;  // 0��1����
    int num = 0;   // intֵ
    while (*t == ' ' || *t == '\r' || *t == '\n' || *t == '\t')
    {
        t++;
    }                                    // ����������
    flag = *t == '-' ? 1 : 0, t = t + 1; // �ж�������
    for (; *t != 0; t++)
    {
        num = num * 10 + (*t - '0');
    }
    return num = flag == 1 ? num * -1 : num * 1;
}