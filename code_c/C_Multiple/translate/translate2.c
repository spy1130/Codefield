#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct _dict
{
    char *key;//���Զ�δ֪��С�ĵ��ʴ��棬��ʱ�򿪱ٿռ�
    char *content;  
} DICT;
void dict_init(DICT **tmp)
{ // Ϊdict���ٿռ�,��ʼ��
    DICT *p;
    p = malloc(sizeof(DICT) * 2);
    p[0].key =malloc(strlen("hello")+1);
    strcpy(p[0].key, "hello");//����holle����p[0].keyָ��Ŀռ�
    p[0].content =malloc(strlen("���")+1);
    strcpy(p[0].content, "���");
    p[0].key =malloc(strlen("world")+1);
    strcpy(p[1].key, "world");
    p[0].content =malloc(strlen("����")+1);
    strcpy(p[1].content, "����");

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
int main()
{
    DICT *dict = NULL;
    dict_init(&dict);
    char cmd[256] = "";     // ���뵥�ʴ���
    char content[256] = ""; // ������Ĵ���
    int ret = 0;            // �ж�search_dict�����Ƿ��ҵ�����
    while (1)
    {
        printf("��������Ҫ����ĵ��ʣ�");
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd) - 1] = 0;                 // �����һ���ַ�ȥ��\n
        ret = search_dict(cmd, dict, 2, content); // 2������Ҫ��
        if (ret == 0)
        {
            printf("word not found\n");
        }
        else
        {
            printf("����Ϊ��%s\n", content);
        }
    }

    system("pause");
    return 0;
}