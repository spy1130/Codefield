#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Father
{
public:
	Father()
	{
		cout << "Father���캯��" << endl;
	}
	~Father()
	{
		cout << "Father��������" << endl;
	}
};

class Son :public Father
{
public:
	Son()
	{
		cout << "Son���캯��" << endl;
	}
	~Son()
	{
		cout << "Son��������" << endl;
	}
};
//�ȵ��ø���Ĺ��캯�����ٵ��ñ���Ĺ��캯����������������˳��֮
void test01()
{
	Son s;
}

class A
{
public:
	A()
	{
		cout << "A �Ĺ���" << endl;
	}
	~A()
	{
		cout << "A ������" << endl;
	}
public:
	Son s;
};

class B
{
public:
	B()
	{
		cout << "B �Ĺ���" << endl;
	}
	~B()
	{
		cout << "B ������" << endl;
	}
};

class C
{
public:
	C()
	{
		cout << "C �Ĺ���" << endl;
	}
	~C()
	{
		cout << "C ������" << endl;
	}
};

class D:public A
{
public:
	D()
	{
		cout << "D �Ĺ���" << endl;
	}
	~D()
	{
		cout << "D ������" << endl;
	}
public:
	B b;
	C c;
};
void test02()
{
	D d;
}
int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

