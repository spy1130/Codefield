#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

void test01()
{
	int a = 10;
	int b = 20;
	(a > b ? a : b) = 100;
	//c++����Ŀ��������ص�����ֵ���ǿռ�
	cout << "a=" << a << endl;
	cout << "b=" << b << endl;
}

int main()
{
	test01();
	system("pause");
	return EXIT_SUCCESS;
}

