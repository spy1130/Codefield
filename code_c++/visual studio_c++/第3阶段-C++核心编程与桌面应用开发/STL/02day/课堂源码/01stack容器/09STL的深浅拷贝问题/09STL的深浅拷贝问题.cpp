#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
using namespace std;

class Maker
{
public:
	Maker(int age)
	{
		this->age = age;
	}
	Maker(const Maker&m)
	{
		cout << "��������" << endl;
	}
public:
	int age;
};

void test()
{
	vector<Maker> v;
	Maker m(10);
	//1.��������Ҫ�ܱ�����
	//2.ע��Ҫǳ��������
	v.push_back(m);
}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

