#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


int main()
{
	//1.���ô���ʱ�������ʼ����
	//int &pRef;err

	//2.����һ����ʼ�����ܸı�����ָ��
	int a = 10;
	int &pRef = a;//��a�Ŀռ�ȡ����ΪpRef;
	int b = 20;
	pRef = b;//��ֵ����

	cout << &a << endl;
	cout << &pRef << endl;//2����ַһ��

	system("pause");
	return EXIT_SUCCESS;
}

