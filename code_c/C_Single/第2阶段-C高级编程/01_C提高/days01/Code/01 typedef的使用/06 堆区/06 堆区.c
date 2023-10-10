#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int * getSpace()
{
	int * p = malloc(sizeof(int)* 5);

	if (p == NULL)
	{
		return NULL;
	}

	for (int i = 0; i < 5;i++)
	{
		p[i] = i + 100;
	}
	return p;

}

void test01()
{
	int * p = getSpace();

	for (int i = 0; i < 5;i++)
	{
		printf("%d\n", p[i]);
	}

	//�ֶ��ڶ������������ݣ��ǵ��ֶ��ͷ�
	free(p);
	p = NULL;

}


//ע������
//�������������û�и�ָ������ڴ棬����������ͬ��ָ�������β������������е�ָ���
void allocateSpace( char * pp )
{
	char * temp =  malloc(100);
	if (temp == NULL)
	{
		return;
	}
	memset(temp, 0, 100);
	strcpy(temp, "hello world");
	pp = temp;

}

void test02()
{
	char * p = NULL;
	allocateSpace(p);

	printf("%s\n", p);
}


void allocateSpace2(char ** pp)
{
	char * temp = malloc(100);
	memset(temp, 0, 100);
	strcpy(temp, "hello world");
	*pp = temp;
}

void test03()
{
	char * p = NULL;

	allocateSpace2(&p);

	printf("%s\n", p);

	free(p);
	p = NULL;

}



int main(){
	//test01();
	//test02();
	test03();
	system("pause");
	return EXIT_SUCCESS;
}