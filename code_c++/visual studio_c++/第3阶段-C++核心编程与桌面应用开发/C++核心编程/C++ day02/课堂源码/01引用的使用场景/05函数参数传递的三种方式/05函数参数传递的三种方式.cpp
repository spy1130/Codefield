#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//ֵ����
void swap(int a, int b)
{
	int tmp = a;
	a = b;
	b = tmp;
}

//ָ�봫��
void swap2(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

//���ô���
void swap3(int &a, int &b)//int &a=a,int &b=b;
{
	int tmp = a;
	a = b;
	b = tmp;
}

//��ӡ
void myprint(int &a,int &b)
{
	cout << "a=" << a << " b=" << b << endl;
}

int main()
{
	int a = 10;
	int b = 20;
	swap(a, b);
	myprint(a, b);//10,20

	swap2(&a, &b);
	myprint(a, b);//20,10

	a = 10;
	b = 20;

	swap3(a, b);
	myprint(a, b);//20,10
	system("pause");
	return EXIT_SUCCESS;
}

