#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//��ǿһ
//�Զ������������
struct Maker
{
	char name[64];
	int age;
};

void test01()
{
	 Maker a;//����Ҫ��struct�Ϳ��Զ������

}
//��ǿ��
struct Maker2
{
	int a;
	void func()//�ṹ���ڿ���д����
	{
		cout << "func" << endl;
	}
};
void test02()
{
	Maker2 a2;
	a2.func();
}

int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

