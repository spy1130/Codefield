#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CALC_NUM 10
#define FILE_PATH "calc.txt"
FILE *open_file(char *way)
{
    FILE *fp = fopen(FILE_PATH, way);
    if (fp == NULL)
    {
        perror("");
        return -1;
    }
    return fp;
}
void get_data()
{
    FILE *fp = open_file("w");
    char abb[] = {'+', '-', '*', '/'};
    srand(time(NULL));
    int a = 0, b = 0;
    char c = 0;
    char cmd[128] = "";
    for (int i = 0; i < CALC_NUM; i++)
    {
        a = rand() % 100 + 1;
        b = rand() % 100 + 1;
        c = rand() % 4;
        sprintf(cmd, "%d%c%d=\n", a, abb[c], b);
        fputs(cmd, fp);
    }
    fclose(fp);
}
void read_data()
{
    FILE *fp = open_file("r");
    int a = 0, b = 0, answer = 0, i = 0;
    char c = 0;
    char buf[128] = "";
    char buff[10][128] = {0};
    char *p = NULL;
    while (1)
    {
        p = fgets(buf, sizeof(buf), fp);
        if (p == NULL)
            break;
        sscanf(buf, "%d%c%d", &a, &c, &b); // �ַ����صĲ��ǵ�ַ���ַ���������ĸ��ַ
        switch (c)
        {
        case '+':
            answer = a + b;
            break;
        case '-':
            answer = a - b;
            break;
        case '*':
            answer = a * b;
            break;
        case '/':
            answer = a / b;
            break;
        }
        sprintf(buff[i], "%d%c%d=%d\n", a, c, b, answer);
        i++;
    }
    fclose(fp); // ���취�����ٹرգ�����Ƶ���
    fp = open_file("w");
    for (int j; j < i; j++)
    {
        fputs(buff[j], fp);
    }
    fclose(fp);
}
int main()
{
    char passwd[128] = "spric";
    char anr[128] = "";
    int zs = 0;
    get_data();
    printf("������10����������,�����ֳ��������Ϊ0,��鿴calc.txt.\n��Ҫ�鿴������������:");
    do
    {
        scanf("%s", anr);
        if (strcmp(anr, passwd) == 0)
        {
            read_data();
            printf("��������,��鿴calc.txt.");
            break;
        }
        else
        {
            printf("�������,�ɲ�������Ŷ,����3��,���g��:");
            zs++;
        }
    } while (zs < 4);
    printf("\n");
    system("pause");
    return 0;
}