#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

void test()
{
	int a = 10;
	const int b = a;//����ñ�����const���εľֲ�������ֵ����ô�����������Ż�
	int *p = (int*)&b;
	*p = 100;
	cout << "b=" << b << endl;
	cout << "*p=" << *p << endl;
}

//�Զ����������ͣ������������Ż�
struct Maker
{
	Maker()//���캯��
	{
		a = 100;
	}
	int a;
};

void test02()
{
	//�������Ͷ������
	//��ʵ��������
	const Maker ma;
	cout << ma.a << endl;
	Maker *p = (Maker*)&ma;
	p->a = 200;
	cout << ma.a << endl;//û���Ż�����Ϊ�����Ż��Զ�����������
}

int main()
{
	test02();

	system("pause");
	return EXIT_SUCCESS;
}

