#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include"test.h"
int myFunc(int a, int b = 0)//int b=0;����Ǻ�����Ĭ�ϲ�������һ����0
{
	return a + b;
}
void test01()
{
	//������Ĭ�ϲ���������
	//�������ڳ�Ҫ�õ��βε�ĳ��ֵ����ż��Ҫʹ������ֵ
	//���Ӻ����������
	cout << myFunc(10, 20) << endl;
	cout << myFunc(10)<< endl;
}

//ע��1��������Ĭ�ϲ�������Ĳ������붼��Ĭ�ϲ���
int myFunc2(int a, int b = 0, int c=2,int d=3)
{
	return a + b + c + d;
}
//ע��2��������������ʵ�ֲ���ͬʱ�к�����Ĭ�ϲ���
void myFunc3(int a, int b);
void myFunc3(int a, int b = 0)
{

}

//������ռλ����,ռλ�����ں������������ʱ����ǰ�Ӽӻ��Ӽ�
void func(int a,int=10)//ռλ����Ҳ��Ĭ��ֵ
{

}

void test02()
{
	func(10);
}
//ռλ������Ĭ�ϲ������
void func2(int=10, int a=0)
{

}
void test03()
{
	func2();
	func2(10);
	func2(10,30);
}

int main()
{
//test01();
	myfunc(10);
	system("pause");
	return EXIT_SUCCESS;
}

