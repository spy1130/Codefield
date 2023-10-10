#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "config.h"

struct Person
{
	char a;
	int b;
};

void test01()
{
	char * filePath = "./config.txt";

	int line = getFileLine(filePath);

	printf("�ļ�����Ч����Ϊ��%d\n", line);

	struct ConfigInfo * pArray = NULL;

	parseFile(filePath, line, &pArray);

	//���� ����key ����value
	printf("heroId = %s\n", getInfoByKey("heroId", pArray, line));
	printf("heroName = %s\n", getInfoByKey("heroName", pArray, line));
	printf("heroAtk = %s\n", getInfoByKey("heroAtk", pArray, line));
	printf("heroDef = %s\n", getInfoByKey("heroDef", pArray, line));
	printf("heroInfo = %s\n", getInfoByKey("heroInfo", pArray, line));


	//�ͷ��ڴ�
	freeSpace(pArray);
	pArray = NULL;



	//�ļ����� codeFile( sourceFile ,  destFile )
	// #  35  תΪ short
	// 0000 0000 0010 0011   << 4
	// 0000 0010 0011 0000   
	// 1000 0000 0000 0000    |

	// 1000 0010 0011 0000  + 0000 ~ 1111 �����  rand()%16     0~ 15
	// 1000 0010 0011 1010


	//����  decodeFile ( sourceFile ,  destFile )
	// 1000 0010 0011 1010  <<1 
	// 000  0010 0011 10100  >> 5
	// 0000 0000  0010 0011
}

int main(){

	test01();

	system("pause");
	return EXIT_SUCCESS;
}