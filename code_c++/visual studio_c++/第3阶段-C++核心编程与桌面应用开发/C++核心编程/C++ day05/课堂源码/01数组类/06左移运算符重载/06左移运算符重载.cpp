#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<string>

class Maker
{
	friend ostream& operator<<(ostream &out, Maker &m);
public:
	Maker(int id,string name)
	{
		this->id = id;
		this->name = name;
	}

public:
	int id;
	string name;
};
//1.�βκ�ʵ����һ������
//2.���ܸı�����еĴ���
//3.ostream�аѿ������캯��˽�л���
//4.���Ҫ��endlһ��ʹ�ã���ô���뷵��ostream�Ķ���
ostream& operator<<(ostream &out, Maker &m)
{
	cout << m.id <<" "<<m.name<< endl;

	return out;
}

void test01()
{
	Maker m(10,"С��");
	cout << m << endl;
	cout << endl;
	/*
	endl��һ������
	operator<<(endl��,ostream������
	*/

	cout << 10;//�ڲ������˻�����������
}

int main()
{
	test01();
	system("pause");
	return EXIT_SUCCESS;
}

