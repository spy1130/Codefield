#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void test()
{
	int a = 10;
	int b = 20;
	printf("%d\n", a > b ? a : b);
	//(a > b ? a : b) = 100;err
	//������ʽ���ص�����ֵ������ֵ�����ص���20
	//20 = 100;
	*(a > b ? &a : &b) = 100;
	printf("b=%d\n", b);
}

int main2()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

