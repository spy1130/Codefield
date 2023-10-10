#pragma  once
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct ConfigInfo
{
	char key[64]; //����ֵ
	char value[64]; //ʵֵ
};


//��ȡ��Ч����
int getFileLine( char * fileName );

//�жϵ�ǰ���Ƿ���Ч
int isValidLine(char * str);

//�����ļ�
void parseFile(char * filePath, int lines, struct ConfigInfo **  configInfo);

//��������ֵ ��ȡ ʵֵ 
char * getInfoByKey(char * key, struct ConfigInfo * configInfo, int line);

//�ͷ���Ϣ
void freeSpace(struct ConfigInfo * configInfo);
