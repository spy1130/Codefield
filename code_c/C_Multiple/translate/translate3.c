#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#define FLIRNAME "./dict.txt"
typedef struct _dict
{
    char *key; // ���Զ�δ֪��С�ĵ��ʴ��棬��ʱ�򿪱ٿռ�
    char *content;
} DICT;
FILE *open_file()
{
    FILE *fp = fopen(FLIRNAME, "r");
    if (fp == NULL)
    {
        perror("");
        return NULL;
    }
    return fp;
}
void filter_buf(char *buf)
{
    int end = strlen(buf) - 1; // ���һ���ַ��±�
    while (buf[end] == ' ' || buf[end] == '\n' || buf[end] == '\t' || buf[end] == '\r')
        end--;
    buf[end + 1] = 0;
}
void dict_init(DICT **tmp, int line)
{ // Ϊdict���ٿռ�,��ʼ��
    DICT *p;
    p = malloc(sizeof(DICT) * line); // ��Դ�dict.txt
    char *q = NULL;
    char buf[256] = "";
    int i = 0;
    FILE *fp = open_file(); // ���ļ�dict.txt
    while (1)
    {
        q = fgets(buf, sizeof(buf), fp); // ��ȡһ��
        if (q == NULL)
            break;
        filter_buf(buf);                    // ���˵�/r/n/t�ո�
        p[i].key = malloc(strlen(buf) + 1); /// ���洢���ʿ��ٿռ�
        strcpy(p[i].key, buf + 1);          //(buf+1)����#��
        q = fgets(buf, sizeof(buf), fp);
        if (q == NULL)
            break;
        filter_buf(buf); // ���˵�/r/n/t�ո�
        p[i].content = malloc(strlen(buf) + 1);
        strcpy(p[i].content, buf + 6); //(buf+6)����"Trans:"
        i++;
    }
    fclose(fp);
    *tmp = p; // ��p���ݿռ����**tmp�����ռ��
}
int search_dict(char *cmd, DICT *dict, int n, char *content)
{
    for (int i = 0; i < n; i++)
    {
        if (strcmp(cmd, dict[i].key) == 0)
        {
            strcpy(content, dict[i].content);
            return 1;
        }
    }
    return 0;
}
int get_file_line()
{
    char *q = NULL;
    char buf[256] = "";
    int i = 0;
    FILE *fp = open_file(); // ���ļ�dict.txt
    while (1)
    {
        q = fgets(buf, sizeof(buf), fp); // ��ȡһ��
        if (q == NULL)
            break;
        q = fgets(buf, sizeof(buf), fp);
        if (q == NULL)
            break;
        i++;
    }
    fclose(fp);
    return i;
}
int main()
{
    DICT *dict = NULL;
    int line = 0;
    line = get_file_line(); // ��ȡ�ļ����������ж��ٸ�����
    dict_init(&dict, line);
    char cmd[256] = "";               // ���뵥�ʴ���
    char content[256] = "";           // ������Ĵ���
    int ret = 0;                      // �ж�search_dict�����Ƿ��ҵ�����
    LARGE_INTEGER lpPerformanceCount; // ��ʱ���õ�
    QueryPerformanceFrequency(&lpPerformanceCount);
    int c1, c2;
    while (1)
    {
        printf("��������Ҫ����ĵ��ʣ�");
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd) - 1] = 0;                     //"/0" �����һ���ַ�ȥ��\n
        QueryPerformanceCounter(&lpPerformanceCount); // ��õ�ǰʱ�䣬��λ��΢��
        c1 = lpPerformanceCount.QuadPart;
        ret = search_dict(cmd, dict, line, content); // line������Ҫ��
        if (ret == 0)
        {
            printf("word not found\n");
        }
        else
        {
            printf("����Ϊ��%s\n", content);
        }
        QueryPerformanceCounter(&lpPerformanceCount);
        c2 = lpPerformanceCount.QuadPart;
        printf("��ʱ%d us\n", c2 - c1);
    }
    system("pause");
    return 0;
}