#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "code.h"

// ����
void codeFile(char *sourceFile, char *destFile)
{
    // #  35
    //  short
    //  0000 0000 0010 0011 <<4
    // 0000 0010 0011 0000
    // 1000 0000 0000 0000
    // 1000 0010 0011 0000 + 0000 ~1111 0~15
    // 1000 0010 0011 0101

    FILE *fp1 = fopen(sourceFile, "r");
    FILE *fp2 = fopen(destFile, "r");

    if (!fp1 || !fp2)
    {
        return;
    }

    char ch;

    while ((ch = fgetc(fp1)) != EOF)
    {
        // ��ÿ���ַ����м���
        short temp = (short)ch;
        temp <<= 4;
        // ͳһ����Ϊ����
        temp = temp | 0x8000;
        // �������
        temp = temp + rand() % 16;
        // �������ַ������ļ���
        fprintf(fp2, "%hd", temp);
    }
}
// ����
void deCodeFile(char *sourceFile, char *destFile)
{
    // 1000 0010 0011 0101 <<1
    // 000 0010 0011 01010 >>5
    // 0000 0000 0010 0011
    FILE *fp1 = fopen(sourceFile, "r");
    FILE *fp2 = fopen(destFile, "r");
    if (!fp1 || !fp2)
    {
        return;
    }
    short temp;
    while (!feof(fp1)) // ֻҪû���ļ�β��һֱ��
    {
        fscanf(fp1, "%hd", &temp);
        temp = temp << 1;
        temp >>= 5;

        char ch = (char)temp;
        fputc(ch, fp2);
    }
    fclose(fp1);
    fclose(fp2);
}