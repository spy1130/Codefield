#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class rule
{
public:
	virtual double getNum(double a, double b) = 0;
};
//��
class rule_add :public rule
{
public:
	virtual double getNum(double a, double b)
	{
		return a + b;
	}
};
//��
class rule_minux :public rule
{
public:
	virtual double getNum(double a, double b)
	{
		return a - b;
	}
};

//��
class rule_take :public rule
{
public:
	virtual double getNum(double a, double b)
	{
		return a * b;
	}
};

//��
class rule_division :public rule
{
public:
	virtual double getNum(double a, double b)
	{
		if (b == 0)
		{
			cout << "��������Ϊ0" << endl;
			return -1.0;
		}
		return a / b;
	}
};

void test()
{
	rule *r = NULL;
	r = new rule_add;
	cout << r->getNum(30,10) << endl;

	r = new rule_minux;
	cout << r->getNum(30, 10) << endl;

	r = new rule_take;
	cout << r->getNum(30, 10) << endl;

	r = new rule_division;
	cout << r->getNum(30, 10) << endl;
}
int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

