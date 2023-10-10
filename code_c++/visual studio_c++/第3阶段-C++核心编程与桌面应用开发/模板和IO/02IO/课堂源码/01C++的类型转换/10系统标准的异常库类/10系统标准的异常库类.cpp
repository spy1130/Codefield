#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<stdexcept>//2013Vs���Բ���
#include<string>
class MyOut_of :public exception
{
public:
	MyOut_of(const char *errorinfo)
	{
		//const char*ת��string
		this->m_Info = string(errorinfo);
	}
	MyOut_of(const string errorinfo)
	{
		this->m_Info = errorinfo;
	}
	const char *  what() const
	{
		//��stringת��const char*
		return this->m_Info.c_str();
	}
public:
	string m_Info;
};
class Maker
{
public:
	Maker(int age)
	{
		if (age<0 || age>150)
		{
			//throw out_of_range("���䲻�ڷ�Χ��");
			throw MyOut_of("�Լ����쳣�࣬���䲻�ڷ�Χ��");
		}
		else
		{
			this->age = age;
		}
	}
public:
	int age;
};

void test()
{
	try
	{
		Maker m(200);
	}
	catch (out_of_range &ex)
	{
		cout << ex.what() << endl;
	}
	catch (MyOut_of &ex)
	{
		cout << ex.what() << endl;
	}
}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

