#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//ջ ע������ ����Ҫ���ؾֲ������ĵ�ַ
int * func()
{
	int a = 10;
	return &a;
}

void test01()
{
	int * p = func();

	//����Ѿ�����Ҫ�ˣ���Ϊa���ڴ��Ѿ����ͷ��ˣ�����û��Ȩ��ȥ��������ڴ�
	printf("a = %d\n", *p);
	printf("a = %d\n", *p);
}


char * getString()
{
	char str[] = "hello world";
	return str;
}

void test02()
{
	char * p = NULL;
	p = getString();
	printf("%s\n", p);
}

int main(){

	//test01();
	test02();
	system("pause");
	return EXIT_SUCCESS;
}