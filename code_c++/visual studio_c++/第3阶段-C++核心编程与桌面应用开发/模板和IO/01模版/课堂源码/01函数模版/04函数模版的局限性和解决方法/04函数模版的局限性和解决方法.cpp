#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<string>
template<class T>
void func(T a, T b)
{
	if (a > b)
	{
		cout << "a>b" << endl;
	}
	else
	{
		cout << "a<=b" << endl;
	}
}
void test()
{
	int arr[20];
	int arr2[10];

	func(arr, arr2);
}

class Maker
{
public:
	Maker(string name,int age)
	{
		this->age = age;
		this->name = name;
	}
public:
	string name;
	int age;
};
template<class T>
void myfunc(T &a, T &b)
{
	if (a > b)
	{
		cout << "a>b" << endl;
	}
	else
	{
		cout << "a<=b" << endl;
	}
}
//��������廯����ģ�棬��Ϊû��ͨ����
//���廯����ģ�棬ע������ĺ���ģ��Ҫ�У����ܾ��廯
template<>void myfunc<Maker>(Maker &a, Maker &b)
{
	cout << "����ģ��ľ��廯" << endl;
	if (a.age > b.age)
	{
		cout << "a>b" << endl;
	}
	else
	{
		cout << "a<=b" << endl;
	}
}

void test02()
{
	Maker m1("aaa", 10);
	Maker m2("bbb", 20);

	myfunc(m1, m2);
}

int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

