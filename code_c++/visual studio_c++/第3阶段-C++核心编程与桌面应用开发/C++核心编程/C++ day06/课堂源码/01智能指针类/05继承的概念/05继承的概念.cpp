#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//�̳е�����1.���븴�á�2.��չ��Ĺ���
class Father
{
public:
	void func1()
	{
		cout << "ԼС���" << endl;
	}
	void func2()
	{
		cout << "��Ǯ" << endl;
	}
};

class Son :public Father
{
public:
	void func3()
	{
		cout << "�в�" << endl;
	}
};

void test()
{
	Son s;
	s.func1();
	s.func2();
	s.func3();
}
int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

