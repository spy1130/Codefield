#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "code.h"

struct Person
{
	char a;
	int b;
};

int main()
{
	srand((unsigned int)time(NULL));

	char *filePath = "./config.txt";

	int line = getFileLine(filePath);

	printf("�ļ�����Ч����Ϊ��%d\n", line);

	struct ConfigInfo *pArray = NULL;

	parseFile(filePath, line, &pArray);

	// ���� ����key ����value
	printf("heroId = %s\n", getInfoByKey("heroId", pArray, line));
	printf("heroName = %s\n", getInfoByKey("heroName", pArray, line));
	printf("heroAtk = %s\n", getInfoByKey("heroAtk", pArray, line));
	printf("heroDef = %s\n", getInfoByKey("heroDef", pArray, line));
	printf("heroInfo = %s\n", getInfoByKey("heroInfo", pArray, line));

	// �ļ�����
	codeFile("config.txt", "configCode.txt");

	// #  35  תΪ short
	// 0000 0000 0010 0011   << 4
	// 0000 0010 0011 0000
	// 1000 0000 0000 0000    |

	// 1000 0010 0011 0000  + 0000 ~ 1111 �����  rand()%16     0~ 15
	// 1000 0010 0011 1010

	// ����
	decodeFile("configCode.txt", "deConfig.txt");
	// 1000 0010 0011 1010  <<1
	// 000  0010 0011 10100  >> 5
	// 0000 0000  0010 0011

	// �ͷ��ڴ�
	freeSpace(pArray);
	pArray = NULL;

	system("pause");
	return EXIT_SUCCESS;
}